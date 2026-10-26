// Behaviour test for 007703b0: src/007703b0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CEnginePrimitiveWeaponTrail::CEnginePrimitiveWeaponTrail(unsigned __int64, const std::list<CWeaponTrailEntry>&, unsigned long, unsigned long, EWeaponTrailType, unsigned char, float, float, float, float, EEngineInterpolationSet)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEnginePrimitiveWeaponTrail::CEnginePrimitiveWeaponTrail(unsigned __int64, const std::list<CWeaponTrailEntry>&, unsigned long, unsigned long, EWeaponTrailType, unsigned char, float, float, float, float, EEngineInterpolationSet) -- primitive type 0x13: copies the trail list, graphic, trail type, alpha and timing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007703B0, length 144 bytes.
// Leaf: CEnginePrimitiveWeaponTrail
// Callees: 0x00BFEA0E malloc (the list's sentinel node), 0x0076FD40 the STLport list range insert
// (`_M_insert_dispatch(pos, first, last, const __false_type&)`, `ret 10h`, copies 28-byte entries).
// Signature from Ego_r.pdb: ??0CEnginePrimitiveWeaponTrail@@QAE@_KABV?$list@VCWeaponTrailEntry@...@std@@KKW4EWeaponTrailType@0@EMMMMW4EEngineInterpolationSet@@@Z
// (12 stack words, `ret 30h`); the 64-bit primitive handle is not stored.
// Layout: PDB CEnginePrimitiveWeaponTrail (0x38) with retail's 4-byte STLport list (the node pointer only), so every
// member after Trail sits 8 bytes below the PDB offset: Trail +0xc, GraphicIndex +0x10, GraphicFrame +0x14,
// TrailType +0x18, SkipPoints +0x1c, FadeStart +0x20, Alpha +0x24, CurrentTime +0x28, FallOffTime +0x2c (0x30).
// The four float parameters feed SkipPoints, CurrentTime, FallOffTime, FadeStart in that order.
// Shape: base inline constructor (interpolation set argument, type immediate, zero channel bytes); the inline
// STLport list copy constructor: the alloc proxy is zeroed, the sentinel node malloc'd and self-linked, then
// `insert(begin(), x.begin(), x.end())` is STLport's inline insert(pos, first, last). Its by-value `pos` parameter
// makes begin()'s node load happen before the argument pushes, and pos is then copy-constructed into its argument
// slot (a 4-byte iterator class with a user copy constructor). It dispatches to the out-of-line range insert with the
// empty `__false_type` tag by const reference. The range insert is a same-TU noinline stand-in forwarding to
// 0x0076FD40, so the tag escapes and VC7.1 homes it over the live alpha argument slot (`lea ebx,[esp+28h]`), as retail does.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class CEnginePrimitiveThemeChannelDesc { public:                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                                // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};

struct CWeaponTrailEntry { unsigned long m_data[7]; };   // 28 bytes (the range insert copies 7 dwords)

struct SFalseType { SFalseType() { } };                  // STLport __false_type dispatch tag
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };   // 0x24 bytes
struct SListIterator {                                   // 4-byte iterator with a user constructor
    SListNodeBase* m_node;
    SListIterator(SListNodeBase* node) : m_node(node) { }
    SListIterator(const SListIterator& other) : m_node(other.m_node) { }
};
struct SListConstIterator { SListNodeBase* m_node; };

struct CWeaponTrailList {                                // STLport list: the alloc proxy's node pointer only
    SListNode* m_node;
    CWeaponTrailList(const CWeaponTrailList& x) : m_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_next = node;
        node->m_prev = node;
        m_node = node;
        insert(begin(), x.cbegin(), x.cend());
    }
    SListIterator begin() { return SListIterator(m_node->m_next); }
    SListConstIterator cbegin() const { SListConstIterator it; it.m_node = m_node->m_next; return it; }
    SListConstIterator cend() const { SListConstIterator it; it.m_node = m_node; return it; }
    void InsertDispatch(SListIterator pos, SListConstIterator first, SListConstIterator last, const SFalseType& tag);
    void insert(SListIterator pos, SListConstIterator first, SListConstIterator last)
    {
        InsertDispatch(pos, first, last, SFalseType());
    }
};

