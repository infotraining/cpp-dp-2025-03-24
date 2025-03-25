#ifndef ENEMY_FACTORY_HPP_
#define ENEMY_FACTORY_HPP_

#include "monsters.hpp"

#include <memory>

namespace Game
{
    class EnemyFactory
    {
    public:
        virtual std::unique_ptr<Enemy> CreateSoldier() = 0;
        virtual std::unique_ptr<Enemy> CreateMonster() = 0;
        virtual std::unique_ptr<Enemy> CreateSuperMonster() = 0;
        virtual ~EnemyFactory() = default;
    };

    class EasyLevelEnemyFactory : public EnemyFactory
    {
    public:
        std::unique_ptr<Enemy> CreateSoldier() override
        {
            return std::make_unique<SillySoldier>();
        }

        std::unique_ptr<Enemy> CreateMonster() override
        {
            return std::make_unique<SillyMonster>();
        }

        std::unique_ptr<Enemy> CreateSuperMonster() override
        {
            return std::make_unique<SillySuperMonster>();
        }
    };

    class DieHardLevelEnemyFactory : public EnemyFactory
    {
    public:
        std::unique_ptr<Enemy> CreateSoldier() override
        {
            return std::make_unique<BadSoldier>();
        }

        std::unique_ptr<Enemy> CreateMonster() override
        {
            return std::make_unique<BadMonster>();
        }

        std::unique_ptr<Enemy> CreateSuperMonster() override
        {
            return std::make_unique<BadSuperMonster>();
        }
    };

    class SuperHardLevelEnemyFactory : public EnemyFactory
    {
    public:
        std::unique_ptr<Enemy> CreateSoldier() override
        {
            return std::make_unique<SuperBadSoldier>();
        }

        std::unique_ptr<Enemy> CreateMonster() override
        {
            return std::make_unique<SuperBadMonster>();
        }

        std::unique_ptr<Enemy> CreateSuperMonster() override
        {
            return std::make_unique<SuperBadSuperMonster>();
        }
    };

    class EnemyCloneFactory : public EnemyFactory
    {
        std::unique_ptr<Soldier> soldier_;
        std::unique_ptr<Monster> monster_;
        std::unique_ptr<SuperMonster> super_monster_;

    public:
        EnemyCloneFactory(std::unique_ptr<Soldier> s, std::unique_ptr<Monster> m, std::unique_ptr<SuperMonster> sm)
            : soldier_(std::move(s))
            , monster_(std::move(m))
            , super_monster_(std::move(sm))
        {}

        std::unique_ptr<Enemy> CreateSoldier() override
        {
            return soldier_->clone();
        }

        std::unique_ptr<Enemy> CreateMonster() override
        {
            return monster_->clone();
        }

        std::unique_ptr<Enemy> CreateSuperMonster() override
        {
            return super_monster_->clone();
        }
    };
}

#endif /*ENEMY_FACTORY_HPP_*/
