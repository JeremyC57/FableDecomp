// CTCTextureDecal::AddDecal(long decal, CCompositeBlendDesc& desc) -- allocate the next decal entry id, append {id, decal, blend desc} to the decal list, mark the component dirty, ask the parent thing for an appearance update and return the id
// Leaf: ?AddDecal@CTCTextureDecal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070A490, length 131 bytes (+ int3 padding). __fastcall, `ret 8`.
//         Ego_r.pdb: `?AddDecal@CTCTextureDecal@@QAEJJAAVCCompositeBlendDesc@@@Z`.
// Callees: CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) @ 0x005BA48A (out of line, twice: into the
//          entry temporary and into the list node), malloc (CRT import thunk) @ 0x00BFEA0E (the 0x20-byte list node),
//          CThing forwarder @ 0x004C7590 (request 4, as the landed DeleteDecal 0x0070A430; the manifest
//          "CRenderTarget::DetachTarget" is propagated).
// Identity: the manifest label "CRenderTarget_EnqueueDecalUpdate" is descriptive; the landed caller CTCWound::CWound::
//           UpdateDecal (0x00757440) calls it as `AddDecal(long, CCompositeBlendDesc&)` and keeps the returned id.
// Layout (retail, as the landed constructor 0x0070A520 / ChangeDecal 0x0070A3E0 / DeleteDecal 0x0070A430): ParentThing +0x04
//         (CTCBase), Decals +0x0C (4-byte STLport list: the sentinel node), CurrentAllocEntryID +0x1C, Dirty +0x20. A list
//         node is {next, prev, CEntry}; CEntry (not in the PDB, 0x18 bytes) = {allocation id, decal, CCompositeBlendDesc}.
// Shape: `long id = CurrentAllocEntryID++;` then `Decals.push_back(CEntry(id, decal, desc))`: the entry temporary is built
//        in the frame (its desc through the out-of-line copy constructor), the STLport push_back -> insert(end(), x) reads
//        the sentinel before the node malloc, the placement copy keeps its null test, and the node is linked before the
//        sentinel with prev read into a local first. Dirty is set before the forwarder call; the id comes back in ebx.
// Recipe (chunk 422): the blend copy constructor, which receives the temporary's address, gets a same-TU
//        `auto_inline(off)` + `__declspec(noinline)` stand-in body; VC7.1 then treats the temporary's leading words as
//        uncaptured and loads id and decal before the first store of the node copy, as retail.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }

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