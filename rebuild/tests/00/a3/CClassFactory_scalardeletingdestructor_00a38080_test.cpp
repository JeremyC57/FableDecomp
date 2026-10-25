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
    CCharString& operator=(const CCharString& o) { Value = o.Value; ++g_strAssign; return *this; }
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

static int g_baseDtor = 0;

class CBaseClass {
public:
    virtual ~CBaseClass() { }
};
class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { ++g_baseDtor; }
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    // test version of resize(n) 0x00A38150 for an empty vector: n default records
    void resize(unsigned int __new_size)
    {
        _M_start = (_Tp*)malloc(__new_size * sizeof(_Tp));
        for (unsigned int i = 0; i < __new_size; ++i)
            new (_M_start + i) _Tp();
        _M_finish = _M_end_of_storage = _M_start + __new_size;
    }
};
}

class CClassFactory : public CBaseClassNonCopyable {
public:
    CClassFactory(unsigned char maxClassTypeID);
    virtual ~CClassFactory();
    std::vector<CClassTypeInfo> ClassTypeInfo;
    unsigned long                MaxClassTypeID;
    unsigned long                NoAllocatedClasses;
    std::list<CClass*>          ClassList;
};

CClassFactory::~CClassFactory()
{
}

CClassFactory::CClassFactory(unsigned char maxClassTypeID)
    : MaxClassTypeID(maxClassTypeID)
{
    NoAllocatedClasses = 0;
    ClassTypeInfo.resize(MaxClassTypeID);
}

int main()
{
    CBaseClass* p = new CClassFactory(5);
    // 5 records (5 strings, 5 type lists) + the factory's own class list
    if (g_strings != 5 || g_lists != 6 || ((CClassFactory*)p)->ClassTypeInfo._M_finish - ((CClassFactory*)p)->ClassTypeInfo._M_start != 5)
        { std::printf("00a38080_TEST FAIL\n"); return 1; }
    delete p;                                               // the scalar deleting destructor
    if (g_strings != 0 || g_lists != 0 || g_baseDtor != 1)
        { std::printf("00a38080_TEST FAIL\n"); return 1; }
    std::printf("00a38080_TEST PASS\n");
    return 0;
}