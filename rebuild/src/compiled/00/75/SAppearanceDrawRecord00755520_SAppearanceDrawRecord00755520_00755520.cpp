// SAppearanceDrawRecord_00755520::~SAppearanceDrawRecord_00755520() -- destroy a CTCLook::OnAppearanceDraw stack record: its red-black tree (+0x58) erased and its head freed, the member at +0x4c destroyed, the counted pointee at +0x3c released
// Leaf: ??1SAppearanceDrawRecord_00755520
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00755520, length 103 bytes.
// Label correction: catalogued as CHitLocation::~CHitLocation, but CHitLocation is 24 bytes (Ego_r.pdb; its destructor
// is the landed 0x00710900) and this body touches +0x5c; its only caller is CTCLook::OnAppearanceDraw (0x007542B0, the
// local at [esp+114h]); the class is named by address.
// Callees: the tree's recursive erase @ 0x004ABA70 (on the tree, root = head->parent; manifest
// "LTextTreeWalkThrough::BuildTreeArray" propagated), free @ 0x00BFEA14, the +0x4c member's destructor @ 0x005BC0D3
// (manifest "CActiveFile::OnReadFinished" propagated), the pointee's virtual slot 1 (Delete) at a zero count.
// Shape: no vtable store (a plain class); members die in reverse order: the tree destructor inlined (as the landed
// ~CRetailTree 0x008353F0: erase, head links reset, size zeroed, head freed), then the out-of-line member, then the
// CIVCountedPointer release (as the landed CHitLocation destructor 0x00710900).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
};
struct SMember_005bc0d3 {                          // 12 bytes, destroyed out of line
    unsigned char _body[0x0c];
    ~SMember_005bc0d3();                           // 0x005BC0D3
};
struct CRetailTree_004aba70 {                      // the retail red-black tree {head, size}
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;
    unsigned long size;
    void Erase(node* root);                        // 0x004ABA70
    ~CRetailTree_004aba70()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};

struct SAppearanceDrawRecord_00755520 {
    unsigned char        _pad00[0x3c];
    CIVCountedPointer    m_pointee;                // +0x3c
    unsigned char        _pad40[0x0c];
    SMember_005bc0d3     m_member4c;               // +0x4c
    CRetailTree_004aba70 m_tree;                   // +0x58
    ~SAppearanceDrawRecord_00755520();
};

SAppearanceDrawRecord_00755520::~SAppearanceDrawRecord_00755520()
{
}