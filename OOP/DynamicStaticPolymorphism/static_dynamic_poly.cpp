#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>
#include <algorithm>

namespace DynamicPolymorphism
{
    class Formatter
    {
    public:
        virtual std::string format(const std::string& data) = 0;
        virtual ~Formatter() = default;
    };

    class UpperCaseFormatter : public Formatter
    {
    public:
        std::string format(const std::string& data) override
        {
            std::string transformed_data{data};

            std::transform(data.begin(), data.end(), transformed_data.begin(), [](char c) { return std::toupper(c); });

            return transformed_data;
        }
    };

    class LowerCaseFormatter : public Formatter
    {
    public:
        std::string format(const std::string& data) override
        {
            std::string transformed_data{data};

            std::transform(data.begin(), data.end(), transformed_data.begin(), [](char c) { return std::tolower(c); });

            return transformed_data;
        }
    };

    class CapitalizeFormatter : public Formatter
    {
    public:
        std::string format(const std::string& data) override
        {
            std::string transformed_data{data};

            if (data.size() >= 1)
            {
                transformed_data[0] = static_cast<char>(std::toupper(data.front()));

                std::transform(data.begin() + 1, data.end(), transformed_data.begin() + 1, [](char c) { return std::tolower(c); });
            }

            return transformed_data;
        }
    };

    class Logger
    {
        std::unique_ptr<Formatter> formatter_;

    public:
        Logger(std::unique_ptr<Formatter> formatter)
            : formatter_{std::move(formatter)}
        { }

        void log(const std::string& data)
        {
            std::cout << "LOG: " << formatter_->format(data) << '\n';
        }
    };
} // namespace DynamicPolymorphism

namespace StaticPolymorphism
{
    template <typename T>
    concept Formatter = requires(T formatter, const std::string& message)
    {
        { formatter.format(message) } -> std::same_as<std::string>;
    };

    struct UpperCaseFormatter
    {
        std::string format(const std::string& message) const
        {
            std::string result = message;
            std::transform(result.begin(), result.end(),
                result.begin(), [](char c) { return std::toupper(c); });
            return result;
        }
    };

    static_assert(Formatter<UpperCaseFormatter>);

    struct CapitalizeFormatter
    {
        std::string format(const std::string& message) const
        {
            std::string result = message;
            result[0] = std::toupper(result[0]);
            return result;
        }
    };

    static_assert(Formatter<CapitalizeFormatter>);

    template <Formatter TFormatter = UpperCaseFormatter>
    class Logger
    {
        TFormatter formatter_;

    public:
        Logger() = default;

        Logger(TFormatter formatter)
            : formatter_(std::move(formatter))
        {
        }

        void log(const std::string& message)
        {
            std::cout << formatter_.format(message) << std::endl;
        }
    };
} // namespace StaticPolymorphism

void dynamic_polymorphism()
{
    using namespace DynamicPolymorphism;

    Logger logger{std::make_unique<UpperCaseFormatter>()};
    logger.log("Hello, World!");

    logger = Logger{std::make_unique<LowerCaseFormatter>()};
    logger.log("Hello, World!");

    logger = Logger{std::make_unique<CapitalizeFormatter>()};
    logger.log("Hello, World!");
}

void static_polymorphism()
{
    using namespace StaticPolymorphism;

    Logger logger{UpperCaseFormatter{}}; // C++17 - CTAD - type deduction
    logger.log("Hello, World!");

    Logger<CapitalizeFormatter> logger2; // explicit type as template argument
    logger2.log("hello, world!");

    Logger logger3;
    logger3.log("hello, world!");
}

class IBase
{
public:
    virtual ~IBase() = default;
    virtual void print() const = 0;
};

class Base : public IBase
{
    std::string name_;
protected:
    std::string name() const
    {
        return name_;
    }
public:
    Base(const std::string& name) : name_(name) {}
    Base(const char* name) : name_(name) {}
    
    virtual ~Base() = default;

    virtual void print() const
    {
        std::cout << "Base: " << name() << '\n';
    }
};

class Derived : public Base // hybrid inheritance - both interface and implementation inheritance
{
public:
    // Derived(const std::string& name) : Base(name) {}
    // Derived(const char* name) : Base(name) {}

    using Base::Base; // inheriting constructors

    void print() const override
    {
        std::cout << "Derived: " << name() << '\n';
    }
};

class Container : std::vector<int>
{
    using BaseImpl = std::vector<int>;
public:
    using BaseImpl::BaseImpl;
      

    size_t size() const
    {
        return BaseImpl::size();
    }

    int& operator[](size_t index)
    {
        return BaseImpl::operator[](index);
    }    

    void add_item(int value)
    {
        BaseImpl::push_back(value);
    }

    void remove_item(int value)
    {
        auto it = std::find(BaseImpl::begin(), BaseImpl::end(), value);
        if (it != BaseImpl::end())
            BaseImpl::erase(it);
    }
};

int main()
{
    Derived derived("derived");
    Base& base = derived;

    dynamic_polymorphism();

    std::cout << "\n\n";

    static_polymorphism();

    std::cout << "\n\n";

    Container container = {1, 2, 3};
    container.add_item(10);
    container.add_item(20);
    container.remove_item(2);

    for(size_t i = 0u; i < container.size(); ++i)
    {
        std::cout << container[i] << ' ';
    }
}