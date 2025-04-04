#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <functional>

using Track = std::vector<char>;

// "Product"
class MusicService
{
public:
    virtual std::optional<Track> get_track(const std::string& title) = 0;
    virtual ~MusicService() = default;
};

// "ConcreteProductA"
class TidalService : public MusicService
{
public:
    TidalService(const std::string& user_name, const std::string& secret)
    {
        std::cout << "Creating TidalService...\n";
    }

    std::optional<Track> get_track(const std::string& title) override
    {
        return Track(title.begin(), title.end());
    }
};

// "ConcreteProductB"
class SpotifyService : public MusicService
{
public:
    SpotifyService(const std::string& user_name, const std::string& secret, int timeout = 30)
    {
        std::cout << "Creating SpotifyService...\n";
    }

    std::optional<Track> get_track(const std::string& title) override
    {
        return Track(title.begin(), title.end());
    }
};

// "ConcreteProductC"
class FilesystemMusicService : public MusicService
{
public:
    FilesystemMusicService(const std::string& directory_path = "/user/music")
    {
        std::cout << "Creating FilesystemMusicService...\n";
    }

    std::optional<Track> get_track(const std::string& title) override
    {
        return std::nullopt;
    }
};

class AppleMusicService : public MusicService
{
    std::string user_name_;
    std::string secret_;
    int port_;

public:
    AppleMusicService(const std::string& user_name, const std::string& secret, int port = 8099)
        : user_name_{user_name}
        , secret_{secret}
        , port_{port}
    {
        std::cout << "Creating AppleMusicService...\n";
    }

    std::optional<Track> get_track(const std::string& title) override
    {
        return Track(title.begin(), title.end());
    }
};

namespace Canonical
{

    // "Creator"
    class MusicServiceCreator
    {
    public:
        virtual std::unique_ptr<MusicService> create_music_service() = 0; // factory method
        virtual ~MusicServiceCreator() = default;
    };

    // "ConcreteCreatorA"
    class TidalServiceCreator : public MusicServiceCreator
    {
        std::string user_name_;
        std::string secret_;

    public:
        TidalServiceCreator(const std::string& user_name, const std::string& secret)
            : user_name_{user_name}
            , secret_{secret}
        {
        }

        std::unique_ptr<MusicService> create_music_service() override
        {
            return std::make_unique<TidalService>(user_name_, secret_);
        }
    };

    // "ConcreteCreatorB"
    class SpotifyServiceCreator : public MusicServiceCreator
    {
        std::string user_name_;
        std::string secret_;
        int timeout_;

    public:
        SpotifyServiceCreator(const std::string& user_name, const std::string& secret, int timeout)
            : user_name_{user_name}
            , secret_{secret}
            , timeout_{timeout}
        {
        }

        std::unique_ptr<MusicService> create_music_service() override
        {
            return std::make_unique<SpotifyService>(user_name_, secret_, timeout_);
        }
    };

    class FsMusicServiceCreator : public MusicServiceCreator
    {
        std::string path_;

    public:
        FsMusicServiceCreator(const std::string& path = "/music")
            : path_{path}
        {
        }

        std::unique_ptr<MusicService> create_music_service() override
        {
            return std::make_unique<FilesystemMusicService>(path_);
        }
    };

    class AppleMusicServiceCreator : public MusicServiceCreator
    {
        std::string user_name_;
        std::string secret_;
        int port_;

    public:
        AppleMusicServiceCreator(const std::string& user_name, const std::string& secret, int port = 8099)
            : user_name_{user_name}
            , secret_{secret}
            , port_{port}
        {
        }

        std::unique_ptr<MusicService> create_music_service() override
        {
            return std::make_unique<AppleMusicService>(user_name_, secret_, port_);
        }
    };

} // namespace Canonical

namespace Modern
{
    using MusicServiceCreator = std::function<std::unique_ptr<MusicService>()>;

    std::unique_ptr<MusicService> create_tidal_service()
    {
        return std::make_unique<TidalService>("tidal_user", "KJH8324d&df");
    }

    class SpotifyServiceCreator
    {
        std::string user_name_;
        std::string secret_;
        int timeout_;

    public:
        SpotifyServiceCreator(const std::string& user_name, const std::string& secret, int timeout)
            : user_name_{user_name}
            , secret_{secret}
            , timeout_{timeout}
        {
        }

        std::unique_ptr<MusicService> operator()()
        {
            return std::make_unique<SpotifyService>(user_name_, secret_, timeout_);
        }
    };

    auto create_fs_service = []() {
        return std::make_unique<FilesystemMusicService>("/music");
    };

    auto create_apple_service = []() {
        return std::make_unique<AppleMusicService>("apple_user", "KJH832", 8099);
    };
}

#endif /*FACTORY_HPP_*/
