// Behaviour test for retail 0x007b7a70: the candidate source below is embedded verbatim (from src/007b7a70.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
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
class CTCOverheadDisplay { public:
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

// ===== harness =====
#include <stdio.h>
#include <string.h>
float g_OverheadValueExpiryFrames = 2.0f;
static void* g_freed[16]; static int g_nfree, g_armed;
extern "C" void __cdecl free(void* p) { if (g_armed) g_freed[g_nfree++] = p; }   /* spy; nodes below are static */
void CTCOverheadDisplay::VFunc00() {}
static CLongFloatListNode nodes[3][4];
static void build(CLongFloatList& l, CLongFloatListNode* hdr, const long* firsts, int n) {
    l.node = hdr; hdr->next = hdr->prev = hdr;
    for (int i = 0; i < n; ++i) { CLongFloatListNode* x = &hdr[1 + i]; x->value.first = firsts[i]; x->value.second = 0.5f * i;
        x->next = hdr; x->prev = hdr->prev; hdr->prev->next = x; hdr->prev = x; }
}
static int count(const CLongFloatList& l) { int c = 0; for (CLongFloatListNode* x = l.node->next; x != l.node; x = x->next) ++c; return c; }
int main() {
    CTCOverheadDisplay d;
    static const long dmg[3] = { 5, -1, 0 };      /* after --: 4 keep, -2 erase (<= -2), -1 keep */
    static const long rep[1] = { -1 };            /* -2 erase */
    static const long ren[2] = { -5, 10 };        /* -6 erase, 9 keep */
    build(d.DamageValues, nodes[0], dmg, 3); build(d.RepairValues, nodes[1], rep, 1); build(d.RenownValues, nodes[2], ren, 2);
    g_armed = 1; d.vfunc_9(); g_armed = 0;
    int ok = g_nfree == 3 && g_freed[0] == &nodes[0][2] && g_freed[1] == &nodes[1][1] && g_freed[2] == &nodes[2][1];
    ok = ok && count(d.DamageValues) == 2 && count(d.RepairValues) == 0 && count(d.RenownValues) == 1;
    ok = ok && nodes[0][1].value.first == 4 && nodes[0][3].value.first == -1 && nodes[2][2].value.first == 9;
    ok = ok && nodes[0][1].next == &nodes[0][3] && nodes[0][3].prev == &nodes[0][1] && nodes[0][3].value.second == 1.0f;
    printf(ok ? "007b7a70_TEST PASS\n" : "007b7a70_TEST FAIL\n"); return ok ? 0 : 1;
}