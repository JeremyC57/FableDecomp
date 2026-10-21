// Behaviour test for 00e5f6b0: src/00e5f6b0.cpp copied verbatim; the script interface, IsDone, the distance test, the
// comment call and the string / script-thing members are stubs that record their calls.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CHenchmanRegionEntranceComment::Main() -- collect every "BodyGuard" thing; walk them and, for the first one within 10 units of this marker, ask the bodyguard quest to make region-entrance comment 2 (priority 2) with this marker's data string, stopping once a comment was made
// Grade: RELOCATION_MATCH -- the vector's operator[] goes through an inline begin() (`*(begin() + n)`), which gives retail's element-address registers (offset in edx, start in eax) and Me's vtable in edx
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5F6B0, length 335 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("BodyGuard" @ 0x012CB3F0), IsDone
//   @ 0x00F35B30, IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), CBaseClass ctor @ 0x0099A2D0 (the default
//   CScriptThing temporary: vtable 0x01238C8C, PImp zeroed), ~CScriptThing @ 0x004AA840, CScriptThing slot 3
//   GetDataString (CCharString by value), CV_BodyGuardScript::MakeHenchmanComment(const CCharString&, long,
//   EHENCHMAN_COMMENT, const CScriptThing&) @ 0x00E5EBD0; CGameScriptInterface (vtable 0x01260F0C) slot 75
//   GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>&).
// Layout: Ego_r.pdb CHenchmanRegionEntranceComment (44 bytes): entity-script base {vptr, Game +4, CScriptThing Me +8},
//   ParentClass (CV_BodyGuardScript*) +0x14, PMasterData +0x18, HenchmanVector (STLport vector<CScriptThing>) +0x1C;
//   CScriptThing = engine/CScriptThing.h {vptr, PImp {Data, Info}} over CBaseClass, modelled locally for its virtuals.
// Shape: `size() == 0` exits through a duplicated `if (IsDone()) return;` tail; the walk is `for (unsigned i ...)` with
//   the element offset strength-reduced into a frame word; the comment result survives the temporaries in bl.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
struct CBaseClass {                               // vtable 0x01231710
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
};
struct CCPPointerInfo;
struct CScriptThing : CBaseClass {                 // vtable 0x01238C8C, 12 bytes
    virtual void v001(); virtual void v002();
    virtual CCharString GetDataString() const;     // slot 3 (+0x0c)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    CScriptThing() : PImp_Data(0), PImp_Info(0) { }
    ~CScriptThing();                               // 0x004AA840
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const; // slot 75 (+0x12c)
};
enum EHENCHMAN_COMMENT { HENCHMAN_COMMENT_REGION_ENTRANCE = 2 };
struct CV_BodyGuardScript {
    bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT comment, const CScriptThing& about = CScriptThing());   // 0x00E5EBD0
};
struct CQ_SunnyvaleMasterData;

class CHenchmanRegionEntranceComment {
public:
    void*                      __vftable;          // +0x00
    CGameScriptInterface*      Game;               // +0x04
    CScriptThing               Me;                 // +0x08
    CV_BodyGuardScript*        ParentClass;        // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;        // +0x18
    std::vector<CScriptThing>  HenchmanVector;     // +0x1C
    bool IsDone();                                 // 0x00F35B30
    __forceinline void WalkHenchmen()
    {
        for (unsigned int i = 0; i < HenchmanVector.size(); ++i) {
            if (IsDone())
                return;
            if (IsDistanceBetweenThingsUnder(Me, HenchmanVector[i], 10.0f)) {
                if (IsDone())
                    return;
                if (ParentClass->MakeHenchmanComment(Me.GetDataString(), 2, HENCHMAN_COMMENT_REGION_ENTRANCE))
                    break;
            }
        }
        IsDone();
    }
    void Main();
};

