#include "starbugs_coffee.hpp"

#include <memory>
#include <cassert>

void client(std::shared_ptr<Coffee> coffee)
{
    std::cout << "Description: " << coffee->get_description() << "; Price: " << coffee->get_total_price() << std::endl;
    coffee->prepare();
}

class CoffeeBuilder
{
    std::shared_ptr<Coffee> coffee_;
public:
    CoffeeBuilder() = default;
    

    template <typename... Ts>
    CoffeeBuilder& add()
    {
        assert(coffee_ != nullptr);

        (..., (coffee_ = std::make_shared<Ts>(coffee_)));
        return *this;
    }

    template <typename T>
    CoffeeBuilder& create_base()
    {
        coffee_ = std::make_shared<T>();
        return *this;
    }

    std::shared_ptr<Coffee> get_coffee()
    {
        return coffee_;
    }
};

int main()
{
    auto cf = 
        std::make_shared<Whipped>(
            std::make_shared<Whisky>(
                std::make_shared<Whisky>(
                    std::make_shared<Espresso>())));

    client(cf);

    CoffeeBuilder cb;
    cb.create_base<Espresso>()
        .add<ExtraEspresso>()
        .add<Whisky, Whisky, Whipped>();

    client(cb.get_coffee());
}
