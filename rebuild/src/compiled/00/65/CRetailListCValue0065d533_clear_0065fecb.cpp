#pragma optimize("s",on)
// CRetailList_CValue_0065d533::clear() -- destroy every node's CValue_0065d533 value (@ 0x0065D533), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065FECB, oracle length 49 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov eax,[edi] ; mov esi,[eax] ; cmp esi,eax ; je E ; push ebx ; L: mov ebx,esi ; mov esi,[esi] ; lea ecx,[ebx+0x8] ; call CValue_0065d533::~CValue_0065d533 ; push ebx ; call free ; cmp esi,[edi] ; pop ecx ; jne L ; pop ebx ; E: mov eax,[edi] ; mov [eax],eax ; mov edi,[edi] ; mov [edi+4],edi ; pop edi ; pop esi ; ret
// Retail std::list::clear for CValue_0065d533 values: walks from head->next to the head (re-read from
// `this` each iteration), advancing the cursor before the old node's value is destroyed
// (@ 0x0065D533) and the node freed (free @ 0x00BFEA14, the argument popped into ecx: a size
// TU), then links the head to itself. The manifest label is propagated. One of 11 bodies.
// Leaf: ?clear@CRetailList_CValue_0065d533@@QAEXXZ
// Naming (NF180, 2026-11-20): the node value was the CValue_0065d533 stand-in; it is the shared CCountedPointer<CVillageTaskPickCrops>
//   (engine/CCountedPointer.h, FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY: the destructor called out of line), so the call binds
//   ??1?$CCountedPointer@VCVillageTaskPickCrops@@@@QAE@XZ, the spelling the self row 0x0065D533 defines (FableWin.pdb). The list is still the row-local
//   CRetailList_CValue_0065d533 (STLport list<CCountedPointer<CVillageTaskPickCrops> >::clear); its own leaf is unchanged (see NOTES).
extern "C" void __cdecl free(void* block);

#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
class CVillageTaskPickCrops;

struct CRetailList_CValue_0065d533 {
    struct node {
        node* next;                        // +0
        node* prev;                        // +4
        CCountedPointer<CVillageTaskPickCrops> value;// +0x8
    };
    node* head;
    void clear();
};

void CRetailList_CValue_0065d533::clear()
{
    node* n = head->next;
    while (n != head) {
        node* victim = n;
        n = n->next;
        victim->value.~CCountedPointer<CVillageTaskPickCrops>();
        free(victim);
    }
    head->next = head;
    head->prev = head;
}