// CNavigatorAStar::Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface) -- allocate an A* navigator and register it with its manager's class factory
// Leaf: ?Create@CNavigatorAStar@@SIPAV1@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1CBF0, 62 bytes, plain `ret` (static __fastcall: manager in ecx, iface in edx).
// Identity: Ego_r.pdb `?Create@CNavigatorAStar@@SIPAV1@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z`.
// Callees: scalar operator new 0x00BFEA1A (0xC4), CNavigatorAStar::CNavigatorAStar 0x00A1CA70 (out of line), and
//   CClass::PostConstructionInit(CClassFactory&) 0x00A36F80 (PDB name; `factory.OnCreateClass(*this)` -- the manifest
//   label CThingFilter_IsValid::operator() is an identical-COMDAT fold of the same 13 bytes). PostConstructionInit is
//   called on the new-expression's result, so it is reached with a null `this` when the allocation fails (the second
//   tail of the function).
class CNavigatorExternalInterface;

class CClassFactory {
public:
    virtual void v00();
};

class CNavigatorManager : public CClassFactory {
public:
    virtual void v00();
};

class CClass {
public:
    virtual void v00();
    void PostConstructionInit(CClassFactory& factory);  // 0x00A36F80
};

class CNavigatorBase : public CClass {                  // 0x24 bytes
public:
    unsigned char _base[0x20];
};

class CNavigatorAStar : public CNavigatorBase {         // 0xC4 bytes
public:
    unsigned char _body[0xA0];
    static CNavigatorAStar* __fastcall Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface);
private:
    CNavigatorAStar(CNavigatorManager& manager, const CNavigatorExternalInterface& iface);   // 0x00A1CA70
};

CNavigatorAStar* __fastcall CNavigatorAStar::Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface)
{
    CNavigatorAStar* navigator = new CNavigatorAStar(manager, iface);
    navigator->PostConstructionInit(manager);
    return navigator;
}