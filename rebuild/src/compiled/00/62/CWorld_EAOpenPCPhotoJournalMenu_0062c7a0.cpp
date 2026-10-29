// CWorld::EAOpenPCPhotoJournalMenu(const CGameEvent* event) -- game-event action: if the event player's hero has the CTCInventory interface (TC 0x11), call its OpenPCPhotojournal()
// Leaf: ?EAOpenPCPhotoJournalMenu@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062C7A0, 78 bytes (+ int3 padding to 80), thiscall `ret 4`.
// Identity (proven): CWorld::EAOpenPCPhotoJournalMenu; evidence: its only reference is a store into the game-event handler table by the static initialiser 0x0121B9B0 (no direct calls); it calls 0x0058F9BC on the hero's TC 0x11 component -- the CTCInventory menu builder whose strings are PC_PHOTOJOURNAL_LIST / PC_UI_PHOTOJOURNAL_FRAME / PC_PHOTOJOURNAL_REVIEW (Ego_r.pdb CTCInventory::OpenPCPhotojournal), and the PDB's only photo-journal event action is ?EAOpenPCPhotoJournalMenu@CWorld@@QAEXPBVCGameEvent@@@Z
// Label: the manifest's "sub_0062c7a0" is a gap-scan placeholder (family H087: six one-call CWorld event actions, each its own
//   function, registered by address in the game-event handler table that the static initialiser 0x0121B9B0 fills).
// Callees: CWorld::GetPlayer(long) 0x0049D850, CPlayer::GetPControlledCreature 0x00487DC0, the interface-map LowerBound 0x0040F020,
//   CTCInventory::OpenPCPhotojournal 0x0058F9BC.
// Layout (retail CThing): interface bitset +0x20 (TC 0x11 -> word 0 mask 0x20000), interface map +0x44 (the landed
//   CWorld::EARemoveCurrentMode 0x0062C4B0 model). Local classes: the generated engine headers declare CGameEvent as a
//   `struct`, but the PDB symbol mangles it `V` (class) -- Review at landing.
// Shape: no null test on the hero; the GetTC key lives in the dead `event` argument slot.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                 // +0x00
    SEntry* last;                                  // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
};
class CThingPlayerCreature : public CThing { };
class CGameEvent {                                 // 0x28 bytes (Ego_r.pdb)
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
};
class CTCInventory {                                  // TC 0x11
public:
    void OpenPCPhotojournal();                             // 0x0058F9BC
};
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
class CWorld {
public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAOpenPCPhotoJournalMenu(const CGameEvent* event);
};

void CWorld::EAOpenPCPhotoJournalMenu(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (hero->HasTC(0x11))
        hero->GetTC<CTCInventory >(0x11)->OpenPCPhotojournal();
}