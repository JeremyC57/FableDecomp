// CThingObject::IsFrameUpdated() const -- the object needs frame updates when CThing says so, when its kind-0x47 component has an active entry, or when its physics is moving
// Leaf: ?IsFrameUpdated@CThingObject
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007040E0, length 106 bytes (+ int3 padding). CThingObject vtable 0x01264DCC slot 44
// (the catalog's "vfunc_44"; Ego_r.pdb has ?IsFrameUpdated@CThingObject@@UBE_NXZ, and slot 44 calls CThing::IsFrameUpdated).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined const GetTC of kind 0x47 = bit 7 of CThing+0x28),
// CThing::IsFrameUpdated @ 0x004C7BF0.
// Layout (retail, local model): CThing component bits +0x20, interface map +0x44, PhysicsTC +0x60; the kind-0x47 component's
// pointer at +0x10; CTCPhysicsBase Moving bit at +0x3c bit 0 (PDB).
// Shape: the component test (out-parameter __forceinline GetInterface: `mov eax,[eax+4] ; mov ecx,[eax+10h]`) is a bool local computed first (bl, `mov bl,1 ; jmp` / `xor bl,bl`); the result is a single
// `||` return (int materialised); the bit-7 HasTC test compiles to `test al,al ; jns`.
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase { public:
    unsigned char _pad00[0x3c];
    bool Moving : 1;                            // +0x3c bit 0
    bool OldMoving : 1;
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0xc];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long kind) const
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(kind);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || kind < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    bool IsFrameUpdated() const;                // 0x004C7BF0
};
struct CTC47 {                                  // component kind 0x47
    unsigned char _pad00[0x10];
    void*         m_pActive;                    // +0x10
};
class CThingObject : public CThing {
public:
    bool IsFrameUpdated() const;
};

bool CThingObject::IsFrameUpdated() const
{
    CTC47* tc;
    bool active = GetInterface(&tc, 0x47) && tc->m_pActive != 0;
    return CThing::IsFrameUpdated() || active || PhysicsTC->Moving;
}