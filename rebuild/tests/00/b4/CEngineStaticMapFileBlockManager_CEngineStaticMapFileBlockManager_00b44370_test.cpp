#include <cstdio>
#include <cstdlib>
// Behaviour test for CEngineStaticMapFileBlockManager::CEngineStaticMapFileBlockManager() @ 0x00B44370: a null counted
// pointer and an empty STLport map whose header node is red with no parent and left/right pointing at itself.
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct SNode : _Rb_tree_node_base {                    // + pair<const long, CFileBlock>
    long Key;
    long RefCount;
    bool LowPriority;
    void* LoadingData_Data;
    void* LoadingData_Info;
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CBlockMap {
public:
    SNode*       _M_header;
    unsigned int _M_node_count;
    CBlockMap() : _M_header(0)
    {
        _M_header = (SNode*)malloc(sizeof(SNode));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
class CBankFileAsyncEntry;
class CEngineStaticMapFileBlockManager {
public:
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;
    CBlockMap                            Blocks;
};
int main()
{
    CEngineStaticMapFileBlockManager m;
    if (m.AsyncBankEntry.Data != 0 || m.AsyncBankEntry.Info != 0) { std::printf("00b44370_TEST FAIL\n"); return 1; }
    SNode* h = m.Blocks._M_header;
    if (h == 0 || m.Blocks._M_node_count != 0) { std::printf("00b44370_TEST FAIL\n"); return 1; }
    if (h->_M_color != false || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h) { std::printf("00b44370_TEST FAIL\n"); return 1; }
    free(h);
    std::printf("00b44370_TEST PASS\n");
    return 0;
}