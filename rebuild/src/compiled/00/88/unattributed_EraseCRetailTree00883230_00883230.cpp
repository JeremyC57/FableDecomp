// CRetailTree_00883230::Erase(SNode*) -- unlink through the __cdecl helper @ 0x0042A34E (node, &head->parent, &head->left, &head->right), destroy the CValue_00a01c10 value at +0x14, free the node, return the decremented size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00883230, oracle length 66 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; push edi ; lea ecx,[eax+0Ch] ; push ecx ; lea edx,[eax+8] ; push edx ; add eax,4 ; push eax ; mov eax,[where] ; push eax ; call Unlink ; mov edi,eax ; add esp,10h ; lea ecx,[edi+0x14] ; call CValue_00a01c10::~CValue_00a01c10 ; test edi,edi ; je F ; push edi ; call free ; add esp,4 ; F: mov eax,[esi+4] ; dec eax ; mov [esi+4],eax ; pop edi ; pop esi ; ret 4
// Dinkumware-style tree erase: the __cdecl unlink helper @ 0x0042A34E takes the node and the
// addresses of the head's parent/left/right links (formed in place with `add eax,4` for the
// last) and returns the unlinked node, whose CValue_00a01c10 value at +0x14 is destroyed
// (@ 0x00A01C10) before the node is freed through the null-tested deallocate (free @
// 0x00BFEA14); the size is decremented and returned (`mov/dec/mov` form). The manifest label
// is propagated; the tree is named descriptively. One of 13 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct CValue_00a01c10 {
    void* m_data;
    ~CValue_00a01c10();                             // 0x00A01C10
};

struct CRetailTree_00883230 {
    struct SNode {
        SNode* left;                       // +0
        SNode* parent;                     // +4
        SNode* left2;                      // +8  (head->left)
        SNode* right;                      // +0xC (head->right)
        unsigned char _pad[0x4];
        CValue_00a01c10 value;                      // +0x14
    };
    SNode* head;                           // +0
    unsigned long size;                    // +4
    unsigned long Erase_CRetailTree_00883230(SNode* where);
};

extern "C" CRetailTree_00883230::SNode* __cdecl Unlink_0042a34e(CRetailTree_00883230::SNode* where, CRetailTree_00883230::SNode** root, CRetailTree_00883230::SNode** leftmost, CRetailTree_00883230::SNode** rightmost);   // 0x0042A34E

unsigned long CRetailTree_00883230::Erase_CRetailTree_00883230(SNode* where)
{
    SNode* victim = Unlink_0042a34e(where, &head->parent, &head->left2, &head->right);
    victim->value.~CValue_00a01c10();
    if (victim)
        free(victim);
    return --size;
}