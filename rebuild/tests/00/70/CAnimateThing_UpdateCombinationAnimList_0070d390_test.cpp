// Behaviour test for 0070d390: final.cpp copied verbatim (free from <cstdlib>), the declared callees stubbed below so
// calls and arguments are observable. CAnimateThing::UpdateCombinationAnimList
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[2048];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CAnimateThing::UpdateCombinationAnimList(list<CAnimSet>& anims, const CAnimSetUpdateInfo& info) -- two passes over the combination anim sets: first drop every finished set whose blend has faded out (or that has no fade), fading the others out by their ChangeAlphaBy; then update each set from the frame info and fade the unfinished fading ones in
// Leaf: UpdateCombinationAnimList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D390, length 370 bytes (+ int3 padding), ret 8; `this` is unused.
// Callees: CWorld::GetFrame @ 0x0049D870 (__fastcall; catalog "GFGetPreMainMemoryUsed" is propagated), CAnimSet::Update
//          @ 0x0070D030 (landed), free @ 0x00BFEA14 (the inline CAnimSet destructor frees InternalAlphas then Anims, then
//          the list node). EPSILON 0.0001f @ 0x0129BA3C, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout: the landed CAnimSet model (0x0070C280 / 0x0070D030: Anims +4, InternalAlphas +0x10, FrameInternalAlphasUpdated
//          +0x28, ChangeAlphaBy +0x38, flag bits +0x51: Finished = 2) in STLport list nodes {next, prev, value}; the list
//          object is the header-node pointer.
// Shape: the first pass walks node pointers (the value's offsets folded), `if (Finished && !FadeOut(set)) erase else next`
//          with the erase block laid after the loops; the second pass names the set (`lea esi,[edi+8]`) because Update
//          takes it as `this`. Each fade is a __forceinline helper: the blend element through a named reference (one
//          vector-start load serves the test, the value and the Last = Current roll; the final Current store re-reads it),
//          GetValue through float locals (`last + (current - last)`: Last kept on the x87 stack across the test's compare),
//          the fade-in sum through a named value local (`value + ChangeAlphaBy`, else VC reassociates), the clamp
//          `if (a < 0) a = 0; else if (a > 1) a = 1;`.
#include <cstdlib>   // free @ 0x00BFEA14
class CWorld { public: static long __fastcall GetFrame(); };          // 0x0049D870
extern const float EPSILON;                            // 0x0129BA3C

struct CAnimSetUpdateInfo {
    float TimePassed;
    float DistanceMoved;
    float PercentageComplete;
};
class CTransitionBlendAlpha { public:
    float Current;
    float Last;
    float GetValue() const
    {
        float last = Last;
        float current = Current;
        return last + (current - last);
    }
};
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    T& operator[](unsigned long n) { return first[n]; }
    ~CRetailVector() { if (first) free(first); }
};
class CAnimSet { public:
    long UpdateMode;                                     // +0
    CRetailVector<short> Anims;                          // +4
    CRetailVector<CTransitionBlendAlpha> InternalAlphas; // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    float CurrentAlpha;                                  // +0x2c
    float LastAlpha;                                     // +0x30
    float LastUpdateChange;                              // +0x34
    float ChangeAlphaBy;                                 // +0x38
    void* PAnimator;                                     // +0x3c
    unsigned long Flags;                                 // +0x40
    unsigned long TypeFlags;                             // +0x44
    long AnimDelay;                                      // +0x48
    long Priority;                                       // +0x4c
    unsigned char Type;                                  // +0x50
    bool Looping : 1;                                    // +0x51 bit 0
    bool Finished : 1;                                   // bit 1
    bool Forwards : 1;                                   // bit 2
    bool HadUpdate : 1;                                  // bit 3
    bool OwnedByMainAnim : 1;                            // bit 4
    void Update(const CAnimSetUpdateInfo& info);         // 0x0070D030
};
inline float GFClamp01(float a)
{
    if (a < 0.0f)
        a = 0.0f;
    else if (a > 1.0f)
        a = 1.0f;
    return a;
}

