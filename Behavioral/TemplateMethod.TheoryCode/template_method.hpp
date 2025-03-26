#ifndef TEMPLATE_METHOD_HPP_
#define TEMPLATE_METHOD_HPP_

#include <iostream>
#include <memory>
#include <random>
#include <string>

struct User
{
    std::string username;
    std::string email;
    std::string phone;
};

class NotificationService
{
public:
    virtual void send(const User& user, const std::string& data) = 0;
    virtual ~NotificationService() = default;
};

// "Decorator"
class NotificationWithLogging : public NotificationService
{
    std::unique_ptr<NotificationService> notification_service_;

public:
    explicit NotificationWithLogging(std::unique_ptr<NotificationService> notification_service)
        : notification_service_{std::move(notification_service)}
    {
    }

    void send(const User& user, const std::string& data) override
    {
        std::cout << "LOG: Sending notification...\n";
        notification_service_->send(user, data);
        std::cout << "LOG: Notification sent!\n";
    }
};

class MailerService : public NotificationService
{
public:
    virtual void send(const User& user, const std::string& data)
    {
        std::cout << "Secret code: " << data << " - send by e-mail to " << user.email << "\n";
    }
};

class SmsService : public NotificationService
{
public:
    virtual void send(const User& user, const std::string& data)
    {
        std::cout << "Secret code: " << data << " - send by sms to " << user.phone << "\n";
    }
};

using Code = std::string;

// "AbstractClass"
class AuthenticationService
{
protected:
    virtual void prompt() { }
    virtual Code generate_secret_code() = 0;
    virtual Code enter_secret_code() = 0;
    virtual void on_success() = 0;
    virtual void on_failure() = 0;

    virtual std::unique_ptr<NotificationService> create_notification_service() const
    {
        return std::make_unique<SmsService>();
    }

public:
    bool login(const User& user)
    {
        prompt();

        Code code = generate_secret_code();

        std::unique_ptr<NotificationService> notification_service = create_notification_service();
        notification_service->send(user, code);

        if (enter_secret_code() == code)
        {
            on_success();
            return true;
        }
        else
        {
            on_failure();
            return false;
        }
    }

    virtual ~AuthenticationService() { }
};

namespace AlternativeTake
{
    class AuthenticationService
    {
    private:
        std::shared_ptr<NotificationService> notification_service_;

    protected:
        virtual void prompt() { }
        virtual Code generate_secret_code() = 0;
        virtual Code enter_secret_code() = 0;
        virtual void on_success() = 0;
        virtual void on_failure() = 0;        

    public:
        AuthenticationService(std::shared_ptr<NotificationService> notification_service)
            : notification_service_{notification_service}
        {
        }

        bool login(const User& user)
        {
            prompt();

            Code code = generate_secret_code();

            notification_service_->send(user, code);

            if (enter_secret_code() == code)
            {
                on_success();
                return true;
            }
            else
            {
                on_failure();
                return false;
            }
        }

        virtual ~AuthenticationService() { }
    };
}

// "ConcreteClass"
class DefaultAuthenticationService : public AuthenticationService
{
protected:
    Code generate_secret_code() override
    {
        std::random_device rd;
        std::mt19937 rnd_engine(rd());
        std::uniform_int_distribution<> distr(1000, 9999);
        return std::to_string(distr(rnd_engine));
    }

    Code enter_secret_code() override
    {
        std::cout << "Enter secret code: ";
        std::string code;
        std::cin >> code;
        return code;
    }

    void on_success() override
    {
        std::cout << "Access granted!\n";
    }

    void on_failure() override
    {
        std::cout << "Access denied!\n";
    }
};

class DefaultAuthenticationServiceWithLogging : public DefaultAuthenticationService
{
protected:
    std::unique_ptr<NotificationService> create_notification_service() const override
    {
        return std::make_unique<NotificationWithLogging>(DefaultAuthenticationService::create_notification_service());
    }
};

// "ConcreteClass"
class MailerAuthenticationService : public DefaultAuthenticationService
{
protected:
    void prompt() override
    {
        std::cout << "###########################\n";
    }

    std::unique_ptr<NotificationService> create_notification_service() const override
    {
        return std::make_unique<MailerService>();
    }
};

#endif /*TEMPLATE_METHOD_HPP_*/