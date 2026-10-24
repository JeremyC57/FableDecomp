// Behaviour test of list<CBoast>::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) (retail 0x005E9280)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

#include <new>
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);                 // 0x0099B720
    ~CWideString();                                        // 0x0099B510
    CWideString& operator=(const CWideString& other);      // 0x0099B7D0
};

class CQuestManager {
public:
    struct CBoast {                                        // Ego_r.pdb, 0x20 bytes; implicit copy / assign / destroy
        long        ID;                                    // +0x00
        long        Cost;                                  // +0x04
        long        Reward;                                // +0x08
        CCharString ScriptName;                            // +0x0c
        CWideString Text;                                  // +0x10
        CWideString Title;                                 // +0x14
        bool        Evil;                                  // +0x18
        bool        Failed;                                // +0x19
        bool        Completed;                             // +0x1a
        long        GroupIndex;                            // +0x1c
    };
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class T> class allocator {                       // STLport allocator over malloc / free
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { free(p); }
};

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x00
    _List_node_base* _M_prev;                              // +0x04
};

template <class T> struct _List_node : _List_node_base {
    T _M_data;                                             // +0x08
};

struct _List_iterator_base {                              // implicit (trivial) copy
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};

// STLport 4.x: `_List_iterator(const iterator&)` is the USER copy constructor of iterator (built in place at esp when
// passed by value) but only a converting constructor of const_iterator, whose implicit copy is a plain `push`.
template <class T, class Ref> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, T&>  iterator;
    typedef _List_iterator<T, Ref> _Self;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    Ref operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _Self& operator++() { _M_incr(); return *this; }
    _Self operator++(int) { _Self tmp = *this; _M_incr(); return tmp; }
};

template <class T> class list {
public:
    typedef _List_node<T>                _Node;
    typedef _List_iterator<T, T&>        iterator;
    typedef _List_iterator<T, const T&>  const_iterator;

    _List_node_base* _M_node;                              // +0x00 the allocated sentinel

    iterator begin() { return _M_node->_M_next; }
    iterator end() { return _M_node; }
    const_iterator begin() const { return _M_node->_M_next; }
    const_iterator end() const { return _M_node; }

    _Node* _M_create_node(const T& x)
    {
        _Node* p = allocator<_Node>().allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node_base* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    template <class InputIter> void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&);
    template <class InputIter> void insert(iterator pos, InputIter first, InputIter last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    iterator erase(iterator position)
    {
        _List_node_base* next_node = position._M_node->_M_next;
        _List_node_base* prev_node = position._M_node->_M_prev;
        _Node* n = (_Node*)position._M_node;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        _Destroy(&n->_M_data);
        allocator<_Node>().deallocate(n, 1);
        return iterator(next_node);
    }
    iterator erase(iterator first, iterator last);
    list& operator=(const list& x);
};

} // namespace _STL

namespace _STL {
template <class T> template <class InputIter>
void list<T>::_M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&)
{
    for (; first != last; ++first)
        insert(pos, *first);
}
} // namespace _STL

template void _STL::list<CQuestManager::CBoast>::_M_insert_dispatch<_STL::list<CQuestManager::CBoast>::const_iterator>(
    _STL::list<CQuestManager::CBoast>::iterator, _STL::list<CQuestManager::CBoast>::const_iterator,
    _STL::list<CQuestManager::CBoast>::const_iterator, const _STL::__false_type&);

// ---- test bodies of the retail callees ----
static int g_strCopies = 0, g_strDtors = 0, g_strAssigns = 0;
static int g_wideCopies = 0, g_wideDtors = 0, g_wideAssigns = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_strCopies; }
CCharString::~CCharString() { ++g_strDtors; }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; ++g_strAssigns; return *this; }
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_wideCopies; }
CWideString::~CWideString() { ++g_wideDtors; }
CWideString& CWideString::operator=(const CWideString& other) { PStringData = other.PStringData; ++g_wideAssigns; return *this; }
static void ResetCounts() { g_strCopies = g_strDtors = g_strAssigns = g_wideCopies = g_wideDtors = g_wideAssigns = 0; }

