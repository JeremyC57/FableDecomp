// Behaviour test for retail 0x004554c6: the candidate source below is embedded verbatim (from src/004554c6.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CInventoryDef::~CInventoryDef() -- free the ten STLport vector buffers in reverse declaration order, then tail-call ~CParentDefClassBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x004554C6, 172 bytes.
// Leaf: ??1CInventoryDef
// Identity: called by the scalar deleting destructor at 0x004554AA, slot 0 of vtable 0x01233404 = CInventoryDef
//   (rebuild/manifest/rtti-vtables.tsv; slot 20 = 0x004554A4 returns sizeof 0xA34). The manifest label
//   "~CPatchTesselationEdgeStrip" is propagated (that PDB class is 0x2c bytes and owns no such buffers).
// Callees: CRT free import thunk 0x00BFEA14 (GOTCHAS: wave 25), ~CParentDefClassBase 0x00430300 (frees the packed
//   SubDefInfoMap buffer at +0x29, restores vtable 0x01230BA0, jumps to ~CDefClassBase 0x009FC550).
// Layout: PDB (Ego_r) member names; retail vectors are 12-byte STLport vectors (PDB: 16), so each retail offset is
//   the PDB offset minus 4 (base) minus 4 per preceding vector: e.g. ExpSpendItemLabelAbility PDB +0x808 -> +0x7E0.
//   Opaque runs between vectors are named by their first PDB member. Local model: engine/CParentDefClassBase.h is a
//   flat generated layout without the destructor this function must call.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);                    // 0x00BFEA14
template<class T> struct CRetailVector {                // STLport vector: begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    ~CRetailVector() { if (m_first) free(m_first); }
};
struct CRGBColour { unsigned long Colour; };
struct CParentDefClassBase {                            // retail 0x3c bytes incl. alignment of the packed tail
    unsigned char _base[0x3c];
    ~CParentDefClassBase();                             // 0x00430300
};
struct CInventoryDef : CParentDefClassBase {
    unsigned char InitialCategoryMovementRepeatDelay_to_TrackSelectedItems[0x20]; // +0x3c..+0x5b
    CRetailVector<long> InitialInventoryClothingItems;  // +0x5c
    CRetailVector<long> InitialInventoryWeaponsItems;   // +0x68
    CRetailVector<long> InitialInventoryStatsItems;     // +0x74
    CRetailVector<long> InitialInventoryStandardItems;  // +0x80
    unsigned char ClothingShadedCircleTLPos_to_ButtonAssignmentTextOffset[0xc8]; // +0x8c..+0x153
    CRetailVector<unsigned long> StatLevelsTexts;       // +0x154
    CRetailVector<CRGBColour> StatLevelsColours;        // +0x160
    unsigned char WeaponsTextSeparator_to_ExperienceItemLabelStat[0x3f0];        // +0x16c..+0x55b
    CRetailVector<long> ExperienceItemLabelStat;        // +0x55c
    CRetailVector<long> ExperienceItemLabelAbility;     // +0x568
    unsigned char ExperienceItemLabelAbility_to_ExpSpendItemLabelStat[0x260];    // +0x574..+0x7d3
    CRetailVector<long> ExpSpendItemLabelStat;          // +0x7d4
    CRetailVector<long> ExpSpendItemLabelAbility;       // +0x7e0
    ~CInventoryDef();
};
CInventoryDef::~CInventoryDef() {}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
static void* g_freed[16]; static int g_nfree, g_base, g_order_ok = 1, g_armed;
extern "C" void __cdecl free(void* p) { if (!g_armed) return; if (g_base) g_order_ok = 0; g_freed[g_nfree++] = p; }   /* spy: records each buffer */
CParentDefClassBase::~CParentDefClassBase() { ++g_base; }
static unsigned char g_storage[sizeof(CInventoryDef)];
int main() {
    static const int off[10] = {0x7e0,0x7d4,0x568,0x55c,0x160,0x154,0x80,0x74,0x68,0x5c};  /* retail free order */
    static char bufs[10][4];
    memset(g_storage, 0, sizeof g_storage);
    for (int i = 0; i < 10; ++i) if (i != 3) *(void**)(g_storage + off[i]) = bufs[i];      /* +0x55c left empty */
    int ok = offsetof(CInventoryDef, ExpSpendItemLabelAbility) == 0x7e0 && offsetof(CInventoryDef, StatLevelsTexts) == 0x154;
    CInventoryDef* d = (CInventoryDef*)g_storage;
    g_armed = 1; d->~CInventoryDef(); g_armed = 0;
    ok = ok && g_base == 1 && g_order_ok && g_nfree == 9;
    for (int i = 0, k = 0; ok && i < 10; ++i) { if (i == 3) continue; ok = g_freed[k++] == bufs[i]; }
    printf(ok ? "004554c6_TEST PASS\n" : "004554c6_TEST FAIL\n"); return ok ? 0 : 1;
}