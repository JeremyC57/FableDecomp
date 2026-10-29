// Behaviour test for 00536a30: src/00536a30.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NUISystem::CList::Die()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CList::Die() -- leave the list registry (when the def asks for it), unregister and kill every scrolling child, then the component's Die
#pragma optimize("y", off)
#pragma optimize("s", on)
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CRefCounted {                               // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CComponentDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    CRefCounted   Ref;                             // +0x04
    unsigned char _pad08[0x20a - 0x08];
    bool          RegisterInListRegistry;          // +0x20a
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* node) : _M_node(node) { }
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) { }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class T> struct list {
    _List_node<T>* _M_node;
    _List_iterator<T> begin() { return _List_iterator<T>(_M_node->_M_next); }
    _List_iterator<T> end() { return _List_iterator<T>(_M_node); }
    _List_iterator<T> erase(_List_iterator<T> position);   // 0x0053C7B9
};
namespace NUISystem {
class CList;
class CComponent;
}
_List_iterator<NUISystem::CList*> __fastcall find(NUISystem::CList* const& value, _List_iterator<NUISystem::CList*> first,
                                                  _List_iterator<NUISystem::CList*> last);   // 0x0053C8D0
#pragma auto_inline(off)
// Same-TU body of the STLport list find instantiation 0x0053C8D0 (template bodies are compiled in the caller's TU): walks
// the list comparing each entry with the value read through its const reference.
__declspec(noinline) _List_iterator<NUISystem::CList*> __fastcall find(NUISystem::CList* const& value, _List_iterator<NUISystem::CList*> first, _List_iterator<NUISystem::CList*> last)
{
    for (; first != last; first._M_node = first._M_node->_M_next)
        if (first._M_node->_M_data == value)
            return first;
    return last;
}
#pragma auto_inline(on)
extern list<NUISystem::CList*> g_ListRegistry;     // 0x013B8A98
struct CInputListener {                            // the input-listener base at +4
    virtual void v00();
    unsigned char _pad04[0x14 - 0x04];
};
namespace NUISystem { class CManager { public:
public:
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveInputListener(CInputListener* listener);   // slot 5 (+0x14)
}; }
namespace NUISystem {
struct CComponentRoot {                            // the primary vtable at +0
    virtual ~CComponentRoot();
};
class CComponent : public CComponentRoot, public CInputListener {
public:
    V16(a) V16(b) V16(c) V16(d) V16(e) V16(f) V4(g0) V4(g1) virtual void h0(); virtual void h1(); virtual void h2();
    virtual CDefPointer<CComponentDef> GetDef();   // slot 108 (+0x1b0) 0x00530EC0
    virtual void Die();                            // slot 109 (+0x1b4) 0x00530720 (CComponent's)
    unsigned char _pad18[0x164 - 0x18];
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
class CList : public CComponent {
public:
    vector<CComponent*> ScrollingChildren;         // +0x164
    virtual void Die();
};

void CList::Die()
{
    bool registered = GetDef()->RegisterInListRegistry;
    if (registered) {
        _List_iterator<CList*> it = find(this, g_ListRegistry.begin(), g_ListRegistry.end());
        if (it != g_ListRegistry.end())
            it = g_ListRegistry.erase(it);
    }
    for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
        CManager::GetInstance()->RemoveInputListener(ScrollingChildren[i]);
        ScrollingChildren[i]->Die();
    }
    CComponent::Die();
}
}

// ---- stubs and driver ----
#define D4(n) void CComponent::n##0() {} void CComponent::n##1() {} void CComponent::n##2() {} void CComponent::n##3() {}
#define D16(n) D4(n##a) D4(n##b) D4(n##c) D4(n##d)
using namespace NUISystem;
static int g_erase = 0, g_remove = 0, g_childDie = 0, g_baseDie = 0, g_getdef = 0, g_destroy = 0;
static CComponentDef g_def; static NUISystem::CManager g_fe; static NUISystem::CComponent* g_child = 0;
list<CList*> g_ListRegistry;
template <> _List_iterator<CList*> list<CList*>::erase(_List_iterator<CList*> position)
{ ++g_erase; _List_node<CList*>* next = position._M_node->_M_next; position._M_node->_M_prev->_M_next = next; next->_M_prev = position._M_node->_M_prev; return _List_iterator<CList*>(next); }
void CComponentDef::v00() {}
void CComponentDef::Destroy() { ++g_destroy; }
void CInputListener::v00() {}
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_fe; }
void NUISystem::CManager::v00() {} void NUISystem::CManager::v01() {} void NUISystem::CManager::v02() {} void NUISystem::CManager::v03() {} void NUISystem::CManager::v04() {}
void NUISystem::CManager::RemoveInputListener(CInputListener*) { ++g_remove; }
namespace NUISystem {
CComponentRoot::~CComponentRoot() {}
D16(a) D16(b) D16(c) D16(d) D16(e) D16(f) D4(g0) D4(g1)
void CComponent::h0() {} void CComponent::h1() {} void CComponent::h2() {}
CDefPointer<CComponentDef> CComponent::GetDef() { ++g_getdef; ++g_def.Ref.Count; static void* raw[1]; CDefPointer<CComponentDef>* p = (CDefPointer<CComponentDef>*)raw; p->Object = &g_def; return *p; }
void CComponent::Die() { if (this == g_child) ++g_childDie; else ++g_baseDie; }
}
int main()
{
    g_def.Ref.Count = 1; g_def.RegisterInListRegistry = true;
    static CList listA, listB; static CComponent child; g_child = &child;
    static _List_node<CList*> head, na, nb; g_ListRegistry._M_node = &head;
    head._M_next = &na; na._M_prev = &head; na._M_next = &nb; nb._M_prev = &na; nb._M_next = &head; head._M_prev = &nb; na._M_data = &listA; nb._M_data = &listB;
    CComponent* kids[2] = { &child, &child }; listB.ScrollingChildren._M_start = kids; listB.ScrollingChildren._M_finish = kids + 2;
    listB.CList::Die();
    CHECK(g_getdef == 1 && g_def.Ref.Count == 1 && g_erase == 1 && na._M_next == &head && head._M_prev == &na);
    CHECK(g_remove == 2 && g_childDie == 2 && g_baseDie == 1);
    listB.CList::Die(); CHECK(g_erase == 1 && g_baseDie == 2);   // no longer registered: not found
    g_def.RegisterInListRegistry = false; listA.ScrollingChildren._M_start = listA.ScrollingChildren._M_finish = 0;
    listA.CList::Die(); CHECK(g_erase == 1 && head._M_next == &na && g_baseDie == 3);
    if (fails == 0) printf("00536a30_TEST PASS\n"); else printf("00536a30_TEST FAIL\n");
    return fails;
}