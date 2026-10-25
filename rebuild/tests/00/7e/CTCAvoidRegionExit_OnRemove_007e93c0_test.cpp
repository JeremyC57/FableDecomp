// Behaviour test for 007e93c0: src/007e93c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCAvoidRegionExit::OnRemove()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCAvoidRegionExit::OnRemove() -- run the base OnRemove, remove the parent thing's entry from the global exit-weight map and recompute the maximum weight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E93C0, length 78 bytes (+ int3 padding).
// Identity: slot 2 of vtable 0x0123EFAC = CTCAvoidRegionExit (rtti-vtables.tsv); the manifest label
// "OnKill" is shared with the slot-14 twin 0x007E9410 and is not trusted for this slot. The base call
// is CTCBase's empty slot-2 implementation @ 0x006867F0 (a direct, qualified call).
// Naming: Ego_r.pdb defines ?OnRemove@CTCAvoidRegionExit@@UAEXXZ and ?OnKill@CTCAvoidRegionExit@@UAEXXZ, the two
//   overrides retail has at slots 2 and 14; slot 14 is OnKill in CTCBase's order (13 OnDie, 15 OnLevelUnload), so
//   slot 2 is OnRemove (CTCBase::OnRemove 0x006867F0, empty; the CTCAssassinRush OnRemove 0x007AA830 / OnKill
//   0x007AA820 thunks and CTCForcePushed OnRemove 0x00798540 / OnKill 0x00798510 sit in the same slots). Was landed
//   as "CTCAvoidRegionExit::vfunc_2".
// Callees: the map's equal_range @ 0x007E9AE0 (thiscall, hidden pair result + the key by const
// reference; manifest "std::map_Tree_Find_Node"), erase(first, last) @ 0x007E99F0 (landed as the
// CRetailTree erase-range; two 4-byte iterators by value), UpdateBestDist @ 0x007E9250 (landed, Ego_r.pdb ?UpdateBestDist@CTCAvoidRegionExit@@QAEXXZ; walks
// the map for the largest weight into the global float @ 0x013BAE28; manifest
// "TreeWalk_FindMaxValue"). Global 0x013BAF00 = a map<CThing*, float> (key +0x10, weight +0x14 of
// the node, unsigned key compare) of every avoid-region exit by its parent thing.
// Shape: the parent pointer reaches equal_range through an inline accessor returning it BY VALUE, so
// the const-reference key is a temporary (retail copies [esi+4] into a stack slot); the pair is
// returned through the hidden pointer into a frame local; each iterator is built in its argument
// slot (`push ecx ; mov eax,esp ; mov [eax],..`, second first).
// The key temporary gets its own frame word here (no argument slot to reuse). Fix (retry 10): equal_range has its same-TU
// body (lower/upper bound walks; with only the declaration the second iterator was loaded late).
// Leaf: ?OnRemove@CTCAvoidRegionExit@@UAEXXZ
struct CThing;

struct SExitWeightNode;                                     // map node: colour, parent, left, right, key +0x10, weight +0x14

struct CExitWeightIterator {                                // 4-byte iterator with a user copy constructor:
    SExitWeightNode* m_pNode;                               // by value it is built in its argument slot
    CExitWeightIterator() { }
    CExitWeightIterator(const CExitWeightIterator& other) : m_pNode(other.m_pNode) { }
};

struct SExitWeightRange {                                   // pair<iterator, iterator>: non-POD, returned by hidden pointer
    CExitWeightIterator first;
    CExitWeightIterator second;
};

