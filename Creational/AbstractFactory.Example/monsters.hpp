#ifndef MONSTERS_HPP_
#define MONSTERS_HPP_

#include <iostream>
#include <string>

namespace Game
{
    class Enemy
    {
    public:
        virtual void action() = 0;
        virtual std::unique_ptr<Enemy> clone() const = 0;
        virtual ~Enemy() = default;
    };

    template <typename TEnemy, typename TBase = Enemy>
    class CloneableEnemy : public TBase
    {
    public: 
        std::unique_ptr<Enemy> clone() const override
        {
            return std::make_unique<TEnemy>(static_cast<const TEnemy&>(*this));
        }
    };

    class Soldier : public Enemy
    {
    };

    class Monster : public Enemy
    {
    };

    class SuperMonster : public Enemy
    {
    };

    class SillySoldier : public CloneableEnemy<SillySoldier, Soldier>
    {
    public:
        void action()
        {
            std::cout << "SillySoldier Action" << std::endl;
        }
    };

    class SillyMonster : public CloneableEnemy<SillyMonster, Monster>
    {
    public:
        void action() override
        {
            std::cout << "SillyMonster Action" << std::endl;
        }
    };

    class SillySuperMonster : public CloneableEnemy<SillySuperMonster, SuperMonster>
    {
    public:
        void action() override
        {
            std::cout << "SillySuperMonster Action" << std::endl;
        }
    };

    class BadSoldier : public CloneableEnemy<BadSoldier, Soldier>
    {
    public:
        void action() override
        {
            std::cout << "BadSoldier Action" << std::endl;
        }
    };

    class BadMonster : public CloneableEnemy<BadMonster, Monster>
    {
    public:
        void action() override
        {
            std::cout << "BadMonster Action" << std::endl;
        }
    };

    class BadSuperMonster : public CloneableEnemy<BadSuperMonster, SuperMonster>
    {
    public:
        void action() override
        {
            std::cout << "BadSuperMonster Action" << std::endl;
        }
    };

    class SuperBadSoldier : public CloneableEnemy<SuperBadSoldier, Soldier>
    {
    public:
        void action() override
        {
            std::cout << "SuperBadSoldier Action" << std::endl;
        }
    };

    class SuperBadMonster : public CloneableEnemy<SuperBadMonster, Monster>
    {   
    public:
        void action() override
        {
            std::cout << "SuperBadMonster Action" << std::endl;
        }
    };

    class SuperBadSuperMonster : public CloneableEnemy<SuperBadSuperMonster, SuperMonster>
    {
    public:
        void action() override
        {
            std::cout << "SuperBadSuperMonster Action" << std::endl;
        }
    };
};

#endif /*MONSTERS_HPP_*/
