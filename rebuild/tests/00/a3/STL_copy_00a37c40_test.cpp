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

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

int main()
{
    {
        CClassTypeInfo src[3];
        CClassTypeInfo dst[3];
        for (int i = 0; i < 3; ++i) {
            src[i].TypeList.Tag = 10 + i;
            src[i].NoAllocated = 100 + i;
            src[i].Used = (i & 1) != 0;
            src[i].Local = true;
            src[i].AllocFunc = AllocA;
            src[i].Name.Value = 1000 + i;
        }
        CClassTypeInfo* r = std::__copy(src, src + 3, dst, std::random_access_iterator_tag(), (int*)0);
        if (r != dst + 3 || g_strAssign != 3 || g_listAssign != 3)
            { std::printf("00a37c40_TEST FAIL\n"); return 1; }
        for (int i = 0; i < 3; ++i)
            if (dst[i].TypeList.Tag != 10 + i || dst[i].NoAllocated != (unsigned long)(100 + i) || dst[i].Used != ((i & 1) != 0)
                || !dst[i].Local || dst[i].AllocFunc != AllocA || dst[i].Name.Value != 1000 + i)
                { std::printf("00a37c40_TEST FAIL\n"); return 1; }
    }
    if (g_strings != 0 || g_lists != 0)
        { std::printf("00a37c40_TEST FAIL\n"); return 1; }
    std::printf("00a37c40_TEST PASS\n");
    return 0;
}