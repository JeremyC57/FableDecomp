// atexit destructor for CRetailTree_erase005e1f30 g_tree_013b8c1c @ 0x013B8C1C -- erase all nodes through 0x005E1F30, reset the head, free it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012290C0, oracle length 84 bytes.
// Grade: candidate -- still UNGRADED in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  mov eax,[g_tree_013b8c1c.size] ; test eax,eax ; je F ; mov eax,[g_tree_013b8c1c.head] ; mov ecx,[eax+4] ; push ecx ; mov ecx,offset g_tree_013b8c1c ; call Erase ; mov eax,[head] ; mov [eax+8],eax ; mov edx,[head] ; mov dword ptr [edx+4],0 ; mov eax,[head] ; mov [eax+0Ch],eax ; mov dword ptr [size],0 ; F: mov eax,[head] ; test eax,eax ; je R ; push eax ; call free ; pop ecx ; R: ret
// The `atexit` thunk (`??__F`, section .text$yd) of a static retail red-black tree {head, size}
// at 0x013B8C1C: its inlined destructor erases the nodes through the __thiscall Erase
// @ 0x005E1F30 when the tree holds any, resets the head's left/parent/right (the head
// pointer re-read per store) and the count, then releases the head node with free
// @ 0x00BFEA14. The object is a translation-unit static with a trivial inline constructor
// (only the destructor thunk is graded). The manifest label is propagated; the object is
// named by its address. One of 15 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct CRetailTree_erase005e1f30 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    void Erase(node* root);                // 0x005E1F30
    CRetailTree_erase005e1f30() : head(0), size(0) { }
    ~CRetailTree_erase005e1f30()
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

static CRetailTree_erase005e1f30 g_tree_013b8c1c;                   // 0x013B8C1C: head at +0, size at +4

CRetailTree_erase005e1f30* Get_g_tree_013b8c1c()
{
    return &g_tree_013b8c1c;
}
