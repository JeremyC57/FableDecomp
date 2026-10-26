// Behaviour test for 00a2cfe0 (CLODStateBlock destructor): the bounding-box counted pointer is released first (the last reference deletes the box and its block), then the bone map is emptied through _M_erase and its header freed.
#include <cstdio>
#include <cstdlib>
static int g_events[8];
static int g_count = 0;
static void test_free(void* p) { g_events[g_count++] = 3; std::free(p); }
class CCPPointerInfo { public:
    long  RefCount;
    void  (*DeleteFunc)(void* data);
    void* Data;
};
static void DeleteBox(void*) { g_events[g_count++] = 1; }
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
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
struct _Rb_tree_node_base {
    char _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct less_CCharString { };
struct CBoneLODMap {
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    less_CCharString    _M_key_compare;
    CBoneLODMap() : _M_header(0), _M_node_count(0) { }
    void _M_erase(_Rb_tree_node_base*) { g_events[g_count++] = 2; }   // stands in for 0x0043020A
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CBoneLODMap()
    {
        clear();
        if (_M_header)
            test_free(_M_header);
    }
};
class C3DBoundingBox { public: float m[6]; };
struct CLODStateBlock {
    CBoneLODMap                     BoneLODs;
    CCountedPointer<C3DBoundingBox> MinXYZRange;
    ~CLODStateBlock();
};
CLODStateBlock::~CLODStateBlock()
{
}
int main()
{
    {
        CLODStateBlock block;
        block.BoneLODs._M_header = static_cast<_Rb_tree_node_base*>(std::malloc(sizeof(_Rb_tree_node_base)));
        block.BoneLODs._M_header->_M_parent = 0;
        block.BoneLODs._M_node_count = 2;
        CCPPointerInfo* info = new CCPPointerInfo;
        info->RefCount = 1; info->DeleteFunc = DeleteBox; info->Data = 0;
        block.MinXYZRange.Info = info;
    }
    if (g_count != 3 || g_events[0] != 1 || g_events[1] != 2 || g_events[2] != 3) { std::printf("00a2cfe0_TEST FAIL\n"); return 1; }
    g_count = 0;
    { CLODStateBlock empty; }
    if (g_count != 0) { std::printf("00a2cfe0_TEST FAIL\n"); return 1; }
    std::printf("00a2cfe0_TEST PASS\n");
    return 0;
}