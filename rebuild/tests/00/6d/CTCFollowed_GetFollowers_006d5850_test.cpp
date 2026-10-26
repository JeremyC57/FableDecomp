// Behaviour test for 006d5850: src/006d5850.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCFollowed::GetFollowers(CRetailVector<CThing*>*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCFollowed::GetFollowers(CRetailVector<CThing*>*) -- culls the dead entries of the follower list, then appends every follower thing to the caller's vector
// Leaf: GetFollowers
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D5850, 90 bytes (`ret 4`).
// Callees: CTCFollowed::CullNullFollowers @ 0x006D5260, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the retail vector's
// reallocating insert @ 0x00436AC0 (labelled "Vector_InsertN").
// Label: the chunk label "CParticleSystem::ClearAllDecals" is propagated; the body is CTCFollowed's (CullNullFollowers on
// `this`, Followers = list<CIntelligentPointer<CThing>> at +0x0c, PDB +12). Leaf named by role.
// Shape: STLport list walk (node data at +8), `out->push_back(follower.Get())` with the retail push_back inlined:
// the rvalue item is homed in the dead argument slot (`mov [esp+10h],eax`) and handed by reference to the
// grow path together with the empty SScalarTag temporary.
#include <new>

class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {                     // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T> struct CRetailList {              // STLport list: one word, the sentinel node
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SListNode<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
};

struct SScalarTag { SScalarTag() { } };

template <class T> struct CRetailVector {            // retail std::vector: first, last, end
    T* first;
    T* last;
    T* end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

class CTCFollowed { public:
    void*                                         m_vtable;     // +0x00
    CThing*                                       m_pThing;     // +0x04
    unsigned char                                 m_pad08[0x04];
    CRetailList< CIntelligentPointer<CThing> >    Followers;    // +0x0c
    void CullNullFollowers();                                   // 0x006D5260
    void GetFollowers(CRetailVector<CThing*>* out);
};

void CTCFollowed::GetFollowers(CRetailVector<CThing*>* out)
{
    CullNullFollowers();
    for (CRetailList< CIntelligentPointer<CThing> >::iterator it = Followers.begin(); it != Followers.end(); ++it)
        out->push_back((*it).Get());
}

// ---- stubs and driver ----
static CThing* g_store[8]; static int g_grow = 0, g_cull = 0, g_get = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CRetailVector<CThing*>* v = (CRetailVector<CThing*>*)vector; CThing** p = v->last; v->last += count; if (v->end < v->last) v->end = v->last; return p;
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_get; return (CBaseObject*)m_pBlock; }
void CTCFollowed::CullNullFollowers() { ++g_cull; }
int main()
{
    SListNode< CIntelligentPointer<CThing> > head, n[3];
    head._M_next = &n[0]; n[0]._M_next = &n[1]; n[1]._M_next = &n[2]; n[2]._M_next = &head;
    n[0]._M_data.m_pBlock = (void*)0x10; n[1]._M_data.m_pBlock = (void*)0x20; n[2]._M_data.m_pBlock = (void*)0x30;
    CTCFollowed f; f.Followers._M_node = &head;
    CRetailVector<CThing*> out; out.first = g_store; out.last = g_store; out.end = g_store + 2;
    f.GetFollowers(&out);
    CHECK(g_cull == 1 && g_get == 3); CHECK(out.last == g_store + 3 && g_grow == 1);
    CHECK(g_store[0] == (CThing*)0x10 && g_store[1] == (CThing*)0x20 && g_store[2] == (CThing*)0x30);
    head._M_next = &head; f.GetFollowers(&out); CHECK(g_cull == 2 && g_get == 3 && out.last == g_store + 3);
    if (fails == 0) printf("006d5850_TEST PASS\n"); else printf("006d5850_TEST FAIL\n");
    return fails;
}