struct CAnimSetNode {                                    // STLport list node
    CAnimSetNode* next;
    CAnimSetNode* prev;
    CAnimSet      value;
};
struct CAnimSetList {                                    // STLport list: the header node
    CAnimSetNode* header;
    CAnimSetNode* begin() const { return header->next; }
    CAnimSetNode* end() const { return header; }
    CAnimSetNode* erase(CAnimSetNode* node)
    {
        CAnimSetNode* next = node->next;
        CAnimSetNode* prev = node->prev;
        prev->next = next;
        next->prev = prev;
        node->value.~CAnimSet();
        free(node);
        return next;
    }
};

class CAnimateThing {
public:
    void UpdateCombinationAnimList(CAnimSetList& anims, const CAnimSetUpdateInfo& info);
};

__forceinline bool FadeOutCombinationAnim(CAnimSet& set)
{
    if (set.ChangeAlphaBy > EPSILON) {
        CTransitionBlendAlpha& alpha = set.InternalAlphas[0];
        if (alpha.GetValue() > EPSILON) {
            float a = GFClamp01(alpha.GetValue() - set.ChangeAlphaBy);
            alpha.Last = alpha.Current;
            set.InternalAlphas[0].Current = a;
            set.FrameInternalAlphasUpdated = CWorld::GetFrame();
            return true;
        }
    }
    return false;
}
__forceinline void FadeInCombinationAnim(CAnimSet& set)
{
    if (!set.Finished && set.ChangeAlphaBy > EPSILON) {
        CTransitionBlendAlpha& alpha = set.InternalAlphas[0];
        float value = alpha.GetValue();
        float a = GFClamp01(value + set.ChangeAlphaBy);
        alpha.Last = alpha.Current;
        set.InternalAlphas[0].Current = a;
        set.FrameInternalAlphasUpdated = CWorld::GetFrame();
    }
}

void CAnimateThing::UpdateCombinationAnimList(CAnimSetList& anims, const CAnimSetUpdateInfo& info)
{
    CAnimSetNode* it = anims.begin();
    while (it != anims.end()) {
        if (it->value.Finished && !FadeOutCombinationAnim(it->value))
            it = anims.erase(it);
        else
            it = it->next;
    }
    for (it = anims.begin(); it != anims.end(); it = it->next) {
        CAnimSet& set = it->value;
        set.Update(info);
        FadeInCombinationAnim(set);
    }
}

