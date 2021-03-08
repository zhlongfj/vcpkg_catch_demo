#include <iostream>
#include <fruit/fruit.h>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

class IRequester
{
public:
    virtual int request(const std::string &data) = 0;
};

class TCPRequester : public IRequester
{
public:
    INJECT(TCPRequester()) = default;
    int request(const std::string &data) override
    {
        //request data from net
        return 0;
    }
};

struct TCPRequest{};
fruit::Component<fruit::Annotated<TCPRequest, IRequester>> getTCPRequesterComponent()
{
    return fruit::createComponent().bind<fruit::Annotated<TCPRequest, IRequester>, TCPRequester>();
}

class UDPRequester : public IRequester
{
public:
    INJECT(UDPRequester()) = default;
    int request(const std::string &data) override
    {
        //request data from net
        return 0;
    }
};

struct UDPRequest{};
fruit::Component<fruit::Annotated<UDPRequest, IRequester>> getUDPRequesterComponent()
{
    return fruit::createComponent().bind<fruit::Annotated<UDPRequest, IRequester>, UDPRequester>();
}

class IDataManager
{
public:
    virtual void setValue(const std::string &key, const std::string &data) = 0;
    virtual std::string getValue(const std::string &key) = 0;
};


class DataManager : public IDataManager
{
public:
    INJECT(DataManager()) = default;
    void setValue(const std::string &key, const std::string &data) override
    {
        //store data to database
        std::cout << "setValue: " << data << std::endl;
    }

    std::string getValue(const std::string &key) override
    {
        //get data from database
        std::cout << "getValue: " << key << std::endl;
        return "test";
    }
};

fruit::Component<IDataManager> getDataManagerComponent()
{
    return fruit::createComponent().bind<IDataManager, DataManager>();
}

class IController
{
public:
    virtual int setValue(const std::string &key, const std::string &value) = 0;
    virtual ~IController() = default; //辅助注入的接口需要显示声明虚析构函数
};

using ControllerFactory = std::function<std::unique_ptr<IController>(std::string)>;
//using ScalerFactory = std::function<std::unique_ptr<Scaler>(double)>;
class LightController : public IController
{
public:
    INJECT(LightController(ASSISTED(std::string) prefix,
        ANNOTATED(TCPRequest, IRequester *) pTCPRequester, ANNOTATED(UDPRequest, IRequester *) pUDPRequester,
        IDataManager *pDataManager))
            : m_prefix(prefix)
            , m_pTCPRequester(pTCPRequester)
            , m_pUDPRequester(pUDPRequester)
            , m_pDataManager(pDataManager){}
//    LightController(const std::string &prefix, IRequester *pRequester, IDataManager *pDataManager)
//    : m_prefix(prefix)
//    , m_pRequester(pRequester)
//    , m_pDataManager(pDataManager){}

    int setValue(const std::string &key, const std::string &value) override
    {
        std::string realValue = m_prefix + value;
        if (m_pDataManager->getValue(key) == realValue)
        {
            return 1;
        }

        auto ret = m_pTCPRequester->request(realValue);
        if (ret != 0)
        {
            return -1;
        }
        m_pDataManager->setValue(key, realValue);
        return 0;
    }

private:
    const std::string m_prefix;
    IRequester *m_pTCPRequester;
    IRequester *m_pUDPRequester;
    IDataManager *m_pDataManager;
};

fruit::Component<fruit::Required<fruit::Annotated<TCPRequest, IRequester>,
        fruit::Annotated<UDPRequest, IRequester>, IDataManager>,
        ControllerFactory> getControllerComponent()
{
    return fruit::createComponent().bind<IController, LightController>();
}

fruit::Component<ControllerFactory> getMainControllerComponent()
{
    return fruit::createComponent()
            .install(getControllerComponent)
            .install(getTCPRequesterComponent)
            .install(getUDPRequesterComponent)
            .install(getDataManagerComponent);
}


class FakeDataManager : public IDataManager
{
public:
    INJECT(FakeDataManager()) = default;
    void setValue(const std::string &key, const std::string &data) override
    {
        m_datas[key] = data;
    }

    std::string getValue(const std::string &key) override
    {
        return m_datas[key];
    }

private:
    std::map<std::string, std::string> m_datas;
};

fruit::Component<IDataManager> getFakeDataManagerComponent()
{
    return fruit::createComponent().bind<IDataManager, FakeDataManager>();
}

class FakeTCPRequester : public IRequester
{
public:
    INJECT(FakeTCPRequester()) = default;
    int request(const std::string &data) override
    {
        return 1;
    }
};

fruit::Component<IRequester> getFakeTCPRequesterComponent()
{
    return fruit::createComponent().bind<IRequester, FakeTCPRequester>();
}

fruit::Component<ControllerFactory> getFakeControllerComponent()
{
    return fruit::createComponent()
            .replace(getDataManagerComponent).with(getFakeDataManagerComponent)
            .replace(getFakeTCPRequesterComponent).with(getFakeTCPRequesterComponent)
            .install(getMainControllerComponent);
}

SCENARIO("test fruit", "test fruit") {
    GIVEN("fruit") {
        WHEN("testing: setValue") {
            THEN("") {
                fruit::Injector<ControllerFactory> injector(getFakeControllerComponent);
                ControllerFactory controller(injector);
                auto ret = controller("ddd")->setValue("brightness", "100");
                CHECK(ret == 0);
                std::cout << "Light setValue result: " << ret << std::endl;
                ret = controller("ddd")->setValue("brightness", "100");
                CHECK(ret == 1);
                std::cout << "Light setValue result: " << ret << std::endl;
            }
        }
    }
}