void CHenchmanRegionEntranceComment::Main()
{
    Game->GetAllThingsWithScriptName("BodyGuard", HenchmanVector);
    if (HenchmanVector.size() == 0) {
        IsDone();
        return;
    }
    WalkHenchmen();
}

// ---- stubs ----
static int isDoneCalls, isDoneTrueAt = -1, distCalls, commentCalls, getAllCalls, nearMask, commentTrueAt = -1;
static char getAllName[32]; static CScriptThing things[8]; static int thingCount;
static const CScriptThing* distB[16]; static const CScriptThing* distA[16]; static float distD[16];
static int commentIdx[16]; static long commentPrio; static EHENCHMAN_COMMENT commentKind; static int strDtors, strCtors;
CCharString::CCharString(const char* text, long length) { ++strCtors; m_pData = (void*)text; CHECK(length == -1); }
CCharString::CCharString(const CCharString& other) { ++strCtors; m_pData = other.m_pData; }
CCharString::~CCharString() { ++strDtors; }
void CBaseClass::v000() { }
CBaseClass::CBaseClass() { }
static const char* dataStr = "MARKER_DATA";
CCharString CScriptThing::GetDataString() const { return CCharString(dataStr); }
CScriptThing::~CScriptThing() { }
void CScriptThing::v001() { }
void CScriptThing::v002() { }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{
    distA[distCalls] = &a; distB[distCalls] = &b; distD[distCalls] = distance;
    int idx = (int)(&b - things); ++distCalls;
    return (nearMask >> idx) & 1;
}
void CGameScriptInterface::v000() { }
void CGameScriptInterface::v001() { }
void CGameScriptInterface::v002() { }
void CGameScriptInterface::v003() { }
void CGameScriptInterface::v004() { }
void CGameScriptInterface::v005() { }
void CGameScriptInterface::v006() { }
void CGameScriptInterface::v007() { }
void CGameScriptInterface::v008() { }
void CGameScriptInterface::v009() { }
void CGameScriptInterface::v010() { }
void CGameScriptInterface::v011() { }
void CGameScriptInterface::v012() { }
void CGameScriptInterface::v013() { }
void CGameScriptInterface::v014() { }
void CGameScriptInterface::v015() { }
void CGameScriptInterface::v016() { }
void CGameScriptInterface::v017() { }
void CGameScriptInterface::v018() { }
void CGameScriptInterface::v019() { }
void CGameScriptInterface::v020() { }
void CGameScriptInterface::v021() { }
void CGameScriptInterface::v022() { }
void CGameScriptInterface::v023() { }
void CGameScriptInterface::v024() { }
void CGameScriptInterface::v025() { }
void CGameScriptInterface::v026() { }
void CGameScriptInterface::v027() { }
void CGameScriptInterface::v028() { }
void CGameScriptInterface::v029() { }
void CGameScriptInterface::v030() { }
void CGameScriptInterface::v031() { }
void CGameScriptInterface::v032() { }
void CGameScriptInterface::v033() { }
void CGameScriptInterface::v034() { }
void CGameScriptInterface::v035() { }
void CGameScriptInterface::v036() { }
void CGameScriptInterface::v037() { }
void CGameScriptInterface::v038() { }
void CGameScriptInterface::v039() { }
void CGameScriptInterface::v040() { }
void CGameScriptInterface::v041() { }
void CGameScriptInterface::v042() { }
void CGameScriptInterface::v043() { }
void CGameScriptInterface::v044() { }
void CGameScriptInterface::v045() { }
void CGameScriptInterface::v046() { }
void CGameScriptInterface::v047() { }
void CGameScriptInterface::v048() { }
void CGameScriptInterface::v049() { }
void CGameScriptInterface::v050() { }
void CGameScriptInterface::v051() { }
void CGameScriptInterface::v052() { }
void CGameScriptInterface::v053() { }
void CGameScriptInterface::v054() { }
void CGameScriptInterface::v055() { }
void CGameScriptInterface::v056() { }
void CGameScriptInterface::v057() { }
void CGameScriptInterface::v058() { }
void CGameScriptInterface::v059() { }
void CGameScriptInterface::v060() { }
void CGameScriptInterface::v061() { }
void CGameScriptInterface::v062() { }
void CGameScriptInterface::v063() { }
void CGameScriptInterface::v064() { }
void CGameScriptInterface::v065() { }
void CGameScriptInterface::v066() { }
void CGameScriptInterface::v067() { }
void CGameScriptInterface::v068() { }
void CGameScriptInterface::v069() { }
void CGameScriptInterface::v070() { }
void CGameScriptInterface::v071() { }
void CGameScriptInterface::v072() { }
void CGameScriptInterface::v073() { }
void CGameScriptInterface::v074() { }
long CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& out) const
{
    ++getAllCalls; strcpy(getAllName, (const char*)name.m_pData);
    out._M_start = things; out._M_finish = things + thingCount; out._M_end_of_storage = things + 8;
    return thingCount;
}
bool CV_BodyGuardScript::MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT comment, const CScriptThing& about)
{
    CHECK(text.m_pData == (void*)dataStr);
    commentIdx[commentCalls] = distCalls - 1; commentPrio = priority; commentKind = comment;
    CHECK(about.PImp_Data == 0 && about.PImp_Info == 0);
    return commentCalls++ == commentTrueAt;
}
bool CHenchmanRegionEntranceComment::IsDone() { return isDoneCalls++ == isDoneTrueAt; }
static void reset(int count, int mask, int doneAt, int trueAt)
{ isDoneCalls = distCalls = commentCalls = getAllCalls = 0; strCtors = strDtors = 0; thingCount = count; nearMask = mask; isDoneTrueAt = doneAt; commentTrueAt = trueAt; getAllName[0] = 0; }

