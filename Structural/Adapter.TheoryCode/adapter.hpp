#ifndef ADAPTER_HPP_
#define ADAPTER_HPP_

#include <iostream>

// "Target"
class Target
{
public:
    virtual void request() = 0;
    virtual ~Target() = default;
};

// "Adaptee"
class Adaptee
{
public:
    virtual ~Adaptee() = default;

    virtual void specific_request()
    {
        std::cout << "Called specific_request()" << std::endl;
    }
};

class BetterAdaptee : public Adaptee
{
public:
    void specific_request() override
    {
        std::cout << "Called BetterAdaptee::specific_request()" << std::endl;
    }
};

// "Adapter"
class ClassAdapter : public Target, private Adaptee
{
public:
    void request() override
    {
        specific_request();
    }
};

// "Adapter"
class ObjectAdapter : public Target
{
private:
    Adaptee& adaptee_;

public:
    ObjectAdapter(Adaptee& adaptee)
        : adaptee_(adaptee)
    {
    }

    void request() override
    {
        adaptee_.specific_request(); // delegation
    }
};

#endif /*ADAPTER_HPP_*/
