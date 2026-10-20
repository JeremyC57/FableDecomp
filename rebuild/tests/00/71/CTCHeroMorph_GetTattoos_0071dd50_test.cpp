// Behaviour test for 0071dd50: src/0071dd50.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCHeroMorph::GetTattoos(vector<CDefPointer<const CTattooDef> >& tattoos) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroMorph::GetTattoos(vector<CDefPointer<const CTattooDef> >& tattoos) const -- refill the caller's vector with the tattoo sub-defs of the hero's tattoo thing defs
// Leaf: GetTattoos
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071DD50, 196 bytes (`ret 4`). The catalog row says "CTCObjectAugmentations::GetDamageMultiplier"
//   (propagated): `this` holds a vector of CThingBaseDef pointers at +0x1c (CTCHeroMorph::Tattoos, PDB +0x2c: the two lists
//   before it are 4 bytes in retail), the callers are CTCHeroMorph/CTCInventoryTrade code (0x0057D42F, 0x0059FCE6) and the PDB
//   has ?GetTattoos@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@...@@@Z.
// Callees: Dinkumware vector clear = __fastcall _Move @ 0x0071FB80 + __fastcall _Destroy_range @ 0x00580A0B (labels propagated),
//   reserve @ 0x00721800, CDefClassBase::GetSubDef<const CTattooDef> @ 0x005B1EFC (labelled GetSubDef<CAbilityDef>), InsertGrow
//   @ 0x00721B10, the def release through vtable slot 1.
// Tried: holder-released def (speed), the same under optimize("s") (everything out of line, frame pointer).
//   Next: lane gap: one dead parameter slot shared by the push_back value and its tag (GOTCHAS chunks 203-204).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CDefBase : CDefVirtual {
    mutable long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    T* operator->() const { return Object; }
};
template <class T> struct CDefHolder {                  // owns the reference GetSubDef handed over
    T* Object;
    __forceinline ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};
