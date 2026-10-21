#include <cstdio>
#include <stdlib.h>
class CBaseIntelligentPointer {
public:
    void* _vfptr_or_pad;                                 // +0x00
    void* PInfo;                                         // +0x04
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                           // +0x08
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> class _List_base {
public:
    _List_node<T>* _M_node;                              // sentinel
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            _Destroy(&tmp->_M_data);
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
}
template class std::_List_base<CBaseIntelligentPointer>;

static int g_dtors;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
#include <new>
template <class T> static std::_List_node<T>* MakeList(std::_List_base<T>& l, int n)
{
    std::_List_node<T>* s = (std::_List_node<T>*)malloc(sizeof(std::_List_node<T>));
    s->_M_next = s; s->_M_prev = s; l._M_node = s;
    for (int i = 0; i < n; ++i) {
        std::_List_node<T>* node = (std::_List_node<T>*)malloc(sizeof(std::_List_node<T>));
        new (&node->_M_data) T;
        node->_M_next = s; node->_M_prev = s->_M_prev; s->_M_prev->_M_next = node; s->_M_prev = node;
    }
    return s;
}
template <class T> static bool Check(const char* tag)
{
    std::_List_base<T> l;
    std::_List_node<T>* s = MakeList(l, 3);
    g_dtors = 0; l.clear();
    bool ok = g_dtors == 3 && l._M_node == s && s->_M_next == s && s->_M_prev == s;
    MakeList(l, 0); free(s); s = l._M_node;
    l.clear();
    ok = ok && g_dtors == 3 && s->_M_next == s && s->_M_prev == s;
    free(s);
    std::printf(ok ? "%s_TEST PASS\n" : "FAIL\n", tag);
    return ok;
}
int main() { Check<CBaseIntelligentPointer>("00440730"); return 0; }