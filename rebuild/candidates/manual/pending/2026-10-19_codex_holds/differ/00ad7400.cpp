// Grade: HELD(bytes match retail) -- test uses a local Tree::clear; propagated CWound::Kill name contradicted by its own comment | Codex source 00/ad/CWound_Kill_00ad7400.cpp, flags , pass pattern 00ad7400_TEST PASS
// Propagated CWound::Kill row -- destroys three STLport red-black trees embedded in a large non-polymorphic record.
// Retail 0x00ad7400, 197 bytes. Trees have headers at +0x84, +0x9c and +0xa8; each clears its root, restores
// sentinel links, zeroes count, and frees the sentinel. The propagated class name is retained only as catalog metadata.
extern "C" void __cdecl free(void* p); // 0x00BFEA14
struct CTreeNode { long color; CTreeNode* parent; CTreeNode* left; CTreeNode* right; };
struct CTreeA { CTreeNode* head; unsigned long count; long compare; void Erase(CTreeNode*); void clear(); };
struct CTreeB { CTreeNode* head; unsigned long count; long compare; void Erase(CTreeNode*); void clear(); };
struct CTreeC { CTreeNode* head; unsigned long count; long compare; void Erase(CTreeNode*); void clear(); };
void CTreeA::clear() { if (count) { Erase(head->parent); head->left=head; head->parent=0; head->right=head; count=0; } if (head) free(head); }
void CTreeB::clear() { if (count) { Erase(head->parent); head->left=head; head->parent=0; head->right=head; count=0; } if (head) free(head); }
void CTreeC::clear() { if (count) { Erase(head->parent); head->left=head; head->parent=0; head->right=head; count=0; } if (head) free(head); }
struct CTreeOwner {
    unsigned char _pad0[0x84]; CTreeA a; unsigned char _gap[0x0c]; CTreeB b; CTreeC c;
    ~CTreeOwner();
};
CTreeOwner::~CTreeOwner() { c.clear(); b.clear(); a.clear(); }