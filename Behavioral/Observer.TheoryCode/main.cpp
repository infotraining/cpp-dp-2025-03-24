#include "observer.hpp"
#include <boost/signals2.hpp>

using namespace std;

// Subject
class TemperatureMonitor : public Observable<TemperatureMonitor, double> // CRTP
{
private:
    double current_temperature_;

public:
    TemperatureMonitor(double temp)
        : current_temperature_{temp}
    {
        std::cout << "Creating an observable " << typeid(*this).name() << this << "\n";
    }

    void set_temperature(double new_temperature)
    {
        if (current_temperature_ != new_temperature)
        {
            current_temperature_ = new_temperature;

            // sending notification to all observers
            notify(*this, current_temperature_);
        }
    }
};

// Observer & Subject
class Fan : public Observer<TemperatureMonitor, double>, public Observable<Fan, const std::string&>
{
    bool is_on_ = false;

public:
    void update(TemperatureMonitor& tm, double current_temperature) override
    {
        if (!is_on_ && current_temperature > 25.0)
            on();

        if (is_on_ && current_temperature < 24.0)
            off();
    }

    void on()
    {
        is_on_ = true;
        notify(*this, "Fan is on...");
    }

    void off()
    {
        is_on_ = false;
        notify(*this, "Fan is off...");
    }
};

// Observer
class ConsoleLogger : public Observer<TemperatureMonitor, double>, public Observer<Fan, const std::string&>
{
public:
    void update(TemperatureMonitor& tm, double current_temperature) override
    {
        std::cout << "ConsoleLogger notified - temperature has changed to " << current_temperature << std::endl;
    }

    void update(Fan& tm, const std::string& message) override
    {
        std::cout << "ConsoleLogger notified - " << message << std::endl;
    }
};

namespace BoostObserver
{
    class TemperatureMonitor
    {
        boost::signals2::signal<void(double)> temperature_changed_;
        double current_temperature_;
    public:
        TemperatureMonitor(double temp)
            : current_temperature_{temp}
        {
        }

        void set_temperature(double new_temperature)
        {
            if (current_temperature_ != new_temperature)
            {
                current_temperature_ = new_temperature;
                temperature_changed_(current_temperature_); // notify all subscribers
            }
        }

        boost::signals2::connection subscribe(std::function<void(double)> slot)
        {
            return temperature_changed_.connect(slot); // register a new subscriber
        }
    };

    class Fan
    {
        bool is_on_ = false;
        boost::signals2::signal<void(const std::string&)> fan_state_changed_;
    public:
        Fan() = default;

        void on_temp_changed(double current_temperature)
        {
            if (!is_on_ && current_temperature > 25.0)
                on();

            if (is_on_ && current_temperature < 24.0)
                off();
        }

        void on()
        {
            is_on_ = true;
            fan_state_changed_("Fan is on...");
        }

        void off()
        {
            is_on_ = false;
            fan_state_changed_("Fan is off...");
        }

        boost::signals2::connection subscribe(std::function<void(const std::string&)> slot)
        {
            return fan_state_changed_.connect(slot);
        }
    };

    class ConsoleLogger
    {
    public:
        void log_temperature(double current_temperature)
        {
            std::cout << "Temperature has changed to " << current_temperature << std::endl;
        }

        void log_fan_state(const std::string& message)
        {
            std::cout << message << std::endl;
        }
    };
}

void boost_observer()
{
    BoostObserver::Fan fan;
    BoostObserver::TemperatureMonitor temp_monitor(21.0);
    BoostObserver::ConsoleLogger console_logger;

    temp_monitor.subscribe([&console_logger](double temp) { console_logger.log_temperature(temp); });
    auto conn = temp_monitor.subscribe([&fan](double temp) { fan.on_temp_changed(temp); });
    fan.subscribe([&console_logger](const std::string& message) { console_logger.log_fan_state(message); });

    temp_monitor.set_temperature(22.0);
    temp_monitor.set_temperature(23.0);
    temp_monitor.set_temperature(24.0);
    temp_monitor.set_temperature(25.0);
    temp_monitor.set_temperature(26.0);

    conn.disconnect();
    
    temp_monitor.set_temperature(25.0);
    temp_monitor.set_temperature(24.0);
    temp_monitor.set_temperature(23.0);
    temp_monitor.set_temperature(21.0);
}

int main()
{
    // Fan fan;
    // TemperatureMonitor temp_monitor(21.0);
    // ConsoleLogger console_logger;

    // temp_monitor.subscribe(&fan);
    // temp_monitor.subscribe(&console_logger);
    // fan.subscribe(&console_logger);

    // temp_monitor.set_temperature(22.0);
    // temp_monitor.set_temperature(23.0);
    // temp_monitor.set_temperature(24.0);
    // temp_monitor.set_temperature(25.0);
    // temp_monitor.set_temperature(26.0);
    // temp_monitor.set_temperature(25.0);
    // temp_monitor.set_temperature(24.0);
    // temp_monitor.set_temperature(23.0);
    // temp_monitor.set_temperature(21.0);

    // temp_monitor.unsubscribe(&fan);

    // temp_monitor.set_temperature(22.0);
    // temp_monitor.set_temperature(23.0);
    // temp_monitor.set_temperature(24.0);
    // temp_monitor.set_temperature(25.0);
    // temp_monitor.set_temperature(26.0);
    // temp_monitor.set_temperature(25.0);
    // temp_monitor.set_temperature(24.0);
    // temp_monitor.set_temperature(23.0);
    // temp_monitor.set_temperature(21.0);

    boost_observer();
}
