// Behaviour test for 00877c40: src/00877c40.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTools::IsLineOfSightFiltered(const C3DVector& from, const C3DVector& to, C3DVector* hitPos, C3DVector* hitNormal, float* distance, CThing** hitThing, ELOSTest test, bool flag)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTools::IsLineOfSightFiltered(const C3DVector& from, const C3DVector& to, C3DVector* hitPos, C3DVector* hitNormal, float* distance, CThing** hitThing, ELOSTest test, bool flag) -- collect the live things of the camera scan list and run IsLineOfSight with them
// Leaf: IsLineOfSightFiltered
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00877C40, length 215 bytes (`ret 20h`; 224 with padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3 per element), the vector's _M_insert_overflow(pos, x,
// __true_type, 1, true) @ 0x0049C370 (manifest "Std_Vector_Insert_Range"), CTools::IsLineOfSight(const
// vector<const CThing*>&, ...) @ 0x00877840, free @ 0x00BFEA14 (the vector storage).
// Layout (Ego_r.pdb, as the landed CTools::FrameUpdate 0x00877D20): CTools ScanThings (vector<CIntelligentPointer<
// CThing const> >, 8-byte elements) +0x04; retail CThing state bits +0x91 (Dead = bit 0).
// Shape: the STLport `_Vector_base` + alloc-proxy local (one zero register), the landed inline push_back (placement
// new under its null test; the empty __true_type temporary homed in a dead argument slot); the end pointer is
// re-read every step; the eight arguments are passed straight through. Retail keeps the vector's finish in edi across the
// GetPItem calls: the local does not escape, because _M_insert_overflow and IsLineOfSight are in retail's TU (VC7.1 escape
// is flow-insensitive) -> same-TU stand-ins for both that pass the bounds by value.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
struct C3DVector { float x, y, z; };
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
};
struct CBaseIntelligentPointer {
    void*  m_vtable;
    void*  m_pItem;
    const CThing* GetPItem() const;                         // 0x00A01B50
};
struct __true_type { __true_type() {} };

