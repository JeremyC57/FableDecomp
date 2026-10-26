// Behaviour test for 0x0098FE70 STLport _Rb_tree::_M_find over CThreadedFileManager's pending-object map: returns the
// node whose key equals k (ordered by physical sort key, then file position, then object address), else the header.
#include <stdio.h>
#include <string.h>
class CThreadedFile { public:
    unsigned char _pad00[0x14];
    unsigned long PhysicalSortKey;             // +0x14
};
class CThreadedFileObjectDesc { public:
    void*         PDst;                        // +0x00
    unsigned long FilePos;                     // +0x04
    unsigned char _pad08[0x10];
};
class CThreadedFileManager {
public:
    class CFileObject { public:
        CThreadedFileObjectDesc Desc;          // +0x00
        unsigned long  UID;                    // +0x18
        long           LoadingStatus;          // +0x1C
        void*          Overlapped;             // +0x20
        CThreadedFileManager* PManager;        // +0x24
        CThreadedFile* PFile;                  // +0x28
    };
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;
        bool operator<(const CPendingObjectSortKey& o) const
        {
            unsigned long key = Object->PFile->PhysicalSortKey;
            unsigned long otherKey = o.Object->PFile->PhysicalSortKey;
            if (key < otherKey)
                return true;
            if (key > otherKey)
                return false;
            if (Object->Desc.FilePos < o.Object->Desc.FilePos)
                return true;
            if (Object->Desc.FilePos > o.Object->Desc.FilePos)
                return false;
            return Object < o.Object;
        }
    };
};
typedef CThreadedFileManager::CPendingObjectSortKey CPendingObjectSortKey;

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
}

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CPendingObjectTree_0098fe70 {            // STLport _Rb_tree<CPendingObjectSortKey, pair<const key, CFileObject*>, ...>
public:
    typedef std::pair<const CPendingObjectSortKey, CThreadedFileManager::CFileObject*> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node*                           _M_header;        // +0x0
    std::less<CPendingObjectSortKey> _M_key_compare;
    unsigned long                    _M_node_count;
    static _Node* _S_left(_Rb_tree_node_base* x) { return (_Node*)x->_M_left; }
    static _Node* _S_right(_Rb_tree_node_base* x) { return (_Node*)x->_M_right; }
    static value_type& _S_value(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field; }
    static const CPendingObjectSortKey& _S_key(_Rb_tree_node_base* x) { return std::_Select1st<value_type>()(_S_value(x)); }
    _Node* _M_root() const { return (_Node*)_M_header->_M_parent; }
    _Node* _M_find(const CPendingObjectSortKey& k) const;
};

CPendingObjectTree_0098fe70::_Node* CPendingObjectTree_0098fe70::_M_find(const CPendingObjectSortKey& k) const
{
    _Node* y = _M_header;                      // last node which is not less than k
    _Node* x = _M_root();                      // current node
    while (x != 0)
        if (!_M_key_compare(_S_key(x), k))
            y = x, x = _S_left(x);
        else
            x = _S_right(x);
    if (y == _M_header || _M_key_compare(k, _S_key(y)))
        y = _M_header;
    return y;
}


typedef CPendingObjectTree_0098fe70 TTree;
typedef TTree::_Node TNode;

int main()
{
    static CThreadedFile files[2];
    files[0].PhysicalSortKey = 10;
    files[1].PhysicalSortKey = 20;
    static CThreadedFileManager::CFileObject objs[6];
    memset(objs, 0, sizeof(objs));
    // ascending keys: (10,100) (10,200) (20,50) (20,60) (20,60 but a higher address)
    objs[0].PFile = &files[0]; objs[0].Desc.FilePos = 100;
    objs[1].PFile = &files[0]; objs[1].Desc.FilePos = 200;
    objs[2].PFile = &files[1]; objs[2].Desc.FilePos = 50;
    objs[3].PFile = &files[1]; objs[3].Desc.FilePos = 60;
    objs[4].PFile = &files[1]; objs[4].Desc.FilePos = 60;
    objs[5].PFile = &files[1]; objs[5].Desc.FilePos = 55;          // not in the tree
    static unsigned char raw[6][sizeof(TNode)];
    TNode* n[6];
    for (unsigned int i = 0; i < 6; ++i) { memset(raw[i], 0, sizeof(TNode)); n[i] = (TNode*)raw[i]; }
    TNode* header = n[5];
    for (unsigned int i = 0; i < 5; ++i)
        const_cast<CPendingObjectSortKey&>(n[i]->_M_value_field.first).Object = &objs[i];
    header->_M_parent = n[2];
    n[2]->_M_left = n[1]; n[2]->_M_right = n[3];
    n[1]->_M_left = n[0];
    n[3]->_M_right = n[4];
    TTree tree;
    tree._M_header = header;
    tree._M_node_count = 5;

    int ok = 1;
    for (unsigned int i = 0; i < 5; ++i) {
        CPendingObjectSortKey k; k.Object = &objs[i];
        if (tree._M_find(k) != n[i]) ok = 0;
    }
    CPendingObjectSortKey missing; missing.Object = &objs[5];
    if (tree._M_find(missing) != header) ok = 0;
    header->_M_parent = 0;                              // empty tree
    CPendingObjectSortKey k0; k0.Object = &objs[0];
    if (tree._M_find(k0) != header) ok = 0;

    printf(ok ? "0098fe70_TEST PASS\n" : "0098fe70_TEST FAIL\n");
    return ok ? 0 : 1;
}