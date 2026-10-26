// NCreatureMode::CModeDragonStrafe::OnDeactivate() -- resume a player-type dragon's animation, mark the mode inactive, and set bit 0 of the interface-0x3a component's flag byte again
// Leaf: ?OnDeactivate@CModeDragonStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC660, 84 bytes (+ int3 padding), `ret`; vtable 0x012686C8 slot 6.
// Callees: CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10 (the mode's address is the suspend reason),
//   CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, type 0x3a = bit 26 of the +0x24 dword).
// Name: FableWin.pdb `?OnDeactivate@CModeDragonStrafe@NCreatureMode@@UAEXXZ` (slot 6).
// Shape: the mirror of CModeDragonStrafe::OnActivate @ 0x008DC600; the inline CModeBase::OnDeactivate (`Active = false`)
//   sits between the resume and the flag set (`or byte ptr [eax+50h],1`).
class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CThingAnimatable {
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

class CThing : public CThingAnimatable { public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                  // +0x10 (1 = player creature)
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

struct CTCFlagged {                        // the type-0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_flag : 1;              // +0x50 bit 0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x10];             // WeaponBasedFilter, FramesBeforeRemovingSelf
    bool          Active;                  // +0x18
    void OnDeactivate() { Active = false; }
};

namespace NCreatureMode {
struct CModeDragonStrafe : CModeBase {     // vtable 0x012686C8 (CModeDragonFlyingBase members from +0x1c)
    void OnDeactivate();
};
}

void NCreatureMode::CModeDragonStrafe::OnDeactivate()
{
    if (ParentThing->m_type == 1)
        ParentThing->SetSuspendAnimation(false, (long)this);
    CModeBase::OnDeactivate();
    CTCFlagged* flagged;
    if (ParentThing->GetInterface(&flagged, 0x3a))
        flagged->m_flag = 1;
}