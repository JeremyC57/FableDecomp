// Behaviour test for CNavigatorFlyer::Create (0x00A18F70): a new navigator of type 4 is built on the manager and the
// interface, then registered with the manager's class factory (CClass::PostConstructionInit ->
// CClassFactory::OnCreateClass), and the new object is returned.
#include <cstdio>

class CClass;
class CNavigatorExternalInterface { public: int Id; };
static int g_built = 0;

class CClassFactory {
public:
    CClass* LastCreated;
    int Created;
    CClassFactory() : LastCreated(0), Created(0) {}
    virtual ~CClassFactory() {}
    virtual void OnCreateClass(CClass& c) { LastCreated = &c; ++Created; }
};

class CNavigatorManager : public CClassFactory {
public:
};

class CClass {
public:
    virtual ~CClass() {}
    void PostConstructionInit(CClassFactory& factory) { factory.OnCreateClass(*this); }
};

class CNavigatorBase : public CClass {
public:
    CNavigatorManager* Manager;
    unsigned long Type;
    const CNavigatorExternalInterface* Interface;
    CNavigatorBase(CNavigatorManager& manager, unsigned long type, const CNavigatorExternalInterface& iface)
        : Manager(&manager), Type(type), Interface(&iface) {}
};

class CNavigatorFlyer : public CNavigatorBase {
public:
    float DestinationPos[3];
    static CNavigatorFlyer* Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface);
    CNavigatorFlyer(CNavigatorManager& manager, const CNavigatorExternalInterface& iface) : CNavigatorBase(manager, 4, iface) { ++g_built; }
};

CNavigatorFlyer* CNavigatorFlyer::Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface)
{
    CNavigatorFlyer* navigator = new CNavigatorFlyer(manager, iface);
    navigator->PostConstructionInit(manager);
    return navigator;
}

int main()
{
    CNavigatorManager manager;
    CNavigatorExternalInterface iface; iface.Id = 7;
    int ok = 1;
    CNavigatorFlyer* a = CNavigatorFlyer::Create(manager, iface);
    if (a == 0 || g_built != 1) ok = 0;
    else {
        if (a->Manager != &manager || a->Type != 4 || a->Interface != &iface) ok = 0;
        if (manager.Created != 1 || manager.LastCreated != static_cast<CClass*>(a)) ok = 0;
    }
    CNavigatorFlyer* b = CNavigatorFlyer::Create(manager, iface);
    if (b == 0 || b == a || manager.Created != 2 || manager.LastCreated != static_cast<CClass*>(b) || g_built != 2) ok = 0;
    delete a;
    delete b;

    if (!ok) { std::printf("00a18f70_TEST FAIL\n"); return 1; }
    std::printf("00a18f70_TEST PASS\n");
    return 0;
}