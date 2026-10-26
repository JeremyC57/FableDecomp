#pragma optimize("s",on)
struct CInner {
    void Destroy_004dbe39(int flag);  // __fastcall member at node+0x10
};

struct CTextTreeNode {
    char pad0[8];
    CTextTreeNode* next;    // +0x8
    CTextTreeNode* child;   // +0xc
    CInner inner;           // +0x10
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class LTextTreeWalkThrough {
public:
    void BuildTreeArray(CTextTreeNode* node);
};

void LTextTreeWalkThrough::BuildTreeArray(CTextTreeNode* node)
{
    while (node) {
        this->BuildTreeArray(node->child);
        CTextTreeNode* nxt = node->next;
        node->inner.Destroy_004dbe39(0);
        free(node);
        node = nxt;
    }
}