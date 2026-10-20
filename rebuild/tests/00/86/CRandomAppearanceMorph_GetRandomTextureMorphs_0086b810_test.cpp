// Behaviour test for 0086b810: src/0086b810.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long&, unsigned long, const CVectorMap<unsigned long, CTextureMorph>&, CArray<CMorphSet::CMorphSetTextureMorph>&) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long&, unsigned long, const CVectorMap<unsigned long, CTextureMorph>&, CArray<CMorphSet::CMorphSetTextureMorph>&) const -- for each original texture in the morph map, pick one of its morphs that is ungrouped or in the requested group at random and append (texture, new texture) to the result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B810, length 230 bytes, `ret 10h` (`this` unused).
// Leaf: GetRandomTextureMorphs
// Called by the landed GetRandomMorphSet (0x0086BF70) for the general and per-body-part texture morphs.
// Callees: CVectorMap<unsigned long, CTextureMorph>::UpperBound 0x0086A8D0 (the end of one key's run), GFROR13
// 0x00497890 (the engine LCG step: seed = seed * 0x24A1 + 0x24DF, then rotated), the result vector's grow path
// 0x0086CA20 (STLport _M_insert_overflow(pos, value, __true_type, 1, true)).
// Layout: map entries are 12 bytes {original texture, CTextureMorph {NewTexture, GroupID}}; the result entries 8 bytes
// {OriginalTexture, NewTexture}; retail vectors are {first, last, end}.
// Shape: the run [it, UpperBound(it->key)) is counted, a random index below the count chosen (`div`), and the run walked
// again to that entry; the record is a named local pushed through the inline push_back (placement copy when there is
// room, else the grow call with an empty tag temporary); the outer iterator jumps to the run's end.
#include <new>
struct SScalarTag { SScalarTag() {} };
template<class T> struct CArray {
    T* m_first; T* m_last; T* m_end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x0086CA20 (T = CMorphSetTextureMorph)
    void push_back(const T& value)
    {
        if (m_last != m_end) {
            new (m_last) T(value);
            ++m_last;
        }
        else
            InsertGrow(m_last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)
struct CKeyPairCompareLess {};
template<class K, class V> struct CVectorMap {
    struct SEntry { K Key; V Value; };
    CArray<SEntry> m_vector; CKeyPairCompareLess m_compare; bool m_sorted;
    SEntry* begin() const { return m_vector.m_first; }
    SEntry* end() const { return m_vector.m_last; }
    SEntry* UpperBound(const K& key) const;              // 0x0086A8D0 (K = unsigned long)
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890

struct CMorphSet {
    struct CMorphSetTextureMorph {
        unsigned long OriginalTexture;                   // +0x00
        unsigned long NewTexture;                        // +0x04
        CMorphSetTextureMorph(unsigned long original, unsigned long newTexture) : OriginalTexture(original), NewTexture(newTexture) {}
        CMorphSetTextureMorph(const CMorphSetTextureMorph& o) : OriginalTexture(o.OriginalTexture), NewTexture(o.NewTexture) {}
    };
};

struct CRandomAppearanceMorph {
    struct CTextureMorph {
        unsigned long NewTexture;                        // +0x00
        unsigned long GroupID;                           // +0x04
    };
    typedef CVectorMap<unsigned long, CTextureMorph> CTextureMorphMap;
    void GetRandomTextureMorphs(unsigned long& seed, unsigned long groupID, const CTextureMorphMap& morphs,
                                CArray<CMorphSet::CMorphSetTextureMorph>& result) const;
};

void CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long& seed, unsigned long groupID, const CTextureMorphMap& morphs,
                                                    CArray<CMorphSet::CMorphSetTextureMorph>& result) const
{
    CTextureMorphMap::SEntry* it = morphs.begin();
    while (it != morphs.end()) {
        CTextureMorphMap::SEntry* runEnd = morphs.UpperBound(it->Key);
        unsigned long count = 0;
        CTextureMorphMap::SEntry* e;
        for (e = it; e != runEnd; ++e)
            if (e->Value.GroupID == 0 || e->Value.GroupID == groupID)
                ++count;
        if (count) {
            seed = seed * 0x24A1 + 0x24DF;
            seed = GFROR13(seed);
            unsigned long pick = seed % count;
            unsigned long index = 0;
            for (e = it; e != runEnd; ++e) {
                if (e->Value.GroupID == 0 || e->Value.GroupID == groupID) {
                    if (index == pick) {
                        result.push_back(CMorphSet::CMorphSetTextureMorph(e->Key, e->Value.NewTexture));
                        break;
                    }
                    ++index;
                }
            }
        }
        it = runEnd;
    }
}

// ---- stubs and driver ----
typedef CRandomAppearanceMorph::CTextureMorphMap Map;
typedef CMorphSet::CMorphSetTextureMorph Morph;
static Morph* g_store; static int g_grow = 0, g_ub = 0, g_ror = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CArray<Morph>* v = (CArray<Morph>*)vector; Morph* p = v->m_last; v->m_last += count; if (v->m_end < v->m_last) v->m_end = v->m_last; return p;
}
template <> Map::SEntry* Map::UpperBound(const unsigned long& key) const
{ ++g_ub; SEntry* e = m_vector.m_first; while (e != m_vector.m_last && !(key < e->Key)) ++e; return e; }
unsigned long __fastcall GFROR13(unsigned long value) { ++g_ror; return (value >> 13) | (value << 19); }
static unsigned long Next(unsigned long& seed) { seed = seed * 0x24A1 + 0x24DF; seed = (seed >> 13) | (seed << 19); return seed; }
int main()
{
    static unsigned char raw[8 * sizeof(Morph)]; g_store = (Morph*)raw;
    Map::SEntry entries[6] = {
        { 1, { 100, 0 } }, { 1, { 101, 2 } }, { 1, { 102, 3 } },     // key 1: group 0 / 2 match for groupID 2
        { 4, { 400, 3 } },                                            // key 4: nothing matches
        { 7, { 700, 2 } }, { 7, { 701, 0 } } };
    Map morphs; morphs.m_vector.m_first = entries; morphs.m_vector.m_last = entries + 6; morphs.m_vector.m_end = entries + 6; morphs.m_sorted = true;
    CArray<Morph> result; result.m_first = g_store; result.m_last = g_store; result.m_end = g_store + 1;
    CRandomAppearanceMorph m;
    unsigned long seed = 12345, s2 = 12345;
    m.GetRandomTextureMorphs(seed, 2, morphs, result);
    CHECK(g_ub == 3); CHECK(g_ror == 2); CHECK(result.m_last == g_store + 2); CHECK(g_grow == 1);
    unsigned long p1 = Next(s2) % 2, p2 = Next(s2) % 2; CHECK(seed == s2);
    CHECK(g_store[0].OriginalTexture == 1 && g_store[0].NewTexture == (p1 ? 101u : 100u));
    CHECK(g_store[1].OriginalTexture == 7 && g_store[1].NewTexture == (p2 ? 701u : 700u));
    // groupID 3: key 1 has 2 matches (group 0, group 3), key 4 one, key 7 one (group 0)
    result.m_last = g_store; result.m_end = g_store + 8; seed = s2 = 99;
    m.GetRandomTextureMorphs(seed, 3, morphs, result);
    CHECK(g_ror == 5 && result.m_last == g_store + 3 && g_grow == 1);
    p1 = Next(s2) % 2; Next(s2); Next(s2); CHECK(seed == s2);
    CHECK(g_store[0].NewTexture == (p1 ? 102u : 100u)); CHECK(g_store[1].OriginalTexture == 4 && g_store[1].NewTexture == 400); CHECK(g_store[2].NewTexture == 701);
    // empty map: nothing
    morphs.m_vector.m_last = entries; seed = 5; m.GetRandomTextureMorphs(seed, 3, morphs, result); CHECK(seed == 5 && g_ror == 5);
    if (fails == 0) printf("0086b810_TEST PASS\n"); else printf("0086b810_TEST FAIL\n");
    return fails;
}