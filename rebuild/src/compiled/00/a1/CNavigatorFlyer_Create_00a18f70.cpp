// CNavigatorFlyer::Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface) -- allocate a flying navigator and register it with its manager's class factory
// Leaf: ?Create@CNavigatorFlyer@@SIPAV1@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A18F70, 70 bytes, plain `ret` (static __fastcall: manager in ecx, iface in edx).
// Identity: Ego_r.pdb `?Create@CNavigatorFlyer@@SIPAV1@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z`;
//   the vtable stored is CNavigatorFlyer's 0x0129CB54 (rtti-vtables.tsv).
// Callees: scalar operator new 0x00BFEA1A (0x30), CNavigatorBase::CNavigatorBase(manager, 4, iface) 0x00A47110, and
//   CClass::PostConstructionInit(CClassFactory&) 0x00A36F80 (see 0x00A1CBF0). Unlike CNavigatorAStar, the protected
//   CNavigatorFlyer constructor is inline (base call + vtable store; DestinationPos has a no-op constructor).
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
    CNavigatorBase(CNavigatorManager& manager, unsigned long type, const CNavigatorExternalInterface& iface);   // 0x00A47110
};

#include "engine/C3DVector.h"

class CNavigatorFlyer : public CNavigatorBase {         // vtable 0x0129CB54, 0x30 bytes
public:
    virtual void v00();
    C3DVector DestinationPos;                           // +0x24
    static CNavigatorFlyer* __fastcall Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface);
protected:
    CNavigatorFlyer(CNavigatorManager& manager, const CNavigatorExternalInterface& iface)
        : CNavigatorBase(manager, 4, iface)
    {
    }
};

CNavigatorFlyer* __fastcall CNavigatorFlyer::Create(CNavigatorManager& manager, const CNavigatorExternalInterface& iface)
{
    CNavigatorFlyer* navigator = new CNavigatorFlyer(manager, iface);
    navigator->PostConstructionInit(manager);
    return navigator;
}