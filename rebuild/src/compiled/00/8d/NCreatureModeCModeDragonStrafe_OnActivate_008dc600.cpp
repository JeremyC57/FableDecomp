// NCreatureMode::CModeDragonStrafe::OnActivate() -- base activation, suspend a player-type dragon's animation for this mode, and clear bit 0 of the interface-0x3a component's flag byte
// Leaf: ?OnActivate@CModeDragonStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC600, 85 bytes (+ int3 padding), `ret`; vtable 0x012686C8 slot 5.
// Callees: CModeBase::OnActivate @ 0x0073AAC0 (qualified), CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10
//   (the mode's address is the suspend reason), CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface,
//   type 0x3a = bit 26 of the +0x24 dword).
// Name: FableWin.pdb `?OnActivate@CModeDragonStrafe@NCreatureMode@@UAEXXZ` (slot 5).
// Shape: as the landed CModeBriarRoseTeleportIntangible::OnActivate @ 0x008E7B90 (same type-0x3a flag clear through a one-bit
//   bitfield: `and byte ptr [eax+50h],0FEh`); ParentThing is re-read for each use.
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
    void OnActivate();                     // 0x0073AAC0
};

namespace NCreatureMode {
struct CModeDragonStrafe : CModeBase {     // vtable 0x012686C8 (CModeDragonFlyingBase members from +0x1c)
    void OnActivate();
};
}

void NCreatureMode::CModeDragonStrafe::OnActivate()
{
    CModeBase::OnActivate();
    if (ParentThing->m_type == 1)
        ParentThing->SetSuspendAnimation(true, (long)this);
    CTCFlagged* flagged;
    if (ParentThing->GetInterface(&flagged, 0x3a))
        flagged->m_flag = 0;
}