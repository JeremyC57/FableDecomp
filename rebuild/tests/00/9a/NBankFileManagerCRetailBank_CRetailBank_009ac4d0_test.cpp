#include <cstdio>
#include <cstdlib>
class CCharString { public: void* m_pStringData; CCharString() : m_pStringData(0) { } };
template <class T> class CCountedPointer { public: T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CContainedBank { unsigned long DataVersion, EntryCount, InfoOffset, InfoSize, Alignment; };
struct Node { bool _M_color; Node* _M_parent; Node* _M_left; Node* _M_right; };
struct TreeBase {
    Node* _M_header;
    TreeBase() : _M_header(0) { _M_header = (Node*)std::malloc(sizeof(Node)); }
};
struct Tree : TreeBase {
    unsigned int _M_node_count;
    Tree() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
struct CBase { virtual ~CBase() { } };
struct CRetailBank : CBase {
    CCountedPointer<int> DiskFile; CCountedPointer<int> ThreadedFile; bool UseDMA; Tree ContainedBanks; CCharString FilePath;
    CRetailBank() : UseDMA(false) { }
};
int main()
{
    CRetailBank b;
    Node* h = b.ContainedBanks._M_header;
    int bad = (b.DiskFile.Data || b.DiskFile.Info || b.ThreadedFile.Data || b.ThreadedFile.Info || b.UseDMA || b.FilePath.m_pStringData);
    if (!h || h->_M_color || h->_M_parent || h->_M_left != h || h->_M_right != h || b.ContainedBanks._M_node_count != 0) bad = 1;
    std::free(h);
    if (bad) { std::printf("009ac4d0_TEST FAIL\n"); return 1; }
    std::printf("009ac4d0_TEST PASS\n");
    return 0;
}