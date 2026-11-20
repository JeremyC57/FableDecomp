#pragma optimize("s",on)
// CRetailList_CValue_0065d574::clear() -- destroy every node's CValue_0065d574 value (@ 0x0065D574), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065FE38, oracle length 49 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov eax,[edi] ; mov esi,[eax] ; cmp esi,eax ; je E ; push ebx ; L: mov ebx,esi ; mov esi,[esi] ; lea ecx,[ebx+0x8] ; call CValue_0065d574::~CValue_0065d574 ; push ebx ; call free ; cmp esi,[edi] ; pop ecx ; jne L ; pop ebx ; E: mov eax,[edi] ; mov [eax],eax ; mov edi,[edi] ; mov [edi+4],edi ; pop edi ; pop esi ; ret
// Retail std::list::clear for CValue_0065d574 values: walks from head->next to the head (re-read from
// `this` each iteration), advancing the cursor before the old node's value is destroyed
// (@ 0x0065D574) and the node freed (free @ 0x00BFEA14, the argument popped into ecx: a size
// TU), then links the head to itself. The manifest label is propagated. One of 11 bodies.
extern "C" void __cdecl free(void* block);

// Name fix (NF202, 2026-11-20, q12230): the element release at 0x0065D574 is CCountedPointer<CVillageTaskTorch>::ReleaseData (FableWin.pdb
//   ?ReleaseData@?$CCountedPointer@VCVillageTaskTorch@@@@QAEXXZ + the body: the destructor is the 5-byte `jmp` COMDAT(s) 0x0065E298 / 0x0065F925 at /Os,
//   `~CCountedPointer() { ReleaseData(); }`, as engine/CCountedPointer.h documents for CComponent / CAction / CMeshDataBank);
//   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (Data zeroed before Info).
//   The node value is the shared CCountedPointer<CVillageTaskTorch> (was the CValue_0065d574 stand-in with an out-of-line destructor); the
//   list type keeps its landed CRetailList_CValue_0065d574 token (in-place, as NF180 0x0065FE9A).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CVillageTaskTorch;


struct CRetailList_CValue_0065d574 {
    struct node {
        node* next;                        // +0
        node* prev;                        // +4
        CCountedPointer<CVillageTaskTorch> value;                      // +0x8
    };
    node* head;
    void clear();
};

void CRetailList_CValue_0065d574::clear()
{
    node* n = head->next;
    while (n != head) {
        node* victim = n;
        n = n->next;
        victim->value.~CCountedPointer<CVillageTaskTorch>();
        free(victim);
    }
    head->next = head;
    head->prev = head;
}