// CRetailTree_erase0070adb0::Erase(node* root) -- STLport _Rb_tree::_M_erase: frees a subtree without rebalancing (recurse right, destroy the value, free the node, continue left)
// Leaf: ?Erase@CRetailTree_erase0070adb0@@QAEXPAUnode@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070ADB0, 67 bytes, `__fastcall`, `ret 4`, self-recursive.
// Identity: the manifest label LTextTreeWalkThrough::BuildTreeArray is propagated across every byte-similar tree erase. The
// landed tree destructors that call this address bind it as CRetailTree_erase0070adb0::Erase(node*) (the catalog convention for
// these instantiations, named by the erase address), so the row keeps that symbol.
// Callees: free @ 0x00BFEA14 (the node and the buffer).
// Layout: STLport node -- colour +0, parent +4, left +8, right +0x0C, value +0x10: a 12-byte key and a malloc-allocated buffer.
// Shape: `while (x) { Erase(x->right); y = x->left; destroy(value); free(x); x = y; }` rotated into a guarded do-while;
// the value's member destructors are inline (out-of-line string destructors called on their addresses).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CRetailBuffer {                             // STLport vector body (start, finish, end of storage), malloc storage
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    ~CRetailBuffer()
    {
        if (_M_start)
            free(_M_start);
    }
};

struct SNodeValue_0070adb0 {
    unsigned long  key[3];                     // +0x10
    CRetailBuffer  second;                     // +0x1C
};

struct CRetailTree_erase0070adb0 {
    struct node {
        unsigned char colour;
        unsigned char _pad[3];
        node*         parent;                      // +0x04
        node*         left;                        // +0x08
        node*         right;                       // +0x0C
        SNodeValue_0070adb0 value;                // +0x10
    };
    node*         head;
    unsigned long size;
    void Erase(node* root);
};

void CRetailTree_erase0070adb0::Erase(node* x)
{
    while (x != 0) {
        Erase(x->right);
        node* y = x->left;
        x->value.~SNodeValue_0070adb0();
        free(x);
        x = y;
    }
}