// Behaviour test for STLport list<CNavQuadTreeMap>::erase(iterator) (0x00A17110): unlink, destroy (name + counted tree), free, return next.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_names = 0, g_deleted = 0, g_freed = 0;
class CCharString { public: void* PData; CCharString() : PData(0) {} ~CCharString() { ++g_names; } };

struct CCPPointerInfo {
    unsigned long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static void DeleteTree(void*) { ++g_deleted; }

template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(CCPPointerInfo* info) : Data(0), Info(info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CNavQuadTree;
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; };
class CNavQuadTreeMap {
public:
    CCountedPointer<CNavQuadTree> PQuadTree;
    long Index;
    C2DBoxI MapArea;
    CCharString SectionName;
    CNavQuadTreeMap(CCPPointerInfo* info, long index) : PQuadTree(info), Index(index) {}
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : _List_node_base { CNavQuadTreeMap _M_data; };
struct iterator { _List_node_base* _M_node; iterator(_List_node_base* n) : _M_node(n) {} iterator(const iterator& o) : _M_node(o._M_node) {} };

struct list {
    _List_node_base* _M_node;
    list() { _M_node = (_List_node_base*)std::malloc(sizeof(_List_node_base)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    void push_back(CCPPointerInfo* info, long index)
    {
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        new (&n->_M_data) CNavQuadTreeMap(info, index);
        n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
    iterator erase(iterator pos)
    {
        _List_node_base* next = pos._M_node->_M_next;
        _List_node_base* prev = pos._M_node->_M_prev;
        _List_node* n = (_List_node*)pos._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~CNavQuadTreeMap();
        ++g_freed;
        std::free(n);
        return iterator(next);
    }
};

int main()
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 0; info->DeleteFunc = DeleteTree; info->Data = 0;
    list l;
    l.push_back(info, 1);
    l.push_back(info, 2);
    l.push_back(0, 3);
    iterator it = l.erase(iterator(l._M_node->_M_next));
    if (((_List_node*)it._M_node)->_M_data.Index != 2 || g_names != 1 || g_deleted != 0 || info->RefCount != 1 || g_freed != 1) {
        std::printf("00a17110_TEST FAIL\n"); return 1;
    }
    it = l.erase(it);
    if (((_List_node*)it._M_node)->_M_data.Index != 3 || g_deleted != 1) { std::printf("00a17110_TEST FAIL\n"); return 1; }
    it = l.erase(it);
    if (it._M_node != l._M_node || l._M_node->_M_next != l._M_node || g_names != 3 || g_freed != 3) { std::printf("00a17110_TEST FAIL\n"); return 1; }
    std::free(l._M_node);
    std::printf("00a17110_TEST PASS\n");
    return 0;
}