#include <cstdio>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
static int g_handleDtors, g_decalDtors, g_nPooled, g_nCleared;
static void* g_pooled[8]; static void* g_cleared[8];
namespace NParticleEngine {

template <class T> inline void _Destroy(T* first, T* last)   // STLport range destroy (bounds by value)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct vector {             // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

class CEnginePrimitiveHandle {                 // 8 bytes, polymorphic
public:
    virtual ~CEnginePrimitiveHandle() { ++g_handleDtors; }
    void* Data;
};

class CDecal {                                 // 0x74 bytes in retail, vtable 0x0129F4FC
public:
    virtual void Delete() {}
    unsigned char                   _body04[0x58];
    vector<vector<long> >           PrimIDList;        // +0x5c CArray<CArray<long> >
    vector<CEnginePrimitiveHandle>  ParentHandleList;  // +0x68 CArray<CEnginePrimitiveHandle>
    ~CDecal() { ++g_decalDtors; }
    static void FASTCALL operator delete(void* p) { g_pooled[g_nPooled++] = p; }
};

template <class T> struct list {               // STLport list: the 4-byte node pointer
    struct _Node { _Node* _M_next; _Node* _M_prev; T _M_data; };
    struct iterator {
        _Node* _M_node;
        T& operator*() const { return _M_node->_M_data; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Node* _M_node;
    iterator begin() const { iterator it; it._M_node = _M_node->_M_next; return it; }
    iterator end() const { iterator it; it._M_node = _M_node; return it; }
    iterator erase(iterator position)
    {
        _Node* next = position._M_node->_M_next;
        _Node* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position._M_node);
        iterator it; it._M_node = next; return it;
    }
    void clear()
    {
        _Node* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

class CParticle;

class CParticleSystem {
public:
    virtual ~CParticleSystem() {}
    unsigned char           _pad04[0x18];
    list<CParticle*>        Particles;             // +0x1c
    list<CDecal*>           Decals;                // +0x20
    unsigned char           _pad24[0x28];
    long                    NoActiveDecals;        // +0x4c
    void ClearDecal(list<CDecal*>::iterator& it);  // 0x00AF5430
    list<CDecal*>::iterator DeleteDecal(list<CDecal*>::iterator& it)
    {
        ClearDecal(it);
        delete *it;
        list<CDecal*>::iterator next = Decals.erase(it);
        --NoActiveDecals;
        return next;
    }
    void DeleteAllDecals();
};

// Stand-in for the same-TU ClearDecal 0x00AF5430 (it sits just above this function in retail): VC7.1 uses the facts of a
// callee compiled earlier in the TU, here that ClearDecal only reads the iterator, so the node stays cached in ebx across the
// call. The real body releases the decal's primitives through *it; this stand-in keeps only that read and an opaque call.
void ReleaseDecalPrimitives(CParticleSystem* system, CDecal* decal) { (void)system; g_cleared[g_nCleared++] = decal; }
void CParticleSystem::ClearDecal(list<CDecal*>::iterator& it)
{
    ReleaseDecalPrimitives(this, *it);
}

void CParticleSystem::DeleteAllDecals()
{
    list<CDecal*>::iterator it = Decals.begin();
    while (it != Decals.end())
        it = DeleteDecal(it);
    Decals.clear();
    NoActiveDecals = 0;
}

} // namespace NParticleEngine

using namespace NParticleEngine;
typedef list<CDecal*>::_Node Node;
int main()
{
    Node* head = (Node*)std::malloc(sizeof(Node)); head->_M_next = head->_M_prev = head;
    static unsigned char raw[3][sizeof(CDecal)];
    CDecal* d[3];
    for (int i = 0; i < 3; ++i) {
        d[i] = new (raw[i]) CDecal;
        d[i]->PrimIDList._M_start = d[i]->PrimIDList._M_finish = d[i]->PrimIDList._M_end_of_storage = 0;
        d[i]->ParentHandleList._M_start = d[i]->ParentHandleList._M_finish = d[i]->ParentHandleList._M_end_of_storage = 0;
    }
    /* decal 1 owns two handles and one inner id list */
    CEnginePrimitiveHandle* h = (CEnginePrimitiveHandle*)std::malloc(2 * sizeof(CEnginePrimitiveHandle));
    new (&h[0]) CEnginePrimitiveHandle; new (&h[1]) CEnginePrimitiveHandle;
    d[1]->ParentHandleList._M_start = h; d[1]->ParentHandleList._M_finish = h + 2; d[1]->ParentHandleList._M_end_of_storage = h + 2;
    vector<long>* ids = (vector<long>*)std::malloc(sizeof(vector<long>));
    ids->_M_start = (long*)std::malloc(4 * sizeof(long)); ids->_M_finish = ids->_M_start + 4; ids->_M_end_of_storage = ids->_M_finish;
    d[1]->PrimIDList._M_start = ids; d[1]->PrimIDList._M_finish = ids + 1; d[1]->PrimIDList._M_end_of_storage = ids + 1;
    CDecal* items[4] = { d[0], 0, d[1], d[2] };
    for (int i = 0; i < 4; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node)); n->_M_data = items[i];
        n->_M_prev = head->_M_prev; n->_M_next = head; head->_M_prev->_M_next = n; head->_M_prev = n;
    }
    static unsigned char sysraw[sizeof(CParticleSystem)];
    CParticleSystem* s = new (sysraw) CParticleSystem;
    s->Decals._M_node = head; s->NoActiveDecals = 4;
    s->DeleteAllDecals();
    if (g_nCleared != 4 || g_cleared[0] != d[0] || g_cleared[1] != 0 || g_cleared[3] != d[2]) { std::printf("00af5770_TEST FAIL\n"); return 1; }
    if (g_decalDtors != 3 || g_nPooled != 3 || g_pooled[0] != d[0] || g_pooled[1] != d[1] || g_handleDtors != 2) { std::printf("00af5770_TEST FAIL\n"); return 1; }
    if (head->_M_next != head || head->_M_prev != head || s->NoActiveDecals != 0) { std::printf("00af5770_TEST FAIL\n"); return 1; }
    std::printf("00af5770_TEST PASS\n");
    return 0;
}
