// vector<CCharString>::_M_range_insert(iterator pos, set<CCharString>::const_iterator first, last, const
// Grade: DIFFER(444v443) -- frame layout: retail passes every empty tag temporary at the first-iterator parameter's address and keeps mid in the dead position slot (8-byte frame); ours gives the tags and mid their own slots (0xc frame), plus the first!=last operand order; helper calls, branches and register use otherwise match
// forward_iterator_tag&) -- (manifest: "CCombatWheel::Insert") STLport range insert from a tree range: insert in place
// when capacity allows (two layouts by the elements after pos), else reallocate to old size + max(old size, n)
// Leaf: CCombatWheel::Insert
// Callees (STLport helpers instantiated out of line): __distance 0x00528040 (cdecl), __advance 0x00528090 (cdecl),
//   __uninitialized_copy ptr 0x0044B120 / tree 0x00527FC0 (__fastcall), __copy_backward_ptrs 0x0049B180, __copy tree
//   0x00528140, malloc 0x00BFEA0E, the vector's clear-and-free 0x00433040.
// Tried: value-initialised tags (a), tags with empty constructors (b, this), STLport inline wrappers taking iterators by value (c).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CCharString { void* PStringData; };
struct forward_iterator_tag { forward_iterator_tag() { } };
struct __false_type { __false_type() { } };
struct STreeNode;
struct CTreeIterator { STreeNode* _M_node; };
long __cdecl __distance(const CTreeIterator& first, const CTreeIterator& last, const forward_iterator_tag&);   // 0x00528040
void __cdecl __advance(CTreeIterator& i, long n, const forward_iterator_tag&);   // 0x00528090
CCharString* __fastcall __uninitialized_copy(CCharString* first, CCharString* last, CCharString* result, const __false_type&);   // 0x0044B120
CCharString* __fastcall __uninitialized_copy(CTreeIterator first, CTreeIterator last, CCharString* result, const __false_type&);   // 0x00527FC0
CCharString* __fastcall __copy_backward_ptrs(CCharString* first, CCharString* last, CCharString* result, const __false_type&);   // 0x0049B180
CCharString* __fastcall __copy(CTreeIterator first, CTreeIterator last, CCharString* result, const forward_iterator_tag&, long*);   // 0x00528140
template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

struct CStringVector {                                // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_clear();                                  // 0x00433040 (destroy the elements, free the block)
    unsigned int size() const { return _M_finish - _M_start; }
    void _M_range_insert(CCharString* position, CTreeIterator first, CTreeIterator last, const forward_iterator_tag&);
};

void CStringVector::_M_range_insert(CCharString* position, CTreeIterator first, CTreeIterator last, const forward_iterator_tag&)
{
    if (first._M_node != last._M_node) {
        unsigned int n = __distance(first, last, forward_iterator_tag());
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
            const unsigned int elems_after = _M_finish - position;
            CCharString* old_finish = _M_finish;
            if (elems_after > n) {
                __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __false_type());
                _M_finish += n;
                __copy_backward_ptrs(position, old_finish - n, old_finish, __false_type());
                __copy(first, last, position, forward_iterator_tag(), (long*)0);
            } else {
                CTreeIterator mid = first;
                __advance(mid, elems_after, forward_iterator_tag());
                __uninitialized_copy(mid, last, _M_finish, __false_type());
                _M_finish += n - elems_after;
                __uninitialized_copy(position, old_finish, _M_finish, __false_type());
                _M_finish += elems_after;
                __copy(first, mid, position, forward_iterator_tag(), (long*)0);
            }
        } else {
            const unsigned int old_size = size();
            const unsigned int len = old_size + (max)(old_size, n);
            CCharString* new_start = len ? (CCharString*)malloc(len * sizeof(CCharString)) : 0;
            CCharString* new_finish = new_start;
            new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
            new_finish = __uninitialized_copy(first, last, new_finish, __false_type());
            new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
            _M_clear();
            _M_start = new_start;
            _M_finish = new_finish;
            _M_end_of_storage = new_start + len;
        }
    }
}
