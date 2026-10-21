// Behaviour test for 00cb7c40: src/00cb7c40.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CScriptBase::RunSpawnedFunctions()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CScriptBase::RunSpawnedFunctions() -- step every spawned script thread once and drop the threads that report they are finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7C40, length 107 bytes (+5 int3 padding).
// PDB: CScriptBase::Threads is list<CCountedPointer<CSpawnedFuncBase> > (retail: the STLport head-node pointer at
// +0x04). Callees: CScriptBase::UpdateSpawnedFunction(CSpawnedFuncBase*) @ 0x00CB7950 (true = finished),
// operator delete @ 0x00BFE9BC (the count block), free @ 0x00BFEA14 (the list node, STLport malloc allocator).
// Shape: `it = Threads.erase(it)` / `++it` loop with end() re-read every pass; STLport's inline erase unlinks
// (next in edi), runs the counted pointer's inline destructor (null test, `dec [count]`, re-read, destroy +
// delete, both words zeroed; one zero register ebp) and frees the node.
struct CSpawnedFuncBase;

struct SCountBlock {
    long References;                                 // +0
    void (__fastcall* Destroy)(void*);               // +4
    void* Object;                                    // +8
};

template <class T> struct CCountedPointer {
    T* Data;
    T* GetPtr() const { return Data; }
    SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->References;
            if (Info->References == 0)
            {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

extern "C" void __cdecl free(void* p);              // 0x00BFEA14

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> struct list {                     // STLport list: the head node pointer
    struct _Node_base { _Node_base* _M_next; _Node_base* _M_prev; };
    struct _Node : _Node_base { T _M_data; };
    struct iterator {
        _Node_base* _M_node;
        iterator(_Node_base* n) : _M_node(n) {}
        T& operator*() const { return ((_Node*)_M_node)->_M_data; }
        T* operator->() const { return &((_Node*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator position)
    {
        _Node_base* next = position._M_node->_M_next;
        _Node_base* prev = position._M_node->_M_prev;
        _Node* n = (_Node*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        _Destroy(&n->_M_data);
        free(n);
        return iterator(next);
    }
};

class CScriptBase {
public:
    virtual ~CScriptBase();
    void RunSpawnedFunctions();
    bool UpdateSpawnedFunction(CSpawnedFuncBase* thread);   // 0x00CB7950
private:
    list<CCountedPointer<CSpawnedFuncBase> > Threads;      // +0x04
};

void CScriptBase::RunSpawnedFunctions()
{
    for (list<CCountedPointer<CSpawnedFuncBase> >::iterator it = Threads.begin(); it != Threads.end(); )
    {
        if (UpdateSpawnedFunction(it->GetPtr()))
            it = Threads.erase(it);
        else
            ++it;
    }
}

// ---- stubs and driver ----

#include <stdlib.h>
static int g_destroyed = 0;
static void __fastcall DestroyFn(void*) { ++g_destroyed; }
static CSpawnedFuncBase* g_finished[8]; static int g_nf = 0; static int g_updates = 0;
bool CScriptBase::UpdateSpawnedFunction(CSpawnedFuncBase* thread)
{
    ++g_updates;
    for (int i = 0; i < g_nf; ++i) if (g_finished[i] == thread) return true;
    return false;
}
CScriptBase::~CScriptBase() {}
typedef list<CCountedPointer<CSpawnedFuncBase> > TList;
static TList::_Node* Push(TList::_Node_base* head, CSpawnedFuncBase* f, long refs)
{
    TList::_Node* n = (TList::_Node*)malloc(sizeof(TList::_Node));
    n->_M_data.Data = f; n->_M_data.Info = (SCountBlock*)malloc(sizeof(SCountBlock));
    n->_M_data.Info->References = refs; n->_M_data.Info->Destroy = DestroyFn; n->_M_data.Info->Object = f;
    n->_M_next = head; n->_M_prev = head->_M_prev; head->_M_prev->_M_next = n; head->_M_prev = n; return n;
}
static int Count(TList::_Node_base* h) { int c = 0; for (TList::_Node_base* n = h->_M_next; n != h; n = n->_M_next) ++c; return c; }
struct CTestScript : CScriptBase { TList& Threads_() { return *(TList*)((char*)this + 4); } };
int main()
{
    static TList::_Node_base head; head._M_next = &head; head._M_prev = &head;
    static unsigned char ss[16]; memset(ss, 0, sizeof ss); CScriptBase* s = (CScriptBase*)ss;
    *(TList::_Node_base**)(ss + 4) = &head;
    CSpawnedFuncBase* a = (CSpawnedFuncBase*)0x10; CSpawnedFuncBase* b = (CSpawnedFuncBase*)0x20; CSpawnedFuncBase* c = (CSpawnedFuncBase*)0x30;
    Push(&head, a, 1); Push(&head, b, 2); Push(&head, c, 1);
    g_finished[g_nf++] = b; g_finished[g_nf++] = c;
    s->RunSpawnedFunctions();
    CHECK(g_updates == 3); CHECK(Count(&head) == 1); CHECK(((TList::_Node*)head._M_next)->_M_data.Data == a);
    CHECK(g_destroyed == 1);                       // c's last reference released; b still referenced elsewhere
    g_finished[g_nf++] = a; s->RunSpawnedFunctions(); CHECK(Count(&head) == 0); CHECK(g_destroyed == 2);
    s->RunSpawnedFunctions(); CHECK(g_updates == 4);
    if (fails == 0) printf("00cb7c40_TEST PASS\n"); else printf("00cb7c40_TEST FAIL\n");
    return fails;
}