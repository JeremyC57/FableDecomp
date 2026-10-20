// Behaviour test for 00667b30: src/00667b30.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingCreatureBase::GetAppropriateCombatAnimation(const CCharString& name)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::GetAppropriateCombatAnimation(const CCharString& name) -- collect every appearance animation called `name` and return the one GetAppropriateCombatAnimationFromList picks
// Leaf: GetAppropriateCombatAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00667B30, length 204 bytes, `ret 4`, const CAnimationEntry* in eax.
// Label correction: the catalog/engine_api name "GetCombatAnimationSet" (returning CAnimationSet*) does not fit the body,
//   which returns the chosen vector element; the name used here is descriptive.
// Callees: CThingBaseDef::GetSubDef<CAppearanceDef> @ 0x0042B0A2, the animation set's lower bound by name @ 0x005DC020
//   (manifest "CVectorMap_LookupOrGetEnd"), the vector's insert-overflow @ 0x005E1420 (declared exactly as the landed
//   CRetailVector_dword::push_back 0x005E1EB0 declares it: the element is a pointer-sized dword), CThingCreatureBase::
//   GetAppropriateCombatAnimationFromList(const vector&) @ 0x00666B50, free @ 0x00BFEA14, the def's virtual Destroy.
// Layout: retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34 (a sorted vector of 0x14-byte {name key,
//   CAnimationEntry} records: first +0, last +4).
// Shape: the appearance def is released right after its animation set is addressed (null-tested local); the candidate
//   vector is a local STLport vector filled by the inline push_back with the entry address as an rvalue (homed in the
//   dead name slot, which the insert-overflow's scalar tag shares); the pick is `*(begin() + index)`.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct CCharString;
struct CAnimationEntry;
struct SAnimationSetEntry {                     // 0x14 bytes
    long Key;                                   // +0x00
    unsigned char Entry[0x10];                  // +0x04 CAnimationEntry
    long GetEntryAddress() const { return (long)Entry; }
};
struct CAnimationSet {
    SAnimationSetEntry* first;                  // +0x00
    SAnimationSetEntry* last;                   // +0x04
    SAnimationSetEntry* LowerBound_005dc020(const CCharString& name);   // 0x005DC020
};

struct SScalarTag { SScalarTag() { } };
struct CRetailVector_dword {                    // STLport vector<const CAnimationEntry*> seen as dwords
    long* first;
    long* last;
    long* end;
    CRetailVector_dword() : first(0), last(0), end(0) { }
    ~CRetailVector_dword() { if (first) free(first); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x005E1420
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
    const long& operator[](long n) const { return *(first + n); }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
__declspec(noinline) void CRetailVector_dword::InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    long* p = (long*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingCreatureBase {
public:
    unsigned char _pad00[0x70];
    CThingBaseDef* PBaseDef;                    // +0x70
    long GetAppropriateCombatAnimationFromList(const CRetailVector_dword& animations);   // 0x00666B50
    const CAnimationEntry* GetAppropriateCombatAnimation(const CCharString& name);
};

const CAnimationEntry* CThingCreatureBase::GetAppropriateCombatAnimation(const CCharString& name)
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(&out);
    CAppearanceDef* def = out.Object;
    CAnimationSet& animations = def->Animation;
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
    CRetailVector_dword candidates;
    SAnimationSetEntry* found = animations.LowerBound_005dc020(name);
    for (SAnimationSetEntry* it = found; it != animations.last && it->Key == found->Key; ++it)
        candidates.push_back(it->GetEntryAddress());
    long index = GetAppropriateCombatAnimationFromList(candidates);
    return (const CAnimationEntry*)candidates[index];
}

// ---- stubs and driver ----
extern "C" void* __cdecl realloc(void* block, unsigned int size);
static int g_grow = 0, g_find = 0, g_destroy = 0, g_lb = 0, g_pick = 0; static void* g_def = 0; static const CCharString* g_name = 0; static long g_pickIndex = 0;
static long g_seen[8]; static long g_seenCount = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CRetailVector_dword* v = (CRetailVector_dword*)vector; long n = v->last - v->first; long cap = (n + (long)count) * 2;
    v->first = (long*)realloc(v->first, cap * sizeof(long)); v->last = v->first + n + count; v->end = v->first + cap; return v->first + n;
}
void* FindDef_CThingBaseDef_GetSubDef_0() { ++g_find; return g_def; }
void CDefPointeeBase::Destroy0() {}
void CDefPointeeBase::Destroy() { ++g_destroy; }
SAnimationSetEntry* CAnimationSet::LowerBound_005dc020(const CCharString& name) { ++g_lb; g_name = &name; return first + 1; }
long CThingCreatureBase::GetAppropriateCombatAnimationFromList(const CRetailVector_dword& animations)
{ ++g_pick; g_seenCount = animations.last - animations.first; for (long i = 0; i < g_seenCount && i < 8; ++i) g_seen[i] = animations[i]; return g_pickIndex; }
int main()
{
    SAnimationSetEntry entries[5]; memset(entries, 0, sizeof(entries));
    entries[0].Key = 1; entries[1].Key = 3; entries[2].Key = 3; entries[3].Key = 3; entries[4].Key = 4;
    CAppearanceDef def; def.RefCount = 2; def.Animation.first = entries; def.Animation.last = entries + 5;
    g_def = &def;
    CThingBaseDef base; base.RefCount = 1; CThingCreatureBase c; c.PBaseDef = &base;
    const CCharString* name = (const CCharString*)&g_grow;
    g_pickIndex = 2;
    const CAnimationEntry* r = c.GetAppropriateCombatAnimation(*name);
    CHECK(g_find == 1 && def.RefCount == 1 && g_destroy == 0); CHECK(g_lb == 1 && g_name == name);
    CHECK(g_pick == 1 && g_seenCount == 3); CHECK(g_seen[0] == entries[1].GetEntryAddress() && g_seen[1] == entries[2].GetEntryAddress() && g_seen[2] == entries[3].GetEntryAddress());
    CHECK(r == (const CAnimationEntry*)entries[3].Entry); CHECK(g_grow >= 1);
    // last reference released: Destroy called; run reaching the end of the set
    entries[4].Key = 3; g_pickIndex = 3;
    r = c.GetAppropriateCombatAnimation(*name);
    CHECK(def.RefCount == 0 && g_destroy == 1); CHECK(g_seenCount == 4 && r == (const CAnimationEntry*)entries[4].Entry);
    if (fails == 0) printf("00667b30_TEST PASS\n"); else printf("00667b30_TEST FAIL\n");
    return fails;
}