// ---- stubs ----
const float EPSILON = 0.0001f;
static long g_frame = 100;
long CWorld::GetFrame() { return ++g_frame; }
void CAnimSet::Update(const CAnimSetUpdateInfo& info) { char b[48]; _snprintf(b, sizeof b, "update%d(%.1f)", (int)Type, info.TimePassed); note(b); }
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
static CAnimSetNode* add(CAnimSetList& l, int type, bool finished, float change, float current, float last)
{
    CAnimSetNode* n = (CAnimSetNode*)malloc(sizeof(CAnimSetNode)); memset(n, 0, sizeof *n);
    n->value.Type = (unsigned char)type; n->value.Finished = finished; n->value.ChangeAlphaBy = change;
    n->value.Anims.first = (short*)malloc(2 * sizeof(short)); n->value.Anims.last = n->value.Anims.first + 2; n->value.Anims.end_of_storage = n->value.Anims.last;
    n->value.InternalAlphas.first = (CTransitionBlendAlpha*)malloc(sizeof(CTransitionBlendAlpha)); n->value.InternalAlphas.last = n->value.InternalAlphas.first + 1; n->value.InternalAlphas.end_of_storage = n->value.InternalAlphas.last;
    n->value.InternalAlphas.first->Current = current; n->value.InternalAlphas.first->Last = last;
    CAnimSetNode* h = l.header; n->next = h; n->prev = h->prev; h->prev->next = n; h->prev = n; return n;
}
static int count(const CAnimSetList& l) { int c = 0; for (CAnimSetNode* n = l.header->next; n != l.header; n = n->next) ++c; return c; }
int main()
{
    CAnimateThing thing; CAnimSetUpdateInfo info; info.TimePassed = 0.5f; info.DistanceMoved = 0; info.PercentageComplete = 0;
    CAnimSetNode& header = *(CAnimSetNode*)calloc(1, sizeof(CAnimSetNode));   // zeroed: the header's value frees nothing
    header.next = &header; header.prev = &header; CAnimSetList list; list.header = &header;
    // empty list: nothing
    g_log[0] = 0; thing.UpdateCombinationAnimList(list, info); CHECKLOG(""); CHECK(count(list) == 0);
    // 1: finished, no fade (ChangeAlphaBy 0) -> erased
    add(list, 1, true, 0.0f, 0.5f, 0.5f);
    // 2: finished, faded out already (value 0) -> erased
    add(list, 2, true, 0.1f, 0.0f, 0.0f);
    // 3: finished, fading: 0.05 - 0.1 -> clamped to 0; Last <- 0.05; stamped; then skipped in pass 2 (finished)
    CAnimSetNode* n3 = add(list, 3, true, 0.1f, 0.05f, 0.3f);
    // 4: unfinished, fading in: 0.95 + 0.1 -> clamped to 1
    CAnimSetNode* n4 = add(list, 4, false, 0.1f, 0.95f, 0.7f);
    // 5: unfinished, no fade: only updated
    CAnimSetNode* n5 = add(list, 5, false, 0.0f, 0.4f, 0.4f);
    // 6: unfinished, fading in within range: 0.2 + 0.25 = 0.45
    CAnimSetNode* n6 = add(list, 6, false, 0.25f, 0.2f, 0.1f);
    // 7: finished, fading in range: 0.5 - 0.2 = 0.3
    CAnimSetNode* n7 = add(list, 7, true, 0.2f, 0.5f, 0.9f);
    g_log[0] = 0; g_frame = 100;
    thing.UpdateCombinationAnimList(list, info);
    CHECKLOG("update3(0.5);update4(0.5);update5(0.5);update6(0.5);update7(0.5);");
    CHECK(count(list) == 5); CHECK(header.next == n3 && n3->next == n4 && n7->next == &header);
    CHECK(n3->value.InternalAlphas.first->Current == 0.0f && near(n3->value.InternalAlphas.first->Last, 0.05f) && n3->value.FrameInternalAlphasUpdated == 101);
    CHECK(n7->value.InternalAlphas.first->Current > 0.29f && n7->value.InternalAlphas.first->Current < 0.31f && near(n7->value.InternalAlphas.first->Last, 0.5f) && n7->value.FrameInternalAlphasUpdated == 102);
    CHECK(n4->value.InternalAlphas.first->Current == 1.0f && near(n4->value.InternalAlphas.first->Last, 0.95f) && n4->value.FrameInternalAlphasUpdated == 103);
    CHECK(near(n5->value.InternalAlphas.first->Current, 0.4f) && near(n5->value.InternalAlphas.first->Last, 0.4f) && n5->value.FrameInternalAlphasUpdated == 0);
    CHECK(near(n6->value.InternalAlphas.first->Current, 0.45f) && near(n6->value.InternalAlphas.first->Last, 0.2f) && n6->value.FrameInternalAlphasUpdated == 104);
    CHECK(n3->value.Finished && n7->value.Finished);
    // run again: 3 (now at 0) and 7 fade further; 7: 0.3 - 0.2 = 0.1 stays
    g_log[0] = 0; thing.UpdateCombinationAnimList(list, info);
    CHECK(count(list) == 4); CHECK(header.next == n4); CHECK(n7->value.InternalAlphas.first->Current > 0.09f && n7->value.InternalAlphas.first->Current < 0.11f);
    CHECKLOG("update4(0.5);update5(0.5);update6(0.5);update7(0.5);");
    while (count(list)) { CAnimSetNode* n = header.next; list.erase(n); }
    if (fails == 0) printf("0070d390_TEST PASS\n"); else printf("0070d390_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}