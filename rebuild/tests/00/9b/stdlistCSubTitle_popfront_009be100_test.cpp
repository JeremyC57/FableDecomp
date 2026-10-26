// Behaviour test for STLport list<CSubTitle>::pop_front (0x009BE100): unlink, destroy the subtitle members, free the node.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_strings = 0, g_wides = 0, g_released = 0, g_freed = 0;

class CCharString { public: void* PData; CCharString() : PData(0) {} ~CCharString() { ++g_strings; } };
class CWideString { public: void* PData; CWideString() : PData(0) {} ~CWideString() { ++g_wides; } };

class CTextEntry {
public:
    mutable long RefCount;
    CTextEntry() : RefCount(0) {}
    virtual ~CTextEntry() {}
    virtual void Release() const { ++g_released; }
};

template <class T> class CIVCountedPointer {
public:
    T* Data;
    CIVCountedPointer(T* d) : Data(d) { if (Data) ++Data->RefCount; }
    ~CIVCountedPointer()
    {
        T* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                data->Release();
            Data = 0;
        }
    }
};

class CSubTitle {
public:
    CIVCountedPointer<const CTextEntry> TextEntry;
    CWideString FormattedText;
    CCharString Sound;
    long FrameStart, FrameEnd;
    float StartTime, EndTime;
    CSubTitle(const CTextEntry* e) : TextEntry(e), FrameStart(0), FrameEnd(0), StartTime(0), EndTime(0) {}
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : _List_node_base { CSubTitle _M_data; };

struct list {
    _List_node_base* _M_node;
    list() { _M_node = (_List_node_base*)std::malloc(sizeof(_List_node_base)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    void push_back(const CTextEntry* e)
    {
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        new (&n->_M_data) CSubTitle(e);
        n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
    void pop_front()
    {
        _List_node_base* pos = _M_node->_M_next;
        _List_node_base* next = pos->_M_next;
        _List_node_base* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        ((_List_node*)pos)->_M_data.~CSubTitle();
        ++g_freed;
        std::free(pos);
    }
};

int main()
{
    CTextEntry shared;
    list l;
    l.push_back(&shared);
    l.push_back(&shared);
    l.push_back(0);
    l.pop_front();
    if (g_strings != 1 || g_wides != 1 || g_released != 0 || shared.RefCount != 1 || g_freed != 1) { std::printf("009be100_TEST FAIL\n"); return 1; }
    l.pop_front();
    if (g_released != 1 || shared.RefCount != 0) { std::printf("009be100_TEST FAIL\n"); return 1; }
    l.pop_front();
    if (g_strings != 3 || g_wides != 3 || g_released != 1 || g_freed != 3 || l._M_node->_M_next != l._M_node) { std::printf("009be100_TEST FAIL\n"); return 1; }
    std::free(l._M_node);
    std::printf("009be100_TEST PASS\n");
    return 0;
}