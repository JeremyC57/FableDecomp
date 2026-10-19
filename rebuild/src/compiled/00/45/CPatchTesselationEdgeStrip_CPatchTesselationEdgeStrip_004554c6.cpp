// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
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