struct CTattooDef : CDefBase { };
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x005B1EFC (T = const CTattooDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CThingBaseDef : CDefClassBase { };
struct SMoveTag { SMoveTag() { } };
struct SScalarTag { SScalarTag() { } };
typedef CDefPointer<const CTattooDef> CTattooDefPointer;
CTattooDefPointer* __fastcall Move_0071fb80(CTattooDefPointer* first, CTattooDefPointer* last, CTattooDefPointer* dest,
                                            const SMoveTag& tag, bool overlap);   // 0x0071FB80
void __fastcall Destroy_00580a0b(CTattooDefPointer* first, CTattooDefPointer* last, const SScalarTag& allocator);   // 0x00580A0B
inline void* operator new(unsigned int, void* where) { return where; }
struct CTattooDefVector {                               // VC6-era Dinkumware vector {first, last, end}
    CTattooDefPointer* m_first;
    CTattooDefPointer* m_last;
    CTattooDefPointer* m_end;
    void reserve(unsigned long count);                  // 0x00721800
    void InsertGrow(CTattooDefPointer* where, const CTattooDefPointer& value, const SScalarTag& tag,
                    unsigned long count, bool noTail);   // 0x00721B10
    void clear()
    {
        CTattooDefPointer* last = m_last;
        CTattooDefPointer* first = m_first;
        CTattooDefPointer* kept = Move_0071fb80(last, last, first, SMoveTag(), false);
        Destroy_00580a0b(kept, m_last, SScalarTag());
        m_last = kept;
    }
    void push_back(const CTattooDefPointer& value)
    {
        if (m_last != m_end) {
            new (m_last) CTattooDefPointer(value);
            ++m_last;
        } else
            InsertGrow(m_last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (InsertGrow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
__declspec(noinline) void CTattooDefVector::InsertGrow(CTattooDefPointer* where, const CTattooDefPointer& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    CTattooDefPointer* p = (CTattooDefPointer*)GrowVectorStorage_s7(this, count);
    if (p)
        new (p) CTattooDefPointer(value);
}
#pragma auto_inline(on)
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CThing;
struct CTCHeroMorph {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    vector< CDefPointer<const CThingBaseDef> > Tattoos; // +0x1c (PDB +0x2c)
    void GetTattoos(CTattooDefVector& tattoos) const;
};

void CTCHeroMorph::GetTattoos(CTattooDefVector& tattoos) const
{
    tattoos.clear();
    tattoos.reserve(Tattoos.size());
    for (unsigned long i = 0; i < Tattoos.size(); ++i) {
        CTattooDefPointer slot;
        Tattoos[i]->GetSubDef(slot);
        CDefHolder<const CTattooDef> tattoo = { slot.Object };
        tattoos.push_back(slot);
    }
}

// ---- stubs and driver ----
static CTattooDef g_defs[3]; static int g_findIndex = 0, g_find = 0, g_release = 0, g_destroy = 0, g_move = 0, g_destroyRange = 0, g_reserve = 0, g_grow = 0;
static unsigned long g_reserveCount = 0; static unsigned char g_raw[8 * sizeof(CTattooDefPointer)];
static void* g_results[4];
void* FindDef_s7r_0() { return g_results[g_find++ & 3]; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() const { ++g_destroy; }
void CTCHeroMorph::v00() {}
CTattooDefPointer* __fastcall Move_0071fb80(CTattooDefPointer* first, CTattooDefPointer* last, CTattooDefPointer* dest, const SMoveTag&, bool overlap)
{ ++g_move; CHECK(!overlap); CHECK(first == last); return dest + (last - first); }
void __fastcall Destroy_00580a0b(CTattooDefPointer* first, CTattooDefPointer* last, const SScalarTag&) { ++g_destroyRange; for (; first != last; ++first) if (first->Object) --first->Object->m_refCount; }
void CTattooDefVector::reserve(unsigned long count) { ++g_reserve; g_reserveCount = count; }
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{ ++g_grow; CTattooDefVector* v = (CTattooDefVector*)storage; CTattooDefPointer* p = v->m_last; v->m_last += count; if (v->m_end < v->m_last) v->m_end = v->m_last; return p; }
int main()
{
    CTattooDefPointer* store = (CTattooDefPointer*)g_raw;
    for (int i = 0; i < 3; ++i) g_defs[i].m_refCount = 1;
    static CThingBaseDef things[3]; CDefPointer<const CThingBaseDef> tattooThings[3];
    for (int i = 0; i < 3; ++i) { things[i].m_refCount = 1; tattooThings[i].Object = &things[i]; }
    static unsigned char raw[sizeof(CTCHeroMorph)]; CTCHeroMorph& m = *(CTCHeroMorph*)raw;
    m.Tattoos._M_start = tattooThings; m.Tattoos._M_finish = tattooThings + 3; m.Tattoos._M_end_of_storage = tattooThings + 3;
    CTattooDefVector out; out.m_first = store; out.m_last = store; out.m_end = store + 2;
    g_results[0] = &g_defs[0]; g_results[1] = 0; g_results[2] = &g_defs[2];
    m.GetTattoos(out);
    CHECK(g_move == 1 && g_destroyRange == 1 && g_reserve == 1 && g_reserveCount == 3 && g_find == 3);
    CHECK(out.m_last == store + 3 && g_grow == 1);
    CHECK(store[0].Object == &g_defs[0] && store[1].Object == 0 && store[2].Object == &g_defs[2]);
    CHECK(g_defs[0].m_refCount == 1 && g_defs[2].m_refCount == 1 && g_destroy == 0);   // +1 for the copy, -1 for the holder
    // refill: clear() keeps nothing (Move of an empty tail), the old entries' references go through Destroy_00580a0b
    g_find = 0; m.GetTattoos(out); CHECK(g_move == 2 && g_destroyRange == 2 && g_defs[0].m_refCount == 0 && out.m_last == store + 3 && g_grow == 1);   // the destroyed old entries dropped one reference each
    CHECK(g_release == 0);
    if (fails == 0) printf("0071dd50_TEST PASS\n"); else printf("0071dd50_TEST FAIL\n");
    return fails;
}