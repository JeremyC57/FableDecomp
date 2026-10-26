// NCreatureMode::CModeStunned::LeafInit() -- on entering the stun, set the stunned creature's targeting flags to 4 and take it out of combat
// Leaf: ?LeafInit@CModeStunned
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8FE0, 126 bytes (+ int3 padding), `ret`; CModeStunned vtable slot 12.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (two inlined lookups: type 9 = bit 9 of the +0x20 dword, type 0x30 = bit 16
//   of the +0x24 dword = CTCCombat), the type-9 component's flag setter @ 0x007EA270 (as the landed
//   CModeOutOfActionBase::OnDeactivate 0x008D8F30; manifest "CObservable::AddExclusiveObserver" is propagated),
//   CTCCombat::RemoveFromCombat @ 0x00733580 (landed).
// Name: FableWin.pdb `?LeafInit@CModeStunned@NCreatureMode@@EAEXXZ` (slot 12 of the out-of-action modes).
// Shape: the first lookup is the out-parameter GetInterface (the component lands as `mov eax,[eax+4] ; mov ecx,eax`), the
//   second HasTC + a returned GetTC (`mov ecx,[eax+4]`); both keys share one frame word; ParentThing is re-read.
class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

class CThing { public:
    unsigned char _pad00[0x20];
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
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
};

class CTCTargeted { public:                  // the type-9 component: a flags word at +0xc with an out-of-line setter
    unsigned char _pad00[0x0c];
    unsigned long m_flags;                 // +0x0c
public:
    void SetTargetingFlags(unsigned long);   // 0x007EA270
};

class CTCCombat { public:                         // interface 0x30
    void RemoveFromCombat();               // 0x00733580
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {
struct CModeStunned : CModeBase {
    void LeafInit();
};
}

void NCreatureMode::CModeStunned::LeafInit()
{
    CTCTargeted* targeted;
    if (ParentThing->GetInterface(&targeted, 9))
        targeted->SetTargetingFlags(4);
    if (ParentThing->HasTC(0x30))
        ParentThing->GetTC<CTCCombat>(0x30)->RemoveFromCombat();
}