struct CExitWeightMap {                                     // map<CThing*, float>, header + size
    SExitWeightNode* m_pHeader;
    unsigned long    m_size;
    SExitWeightRange EqualRange(CThing* const& key);        // 0x007E9AE0
    void Erase(CExitWeightIterator first, CExitWeightIterator last);   // 0x007E99F0
};
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work Erase does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the Erase instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void CExitWeightMap::Erase(CExitWeightIterator first, CExitWeightIterator last)
{
    long r = TmplSink_s7_0(first.m_pNode, (long)last.m_pNode);   // the range's node pointers read by value
    (void)r;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// Same-TU body of the map's equal_range instantiation (0x007E9AE0): STLport lower_bound / upper_bound walks over the raw
// node links (parent +4, left +8, right +0xc, key +0x10), the key read through its const reference; the iterator pair
// returned through the hidden pointer.
__declspec(noinline) SExitWeightRange CExitWeightMap::EqualRange(CThing* const& key)
{
    char* head = (char*)m_pHeader;
    char* lower = head;
    char* x = *(char**)(head + 4);
    while (x != 0) {
        if (!(*(CThing**)(x + 0x10) < key)) {
            lower = x;
            x = *(char**)(x + 8);
        } else
            x = *(char**)(x + 0xc);
    }
    char* upper = head;
    x = *(char**)(head + 4);
    while (x != 0) {
        if (key < *(CThing**)(x + 0x10)) {
            upper = x;
            x = *(char**)(x + 8);
        } else
            x = *(char**)(x + 0xc);
    }
    SExitWeightRange range;
    range.first.m_pNode = (SExitWeightNode*)lower;
    range.second.m_pNode = (SExitWeightNode*)upper;
    return range;
}
#pragma auto_inline(on)
extern CExitWeightMap g_AvoidRegionExitWeights;             // 0x013BAF00

class CTCBase {                                             // engine/CTCBase.h layout (0xc bytes); a local model:
public:                                                     // the flattened header cannot carry the virtuals
    virtual ~CTCBase();                                     // +0x00 vptr (slot 0)
    virtual void OnRemove();                                // 0x006867F0 (empty base implementation, slot 2)
    virtual void OnKill();                                  // 0x006868F0 (empty base implementation, slot 14)
    virtual void OnSetInLimbo(bool flag);                   // 0x00686910 (empty base implementation, slot 16)
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CThing* GetParentThing() const { return ParentThing; }  // by value: the key is an rvalue temporary
};

class CTCAvoidRegionExit : public CTCBase {
public:
    unsigned char _body[0x20 - 0x0c];                       // EntranceMarker, ExitMarker, NextFrameCheck
    void UpdateBestDist();                             // 0x007E9250
    __forceinline void RemoveFromExitWeights()
    {
        SExitWeightRange range = g_AvoidRegionExitWeights.EqualRange(GetParentThing());
        g_AvoidRegionExitWeights.Erase(range.first, range.second);
        UpdateBestDist();
    }
    virtual void OnRemove();
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work UpdateBestDist does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CTCAvoidRegionExit::UpdateBestDist @ 0x007E9250 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) void CTCAvoidRegionExit::UpdateBestDist()
{
    NearSink_s7_0(this, 0);
}
#pragma auto_inline(on)

void CTCAvoidRegionExit::OnRemove()
{
    CTCBase::OnRemove();
    RemoveFromExitWeights();
}

// ---- stubs and driver ----

CExitWeightMap g_AvoidRegionExitWeights;
static int g_base = 0;
CTCBase::~CTCBase() {}
void CTCBase::OnRemove() { ++g_base; }
void CTCBase::OnKill() { ++g_base; }
void CTCBase::OnSetInLimbo(bool flag) { ++g_base; }
static int g_erases = 0; static const void* g_eraseFirst = 0; static long g_eraseLast = 0;
long TmplSink_s7_0(const void* self, long seen) { ++g_erases; g_eraseFirst = self; g_eraseLast = seen; return 0; }
static int g_updates = 0;
long NearSink_s7_0(const void* self, long seen) { ++g_updates; return 0; }
struct SRawNode { long color; SRawNode* parent; SRawNode* left; SRawNode* right; CThing* key; float weight; };
int main()
{
    static SRawNode head, a, b, c; static char things[3];
    CThing* ka = (CThing*)&things[0]; CThing* kb = (CThing*)&things[1]; CThing* kc = (CThing*)&things[2];
    head.parent = &b; b.left = &a; b.right = &c; a.key = ka; b.key = kb; c.key = kc;
    g_AvoidRegionExitWeights.m_pHeader = (SExitWeightNode*)&head;
    static CTCAvoidRegionExit g; g.ParentThing = kb;
    g.OnRemove(); CHECK(g_base == 1);
    CHECK(g_erases == 1); CHECK(g_eraseFirst == &b); CHECK(g_eraseLast == (long)&c); CHECK(g_updates == 1);
    g.ParentThing = kc; g_erases = 0; g_base = 0; g_updates = 0;
    CTCBase* pg = &g; pg->OnRemove(); CHECK(g_base == 1);   // through the vtable
    CHECK(g_eraseFirst == &c); CHECK(g_eraseLast == (long)&head);
    if (fails == 0) printf("007e93c0_TEST PASS\n"); else printf("007e93c0_TEST FAIL\n");
    return fails;
}