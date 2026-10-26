// Behaviour test for 00856f00: src/00856f00.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CCombatActionBase::IsThingInStruckThingList(const CThingGameObject& thing) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatActionBase::IsThingInStruckThingList(const CThingGameObject& thing) const -- find the active strike event that finishes first and report whether the thing is in its struck-thing list
// Leaf: IsThingInStruckThingList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856F00, length 121 bytes (+7 int3 padding), `ret 4`.
// Label correction: the catalog row says "CTCHeroExperience::GetRoughExperienceLevel" (a `long ()` member); this body is a
// CCombatActionBase member (FramesRunning +0x1c, StrikeEvents +0xbc) taking one reference and returning bool:
// Ego_r.pdb ?IsThingInStruckThingList@CCombatActionBase@@QBE_NABVCThingGameObject@@@Z.
// Callees: std::for_each<list<CCountedPointer<CStrikeEvent>>::const_iterator, CGetActiveStrikeEventThatIsFirstToFinish>
// @ 0x0085A960 (`__fastcall`: hidden result in ecx, iterators and the 12-byte functor on the stack).
// Shape: the functor (CurrentFrame = FramesRunning, PBestStrikeEvent = 0, SmallestEndFrame = 1000000) is a NAMED local with
// the implicit copy ctor: VC keeps it in registers (eax / xor edx / mov esi,0F4240h) and copies it into the argument area
// through ebx (a temporary argument or a declared copy ctor builds it there with immediates); `end()` is the list head, `begin()` its next; the struck list is searched with an
// index loop (STLport `*(begin() + i)`, size re-read each pass); one shared `xor al,al` exit.
// Layout: retail CCreatureActionBase is 0xa8 (PDB 0xb4), so StrikeEvents (PDB +0xc8) sits at +0xbc; CStrikeEvent (0x28 in
// retail): StruckThingList +0x1c (vector<CThingGameObject*>).
class CThingGameObject;
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CStrikeEvent {
    bool  Legacy;                               // +0x00
    bool  PrimaryLegacy;                        // +0x01
    bool  Weapon;                               // +0x02
    void* HelperPointName;                      // +0x04 CCharString
    long  StrikeEventIndex;                     // +0x08
    long  StartFrame;                           // +0x0c
    long  EndFrame;                             // +0x10
    void* PWeapon[2];                           // +0x14 CIntelligentPointer<const CThing>
    CRetailVector<CThingGameObject*> StruckThingList;   // +0x1c
};
template <class T> class CCountedPointer { public: T* Data; void* Info; };
struct SStrikeNode {
    SStrikeNode* _M_next;
    SStrikeNode* _M_prev;
    CCountedPointer<CStrikeEvent> _M_data;
};
struct CStrikeListConstIterator {               // STLport _List_iterator (one node pointer)
    SStrikeNode* _M_node;
    CStrikeListConstIterator(SStrikeNode* n) : _M_node(n) {}
};
struct CStrikeEventList {                       // STLport list (4 bytes: the head node)
    SStrikeNode* _M_node;
    CStrikeListConstIterator begin() const { return CStrikeListConstIterator(_M_node->_M_next); }
    CStrikeListConstIterator end() const { return CStrikeListConstIterator(_M_node); }
};
struct CGetActiveStrikeEventThatIsFirstToFinish {
    long          CurrentFrame;
    CStrikeEvent* PBestStrikeEvent;
    long          SmallestEndFrame;
    CGetActiveStrikeEventThatIsFirstToFinish(long frame) : CurrentFrame(frame), PBestStrikeEvent(0), SmallestEndFrame(1000000) {}

};
CGetActiveStrikeEventThatIsFirstToFinish __fastcall for_each(CStrikeListConstIterator first, CStrikeListConstIterator last,
                                                             CGetActiveStrikeEventThatIsFirstToFinish f);   // 0x0085A960

