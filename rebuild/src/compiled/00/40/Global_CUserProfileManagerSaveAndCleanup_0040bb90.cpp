// Leaf: ?ResetTutorials@CUserProfileManager@@
// Grade: RELOCATION_MATCH 191/191 on the real span (catalog row 188 stopped 3 bytes short); base flags
// CUserProfileManager::ResetTutorials() -- forget every tutorial category already given, save the profile, then drain the
//   queue of pending tutorial categories.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040BB90, 191 bytes (the catalog row stopped 3 bytes short at 188), __thiscall, plain `ret`;
//   no direct callers (catalogued as the free function CUserProfileManager_SaveAndCleanup).
// Label correction (probable): Ego_r.pdb lists `?ResetTutorials@CUserProfileManager@@QAEXXZ` (void, no arguments); of the
//   class's void() members it is the one whose name fits a body that touches only GivenTutorialCategories, Save and
//   PendingTutorialCategories (the others are Apply*, Load, Save, Reset*Inputs*, ProcessPendingTutorials, ...). Not yet
//   proven by a caller or vtable slot (the function has no direct callers).
// Callees: CUserProfileManager::Save 0x0040A740, free 0x00BFEA14 (the deque node deallocation).
// Layout: Ego_r.pdb CUserProfileManager with retail 12-byte STLport vectors (the PDB build carries a 16-byte debug vector):
//   GivenTutorialCategories vector<ETutorialCategory> +0x8C (PDB +0x98), PendingTutorialCategories queue<deque<..>> +0x98
//   (PDB +0xA8): deque _M_start {cur +0x98, first +0x9C, last +0xA0, node +0xA4}, _M_finish.cur +0xA8.
// Shape: STLport vector::clear() is erase(begin(), end()) = copy(end, end, begin) -- an enum is not a __type_traits
//   trivial type, so the copy is the counted element loop (count is always 0 here) and _M_finish takes its result;
//   `while (!q.empty()) q.pop()` is deque::pop_front with the 128-byte node buffer (32 enums per node) and
//   _M_pop_front_aux inlined.
#include <stdlib.h>
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void clear()
    {
        T* first = _M_finish;
        T* result = _M_start;
        for (long n = _M_finish - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        _M_finish = result;
    }
};

template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    bool operator==(const _Deque_iterator& x) const { return _M_cur == x._M_cur; }
    void _M_set_node(T** new_node) { _M_last = (_M_first = *(_M_node = new_node)) + 0x80 / sizeof(T); }
};

template <class T> struct deque {
    _Deque_iterator<T> _M_start;                 // +0x00
    _Deque_iterator<T> _M_finish;                // +0x10
    T**                _M_map;                   // +0x20
    unsigned long      _M_map_size;              // +0x24
    bool empty() const { return _M_finish == _M_start; }
    void _M_deallocate_node(T* p) { if (p) free(p); }
    void pop_front()
    {
        if (_M_start._M_cur != _M_start._M_last - 1) {
            ++_M_start._M_cur;
        } else {
            _M_deallocate_node(_M_start._M_first);
            _M_start._M_set_node(_M_start._M_node + 1);
            _M_start._M_cur = _M_start._M_first;
        }
    }
};

template <class T> struct queue {
    deque<T> c;
    bool empty() const { return c.empty(); }
    void pop() { c.pop_front(); }
};

class CUserProfileManager {
public:
    unsigned char             _pad00[0x8c];
    vector<ETutorialCategory> GivenTutorialCategories;     // +0x8C
    queue<ETutorialCategory>  PendingTutorialCategories;   // +0x98
    void Save();                                           // 0x0040A740
    void ResetTutorials();
};

void CUserProfileManager::ResetTutorials()
{
    GivenTutorialCategories.clear();
    Save();
    while (!PendingTutorialCategories.empty())
        PendingTutorialCategories.pop();
}
