// CAIStateGroup_MinionListenToStory::SitDown() -- pick a random sit animation through a sit filter, build a listen-sit mode named after the animation's group (wrapped in a counted pointer) and perform the sit-down action with it; true when the action finished
// Leaf: ?SitDown@CAIStateGroup_MinionListenToStory@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F830, 390 bytes, plain `ret`.
// Callees: CThingCreatureBase::GetRandomAnimFromNameWithFilter @ 0x00662EF0 (the global sit name @ 0x013BB66C, a filter
//          object with vtable 0x0127261C / base 0x0122FFCC), operator new @ 0x00BFEA1A, CDefStringTable::GetString
//          @ 0x009D49B0 (the table @ 0x013CA828), CModeSitBase ctor @ 0x008D7CA0 (two vtables 0x01267BBC / 0x01267568),
//          CCharString default ctor @ 0x0099E4B0 / operator= @ 0x0099EFB0 / dtor @ 0x0099EAE0, the name hash @ 0x004014A0
//          (__fastcall), the counted pointer's deleter @ 0x008AA810, CAIStateGroupBase::IsStopping @ 0x008FD010,
//          CCreatureAction_SitDown ctor @ 0x008AB9D0 / dtor @ 0x008ABAA0, AIAction_PerformAction @ 0x0097D8B0,
//          operator delete @ 0x00BFE9BC.
// Shape: the filter temporary is vtable-only; the mode is `new CModeSitListen(this, anim->GroupName)` -- the group name
//          converts through CDefString's inline operator CCharString (the conditional temporary's flag is set after the
//          call), the ctor fetches the creature itself (loaded after GetString) and builds its CAnimFilterOfGroup member at
//          +0x2c (string assign, one-test CRC over the temporary); the counted pointer is the landed `Object(object)`,
//          `if/else` block form (new re-reading Object), passed by value to the action ctor.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04).
struct CCharStringData { const char* Chars; long Length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
struct CDefStringTable {
    CCharString GetString(long index) const;    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
struct CDefString {
    long Index;
    operator CCharString() const { return g_DefStringTable.GetString(Index); }
};
struct CAnimationEntry {
    long BankIndex;                             // +0x00
    CDefString AnimName;                        // +0x04
    CDefString GroupName;                       // +0x08
};
extern CCharString g_SitAnimName;               // 0x013BB66C
struct CAnimFilterBase {
    virtual void v00();                         // vtable 0x0122FFCC
    ~CAnimFilterBase() { }
};
struct CAnimFilterSit : CAnimFilterBase {       // vtable 0x0127261C (descriptive; vtable only)
    virtual void v00();
};
struct CThing {
    CAnimationEntry* GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter);   // 0x00662EF0
};
struct CModeSitBase {
    virtual void v00();
    unsigned char _pad[0x28];
    CModeSitBase(CThing* creature);             // 0x008D7CA0
};
struct CAnimFilterOfGroup {                     // vtable 0x01267568
    virtual void v00();
    CCharString Group;                          // +0x04
    unsigned long GroupCRC;                     // +0x08
    CAnimFilterOfGroup(const CCharString& group)
    {
        Group = group;
        const char* data;
        unsigned long length;
        if (group.PStringData == 0) {
            length = 0;
            data = "";                          // 0x0129AAF4
        }
        else {
            length = group.PStringData->Length;
            data = group.PStringData->Chars;
        }
        GroupCRC = Calc_004014a0(0, data, length);
    }
};
struct CAIStateGroupBase;
struct CModeSitListen : CModeSitBase {          // 0x38 bytes
    CAnimFilterOfGroup Filter;                  // +0x2c
    __forceinline CModeSitListen(const CAIStateGroupBase* group, const CCharString& anim);
    virtual void v00();
};
void __fastcall DeleteSitMode(void* object);   // 0x008AA810
struct CCountBlock {
    long Count;
    void (__fastcall* Deleter)(void*);
    void* Object;
    CCountBlock(void* object) { Object = object; Count = 1; Deleter = DeleteSitMode; }
};
template <class T> struct CCountedPointer {
    T* Object;
    CCountBlock* Block;
    CCountedPointer(T* object) : Object(object)
    {
        if (object)
            Block = new CCountBlock(Object);
        else
            Block = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Block(other.Block)
    {
        if (Block)
            ++Block->Count;
    }
    ~CCountedPointer()
    {
        if (Block && --Block->Count == 0) {
            Block->Deleter(Block->Object);
            delete Block;
        }
    }
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    unsigned char _own[0x08];
    CCreatureAction_SitDown(CThing* creature, CAnimationEntry* anim, CCountedPointer<CModeSitBase> mode);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                 // 0x008ABAA0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
CModeSitListen::CModeSitListen(const CAIStateGroupBase* group, const CCharString& anim)
    : CModeSitBase(group->GetCreature()), Filter(anim)
{
}
class CAIStateGroup_MinionListenToStory : public CAIStateGroupBase {
protected:
    bool SitDown();
};

bool CAIStateGroup_MinionListenToStory::SitDown()
{
    CAnimationEntry* anim = GetCreature()->GetRandomAnimFromNameWithFilter(g_SitAnimName, CAnimFilterSit());
    CCountedPointer<CModeSitBase> mode(new CModeSitListen(this, anim->GroupName));
    if (IsStopping())
        return false;
    if (AIAction_PerformAction(&CCreatureAction_SitDown(GetCreature(), anim, mode)) != 2)
        return false;
    return true;
}