// CRetailTree_erase004ad690::~CRetailTree_erase004ad690() -- std::map/set destructor: erase all nodes through 0x004AD690, reset the head, free it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ADFF0, oracle length 66 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call Erase ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ; F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Destructor of the retail red-black tree {head, size}: when the tree holds nodes the recursive
// erase @ 0x004AD690 (a __fastcall member taking the root, head->parent) frees them, then the
// head's left/parent/right are reset through `head` re-read per store and size is zeroed; finally
// the head node itself is released with free @ 0x00BFEA14 (null-checked). Only the erase callee
// distinguishes the instantiations, so the class is named by it. One of 119 bodies.
extern "C" void __cdecl free(void* block);

struct CRetailTree_erase004ad690 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    void Erase(node* root);                // 0x004AD690
    ~CRetailTree_erase004ad690();
};

CRetailTree_erase004ad690::~CRetailTree_erase004ad690()
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