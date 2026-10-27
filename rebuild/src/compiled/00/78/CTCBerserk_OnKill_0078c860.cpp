// CTCBerserk::OnKill() -- on kill, restart the creature's current mode
// Leaf: ?OnKill@CTCBerserk@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078C860, 76 bytes, thiscall, plain `ret`.
// Identity (proven): slot 14 (OnKill, CTCBase order) of vtable 0x0126BB94 (RTTI .?AVCTCBerserk@@); Ego_r.pdb ?OnKill@CTCBerserk@@UAEXXZ; restarts the creature mode manager's current mode (interface 0x31, bound ?RestartCurrentMode@CTCCreatureModeManager@@QAEXXZ).
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
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                 // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                       // +0x20
    CInterfaceMap m_interfaces;                           // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCBerserk {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    CThing* ParentThing;                                  // +0x04 (CTCBase)
    virtual void OnKill();                              // slot 14
};
void CTCBerserk::OnKill()
{
    CTCCreatureModeManager* tc;
    ParentThing->GetInterface(&tc, 0x31);
    tc->RestartCurrentMode();
}