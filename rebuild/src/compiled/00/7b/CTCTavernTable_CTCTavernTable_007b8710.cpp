// CTCTavernTable::CTCTavernTable(CThing* owner) -- tavern-table component constructor: CTCBase(owner), empty occupant/tankard/food/seat containers, cleared flags, available for use, and the lengths of the five action-point name prefixes
// Leaf: ??0CTCTavernTable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B8710, 303 bytes (`ret 4`). Vtable 0x0126DBB4 (CTCTavernTable).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; CCharString(const char*, long) 0x0099EBF0 ("ACTION_POINT_SIT_" 0x0126DB98,
//          "ACTION_POINT_GAME_MASTER_" 0x0126DB7C, "ACTION_POINT_HERO_" 0x0126DB68, "DUMMY_POINT_TANKARD_" 0x0126DB50,
//          "ACTION_POINT_MAID_" 0x0126DB3C); ~CCharString 0x0099EAE0.
// Layout (retail; Ego_r.pdb names, STLport containers 12 bytes, vector<bool> included): BeingServed +0x0c, Occupants
//          +0x10, Tankards +0x1c, Food +0x28, SeatsCurrentlyUsed +0x34, NumPeopleSeated +0x40, SomeoneTalking +0x44,
//          SomeoneLaughing +0x45, ActionPoint{Sit,GameMaster,Hero,Tankard,Maid}Strlen +0x48..+0x58, AvailableForUse +0x5c.
// Shape: the containers are member objects with inline zeroing ctors, so their stores follow the vtable store in member
//        order and share the ebx zero; each Strlen is the inline length of a temporary CCharString (`data ? data->Length
//        : 0`), the temporary destroyed right after; VC duplicates the epilogue into both arms of the last length test.
class CThing;
struct CThingAICreature;

struct CCharStringData {
    char* Text;                                 // +0x00
    long  Length;                               // +0x04
};
struct CCharString {
    CCharStringData* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    long GetLength() const { return m_data == 0 ? 0 : m_data->Length; }
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* owner);                     // 0x00686800
};

template<class T> struct CRetailVector {        // retail STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CIntelligentPointerSlot { void* _vfptr; void* m_pItem; };   // CIntelligentPointer<CThingObject> element

struct CTCTavernTable : CTCBase {               // vtable 0x0126DBB4
    bool  BeingServed;                          // +0x0c
    CRetailVector<CThingAICreature*>        Occupants;    // +0x10
    CRetailVector<CIntelligentPointerSlot>  Tankards;     // +0x1c
    CRetailVector<CIntelligentPointerSlot>  Food;         // +0x28
    CRetailVector<unsigned long>            SeatsCurrentlyUsed;   // +0x34 (vector<bool>)
    long  NumPeopleSeated;                      // +0x40
    bool  SomeoneTalking;                       // +0x44
    bool  SomeoneLaughing;                      // +0x45
    long  ActionPointSitStrlen;                 // +0x48
    long  ActionPointGameMasterStrlen;          // +0x4c
    long  ActionPointHeroStrlen;                // +0x50
    long  ActionPointTankardStrlen;             // +0x54
    long  ActionPointMaidStrlen;                // +0x58
    bool  AvailableForUse;                      // +0x5c
    virtual void v00();
    CTCTavernTable(CThing* owner);
};

CTCTavernTable::CTCTavernTable(CThing* owner)
    : CTCBase(owner), BeingServed(false), NumPeopleSeated(0), SomeoneTalking(false), SomeoneLaughing(false),
      AvailableForUse(true)
{
    ActionPointSitStrlen = CCharString("ACTION_POINT_SIT_").GetLength();
    ActionPointGameMasterStrlen = CCharString("ACTION_POINT_GAME_MASTER_").GetLength();
    ActionPointHeroStrlen = CCharString("ACTION_POINT_HERO_").GetLength();
    ActionPointTankardStrlen = CCharString("DUMMY_POINT_TANKARD_").GetLength();
    ActionPointMaidStrlen = CCharString("ACTION_POINT_MAID_").GetLength();
}