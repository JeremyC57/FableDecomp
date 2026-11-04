// CTCBerserk::OnRemove() -- on removal, restart the creature's current mode
// Leaf: ?OnRemove@CTCBerserk@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078C8B0, 76 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (OnRemove, CTCBase order) of vtable 0x0126BB94 (RTTI .?AVCTCBerserk@@); Ego_r.pdb ?OnRemove@CTCBerserk@@UAEXXZ; restarts the creature mode manager's current mode (interface 0x31).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline CThing::GetInterface, interface 0x31),
//   CTCCreatureModeManager::RestartCurrentMode() 0x0073A840.
// Shape (family M169, representative 0x0061A360): `CTCCreatureModeManager* tc; ParentThing->GetInterface(&tc, 0x31); tc->RestartCurrentMode();` --
//   the GetInterface result is not tested (an absent component calls through the uninitialised `push ecx` word, as retail
//   does); the GetInterface of the landed CTCWillLightning::StartLoopingSound 0x007A0400. Speed TU.
#pragma warning(disable: 4700)
class CTCBase;
class CTCCreatureModeManager {
public:
    void RestartCurrentMode();                                   // 0x0073A840
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                       // +0x20
    CInterfaceMap m_interfaces;                           // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCBerserk {
public:
    virtual void v00();
    virtual void v01();
    CThing* ParentThing;                                  // +0x04 (CTCBase)
    virtual void OnRemove();                              // slot 2
};
void CTCBerserk::OnRemove()
{
    CTCCreatureModeManager* tc;
    ParentThing->GetInterface(&tc, 0x31);
    tc->RestartCurrentMode();
}