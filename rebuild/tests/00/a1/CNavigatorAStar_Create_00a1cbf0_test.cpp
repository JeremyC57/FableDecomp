// Behaviour test for CNavigatorAStar::Create (0x00A1CBF0): a new navigator of type 3 is built on the manager and the
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

class CNavigatorAStar : public CNavigatorBase {
public:
    unsigned char _body[0xA0];
    static CNavigatorAStar* Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface);
    CNavigatorAStar(CNavigatorManager& manager, const CNavigatorExternalInterface& iface) : CNavigatorBase(manager, 3, iface) { ++g_built; }
};

CNavigatorAStar* CNavigatorAStar::Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface)
{
    CNavigatorAStar* navigator = new CNavigatorAStar(manager, iface);
    navigator->PostConstructionInit(manager);
    return navigator;
}

int main()
{
    CNavigatorManager manager;
    CNavigatorExternalInterface iface; iface.Id = 7;
    int ok = 1;
    CNavigatorAStar* a = CNavigatorAStar::Create(manager, iface);
    if (a == 0 || g_built != 1) ok = 0;
    else {
        if (a->Manager != &manager || a->Type != 3 || a->Interface != &iface) ok = 0;
        if (manager.Created != 1 || manager.LastCreated != static_cast<CClass*>(a)) ok = 0;
    }
    CNavigatorAStar* b = CNavigatorAStar::Create(manager, iface);
    if (b == 0 || b == a || manager.Created != 2 || manager.LastCreated != static_cast<CClass*>(b) || g_built != 2) ok = 0;
    delete a;
    delete b;

    if (!ok) { std::printf("00a1cbf0_TEST FAIL\n"); return 1; }
    std::printf("00a1cbf0_TEST PASS\n");
    return 0;
}