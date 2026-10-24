// Behaviour test for CUserProfileManager::ResetTutorials (0x0040BB90): the given-tutorial vector is emptied (storage kept)
// before Save runs, Save runs exactly once, and afterwards the pending-tutorial deque is drained across node boundaries:
// the start iterator walks to the finish position, every fully consumed node is released, the map pointer advances.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---- reconstruction under test (verbatim copy of src/0040bb90.cpp) ----
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

// ---- harness ----
static int g_fail;
static int g_saves;
static long g_givenAtSave = -1, g_pendingAtSave = -1;
static CUserProfileManager* g_mgr;

void CUserProfileManager::Save()
{
    ++g_saves;
    g_givenAtSave = (long)(GivenTutorialCategories._M_finish - GivenTutorialCategories._M_start);
    g_pendingAtSave = PendingTutorialCategories.empty() ? 0 : 1;
}

int main()
{
    static unsigned char store[sizeof(CUserProfileManager)];
    CUserProfileManager& m = *(CUserProfileManager*)store;
    ETutorialCategory given[5];
    ETutorialCategory* nodes[4];
    int i;
    memset(store, 0, sizeof(store));
    g_mgr = &m;
    for (i = 0; i < 3; ++i) nodes[i] = (ETutorialCategory*)malloc(0x80);
    nodes[3] = 0;
    m.GivenTutorialCategories._M_start = given;
    m.GivenTutorialCategories._M_finish = given + 5;
    m.GivenTutorialCategories._M_end_of_storage = given + 5;
    deque<ETutorialCategory>& d = m.PendingTutorialCategories.c;
    d._M_map = nodes; d._M_map_size = 4;
    d._M_start._M_node = nodes; d._M_start._M_first = nodes[0]; d._M_start._M_last = nodes[0] + 32; d._M_start._M_cur = nodes[0] + 20;
    d._M_finish._M_node = nodes + 2; d._M_finish._M_first = nodes[2]; d._M_finish._M_last = nodes[2] + 32; d._M_finish._M_cur = nodes[2] + 7;

    m.ResetTutorials();
    if (g_saves != 1 || g_givenAtSave != 0 || g_pendingAtSave != 1) { printf("FAIL save order %d %ld %ld\n", g_saves, g_givenAtSave, g_pendingAtSave); g_fail = 1; }
    if (m.GivenTutorialCategories._M_finish != given || m.GivenTutorialCategories._M_start != given || m.GivenTutorialCategories._M_end_of_storage != given + 5) {
        printf("FAIL vector not cleared in place\n"); g_fail = 1;
    }
    if (!m.PendingTutorialCategories.empty() || d._M_start._M_cur != nodes[2] + 7 || d._M_start._M_node != nodes + 2 ||
        d._M_start._M_first != nodes[2] || d._M_start._M_last != nodes[2] + 32) {
        printf("FAIL deque not drained to the finish position\n"); g_fail = 1;
    }

    // already empty: just the save
    m.GivenTutorialCategories._M_finish = given + 2;
    m.ResetTutorials();
    if (g_saves != 2 || g_pendingAtSave != 0 || m.GivenTutorialCategories._M_finish != given || d._M_start._M_node != nodes + 2) {
        printf("FAIL empty-queue path\n"); g_fail = 1;
    }
    free(nodes[2]);
    if (g_fail) return 1;
    printf("0040bb90_TEST PASS\n");
    return 0;
}