// CTCKraken::CreateTentacles() -- unless the game is loading, spawns one tentacle creature (the current phase's tentacle def) per kraken carry slot: each is linked back to the kraken, kept out of the save, made non-persisting, faced along its slot's angle at the slot's dummy, and appended to the tentacle list; the attacking-tentacle cursor is reset
// Grade: DIFFER(1245v1245) -- same length and instruction stream; the frame is 8 bytes larger (0x70 v 0x68: retail lets the loop index share the push_back name-string slot and the slot-entry pair share the list-element temporary, ours gives them their own words) and `this` / the zero register swap (retail ebx = this, ebp = 0, so the init bools are immediates; ours ebp = this, ebx = 0 and `mov byte ptr [..],bl`); a {} block around the slot pushes was inert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FC210, 1245 bytes, plain `ret`; 8-aligned ebp frame.
// Callees: CTCBase::GetGameSubsystem 0x00686D60 (+ its slot 9, a "loading" query; name descriptive),
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410 /
//   GetDef<CCarrySlotDef> 0x006A3060, vector<pair<long,float> >::_M_insert_overflow 0x006B5BE0,
//   CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CThing::GetPosition 0x004C73D0,
//   CThingAICreature::Create 0x00833800 (`__fastcall`, the init a default-argument temporary), CVectorMap LowerBound
//   0x0040F020 (types 0xE3 / 9), CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   ~ 0x00A01C10 (vtable 0x01238C6C), CThing::SetAsGameStatePersisted 0x004C7880, CDefStringTable::GetString 0x009D49B0
//   (g_DefStringTable 0x013CA828), CTCKrakenTentacle::SetFacingAngleAndDummy 0x007F15D0, malloc 0x00BFEA0E / free
//   0x00BFEA14, the creature's slot 28 (+0x70).
// Shape: the carry-slot table is a local STLport vector filled by four inline push_backs (the first one goes straight to
//   the grow path: the vector was just zeroed); the tentacle component and the slot-9 component are function-scope
//   locals assigned only when present (the absent path reuses the previous value); the kraken link and the list
//   element are CIntelligentPointer<CThing> temporaries (the 8-byte loop temporary aligns the frame).
// Layout: CTCKraken retail (4-byte STLport lists): PKrakenDef +0x0C, Tentacles +0x44, AttackingTentacleIterator +0x48,
//   Phase +0x60; CKrakenDef TentacleDefIndex vector +0x6C; CThing bitset +0x20, player byte +0x90, flags +0x91 / +0x15C;
//   CTCKrakenTentacle PKraken +0x10; CCarrySlotDef DummyPosName +0x28.
struct CTCBase;

extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefStringTable { CCharString GetString(long index); };   // 0x009D49B0
extern CDefStringTable g_DefStringTable;               // 0x013CA828
struct C3DVector { float X, Y, Z; };

struct __true_type { __true_type() {} };
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}
typedef std::pair<long, float> CSlotEntry;
struct CSlotVector {                                    // STLport vector<pair<long,float> > (12 bytes)
    CSlotEntry* _M_start;
    CSlotEntry* _M_finish;
    CSlotEntry* _M_end_of_storage;
    CSlotVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CSlotVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CSlotEntry& operator[](unsigned long n) { return _M_start[n]; }
    void _M_insert_overflow(CSlotEntry* position, const CSlotEntry& x, const __true_type&, unsigned long fillLength,
                            bool atEnd);   // 0x006B5BE0
    void push_back(const CSlotEntry& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CSlotEntry(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
// Same-TU stand-in for the grow path 0x006B5BE0 (the real body reallocates): it reads the value and writes only the
// vector's own words, which tells VC7.1 the vector's address does not escape (finish stays in a register, the first
// push_back on the fresh vector goes straight to the grow call).
#pragma auto_inline(off)
void CSlotVector::_M_insert_overflow(CSlotEntry* position, const CSlotEntry& x, const __true_type&, unsigned long fillLength,
                                     bool atEnd)
{
    if (x.first == 0)
        return;
    _M_start = position;
    _M_finish = position + fillLength;
    if (atEnd)
        _M_end_of_storage = _M_finish;
}
#pragma auto_inline(on)

struct CBaseObject {                                    // polymorphic root of things (vptr at +0)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetDrawable(bool drawable);            // slot 28 (+0x70), name descriptive
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* PItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CBaseObject* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing : CBaseObject {
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x90 - 0x4c];
    signed char   Player;                               // +0x90
    unsigned char m_flags91;                            // +0x91
    unsigned char _pad92[0x15c - 0x92];
    unsigned char m_flags15c;                           // +0x15C
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long type, T*)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CThingPhysical;
struct CThingBuilding;
struct CThingAICreatureInit {                           // Ego_r.pdb, 28 bytes
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString ScriptName;                             // +0x08
    CCharString OverridingBrainName;                    // +0x0C
    bool PlayerCharacter;                               // +0x10
    bool Drawable;                                      // +0x11
    bool PlaceAtEndOfList;                              // +0x12
    bool EnableCreatureAutoPlacing;                     // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    CThingAICreatureInit(const CCharString& scriptName = "", const CCharString& brainName = "")
        : OriginalThing(0), PreviousThing(0), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(false), Drawable(true), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};
struct CThingAICreature : CThing {
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                               const CThingAICreatureInit& init = CThingAICreatureInit());   // 0x00833800
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CCarrySlotDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long DummyPosName;                                  // +0x28 CDefString (string-table index)
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);     // 0x009AD410
    bool GetDef(long index, CDefPointer<CCarrySlotDef>* out);    // 0x006A3060
};
struct CLongVector {                                    // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long n) { return _M_start[n]; }
};
struct CKrakenDef {
    unsigned char _pad00[0x6c];
    CLongVector   TentacleDefIndex;                     // +0x6C
};
struct CGameSubsystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsLoading();                           // slot 9 (+0x24), name descriptive
};

