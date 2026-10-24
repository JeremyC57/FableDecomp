#include <cstdio>
#include <cstdlib>
#include <new>

static int g_frees = 0;
static void test_free(void* p) { ++g_frees; std::free(p); }

static int g_charDtors = 0, g_wideDtors = 0, g_deletes = 0;

class CCharString {
public:
    CCharString() : PStringData(0) {}
    ~CCharString() { ++g_charDtors; }
    void* PStringData;
};

class CWideString {
public:
    CWideString() : PStringData(0) {}
    ~CWideString() { ++g_wideDtors; }
    void* PStringData;
};

class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(0) {}
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() const { ++g_deletes; }
    long Release() const
    {
        if (--IVRefCount == 0) {
            Delete();
            return 0;
        }
        return (long)IVRefCount;
    }
    mutable unsigned long IVRefCount;
};

namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase {
};
}

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        if (Object) {
            InternalGetObject()->Release();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* InternalGetObject() const { return (CIVCountedPointeeBase*)Object; }
    T* Object;
};

struct CSubTitle {
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;
    CWideString FormattedText;
    CCharString Sound;
    long        FrameStart;
    long        FrameEnd;
    float       StartTime;
    float       EndTime;
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) test_free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template <class _Tp, class _Alloc> void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
}

typedef _STL::_List_node<CSubTitle> Node;

int main()
{
    _STL::_List_base<CSubTitle, _STL::allocator<CSubTitle> > subtitles;
    Node* sentinel = (Node*)std::malloc(sizeof(Node));
    sentinel->_M_next = sentinel->_M_prev = sentinel;
    subtitles._M_node._M_data = sentinel;

    NGameText::CTextEntry shared, single;
    shared.IVRefCount = 2;                    // held by two subtitles
    single.IVRefCount = 1;                    // held by one: released to zero -> Delete()
    const NGameText::CTextEntry* entries[3] = { &shared, &single, 0 };
    for (int i = 0; i < 3; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        new (&n->_M_data) CSubTitle();
        n->_M_data.TextEntry.Object = entries[i] ? entries[i] : &shared;
        if (i == 2) n->_M_data.TextEntry.Object = &shared;
        n->_M_next = sentinel;
        n->_M_prev = sentinel->_M_prev;
        sentinel->_M_prev->_M_next = n;
        sentinel->_M_prev = n;
    }
    subtitles.clear();
    if (g_frees != 3 || g_charDtors != 3 || g_wideDtors != 3 || g_deletes != 2 || shared.IVRefCount != 0 ||
        single.IVRefCount != 0 || sentinel->_M_next != sentinel || sentinel->_M_prev != sentinel) {
        std::printf("009be060_TEST FAIL\n"); return 1;
    }
    subtitles.clear();                        // an empty list: nothing freed, still self-linked
    if (g_frees != 3 || sentinel->_M_next != sentinel) { std::printf("009be060_TEST FAIL\n"); return 1; }
    std::free(sentinel);
    std::printf("009be060_TEST PASS\n");
    return 0;
}