void InsertRange_0076fd40(CWeaponTrailList* list, SListNodeBase* pos, SListNodeBase* first, SListNodeBase* last);
#pragma auto_inline(off)
__declspec(noinline) void CWeaponTrailList::InsertDispatch(SListIterator pos, SListConstIterator first, SListConstIterator last,
                                                           const SFalseType&)   // 0x0076FD40 stand-in
{
    InsertRange_0076fd40(this, pos.m_node, first.m_node, last.m_node);
}
#pragma auto_inline(on)

struct CEnginePrimitiveWeaponTrail : CEnginePrimitive {
    CWeaponTrailList Trail;                              // +0x0c
    unsigned long    GraphicIndex;                       // +0x10
    unsigned long    GraphicFrame;                       // +0x14
    long             TrailType;                          // +0x18 EWeaponTrailType
    float            SkipPoints;                         // +0x1c
    float            FadeStart;                          // +0x20
    unsigned char    Alpha;                              // +0x24
    float            CurrentTime;                        // +0x28
    float            FallOffTime;                        // +0x2c   sizeof == 0x30 (retail)
    CEnginePrimitiveWeaponTrail(unsigned __int64 handle, const CWeaponTrailList& trail, unsigned long graphicIndex,
                                unsigned long graphicFrame, long trailType, unsigned char alpha, float skipPoints,
                                float currentTime, float fallOffTime, float fadeStart, long interpolationSet);
};

CEnginePrimitiveWeaponTrail::CEnginePrimitiveWeaponTrail(unsigned __int64 handle, const CWeaponTrailList& trail,
                                                         unsigned long graphicIndex, unsigned long graphicFrame,
                                                         long trailType, unsigned char alpha, float skipPoints,
                                                         float currentTime, float fallOffTime, float fadeStart,
                                                         long interpolationSet)
    : CEnginePrimitive(0x13, interpolationSet)           // ENGINE_PRIMITIVE_WEAPON_TRAIL
    , Trail(trail)
    , GraphicIndex(graphicIndex)
    , GraphicFrame(graphicFrame)
    , TrailType(trailType)
    , SkipPoints(skipPoints)
    , FadeStart(fadeStart)
    , Alpha(alpha)
    , CurrentTime(currentTime)
    , FallOffTime(fallOffTime)
{
}

// ---- stubs and driver ----
#include <new>
static int g_inserts = 0;
static CWeaponTrailList* g_list = 0;
static SListNodeBase *g_pos = 0, *g_first = 0, *g_last = 0;
void InsertRange_0076fd40(CWeaponTrailList* list, SListNodeBase* pos, SListNodeBase* first, SListNodeBase* last)
{
    ++g_inserts; g_list = list; g_pos = pos; g_first = first; g_last = last;
}
int main()
{
    SListNode sentinel, n1;
    sentinel.m_next = &n1; sentinel.m_prev = &n1; n1.m_next = &sentinel; n1.m_prev = &sentinel;
    CWeaponTrailList* src = (CWeaponTrailList*)new unsigned char[sizeof(CWeaponTrailList)];
    src->m_node = &sentinel;
    unsigned char raw[sizeof(CEnginePrimitiveWeaponTrail)];
    CEnginePrimitiveWeaponTrail* p = new (raw) CEnginePrimitiveWeaponTrail(0x1234567890ull, *src, 7, 9, 2, 0x80,
                                                                          1.5f, 2.5f, 3.5f, 4.5f, 5);
    CHECK(p->Type == 0x13 && p->InterpolationSet == 5);
    CHECK(p->ThemeChannelDesc.ChannelIndex0 == 0 && p->ThemeChannelDesc.ChannelIndex1 == 0 && p->ThemeChannelDesc.ChannelBlendFactor == 0);
    SListNode* node = p->Trail.m_node;
    CHECK(node != 0 && node->m_next == node && node->m_prev == node);
    CHECK(g_inserts == 1 && g_list == &p->Trail && g_pos == node && g_first == &n1 && g_last == &sentinel);
    CHECK(p->GraphicIndex == 7 && p->GraphicFrame == 9 && p->TrailType == 2 && p->Alpha == 0x80);
    CHECK(p->SkipPoints == 1.5f && p->CurrentTime == 2.5f && p->FallOffTime == 3.5f && p->FadeStart == 4.5f);
    if (fails == 0) printf("007703b0_TEST PASS\n");
    return fails;
}
