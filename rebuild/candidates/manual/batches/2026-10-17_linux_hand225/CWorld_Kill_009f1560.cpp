// CWorld::Kill-shaped propagated row -- clears three STLport red-black trees and restores the intermediate-base vtable
// Retail 0x009f1560, 185 bytes.  The propagated CWound::Kill label is not trusted: the body is a destructor shell for
// a polymorphic object with three 12-byte STLport tree headers at +4, +0x10, and +0x1c; each clears its root,
// re-links the sentinel, frees the sentinel, then the intermediate base vtable is restored at +0.
extern "C" void __cdecl free(void* p); // 0x00BFEA14
struct CTreeNode { long color; CTreeNode* parent; CTreeNode* left; CTreeNode* right; };
struct CTreeA { CTreeNode* head; unsigned long count; long compare; void Erase(CTreeNode*); void clear(); };
struct CTreeB { CTreeNode* head; unsigned long count; long compare; void Erase(CTreeNode*); void clear(); };
struct CTreeC { CTreeNode* head; unsigned long count; long compare; void Erase(CTreeNode*); void clear(); };
// 0x009F0D40
// 0x00579435
// 0x00579435
void CTreeA::clear() { if (count) { Erase(head->parent); head->left=head; head->parent=0; head->right=head; count=0; } if (head) free(head); }
void CTreeB::clear() { if (count) { Erase(head->parent); head->left=head; head->parent=0; head->right=head; count=0; } if (head) free(head); }
void CTreeC::clear() { if (count) { Erase(head->parent); head->left=head; head->parent=0; head->right=head; count=0; } if (head) free(head); }
struct __declspec(novtable) CTreeBase { virtual ~CTreeBase() {} };
struct __declspec(novtable) CTreeOwner : CTreeBase {
    CTreeA a; CTreeB b; CTreeC c;
    ~CTreeOwner();
};
CTreeOwner::~CTreeOwner() { c.clear(); b.clear(); a.clear(); }
