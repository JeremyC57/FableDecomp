// CRetailTree_erase0056d440::Erase(node* root) -- STLport _Rb_tree::_M_erase: frees a subtree without rebalancing (recurse right, destroy the value, free the node, continue left)
// Leaf: ?Erase@CRetailTree_erase0056d440@@QAEXPAUnode@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056D440, 73 bytes, `__fastcall`, `ret 4`, self-recursive.
// Identity: the manifest label LTextTreeWalkThrough::BuildTreeArray is propagated across every byte-similar tree erase. The
// landed tree destructors that call this address bind it as CRetailTree_erase0056d440::Erase(node*) (the catalog convention for
// these instantiations, named by the erase address), so the row keeps that symbol.
// Callees: free @ 0x00BFEA14 (the node), ~CCharString @ 0x0099EAE0, ~CWideString @ 0x0099B510.
// Layout: STLport node -- colour +0, parent +4, left +8, right +0x0C, value +0x10: a 16-byte key, then CCharString +0x20 and two CWideStrings +0x24/+0x28 (destroyed last to first).
// Shape: `while (x) { Erase(x->right); y = x->left; destroy(value); free(x); x = y; }` rotated into a guarded do-while;
// the value's member destructors are inline (out-of-line string destructors called on their addresses).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                // 0x0099B510
};

struct SNodeValue_0056d440 {
    unsigned char  key[0x10];                  // +0x10
    CCharString    name;                       // +0x20
    CWideString    text0;                      // +0x24
    CWideString    text1;                      // +0x28
};

struct CRetailTree_erase0056d440 {
    struct node {
        unsigned char colour;
        unsigned char _pad[3];
        node*         parent;                      // +0x04
        node*         left;                        // +0x08
        node*         right;                       // +0x0C
        SNodeValue_0056d440 value;                // +0x10
    };
    node*         head;
    unsigned long size;
    void Erase(node* root);
};

void CRetailTree_erase0056d440::Erase(node* x)
{
    while (x != 0) {
        Erase(x->right);
        node* y = x->left;
        x->value.~SNodeValue_0056d440();
        free(x);
        x = y;
    }
}