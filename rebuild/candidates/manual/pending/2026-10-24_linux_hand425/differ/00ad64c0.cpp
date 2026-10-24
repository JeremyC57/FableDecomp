// _STL::map<unsigned long, CContractionReference>::operator[](const unsigned long& k) -- find a vertex's pair contraction by partner index, inserting an empty reference when absent (COptimisedVertex::PairContractions)
// Grade: DIFFER(163v158) -- (1) the 0x00AD6420 scheduling slot: retail stores the late Data=0 word after the result-pointer push, the lane before the iterator copy; (2) the post-call release: retail `mov esi,[esp+14h] ; test esi,esi ; je ; dec [esi] ; jne`, the lane loads eax, copies to esi and compares `dec [eax] ; cmp [esi],0`
// Leaf: ??A?$map@KUCContractionReference
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD64C0, 0x9E bytes (+ int3 padding), thiscall, `ret 4`, returns &(*i).second.
// Identity: the callee 0x00AD52A0 is the landed _Rb_tree::insert_unique(iterator, const value_type&) of
// pair<unsigned, QuadricOptimiserInternals::CContractionReference> (node: key +0x10, Data +0x14, Info +0x18, SubIndex
// +0x1C); Ego_r.pdb COptimisedVertex::PairContractions is map<unsigned long, CContractionReference>. Catalog
// "BinaryTree_FindOrInsert" is descriptive.
// Callees: _Rb_tree::insert_unique(iterator, const value_type&) @ 0x00AD52A0, the count block's DeleteFunc (+4, data
// in ecx), operator delete @ 0x00BFE9BC (the value's counted-pointer release after the insert).
// Shape: CContractionReference has NO user default constructor: value-initialising the non-POD temporary leaves SubIndex
// indeterminate, which reproduces retail's `mov [esp+14h],0 ; mov eax,[esp+14h] ; mov [esp+18h],eax` (SubIndex read from the slot
// VC shares with Info). The release reads Info into a local (no re-read of the escaped pair slot).
// STLport operator[] (twin of 0x00AD6420 and the landed 0x009F1470): inline lower_bound, `if (i == end() ||
// key_comp()(k, (*i).first)) i = insert(i, value_type(k, __default_constructed((CContractionReference*)0)))`; the
// pair temporary's destructor (CCountedPointer release, `dec [esi] ; jne`) runs after the call.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CCPPointerInfo {
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);               // +0x04
    void*         Data;                                      // +0x08
};

template<class T> struct CCountedPointer {
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& x) : Data(x.Data), Info(x.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { Release(); }
    void Release()
    {
        CCPPointerInfo* info = Info;
        if (info && --info->RefCount == 0) {
            info->DeleteFunc(info->Data);
            delete info;
        }
    }
};

namespace QuadricOptimiserInternals {
struct CVertexPairContraction;
struct CContractionReference {                               // 12 bytes
    CCountedPointer<CVertexPairContraction> Contraction;     // +0x00
    unsigned long                           SubIndex;        // +0x08
};
}

namespace _STL {

template <class _T1, class _T2> struct pair
{
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less
{
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base
{
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base
{
    _Value _M_value_field;                          // +0x10
};

template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map
{
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    typedef _Rb_tree_node<value_type> _Node;

    _Rb_tree_node_base* _M_header;                  // +0x00 (alloc proxy)
    unsigned int        _M_node_count;              // +0x04
    _Compare            _M_key_compare;             // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field.first; }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const _Key& __k)
    {
        _Rb_tree_node_base* __y = _M_header;        // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_header->_M_parent;
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const value_type& __x);    // 0x00AD52A0 (_Rb_tree::insert_unique)
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

template class map<unsigned long, QuadricOptimiserInternals::CContractionReference>;

} // namespace _STL
