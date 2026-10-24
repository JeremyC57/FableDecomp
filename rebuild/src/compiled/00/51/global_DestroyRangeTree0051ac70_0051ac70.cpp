// DestroyRange_Tree_0051ac70(CRetailTree_erase004a9cb0* first, CRetailTree_erase004a9cb0* last, CRetailAllocator allocator) -- range destructor for 12-byte STLport set/map elements: clears each tree (top erase level inline) and frees its header
// Leaf: DestroyRange_Tree_0051ac70
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051AC70, 112 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx), `ret 4`.
// Identity: the manifest label LTextTreeWalkThrough::Destructor_DeleteNodes is propagated. The body is the STLport
// `_Destroy_range(first, last)` for a vector of trees whose `_M_erase` is 0x004A9CB0 (bound as CRetailTree_erase004a9cb0::Erase
// by the landed tree destructors 0x004AC140 / 0x00517850; its nodes carry no destructible value). Named by shape like the
// landed DestroyRange_CountedPointer_00ad37e0; the element type is not identified.
// Callees: CRetailTree_erase004a9cb0::Erase @ 0x004A9CB0 (the right subtrees), free @ 0x00BFEA14 (nodes and headers).
// Shape: the element destructor (clear + header free) is inline, and clear()'s `_M_erase(root)` is expanded ONE level
// (VC7.1 inlines a directly recursive inline function once: inline_recursion off), so only the right-subtree calls go out
// of line; `last` is spilled to the push-ecx slot because ebx carries the left child inside the expanded loop; the zero
// register is ebp.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CRetailTree_erase004a9cb0 {                 // STLport set/map with a trivially destroyed value (12 bytes)
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;                            // +0x00
    unsigned long size;                            // +0x04
    unsigned long _comp;                           // +0x08
    void Erase(node* x)                            // 0x004A9CB0 (out-of-line copy)
    {
        while (x != 0) {
            Erase(x->right);
            node* y = x->left;
            free(x);
            x = y;
        }
    }
    ~CRetailTree_erase004a9cb0()
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

struct CRetailAllocator { };

void __fastcall DestroyRange_Tree_0051ac70(CRetailTree_erase004a9cb0* first, CRetailTree_erase004a9cb0* last, CRetailAllocator allocator)
{
    for (; first != last; ++first)
        first->~CRetailTree_erase004a9cb0();
}