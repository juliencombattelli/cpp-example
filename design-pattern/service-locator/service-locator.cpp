///////////////////////////////////////////////////////////////////////////////
// Service locator implementation using C++17
//      Inspired from http://gameprogrammingpatterns.com/service-locator.html
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <utility>
#include <memory>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////
// Services database class
//      All services types in TServices must be default constructible
///////////////////////////////////////////////////////////////////////////////
template<typename... TServices>
    requires std::conjunction_v<std::is_default_constructible<TServices>...>
class ServiceDB
{
public:
    template<typename TService>
    TService& get() { return std::get<TService>(m_db); }

private:
    std::tuple<TServices...> m_db;
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Locator template class
//      The locator is responsible for providing a service instance. If no 
//      instance have been provided, return a NullService which typically
//      do noting
///////////////////////////////////////////////////////////////////////////////
template<typename TService, typename TNullService>
class Locator
{
public:
    static void init() { m_service = &m_nullService; }
    static TService* getService() { return m_service; }
    static void provide(TService* service) { m_service = service; }

private:
    inline static TService* m_service = nullptr;
    inline static TNullService m_nullService;
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Service interfaces
//      Two services are provided in this example: an audio service responsible 
//      for playing sound, a log service responsible for printing messages
///////////////////////////////////////////////////////////////////////////////
struct IAudioService
{
    virtual ~IAudioService() = default;
    virtual void playSound(int id) = 0;
};

struct ILogService
{
    virtual ~ILogService() = default;
    virtual void log(const std::string& msg) = 0;
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// NullServices types
//      They provide default implementation of a service
///////////////////////////////////////////////////////////////////////////////
struct NullAudio : IAudioService
{
    void playSound(int id) override
    {
        std::cout << "NullAudio::playsound(" << id << ")\n";
    }
};

struct NullLog : ILogService
{
    void log(const std::string& msg) override
    {
        std::cout << "NullLog::log(" << msg << ")\n";
    }
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Locator aliases
//      Associate a service interface and a null service
///////////////////////////////////////////////////////////////////////////////
using AudioLocator = Locator<IAudioService, NullAudio>;
using LogLocator = Locator<ILogService, NullLog>;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Services concrete types
//      They implement the methods of their interface to provide a service, 
//      like play sound from console application or log into a terminal
///////////////////////////////////////////////////////////////////////////////
struct ConsoleAudio : IAudioService
{
    void playSound(int id) override
    {
        std::cout << "ConsoleAudio::playsound(" << id << ")\n";
    }
};

struct ConsoleLog : ILogService
{
    void log(const std::string& msg) override
    {
        std::cout << "ConsoleLog::log(" << msg << ")\n";
    }
};
///////////////////////////////////////////////////////////////////////////////


int main()
{
    // Create the database    
    ServiceDB<ConsoleAudio, ConsoleLog> sdb;

    // Initialize the locators, default service location is NullService
    AudioLocator::init();
    LogLocator::init();

    // Provide a ConsoleAudio object from the services database
    AudioLocator::provide(&sdb.get<ConsoleAudio>());

    // Provide nothing for the LogLocator, defaulting to NullLog
    // LogLocator::provide(...);

   
    // ....
    
    
    // Get the services instance
    IAudioService* audio = AudioLocator::getService();
    ILogService* logger = LogLocator::getService();

    // Use them
    audio->playSound(1);
    logger->log("sound played");

    return 0;
}