class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    unsigned char    _pad04[0x1c - 0x04];
    long             FramesRunning;             // +0x1c
    unsigned char    _pad20[0xbc - 0x20];
    CStrikeEventList StrikeEvents;              // +0xbc
    bool IsThingInStruckThingList(const CThingGameObject& thing) const;
};

bool CCombatActionBase::IsThingInStruckThingList(const CThingGameObject& thing) const
{
    CGetActiveStrikeEventThatIsFirstToFinish finder(FramesRunning);
    CStrikeEvent* event = for_each(StrikeEvents.begin(), StrikeEvents.end(), finder).PBestStrikeEvent;
    if (event) {
        for (unsigned int i = 0; i < event->StruckThingList.size(); ++i)
            if (event->StruckThingList[i] == &thing)
                return true;
    }
    return false;
}

// ---- stubs and driver ----
#include <cstdlib>
static long g_seenFrame = -1; static long g_seenBest = -1; static void* g_seenPBest = (void*)1;
CGetActiveStrikeEventThatIsFirstToFinish __fastcall for_each(CStrikeListConstIterator first, CStrikeListConstIterator last,
                                                             CGetActiveStrikeEventThatIsFirstToFinish f)
{
    g_seenFrame = f.CurrentFrame; g_seenBest = f.SmallestEndFrame; g_seenPBest = f.PBestStrikeEvent;
    for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
        CStrikeEvent* e = first._M_node->_M_data.Data;
        if (e->StartFrame <= f.CurrentFrame && f.CurrentFrame <= e->EndFrame && e->EndFrame < f.SmallestEndFrame) {
            f.SmallestEndFrame = e->EndFrame; f.PBestStrikeEvent = e;
        }
    }
    return f;
}
CCombatActionBase::~CCombatActionBase() { }
struct CTestAction : CCombatActionBase { };
int main()
{
    CThingGameObject* things[4] = { (CThingGameObject*)0x1000, (CThingGameObject*)0x2000, (CThingGameObject*)0x3000, (CThingGameObject*)0x4000 };
    CStrikeEvent e1; memset(&e1, 0, sizeof(e1)); e1.StartFrame = 0; e1.EndFrame = 50;
    e1.StruckThingList._M_start = things; e1.StruckThingList._M_finish = things + 2; e1.StruckThingList._M_end_of_storage = things + 2;
    CStrikeEvent e2; memset(&e2, 0, sizeof(e2)); e2.StartFrame = 5; e2.EndFrame = 20;
    e2.StruckThingList._M_start = things + 2; e2.StruckThingList._M_finish = things + 3; e2.StruckThingList._M_end_of_storage = things + 3;
    SStrikeNode head, n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head; head._M_prev = &n2; n2._M_prev = &n1; n1._M_prev = &head;
    n1._M_data.Data = &e1; n2._M_data.Data = &e2;
    CTestAction* a = (CTestAction*)calloc(1, sizeof(CTestAction));
    a->StrikeEvents._M_node = &head;
    a->FramesRunning = 10;   // both active, e2 finishes first: only things[2] is struck
    CHECK(a->IsThingInStruckThingList(*things[2]));
    CHECK(g_seenFrame == 10 && g_seenBest == 1000000 && g_seenPBest == 0);
    CHECK(!a->IsThingInStruckThingList(*things[0]));
    CHECK(!a->IsThingInStruckThingList(*things[3]));
    a->FramesRunning = 30;   // only e1 active: things[0], things[1]
    CHECK(a->IsThingInStruckThingList(*things[0]) && a->IsThingInStruckThingList(*things[1]));
    CHECK(!a->IsThingInStruckThingList(*things[2]));
    a->FramesRunning = 99;   // nothing active
    CHECK(!a->IsThingInStruckThingList(*things[0]));
    e1.EndFrame = 100; e1.StruckThingList._M_finish = things;   // active but empty struck list
    CHECK(!a->IsThingInStruckThingList(*things[0]));
    if (fails == 0) printf("00856f00_TEST PASS\n");
    return fails;
}
