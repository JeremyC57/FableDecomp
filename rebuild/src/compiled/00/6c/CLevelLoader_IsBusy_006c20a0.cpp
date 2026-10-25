// CLevelLoader::IsBusy() const -- whether level objects are still queued for loading: !ObjectList.empty()
// Leaf: ?IsBusy@CLevelLoader@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C20A0, 15 bytes: mov eax,[ecx+14h] ; mov edx,[eax] ; xor ecx,ecx ; cmp edx,eax ;
//   setne cl ; mov al,cl ; ret
// Identity (found through FINDINGS chunk 388's callee 0x4FB4A0 CWorldMap::IsLoadingAnyMap, which is `mov ecx,
//   [ecx+0BCh] ; jmp here`; the catalog's CTCUserActionPointBase::IsActionPointFree with a `__fastcall` stand-in body is
//   wrong): struct_layouts CLevelLoader {ObjectList list<CCountedPointer<CLevelLoader::CObject> > +0x14} = the list whose
//   sentinel this tests (STLport list: one node pointer, empty() = `_M_node->_M_next == _M_node`); Ego_r.pdb names
//   ?IsBusy@CLevelLoader@@QBE_NXZ, the only bool() const member of CLevelLoader (the others are Add, Update and the
//   virtual SuspendableProcess); xrefs: all 8 call sites are level-loading code -- CWorld::Load 0x004A1AA3,
//   CWorldMap::IsLoadingAnyMap 0x004FB4A6 (tail jump), CWorldMap::LoadRegion 0x00500DA6, LoadThingsWithSectionNames
//   0x005010D6, 0x0050133A, CWorldMap::UpdateNavMaps 0x00501C86, CWorldMap::CacheMaps 0x00502948 / 0x00502962.
// Layout: CLevelLoader's +0x00..+0x13 is its suspendable-process base (vptr first; SuspendableProcess is a protected
//   virtual); the shared engine/CLevelLoader.h keeps the PDB's 12-byte Dinkumware list, so the members after it do not
//   agree with retail and a local model is used.
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
};

namespace std {
template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Alloc> _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
};
}

class CLevelLoader {
public:
    class CObject;
    bool IsBusy() const;
protected:
    unsigned char _base[0x14];                                          // +0x00 suspendable-process base (vptr first)
    std::list<CCountedPointer<CObject> > ObjectList;                    // +0x14
};

bool CLevelLoader::IsBusy() const
{
    return !ObjectList.empty();
}
