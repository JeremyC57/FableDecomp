// CTCActivationReceptorChest::Activate() -- the base receptor Activate, then Open the thing's chest component (interface 0x3c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFC70, length 85 bytes.
// Leaf: ?Activate@CTCActivationReceptorChest
// Callees: CTCActivationReceptorBase::Activate @ 0x007F76E0 (non-virtual base call; the manifest's labels for 0x007F7700
// are propagated), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCChest::Open @ 0x00750540.
// Ego_r.pdb: `?Activate@CTCActivationReceptorChest@@UAEXXZ` (slot 28; the chunk's "vfunc_28").
// Interface 0x3c = word 1, bit 28 of the presence bitset (`test dword ptr [eax+24h],10000000h`).
// Shape: the landed out-parameter GetInterface template (0x007504E0 / 0x0069F750 family): the presence bitset word
// test, LowerBound on the +0x44 map with the key in the `push ecx` frame word, end-or-larger-key -> the end entry, the
// component left in eax. The out pointer is NOT initialised: on the absent path retail reads the uninitialised local
// (`mov eax,[esp+N]`) and still makes the call, which the same source reproduces.
struct CTCBaseInterface;
struct CInterfaceMap {                         // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];            // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap m_interfaces;                // +0x44 Components
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

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

class CTCActivationReceptorBase : public CTCBase { public:   // retail 0x2c bytes (see 0x007F7710; engine header has the PDB layout)
    unsigned char _body[0x20];                 // +0x0c Activated .. +0x28 FrameAtLastActivation
    virtual void Activate();                   // slot 28: 0x007F76E0
    virtual void Deactivate();                 // slot 29: 0x007F7700
};

class CTCChest { public:
    void Open();                               // 0x00750540
    void Close();                              // 0x007506C0
};

struct CTCActivationReceptorChest : CTCActivationReceptorBase {
    virtual void Activate();
    virtual void Deactivate();
};

void CTCActivationReceptorChest::Activate()
{
    CTCActivationReceptorBase::Activate();
    CTCChest* chest;
    ParentThing->GetInterface(&chest, 0x3c);
    chest->Open();
}