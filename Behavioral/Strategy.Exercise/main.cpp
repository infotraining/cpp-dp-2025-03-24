#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

struct StatResult
{
    std::string description;
    double value;

    StatResult(const std::string& desc, double val)
        : description(desc)
        , value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;

class Statistics
{
public:
    virtual Results calculateStatistics(const Data& data) const = 0;
    virtual ~Statistics() = default;
};

class AverageStat : public Statistics
{
public:
    Results calculateStatistics(const Data& data) const override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double avg = sum / data.size();

        return Results{StatResult("Avg", avg)};
    }
};

class MinStat : public Statistics
{
public:
    Results calculateStatistics(const Data& data) const override
    {
        double min = *(std::min_element(data.begin(), data.end()));

        return Results{StatResult("Min", min)};
    }
};

class MaxStat : public Statistics
{
public:
    Results calculateStatistics(const Data& data) const override
    {
        double max = *(std::max_element(data.begin(), data.end()));

        return Results{StatResult("Max", max)};
    }
};

class SumStat : public Statistics
{
public:
    Results calculateStatistics(const Data& data) const override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);

        return Results{StatResult("Sum", sum)};
    }
};

class StatGroup : public Statistics
{
    std::vector<std::shared_ptr<Statistics>> stats_;

public:
    void add(std::shared_ptr<Statistics> stat)
    {
        stats_.push_back(stat);
    }

    Results calculateStatistics(const Data& data) const override
    {
        Results results;
        for (const auto& stat : stats_)
        {
            Results res = stat->calculateStatistics(data);
            results.insert(results.end(), res.begin(), res.end());
        }

        return results;
    }
};

class DataAnalyzer
{
    std::shared_ptr<Statistics> statistics_;
    Data data_;
    Results results_;

public:
    DataAnalyzer(std::shared_ptr<Statistics> stat)
        : statistics_{stat}
    {
    }

    void load_data(const std::string& file_name)
    {
        data_.clear();
        results_.clear();

        std::ifstream fin(file_name.c_str());
        if (!fin)
            throw std::runtime_error("File not opened");

        double d;
        while (fin >> d)
        {
            data_.push_back(d);
        }

        std::cout << "File " << file_name << " has been loaded...\n";
    }

    void set_statistics(std::shared_ptr<Statistics> stat)
    {
        statistics_ = stat;
    }

    void calculate()
    {
        Results results = statistics_->calculateStatistics(data_);
        results_.insert(results_.end(), results.begin(), results.end());
    }

    const Results& results() const
    {
        return results_;
    }
};

namespace AlternativeTake
{
    using Statistics = std::function<Results(const Data&)>;

    class AverageStat
    {
    public:
        Results operator()(const Data& data) const
        {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);
            double avg = sum / data.size();

            return Results{StatResult("Avg", avg)};
        }
    };

    Results min(const Data& data)
    {
        double min = *(std::min_element(data.begin(), data.end()));

        return Results{StatResult("Min", min)};
    }

    auto MaxStat = [](const Data& data)
    {
        double max = *(std::max_element(data.begin(), data.end()));

        return Results{StatResult("Max", max)};
    };

    class SumStat
    {
    public:
        Results operator()(const Data& data) const
        {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);

            return Results{StatResult("Sum", sum)};
        }
    };

    class StatGroup
    {
        std::vector<Statistics> stats_;

    public:
        void add(Statistics stat)
        {
            stats_.push_back(stat);
        }

        Results operator()(const Data& data) const
        {
            Results results;
            for (const auto& stat : stats_)
            {
                Results res = stat(data);
                results.insert(results.end(), res.begin(), res.end());
            }

            return results;
        }
    };

    class DataAnalyzer
    {
        Statistics statistics_;
        Data data_;
        Results results_;

    public:
        DataAnalyzer(Statistics stat)
            : statistics_{stat}
        {
        }

        void load_data(const std::string& file_name)
        {
            data_.clear();
            results_.clear();

            std::ifstream fin(file_name.c_str());
            if (!fin)
                throw std::runtime_error("File not opened");

            double d;
            while (fin >> d)
            {
                data_.push_back(d);
            }

            std::cout << "File " << file_name << " has been loaded...\n";
        }

        void set_statistics(Statistics stat)
        {
            statistics_ = stat;
        }

        void calculate()
        {
            Results results = statistics_(data_);
            results_.insert(results_.end(), results.begin(), results.end());
        }

        const Results& results() const
        {
            return results_;
        }
    };
}

void show_results(const Results& results)
{
    for (const auto& rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

int main()
{
    std::shared_ptr<Statistics> avg = std::make_shared<AverageStat>();
    auto min_max = std::make_shared<StatGroup>();
    min_max->add(std::make_shared<MinStat>());
    min_max->add(std::make_shared<MaxStat>());
    std::shared_ptr<Statistics> sum = std::make_shared<SumStat>();

    auto std_stats = std::make_shared<StatGroup>();
    std_stats->add(avg);
    std_stats->add(min_max);
    std_stats->add(sum);

    DataAnalyzer da{std_stats};
    da.load_data("stats_data.dat");
    da.calculate();
    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_stats_data.dat");
    da.calculate();
    show_results(da.results());
}
