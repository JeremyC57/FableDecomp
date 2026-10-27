// CMap::GetScriptedMapBrush(long) -- the index-th scripted map brush of the map's brush list; rethrows when the index is past the end
// Leaf: ?GetScriptedMapBrush@CMap@@QAEAAVCScriptedMapBrush@@J@Z
// Flags: /EHsc
// Retail: Fable.exe @ 0x0081E9C0, 43 bytes, thiscall `ret 4`, CScriptedMapBrush& in eax.
// Identity (proven by order and shape): Ego_r.pdb defines the four CMap brush getters back to back as
// PeekScriptedMapBrush(long) const, PeekScriptedMapBrush(const CCharString&) const, GetScriptedMapBrush(long),
// GetScriptedMapBrush(const CCharString&); retail has four bodies in the same order at 0x0081E930 (index walk),
// 0x0081E960 (name walk, landed), 0x0081E9C0 (index walk, this row) and 0x0081E9F0 (name walk, landed) -- so this is the
// third, ?GetScriptedMapBrush@CMap@@QAEAAVCScriptedMapBrush@@J@Z. All four walk the list at +0xC = Ego_r.pdb CMap
// ScriptedMapBrushes (std::list<CScriptedMapBrush>) and return the node's value (+8). The row has no direct caller
// (callers would be in script code reached by inlining or none at all).
// Label correction: catalog "_gapscan::sub_0081e9c0".
// Callees: 0x00BFEB84 _CxxThrowException(0, 0) = `throw;`.
// Layout: no engine header for CMap: local model (vptr, CachedVolumes +8, ScriptedMapBrushes +0xC, retail offsets before
// the first STL member are the PDB's). STLport 4.6 list: one node pointer (the sentinel); nodes { next, prev, value }.
// Shape: the counter in edx starts at 0, the empty-list test comes first, then `cmp edx,index ; je found` heads the loop;
// the rethrow sits after the loop and falls into the found block.
class CScriptedMapBrush;
class CCachedVolumes;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    class iterator {
    public:
        iterator(_List_node_base* __x) : _M_node(__x) { }
        _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& __y) const { return _M_node != __y._M_node; }
        _List_node_base* _M_node;
    };
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
};
}

class CScriptedMapBrush {
public:
    unsigned char _data[4];                                 // opaque here
};

class CMap {
public:
    virtual ~CMap();
    CScriptedMapBrush& GetScriptedMapBrush(long index);

    unsigned char _pad04[4];
    CCachedVolumes* CachedVolumes;                          // +0x08 boost::scoped_ptr<CCachedVolumes>
    std::list<CScriptedMapBrush> ScriptedMapBrushes;        // +0x0C
};

CScriptedMapBrush& CMap::GetScriptedMapBrush(long index)
{
    long i = 0;
    for (std::list<CScriptedMapBrush>::iterator it = ScriptedMapBrushes.begin(); it != ScriptedMapBrushes.end(); ++it, ++i)
    {
        if (i == index)
            return *it;
    }
    throw;
}