template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
struct CThingConstVector : _Vector_base<const CThing*> {   // STLport vector<const CThing*>, 12 bytes
    CThingConstVector() : _Vector_base<const CThing*>(allocator<const CThing*>()) { }
    void _M_insert_overflow(const CThing** position, const CThing* const& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x0049C370
    void push_back(const CThing* const& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) const CThing*(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CScanVector {                                        // STLport vector<CIntelligentPointer<CThing const> >
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
};

struct CTools {
    void*       CameraManager;                              // +0x00
    CScanVector ScanThings;                                 // +0x04
    bool IsLineOfSight(const CThingConstVector& things, const C3DVector& from, const C3DVector& to, C3DVector* hitPos,
                       C3DVector* hitNormal, float* distance, CThing** hitThing, long test, bool flag);   // 0x00877840
    bool IsLineOfSightFiltered(const C3DVector& from, const C3DVector& to, C3DVector* hitPos, C3DVector* hitNormal,
                               float* distance, CThing** hitThing, long test, bool flag);
};

void InsertOverflow_0049c370(const CThing*** start, const CThing*** finish, const CThing*** endOfStorage, const CThing** position,
                             const CThing* const& x, unsigned long fillLength, bool atEnd);
bool IsLineOfSight_00877840(CTools* tools, const CThing* const* first, const CThing* const* last, const C3DVector& from, const C3DVector& to,
                            C3DVector* hitPos, C3DVector* hitNormal, float* distance, CThing** hitThing, long test, bool flag);
// Stand-ins for the vector<const CThing*> _M_insert_overflow instantiation (0x0049C370) and CTools::IsLineOfSight (0x00877840),
// both in retail's TU: the bounds pass by value, so the local vector does not escape.
__declspec(noinline) void CThingConstVector::_M_insert_overflow(const CThing** position, const CThing* const& x, const __true_type&, unsigned long fill_len, bool atend)
{
    const CThing** start = _M_start;
    const CThing** finish = _M_finish;
    const CThing** endOfStorage = _M_end_of_storage._M_data;
    InsertOverflow_0049c370(&start, &finish, &endOfStorage, position, x, fill_len, atend);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage._M_data = endOfStorage;
}
__declspec(noinline) bool CTools::IsLineOfSight(const CThingConstVector& things, const C3DVector& from, const C3DVector& to, C3DVector* hitPos,
                                                C3DVector* hitNormal, float* distance, CThing** hitThing, long test, bool flag)
{
    return IsLineOfSight_00877840(this, things._M_start, things._M_finish, from, to, hitPos, hitNormal, distance, hitThing, test, flag);
}

bool CTools::IsLineOfSightFiltered(const C3DVector& from, const C3DVector& to, C3DVector* hitPos, C3DVector* hitNormal,
                                   float* distance, CThing** hitThing, long test, bool flag)
{
    CThingConstVector things;
    for (CBaseIntelligentPointer* it = ScanThings._M_start; it != ScanThings._M_finish; ++it) {
        if (it->GetPItem() && !it->GetPItem()->Dead)
            things.push_back(it->GetPItem());
    }
    return IsLineOfSight(things, from, to, hitPos, hitNormal, distance, hitThing, test, flag);
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_getCalls = 0, g_overflows = 0, g_los = 0;
static const CThing* g_seen[8]; static long g_seenCount = 0;
static const C3DVector* g_from; static const C3DVector* g_to; static C3DVector* g_hitPos; static C3DVector* g_hitNormal;
static float* g_dist; static CThing** g_hitThing; static long g_test; static bool g_flag; static CTools* g_tools;
const CThing* CBaseIntelligentPointer::GetPItem() const { ++g_getCalls; return (const CThing*)m_pItem; }
void InsertOverflow_0049c370(const CThing*** start, const CThing*** finish, const CThing*** endOfStorage, const CThing** position,
                             const CThing* const& x, unsigned long fillLength, bool atEnd)
{
    ++g_overflows;
    CHECK(position == *finish && fillLength == 1 && atEnd);
    long size = (long)(*finish - *start); long cap = size ? size * 2 : 1;
    const CThing** p = (const CThing**)malloc(cap * sizeof(const CThing*));
    for (long i = 0; i < size; ++i) p[i] = (*start)[i];
    p[size] = x;
    if (*start) free(*start);
    *start = p; *finish = p + size + 1; *endOfStorage = p + cap;
}
bool IsLineOfSight_00877840(CTools* tools, const CThing* const* first, const CThing* const* last, const C3DVector& from, const C3DVector& to,
                            C3DVector* hitPos, C3DVector* hitNormal, float* distance, CThing** hitThing, long test, bool flag)
{
    ++g_los; g_tools = tools; g_seenCount = (long)(last - first);
    for (long i = 0; i < g_seenCount && i < 8; ++i) g_seen[i] = first[i];
    g_from = &from; g_to = &to; g_hitPos = hitPos; g_hitNormal = hitNormal; g_dist = distance; g_hitThing = hitThing; g_test = test; g_flag = flag;
    return g_seenCount == 3;
}
static CThing* MakeThing(bool dead) { CThing* t = (CThing*)calloc(1, sizeof(CThing)); t->Dead = dead; return t; }
int main()
{
    CTools tools; tools.CameraManager = 0;
    CThing* a = MakeThing(false); CThing* b = MakeThing(true); CThing* c = MakeThing(false); CThing* d = MakeThing(false); CThing* e = MakeThing(false);
    CBaseIntelligentPointer scan[6];
    CThing* items[6] = { a, b, 0, c, d, e };
    for (int i = 0; i < 6; ++i) { scan[i].m_vtable = 0; scan[i].m_pItem = items[i]; }
    C3DVector from = { 1, 2, 3 }, to = { 4, 5, 6 }, hp, hn; float dist; CThing* hit;
    // empty scan list: IsLineOfSight with no things
    tools.ScanThings._M_start = scan; tools.ScanThings._M_finish = scan; tools.ScanThings._M_end_of_storage = scan;
    CHECK(!tools.IsLineOfSightFiltered(from, to, &hp, &hn, &dist, &hit, 2, true));
    CHECK(g_los == 1 && g_seenCount == 0 && g_overflows == 0 && g_getCalls == 0);
    // live things only (b is dead, the null pointer is skipped); arguments pass straight through
    tools.ScanThings._M_finish = scan + 5;
    CHECK(tools.IsLineOfSightFiltered(from, to, &hp, &hn, &dist, &hit, 5, false));
    CHECK(g_los == 2 && g_seenCount == 3 && g_seen[0] == a && g_seen[1] == c && g_seen[2] == d);
    CHECK(g_tools == &tools && g_from == &from && g_to == &to && g_hitPos == &hp && g_hitNormal == &hn && g_dist == &dist && g_hitThing == &hit);
    CHECK(g_test == 5 && !g_flag);
    CHECK(g_getCalls == 3 * 3 + 2 + 1);    // live: 3 calls each; dead: 2; null: 1
    // growth: capacity 1 -> 2 -> 4 takes three overflow calls for five things, the fourth placement-stored
    g_overflows = 0;
    tools.ScanThings._M_finish = scan + 6;
    CHECK(!tools.IsLineOfSightFiltered(from, to, &hp, &hn, &dist, &hit, 1, true));
    CHECK(g_seenCount == 4 && g_seen[0] == a && g_seen[1] == c && g_seen[2] == d && g_seen[3] == e && g_overflows == 3);
    if (fails == 0) printf("00877c40_TEST PASS\n");
    return fails;
}
