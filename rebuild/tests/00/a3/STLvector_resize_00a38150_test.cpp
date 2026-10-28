#include <cstdio>
#include <stdlib.h>
#include <new>

static int g_strings = 0;       // live CCharStrings
static int g_strAssign = 0;
static int g_lists = 0;         // live list sentinels
static int g_listAssign = 0;

class CClass;

class CCharString {
public:
    int Value;
    CCharString() : Value(0) { ++g_strings; }
    CCharString(const CCharString& o) : Value(o.Value) { ++g_strings; }
    ~CCharString() { --g_strings; }
    const CCharString& operator=(const CCharString& o) { Value = o.Value; ++g_strAssign; return *this; }
};

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> class list {
public:
    _List_node_base* _M_node;                               // the sentinel pointer only
    int Tag;                                                // test-only payload copied by operator=
    list() : Tag(0)
    {
        _M_node = (_List_node_base*)malloc(sizeof(_List_node_base));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        ++g_lists;
    }
    list(const list& o) : Tag(o.Tag)
    {
        _M_node = (_List_node_base*)malloc(sizeof(_List_node_base));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        ++g_lists;
    }
    ~list() { free(_M_node); --g_lists; }
    list& operator=(const list& o) { Tag = o.Tag; ++g_listAssign; return *this; }
};
}

typedef CClass* (*ClassAllocFunc)();

class CClassTypeInfo {
public:
    std::list<CClass*> TypeList;
    unsigned long       NoAllocated;
    bool                Used;
    bool                Local;
    ClassAllocFunc      AllocFunc;
    CCharString         Name;
    CClassTypeInfo()
        : TypeList()
        , Name()
    {
        Used = false;
        NoAllocated = 0;
        Local = false;
        AllocFunc = 0;
    }
};

static CClass* AllocA() { return 0; }

static unsigned int g_n = 0;
static int g_seenDefault = 0;
static int g_liveAtCall = 0;

namespace std {
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    // test stub for resize(n, x) 0x00A380E0: records n and whether x is a default record
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        g_n = __new_size;
        g_liveAtCall = g_strings;
        if (__x.NoAllocated == 0 && !__x.Used && !__x.Local && __x.AllocFunc == 0 && __x.Name.Value == 0
            && __x.TypeList._M_node->_M_next == __x.TypeList._M_node)
            ++g_seenDefault;
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}

int main()
{
    std::vector<CClassTypeInfo> v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.resize(42);
    if (g_n != 42 || g_seenDefault != 1 || g_liveAtCall != 1 || g_strings != 0 || g_lists != 0)
        { std::printf("00a38150_TEST FAIL\n"); return 1; }
    std::printf("00a38150_TEST PASS\n");
    return 0;
}