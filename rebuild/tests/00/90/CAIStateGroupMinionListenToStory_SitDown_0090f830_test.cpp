// Behaviour test for 0090f830: src/0090f830.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_MinionListenToStory::SitDown()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CCharStringData { public: const char* Chars; long Length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
struct CDefStringTable {
    CCharString GetString(long index) const;    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
class CDefString { public:
    long Index;
    operator CCharString() const { return g_DefStringTable.GetString(Index); }
};
class CAnimationEntry { public:
    long BankIndex;                             // +0x00
    CDefString AnimName;                        // +0x04
    CDefString GroupName;                       // +0x08
};
extern CCharString g_SitAnimName;               // 0x013BB66C
class CAnimFilterBase { public:
    virtual void v00();                         // vtable 0x0122FFCC
    ~CAnimFilterBase() { }
};
struct CAnimFilterSit : CAnimFilterBase {       // vtable 0x0127261C (descriptive; vtable only)
    virtual void v00();
};
class CThing { public:
    CAnimationEntry* GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter);   // 0x00662EF0
};
struct CModeSitBase {
    virtual void v00();
    unsigned char _pad[0x28];
    CModeSitBase(CThing* creature);             // 0x008D7CA0
};
class CAnimFilterOfGroup { public:                     // vtable 0x01267568
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
class CAIStateGroupBase;
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
template <class T> class CCountedPointer { public:
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
class CCreatureActionBase { public:
    void* __vftable;
    unsigned char _body[0xa4];
};
class CCreatureAction_SitDown : public CCreatureActionBase { public:
    unsigned char _own[0x08];
    CCreatureAction_SitDown(CThing* creature, CAnimationEntry* anim, CCountedPointer<CModeSitBase> mode);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                 // 0x008ABAA0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
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

// ---- stubs and driver ----
// ---- strings: the data blocks live in a static pool; the dtor only counts (shallow implicit copies are harmless) ----
static int strCtors, strDtors, strAssigns;
static CCharStringData pool[8];
static int poolUsed;
CCharString::CCharString() : PStringData(0) { ++strCtors; }
CCharString::~CCharString() { ++strDtors; }
const CCharString& CCharString::operator=(const CCharString& other) { ++strAssigns; PStringData = other.PStringData; return *this; }
static unsigned long crcSeed, crcLength; static const void* crcData; static int crcCalls;
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length)
{
    ++crcCalls; crcSeed = seed; crcData = data; crcLength = length;
    unsigned long h = 0x1234;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + ((const unsigned char*)data)[i];
    return h;
}
CDefStringTable g_DefStringTable;
CCharString g_SitAnimName;
static long tableIndex; static int tableCalls; static bool tableEmpty;
CCharString CDefStringTable::GetString(long index) const
{
    ++tableCalls; tableIndex = index;
    CCharString s;
    if (!tableEmpty) {
        CCharStringData* d = &pool[poolUsed++ & 7];
        d->Chars = "SIT_GROUP"; d->Length = 9;
        s.PStringData = d;
    }
    --strDtors;    // compensates the local's dtor (VC7.1 has no NRVO: the result is a copy)
    return s;
}
void CAnimFilterBase::v00() { }
void CAnimFilterSit::v00() { }
void CAnimFilterOfGroup::v00() { }
static CAnimationEntry entry;
static const CCharString* animName; static const CAnimFilterBase* animFilter; static CThing* animThing;
CAnimationEntry* CThing::GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter)
{
    animThing = this; animName = &name; animFilter = &filter;
    return &entry;
}
static CThing* baseCreature; static int baseCtors;
CModeSitBase::CModeSitBase(CThing* creature) { ++baseCtors; baseCreature = creature; }
void CModeSitBase::v00() { }
void CModeSitListen::v00() { }
static void* deletedMode; static int deletes;
void __fastcall DeleteSitMode(void* object) { ++deletes; deletedMode = object; }
static CModeSitBase* actionMode; static long actionCount; static CThing* actionCreature; static CAnimationEntry* actionAnim;
static int actionCtors, actionDtors;
CCreatureAction_SitDown::CCreatureAction_SitDown(CThing* creature, CAnimationEntry* anim, CCountedPointer<CModeSitBase> mode)
{
    ++actionCtors; actionCreature = creature; actionAnim = anim; actionMode = mode.Object;
    actionCount = mode.Block ? mode.Block->Count : -1;
}
CCreatureAction_SitDown::~CCreatureAction_SitDown() { ++actionDtors; }
CAIStateGroupBase::~CAIStateGroupBase() { }
static bool stopping; static long performResult; static CCreatureActionBase* performed; static int performCalls;
bool CAIStateGroupBase::IsStopping() { return stopping; }
long CAIStateGroupBase::AIAction_PerformAction(CCreatureActionBase* action) { ++performCalls; performed = action; return performResult; }
struct CTestGroup : CAIStateGroup_MinionListenToStory {
    bool Run() { return SitDown(); }
};
static void Reset(bool stop, long result, bool empty)
{
    stopping = stop; performResult = result; tableEmpty = empty;
    strCtors = strDtors = strAssigns = 0; crcCalls = 0; tableCalls = 0; baseCtors = 0; deletes = 0; deletedMode = 0;
    actionCtors = actionDtors = 0; performCalls = 0; actionMode = 0; actionCount = 0;
}
int main()
{
    static CThing creature;
    static CAIBrain brain;
    brain.PCreature = &creature;
    CTestGroup group;
    group.PBrain = &brain;
    entry.GroupName.Index = 77;
    // performed, finished
    Reset(false, 2, false);
    CHECK(group.Run() == true);
    CHECK(animThing == &creature && animName == &g_SitAnimName && animFilter != 0);
    CHECK(tableCalls == 1 && tableIndex == 77);
    CHECK(baseCtors == 1 && baseCreature == &creature);
    CHECK(strAssigns == 1 && crcCalls == 1 && crcSeed == 0 && crcLength == 9 && memcmp(crcData, "SIT_GROUP", 9) == 0);
    CHECK(actionCtors == 1 && actionDtors == 1 && performCalls == 1 && actionCreature == &creature && actionAnim == &entry);
    CHECK(actionMode != 0 && actionCount == 2);
    CHECK(((CModeSitListen*)actionMode)->Filter.GroupCRC != 0 && ((CModeSitListen*)actionMode)->Filter.Group.PStringData != 0);
    CHECK(deletes == 1 && deletedMode == actionMode);
    CHECK(strCtors == strDtors + 1);   // the mode's Group string is never destroyed (the deleter is a stub)
    // performed, not finished
    Reset(false, 1, false);
    CHECK(group.Run() == false);
    CHECK(performCalls == 1 && actionDtors == 1 && deletes == 1);
    // stopping: no action, the mode is still released
    Reset(true, 2, false);
    CHECK(group.Run() == false);
    CHECK(performCalls == 0 && actionCtors == 0 && deletes == 1 && tableCalls == 1);
    // empty group name: CRC over the empty literal
    Reset(false, 2, true);
    CHECK(group.Run() == true);
    CHECK(crcCalls == 1 && crcLength == 0 && crcData != 0 && *(const char*)crcData == 0);
    char buf[64];
    _snprintf(buf, sizeof(buf), "%s", fails == 0 ? "0090f830_TEST PASS" : "0090f830_TEST FAIL");
    printf("%s\n", buf);
    return fails;
}
