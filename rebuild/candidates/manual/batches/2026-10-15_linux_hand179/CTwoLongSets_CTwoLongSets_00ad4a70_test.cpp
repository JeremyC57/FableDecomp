#include <cstdio>
#include <cstdlib>
struct CRbNode { char Color; CRbNode* Parent; CRbNode* Left; CRbNode* Right; long Value; };
static int g_erased, g_freedHeaders;
struct CLongSet
{
    CRbNode* _M_header;
    unsigned int _M_node_count;
    void _M_erase(CRbNode* x) { while (x) { _M_erase(x->Right); CRbNode* l = x->Left; std::free(x); ++g_erased; x = l; } }
    void clear()
    {
        if (_M_node_count != 0)
        {
            _M_erase(_M_header->Parent);
            _M_header->Left = _M_header;
            _M_header->Parent = 0;
            _M_header->Right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CLongSet()
    {
        clear();
        if (_M_header != 0) { std::free(_M_header); ++g_freedHeaders; }
    }
};
struct CTwoLongSets { unsigned int Key; CLongSet SetA; CLongSet SetB; ~CTwoLongSets(); };
CTwoLongSets::~CTwoLongSets() {}
static CRbNode* NewNode() { CRbNode* n = (CRbNode*)std::malloc(sizeof(CRbNode)); n->Parent = n->Left = n->Right = 0; return n; }
int main()
{
    {
        CTwoLongSets s;
        s.SetA._M_header = NewNode(); s.SetA._M_node_count = 2;
        CRbNode* root = NewNode(); root->Left = NewNode();
        s.SetA._M_header->Parent = root;
        s.SetB._M_header = NewNode(); s.SetB._M_node_count = 0;
    }
    if (g_erased != 2 || g_freedHeaders != 2) { std::printf("00ad4a70_TEST FAIL\n"); return 1; }
    std::printf("00ad4a70_TEST PASS\n");
    return 0;
}