int main()
{
    static CGameScriptInterface game; static CV_BodyGuardScript parent;
    CHenchmanRegionEntranceComment c; memset(&c, 0, sizeof(c));
    { CScriptThing me; memcpy(&c.Me, &me, sizeof(me)); }
    c.Game = &game; c.ParentClass = &parent;

    // 1: empty list -> one IsDone, no walk
    reset(0, 0, -1, -1); c.Main();
    CHECK(getAllCalls == 1 && strcmp(getAllName, "BodyGuard") == 0 && isDoneCalls == 1 && distCalls == 0);
    // 2: four things, none near -> four distance tests (each against Me, 10.0), IsDone before each and once after
    reset(4, 0, -1, -1); c.Main();
    CHECK(distCalls == 4 && isDoneCalls == 5 && commentCalls == 0);
    for (int k = 0; k < 4; ++k) CHECK(distA[k] == &c.Me && distB[k] == &things[k] && distD[k] == 10.0f);
    // 3: things 1 and 3 near, the first comment fails, the second succeeds -> break, then the trailing IsDone
    reset(5, (1 << 1) | (1 << 3), -1, 1); c.Main();
    CHECK(commentCalls == 2 && commentIdx[0] == 1 && commentIdx[1] == 3 && distCalls == 4);
    CHECK(commentPrio == 2 && commentKind == HENCHMAN_COMMENT_REGION_ENTRANCE);
    CHECK(isDoneCalls == 4 + 2 + 1);
    CHECK(strCtors == strDtors);
    // 4: IsDone true at the top of the third iteration -> return without the trailing IsDone
    reset(5, 0, 2, -1); c.Main();
    CHECK(distCalls == 2 && isDoneCalls == 3);
    // 5: IsDone true right after a near hit -> no comment
    reset(3, 1 << 0, 1, -1); c.Main();
    CHECK(distCalls == 1 && commentCalls == 0 && isDoneCalls == 2);

    if (fails == 0) printf("00e5f6b0_TEST PASS\n");
    return fails;
}