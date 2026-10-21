#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour test for NUISystem::CUIDef::~CUIDef (0x006301A0): every owned buffer is released once, maps are cleared
// (erase from the root, header links reset) before their header is freed, every state element is destroyed, and the
// def base releases its SubDefInfoMap buffer last.
static int g_frees = 0, g_erases = 0, g_stateDtors = 0, g_wideDtors = 0, g_resourceDtors = 0;
static void FreeBlock(void* p) { ++g_frees; std::free(p); }

struct CResource_Calls { ~CResource_Calls() { ++g_resourceDtors; } };
struct CDefPointeeBase : CResource_Calls { ~CDefPointeeBase() { } };

template<class T> inline void Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CRetailVector()
    {
        Destroy(_M_start, _M_finish);
        if (_M_start)
            FreeBlock(_M_start);
    }
};
struct CRbTreeNode { long _M_color; CRbTreeNode* _M_parent; CRbTreeNode* _M_left; CRbTreeNode* _M_right; };
struct CRbTree {
    CRbTreeNode* _M_header; unsigned long _M_node_count;
    CRbTree() : _M_header(0), _M_node_count(0) {}
    void _M_erase(CRbTreeNode* x) { if (x) { _M_erase(x->_M_left); _M_erase(x->_M_right); ++g_erases; FreeBlock(x); } }
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
    ~CRbTree() { clear(); if (_M_header) FreeBlock(_M_header); }
};
struct CWideString { ~CWideString() { ++g_wideDtors; } };
struct CUIStateDef { virtual ~CUIStateDef() { ++g_stateDtors; } int x; };

struct CParentDefClassBase : CDefPointeeBase { CRetailVector<unsigned long> SubDefInfoMap; };
struct CUIDef : CParentDefClassBase {
    CRetailVector<CUIStateDef> States;
    CWideString TextValue;
    CRbTree Sprites;
    CRetailVector<long> Children;
    CRetailVector<unsigned long> HorizontalSeparations, VerticalSeparations, NonScrollingChildren;
    CRbTree ActionMap, ActionMapAliases;
    CRetailVector<unsigned long> ActionOrder;
    CWideString MovieFilename;
    CRetailVector<unsigned long> SwappingStates;
    CRetailVector<float> SwappingTimes;
    CRetailVector<unsigned long> ShapeChildren;
    ~CUIDef() { }
};

static CRbTreeNode* NewNode() { CRbTreeNode* n = (CRbTreeNode*)std::malloc(sizeof(CRbTreeNode)); n->_M_parent = n->_M_left = n->_M_right = 0; return n; }

int main()
{
    {
        CUIDef d;
        d.SubDefInfoMap._M_start = (unsigned long*)std::malloc(4); d.SubDefInfoMap._M_finish = d.SubDefInfoMap._M_start + 1;
        CUIStateDef* states = (CUIStateDef*)std::malloc(3 * sizeof(CUIStateDef));
        for (int i = 0; i < 3; ++i) new (&states[i]) CUIStateDef();
        d.States._M_start = states; d.States._M_finish = states + 3;
        d.Children._M_start = (long*)std::malloc(4); d.Children._M_finish = d.Children._M_start + 1;
        d.ShapeChildren._M_start = (unsigned long*)std::malloc(4); d.ShapeChildren._M_finish = d.ShapeChildren._M_start + 1;
        d.SwappingTimes._M_start = (float*)std::malloc(4); d.SwappingTimes._M_finish = d.SwappingTimes._M_start + 1;
        // Sprites: header + two nodes; ActionMap: header only (empty); ActionMapAliases: none.
        d.Sprites._M_header = NewNode();
        CRbTreeNode* root = NewNode(); root->_M_left = NewNode();
        d.Sprites._M_header->_M_parent = root; d.Sprites._M_node_count = 2;
        d.ActionMap._M_header = NewNode();
    }
    // frees: SubDefInfoMap, States, Children, ShapeChildren, SwappingTimes (5) + Sprites header + 2 nodes + ActionMap header (4)
    int bad = g_frees != 9 || g_erases != 2 || g_stateDtors != 3 || g_wideDtors != 2 || g_resourceDtors != 1;
    if (bad) { std::printf("006301a0_TEST FAIL\n"); return 1; }
    std::printf("006301a0_TEST PASS\n");
    return 0;
}
