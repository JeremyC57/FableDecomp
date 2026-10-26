// Behaviour test of list<CBoast>::operator=(const list& x) (retail 0x005E9630)
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

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);                 // 0x0099B720
    ~CWideString();                                        // 0x0099B510
    CWideString& operator=(const CWideString& other);      // 0x0099B7D0
};

class CQuestManager {
public:
    class CBoast { public:                                        // Ego_r.pdb, 0x20 bytes; implicit copy / assign / destroy
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

namespace std {

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

} // namespace std

namespace std {
template <class T> list<T>& list<T>::operator=(const list<T>& x)
{
    if (this != &x) {
        iterator first1 = begin();
        iterator last1 = end();
        const_iterator first2 = x.begin();
        const_iterator last2 = x.end();
        while (first1 != last1 && first2 != last2)
            *first1++ = *first2++;
        if (first2 == last2)
            erase(first1, last1);
        else
            insert(last1, first2, last2);
    }
    return *this;
}
} // namespace std

template class std::list<CQuestManager::CBoast>;

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
typedef std::list<CBoast> CBoastList;
typedef std::_List_node<CBoast> CBoastNode;
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
    l._M_node = (std::_List_node_base*)malloc(sizeof(CBoastNode));
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
    std::_List_node_base* p = l._M_node->_M_next;
    for (int i = 0; i < n; ++i, p = p->_M_next)
        if (p == l._M_node || p->_M_next->_M_prev != p || !BoastIs(((CBoastNode*)p)->_M_data, ids[i]))
            return false;
    return p == l._M_node && l._M_node->_M_next->_M_prev == l._M_node;
}

namespace std {
template <class T> typename list<T>::iterator list<T>::erase(iterator first, iterator last)   // 0x005E90B0
{
    while (first != last)
        erase(first++);
    return last;
}
template <class T> template <class InputIter>
void list<T>::_M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&)   // 0x005E9280
{
    for (; first != last; ++first)
        insert(pos, *first);
}
} // namespace std

int main()
{
    static const long a4[4] = { 1, 2, 3, 4 };
    static const long b2[2] = { 8, 9 };
    static const long c3[3] = { 10, 11, 12 };
    CBoastList longer, shorter, three, one;
    InitList(longer, a4, 4);
    InitList(shorter, b2, 2);
    ResetCounts();
    CBoastList& r = (longer = shorter);                     // assign two, erase the other two
    bool ok = &r == &longer && ListIs(longer, b2, 2) && ListIs(shorter, b2, 2) && g_strAssigns == 2 && g_wideAssigns == 4
           && g_strDtors == 2 && g_wideDtors == 4 && g_strCopies == 0;
    static const long d1[1] = { 20 };
    InitList(one, d1, 1);
    InitList(three, c3, 3);
    ResetCounts();
    one = three;                                            // assign one, insert the other two
    ok = ok && ListIs(one, c3, 3) && g_strAssigns == 1 && g_strCopies == 2 && g_wideCopies == 4 && g_strDtors == 0;
    ResetCounts();
    one = one;                                              // self-assignment is a no-op
    ok = ok && ListIs(one, c3, 3) && g_strAssigns == 0 && g_strCopies == 0 && g_strDtors == 0;
    if (!ok) { std::printf("005e9630_TEST FAIL\n"); return 1; }
    std::printf("005e9630_TEST PASS\n");
    return 0;
}