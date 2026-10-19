// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CTCOverheadDisplay::vfunc_9() -- age the damage/repair/renown overhead values one frame and erase the expired ones
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B7A70, 240 bytes; slot 9 of vtable 0x0126DA94 = CTCOverheadDisplay (rtti-vtables.tsv).
// Each list is an STLport list<pair<long,float> > (one header-node pointer; node = next, prev, pair); erased nodes go to
//   the CRT free import thunk 0x00BFEA14 (GOTCHAS: wave 25). The expiry threshold is an unnamed float global in .bss
//   at 0x013BAE68 (no label); a value is erased once its frame count is <= -threshold.
// Layout: PDB (Ego_r) CTCOverheadDisplay names: DamageValues +0x0c, RepairValues +0x10, RenownValues +0x14 (retail lists
//   are 4 bytes, PDB 12). Local model: no engine/ header exists for CTCOverheadDisplay.
#include <stddef.h>
extern "C" void __cdecl free(void*);                    // 0x00BFEA14
struct CLongFloatPair { long first; float second; };
struct CLongFloatListNode { CLongFloatListNode* next; CLongFloatListNode* prev; CLongFloatPair value; };
struct CLongFloatList { CLongFloatListNode* node; };    // STLport list<pair<long,float> >: the header node
struct CTCOverheadDisplay {
    virtual void VFunc00();
    unsigned char _base04[8];                           // CTCBase: +0x04..+0x0b
    CLongFloatList DamageValues;                        // +0x0c
    CLongFloatList RepairValues;                        // +0x10
    CLongFloatList RenownValues;                        // +0x14
    void vfunc_9();
};
extern float g_OverheadValueExpiryFrames;               // 0x013BAE68
void CTCOverheadDisplay::vfunc_9()
{
    CLongFloatListNode* n = DamageValues.node->next;
    while (n != DamageValues.node) {
        --n->value.first;
        if ((float)n->value.first <= -g_OverheadValueExpiryFrames) {
            CLongFloatListNode* next = n->next; CLongFloatListNode* prev = n->prev;
            prev->next = next; next->prev = prev; free(n); n = next;
        } else n = n->next;
    }
    n = RepairValues.node->next;
    while (n != RepairValues.node) {
        --n->value.first;
        if ((float)n->value.first <= -g_OverheadValueExpiryFrames) {
            CLongFloatListNode* next = n->next; CLongFloatListNode* prev = n->prev;
            prev->next = next; next->prev = prev; free(n); n = next;
        } else n = n->next;
    }
    n = RenownValues.node->next;
    while (n != RenownValues.node) {
        --n->value.first;
        if ((float)n->value.first <= -g_OverheadValueExpiryFrames) {
            CLongFloatListNode* next = n->next; CLongFloatListNode* prev = n->prev;
            prev->next = next; next->prev = prev; free(n); n = next;
        } else n = n->next;
    }
}