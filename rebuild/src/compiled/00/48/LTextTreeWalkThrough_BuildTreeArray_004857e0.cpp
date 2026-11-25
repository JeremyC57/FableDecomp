#pragma optimize("s",on)

// Sub-object embedded at TreeNode+0x10.  Its cleanup is an out-of-line
// __fastcall member (this in ecx) taking no arguments -> emits
//   lea ecx,[esi+0x10]
//   call <Sub::Cleanup>
struct SubObject {
};

class TreeNode { public:
    char pad0[8];
    TreeNode* next;      // +0x8
    TreeNode* child;     // +0xc
    SubObject sub;       // +0x10
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x0099B510 ??1CWideString@@QAE@XZ (was ?Cleanup_0099b510@SubObject@@QAEXXZ)
class CWideString { public: ~CWideString(); };
void CTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    if (node) {
        do {
            this->BuildTreeArray(node->child);
            TreeNode* next = node->next;
            reinterpret_cast<CWideString*>(&(node->sub))->CWideString::~CWideString();
            free(node);
            node = next;
        } while (node);
    }
}