struct CTCKrakenTentacle {                              // interface 0xE3
    unsigned char _pad00[0x10];
    CIntelligentPointer<CThing> PKraken;                // +0x10
    void SetFacingAngleAndDummy(float facingAngle, const CCharString& dummyName);   // 0x007F15D0
};
struct CTCCollidable {                                  // interface 9 (name descriptive)
    unsigned char _pad00[0x10];
    bool Active;                                        // +0x10
};

struct SListNode {                                      // STLport _List_node<CIntelligentPointer<CThing> >
    SListNode* _M_next;
    SListNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;
};
struct CThingList {                                     // STLport list (retail: the node pointer only)
    SListNode* _M_node;
    void push_back(const CIntelligentPointer<CThing>& x)
    {
        SListNode* position = _M_node;
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        new (&node->_M_data) CIntelligentPointer<CThing>(x);
        node->_M_next = position;
        node->_M_prev = position->_M_prev;
        position->_M_prev->_M_next = node;
        position->_M_prev = node;
    }
};

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct CTCKraken {
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[4];
    CKrakenDef*   PKrakenDef;                           // +0x0C CDefPointer<const CKrakenDef>
    unsigned char _pad10[0x44 - 0x10];
    CThingList    Tentacles;                            // +0x44
    SListNode*    AttackingTentacleIterator;            // +0x48
    unsigned char _pad4c[0x60 - 0x4c];
    unsigned long Phase;                                // +0x60 EKrakenBossPhase
    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60 (CTCBase)
    CDefinitionManager* GetDefinitionManager();         // 0x006869D0 (CTCBase)
    void CreateTentacles();
};

void CTCKraken::CreateTentacles()
{
    if (GetGameSubsystem()->IsLoading())
        return;
    CDefinitionManager* definitions = GetDefinitionManager();
    CSlotVector slots;
    slots.push_back(CSlotEntry(definitions->GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_01"), 0.375f));
    slots.push_back(CSlotEntry(definitions->GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_02"), 0.625f));
    slots.push_back(CSlotEntry(definitions->GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_03"), 0.875f));
    slots.push_back(CSlotEntry(definitions->GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_04"), 0.125f));
    long tentacleDef = PKrakenDef->TentacleDefIndex[MIN(PKrakenDef->TentacleDefIndex.size() - 1, Phase)];
    CTCKrakenTentacle* tentacleTC;
    CTCCollidable* collidable;
    for (unsigned long i = 0; i < slots.size(); ++i) {
        CThingAICreature* tentacle = CThingAICreature::Create(tentacleDef, ParentThing->GetPosition(), ParentThing->Player);
        if (tentacle->HasTC(0xe3))
            tentacleTC = tentacle->GetTC(0xe3, (CTCKrakenTentacle*)0);
        tentacleTC->PKraken = ParentThing;
        tentacle->SetAsGameStatePersisted(false);
        tentacle->m_flags91 |= 2;
        tentacle->m_flags15c &= ~0x20;
        if (tentacle->HasTC(9))
            collidable = tentacle->GetTC(9, (CTCCollidable*)0);
        collidable->Active = false;
        CDefPointer<CCarrySlotDef> slotDef;
        definitions->GetDef(slots[i].first, &slotDef);
        tentacleTC->SetFacingAngleAndDummy(slots[i].second, g_DefStringTable.GetString(slotDef->DummyPosName));
        tentacle->SetDrawable(false);
        Tentacles.push_back(CIntelligentPointer<CThing>(tentacle));
    }
    if (ParentThing->HasTC(9))
        collidable = ParentThing->GetTC(9, (CTCCollidable*)0);
    collidable->Active = false;
    AttackingTentacleIterator = Tentacles._M_node;
}