typedef CQuestManager::CBoast CBoast;
typedef _STL::list<CBoast> CBoastList;
typedef _STL::_List_node<CBoast> CBoastNode;
struct SRawBoast {                                         // CBoast's layout (it has no default constructor)
    long ID, Cost, Reward;
    void* ScriptName;
    void* Text;
    void* Title;
    bool Evil, Failed, Completed;
    long GroupIndex;
};
static int g_script[32], g_text[32], g_title[32];
static CBoastNode* NewBoastNode(long id)                   // every field derived from the id
{
    SRawBoast raw;
    raw.ID = id;
    raw.Cost = 1000 + id;
    raw.Reward = 2000 + id;
    raw.ScriptName = &g_script[id];
    raw.Text = &g_text[id];
    raw.Title = &g_title[id];
    raw.Evil = (id & 1) != 0;
    raw.Failed = (id & 2) != 0;
    raw.Completed = (id & 4) != 0;
    raw.GroupIndex = 3000 + id;
    CBoastNode* n = (CBoastNode*)malloc(sizeof(CBoastNode));
    new (&n->_M_data) CBoast(*(const CBoast*)&raw);
    return n;
}
static void InitList(CBoastList& l, const long* ids, int n)
{
    l._M_node = (_STL::_List_node_base*)malloc(sizeof(CBoastNode));
    l._M_node->_M_next = l._M_node->_M_prev = l._M_node;
    for (int i = 0; i < n; ++i) {
        CBoastNode* node = NewBoastNode(ids[i]);
        node->_M_next = l._M_node;
        node->_M_prev = l._M_node->_M_prev;
        l._M_node->_M_prev->_M_next = node;
        l._M_node->_M_prev = node;
    }
}
static bool BoastIs(const CBoast& b, long id)
{
    return b.ID == id && b.Cost == 1000 + id && b.Reward == 2000 + id && b.ScriptName.PStringData == &g_script[id]
        && b.Text.PStringData == &g_text[id] && b.Title.PStringData == &g_title[id] && b.Evil == ((id & 1) != 0)
        && b.Failed == ((id & 2) != 0) && b.Completed == ((id & 4) != 0) && b.GroupIndex == 3000 + id;
}
static bool ListIs(const CBoastList& l, const long* ids, int n)   // forward order, back links and every field
{
    _STL::_List_node_base* p = l._M_node->_M_next;
    for (int i = 0; i < n; ++i, p = p->_M_next)
        if (p == l._M_node || p->_M_next->_M_prev != p || !BoastIs(((CBoastNode*)p)->_M_data, ids[i]))
            return false;
    return p == l._M_node && l._M_node->_M_next->_M_prev == l._M_node;
}

int main()
{
    static const long sids[3] = { 5, 6, 7 };
    static const long dids[2] = { 1, 2 };
    CBoastList src, dst;
    InitList(src, sids, 3);
    InitList(dst, dids, 2);
    CBoastList::iterator pos = dst.begin();
    ++pos;
    const CBoastList& csrc = src;
    ResetCounts();
    dst._M_insert_dispatch(pos, csrc.begin(), csrc.end(), _STL::__false_type());   // copies 5, 6, 7 before boast 2
    static const long want[5] = { 1, 5, 6, 7, 2 };
    bool ok = ListIs(dst, want, 5) && ListIs(src, sids, 3) && g_strCopies == 3 && g_wideCopies == 6 && g_strDtors == 0;
    dst._M_insert_dispatch(dst.end(), csrc.end(), csrc.end(), _STL::__false_type());   // empty range
    ok = ok && ListIs(dst, want, 5) && g_strCopies == 3;
    if (!ok) { std::printf("005e9280_TEST FAIL\n"); return 1; }
    std::printf("005e9280_TEST PASS\n");
    return 0;
}