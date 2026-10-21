// CTCCarrying::OnSetInLimbo(bool inLimbo) -- passes the limbo state on to everything the thing carries: collects the carried things and calls CThing::SetInLimbo(inLimbo) on each
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A2370, length 97 bytes (`ret 4`); vtable 0x012433F4 slot 16 (rtti-vtables.tsv:
// CTCCarrying; slot 16 is OnSetInLimbo in the CTC family; PDB `?OnSetInLimbo@CTCCarrying@@UAEX_N@Z`). The manifest's
// "CEngineLandscapeMap::RenderBackground" is propagated and wrong.
// Callees: CTCCarrying::GetAllCarriedThings(vector<CThing*>&) const @ 0x006A2220 (manifest "COptimisedPrimitive::RemapGroups"),
// SGI for_each @ 0x00528720 (/Gr `__fastcall`: hidden functor result in ecx, first in edx, last + the 8-byte binder on the stack;
// manifest "_For_each<CTargetPoint*,...>" is a byte-identical body), CThing::SetInLimbo(bool) @ 0x004C8CF0 (the bound member
// pointer, pushed as an immediate), free @ 0x00BFEA14 (vector storage).
// Shape: `for_each(v.begin(), v.end(), bind2nd(mem_fun(&CThing::SetInLimbo), inLimbo))` -- the binder is {member pointer, bool}
// built from the immediate and a byte frame word; the vector is a zero-initialised 12-byte local freed by its destructor.
// The same-TU GetAllCarriedThings (0x006A2220, just above in retail) needs a body that READS and WRITES the vector's
//   finish/end words (append, growing through a stand-in allocator): with only a declaration, or a stand-in writing word 0,
//   VC7.1 drops the caller's zero store of the finish word (diff 26 -> 3 -> 0).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CThing {
public:
    void SetInLimbo(bool inLimbo);                 // 0x004C8CF0
};

template <class T>
struct CRetailVector {                             // STLport vector<T*>: start, finish, end of storage
    T* m_pStart;
    T* m_pFinish;
    T* m_pEndOfStorage;
    CRetailVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~CRetailVector()
    {
        if (m_pStart)
            free(m_pStart);
    }
    T* begin() const { return m_pStart; }
    T* end() const { return m_pFinish; }
};

template <class R, class T, class A>
struct mem_fun1_t {                                // std::mem_fun1_t
    R (T::*m_pmf)(A);
    explicit mem_fun1_t(R (T::*pmf)(A)) : m_pmf(pmf) { }
    R operator()(T* p, A a) const { return (p->*m_pmf)(a); }
};

template <class R, class T, class A>
inline mem_fun1_t<R, T, A> mem_fun(R (T::*pmf)(A)) { return mem_fun1_t<R, T, A>(pmf); }

template <class Op, class A>
struct binder2nd {                                 // std::binder2nd: the operation and the bound second argument
    Op op;
    A  value;
    binder2nd(const Op& o, const A& v) : op(o), value(v) { }
};

template <class Op, class A>
inline binder2nd<Op, A> bind2nd(const Op& op, const A& value) { return binder2nd<Op, A>(op, value); }

template <class It, class F>
F __fastcall for_each(It first, It last, F f);     // 0x00528720 (SGI for_each, /Gr; manifest label propagated)

class CTCCarrying {
public:
    void GetAllCarriedThings(CRetailVector<CThing*>& things) const;   // 0x006A2220
    void OnSetInLimbo(bool inLimbo);
};
void* GrowCarriedStorage_s7(void* old, unsigned long size, unsigned long capacity);   // stand-in: realloc
CThing* NextCarriedThing_s7(const CTCCarrying* carrying, long index);   // stand-in: the carried slot's GetPItem
#pragma auto_inline(off)
// Same-TU stand-in for GetAllCarriedThings (0x006A2220, compiled just above this function in retail): append each carried
// thing to the caller's vector in place (the retail body's inline push_back; growth left out). It reads and writes the
// vector's finish / end-of-storage words, so VC7.1 keeps the caller's zero stores and knows the vector does not escape.
__declspec(noinline) void CTCCarrying::GetAllCarriedThings(CRetailVector<CThing*>& things) const
{
    for (long i = 0; ; ++i) {
        CThing* thing = NextCarriedThing_s7(this, i);
        if (!thing)
            return;
        if (things.m_pFinish == things.m_pEndOfStorage) {         // growth: copy into a larger block (stand-in allocator)
            unsigned long size = things.m_pFinish - things.m_pStart;
            CThing** block = (CThing**)GrowCarriedStorage_s7(things.m_pStart, size, size * 2 + 4);
            things.m_pStart = block;
            things.m_pFinish = block + size;
            things.m_pEndOfStorage = block + size * 2 + 4;
        }
        *things.m_pFinish = thing;
        ++things.m_pFinish;
    }
}
#pragma auto_inline(on)

void CTCCarrying::OnSetInLimbo(bool inLimbo)
{
    CRetailVector<CThing*> things;
    GetAllCarriedThings(things);
    for_each(things.begin(), things.end(), bind2nd(mem_fun(&CThing::SetInLimbo), inLimbo));
}