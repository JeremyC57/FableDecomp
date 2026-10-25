// Behaviour test for 0x0070A490 (source body copied from src/0070a490.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif

struct CRGBColour { unsigned char B, G, R, A; };
class CCompositeBlendDesc {                     // Ego_r.pdb, 16 bytes
public:
    unsigned long TextureIndex;                 // +0x00
    long          BlendType;                    // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                // +0x08
    CRGBColour    SpecularColour;               // +0x0C
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

class CThing {
public:
    void ForwardToGraphicAppearance(long request);   // 0x004C7590
};

namespace _STL {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                   // +0x00
    _List_node_base* _M_prev;                   // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x08
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp>& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator end() { return this->_M_node._M_data; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

#pragma auto_inline(off)
// Same-TU stand-in for 0x005BA48A (memberwise; never keeps `other`).
__declspec(noinline) CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
    : TextureIndex(other.TextureIndex), BlendType(other.BlendType), DiffuseColour(other.DiffuseColour), SpecularColour(other.SpecularColour)
{
}
#pragma auto_inline(on)

class CTCTextureDecal {
public:
    class CEntry {                              // 0x18 bytes (not in the PDB)
    public:
        long AllocEntryID;                      // +0x00
        long Decal;                             // +0x04
        CCompositeBlendDesc Desc;               // +0x08
        CEntry(long id, long decal, const CCompositeBlendDesc& desc) : AllocEntryID(id), Decal(decal), Desc(desc) { }
    };
    void* __vftable;                            // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];
    _STL::list<CEntry> Decals;                  // +0x0c (4-byte STLport list)
    unsigned char EngineDecals[0xc];            // +0x10 vector<CEngineTextureDecal>
    long CurrentAllocEntryID;                   // +0x1c
    bool Dirty;                                 // +0x20
    long AddDecal(long decal, CCompositeBlendDesc& desc);
};

long CTCTextureDecal::AddDecal(long decal, CCompositeBlendDesc& desc)
{
    long id = CurrentAllocEntryID++;
    Decals.push_back(CEntry(id, decal, desc));
    Dirty = true;
    ParentThing->ForwardToGraphicAppearance(4);
    return id;
}

// ---- test harness ----
static long g_lastRequest = -1, g_requests = 0;
void CThing::ForwardToGraphicAppearance(long request) { g_lastRequest = request; ++g_requests; }
int main()
{
    int ok = 1;
    CThing thing;
    CTCTextureDecal td;
    memset(&td, 0, sizeof td);
    _STL::_List_node<CTCTextureDecal::CEntry>* sentinel = (_STL::_List_node<CTCTextureDecal::CEntry>*)malloc(sizeof(_STL::_List_node<CTCTextureDecal::CEntry>));
    sentinel->_M_next = sentinel->_M_prev = sentinel;
    td.Decals._M_node._M_data = sentinel;
    td.ParentThing = &thing;
    td.CurrentAllocEntryID = 5;
    CCompositeBlendDesc* d = (CCompositeBlendDesc*)malloc(sizeof(CCompositeBlendDesc));
    memset(d, 0, sizeof *d);
    d->TextureIndex = 77; d->BlendType = 3; d->DiffuseColour.G = 4;
    long id1 = td.AddDecal(1234, *d);
    d->TextureIndex = 88;
    long id2 = td.AddDecal(5678, *d);
    if (id1 != 5 || id2 != 6 || td.CurrentAllocEntryID != 7 || !td.Dirty || g_requests != 2 || g_lastRequest != 4) ok = 0;
    _STL::_List_node<CTCTextureDecal::CEntry>* a = (_STL::_List_node<CTCTextureDecal::CEntry>*)sentinel->_M_next;
    _STL::_List_node<CTCTextureDecal::CEntry>* b = (_STL::_List_node<CTCTextureDecal::CEntry>*)a->_M_next;
    if (b->_M_next != sentinel || sentinel->_M_prev != b || b->_M_prev != a || a->_M_prev != sentinel) ok = 0;
    if (a->_M_data.AllocEntryID != 5 || a->_M_data.Decal != 1234 || a->_M_data.Desc.TextureIndex != 77 ||
        a->_M_data.Desc.BlendType != 3 || a->_M_data.Desc.DiffuseColour.G != 4) ok = 0;
    if (b->_M_data.AllocEntryID != 6 || b->_M_data.Decal != 5678 || b->_M_data.Desc.TextureIndex != 88) ok = 0;
    printf("0070a490_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}