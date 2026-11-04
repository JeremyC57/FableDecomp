// CVertexGroupBlockList::RemoveAllSubsets() -- drop every block whose groups are a proper subset of another block's: keep the rest in order in a new list and assign it back
// Grade: DIFFER(286v280) -- dl=77: same-TU IsProperSubsetOf 0x00AD8CA0 + map::find bodies give the pointer-walking inner loop; register allocation and the new-block copy order differ
// Leaf: ?RemoveAllSubsets@CVertexGroupBlockList@@QAEXXZ
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIterator> void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00ADC190
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {     // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector(const vector<_Tp, _Alloc>& __x);                // out of line per element type
    ~vector()
    {
        __destroy_aux(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x00ADCA30
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);                 // 0x00ADCE60
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
};
namespace std {
template <class _Tp> struct less { bool _M_pad; };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    struct _Node { long _M_color; _Node* _M_parent; _Node* _M_left; _Node* _M_right; _Key _M_key; _Tp _M_value; };
    struct const_iterator { _Node* _M_node; const_iterator(_Node* n) : _M_node(n) { } };
    _Node* _M_header;                                      // +0x00
    unsigned long _M_node_count;                           // +0x04
    _Compare _M_key_compare;                               // +0x08
    map(const map<_Key, _Tp, _Compare, _Alloc>& __x);       // 0x00A96E40 for <unsigned long, long>
    const_iterator find(const _Key& __k) const;            // 0x006699C0 (the template body is in every TU that uses it)
};
}
#pragma auto_inline(off)
template <class _Key, class _Tp, class _Compare, class _Alloc>
__declspec(noinline) typename std::map<_Key, _Tp, _Compare, _Alloc>::const_iterator std::map<_Key, _Tp, _Compare, _Alloc>::find(const _Key& __k) const
{
    _Node* __y = _M_header;
    _Node* __x = _M_header->_M_parent;
    while (__x) {
        if (!(__x->_M_key < __k)) {
            __y = __x;
            __x = __x->_M_left;
        }
        else
            __x = __x->_M_right;
    }
    if (__y == _M_header || __k < __y->_M_key)
        __y = _M_header;
    return const_iterator(__y);
}
#pragma auto_inline(on)
class CVertexGroupBlock {                                   // Ego_r.pdb; retail 24 bytes
public:
    CArray<unsigned long> Groups;                         // +0x00
    std::map<unsigned long, long> GroupToIndexMap;        // +0x0C
    __forceinline bool HasGroup(unsigned long group) const { return GroupToIndexMap.find(group)._M_node != GroupToIndexMap._M_header; }
    bool IsProperSubsetOf(const CVertexGroupBlock& other) const;   // 0x00AD8CA0
};
// Same TU in retail (landed 0x00AD8CA0).
bool CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) const
{
    unsigned long count = Groups.size();
    if (count < other.Groups.size()) {
        for (unsigned long i = 0; i < count; ++i) {
            if (!other.HasGroup(Groups[i]))
                return false;
        }
        return true;
    }
    return false;
}
class CVertexGroupBlockList { public:
    CArray<CVertexGroupBlock> Blocks;                     // +0x00
    void RemoveAllSubsets();
};

void CVertexGroupBlockList::RemoveAllSubsets()
{
    CArray<CVertexGroupBlock> newBlocks;
    for (unsigned long i = 0; i < Blocks.size(); ++i) {
        const CVertexGroupBlock& block = Blocks[i];
        unsigned long count = Blocks.size();
        for (unsigned long j = 0; j < count; ++j) {
            if (block.IsProperSubsetOf(Blocks[j]))
                goto next;
        }
        newBlocks.push_back(block);
    next:;
    }
    Blocks = newBlocks;
}
