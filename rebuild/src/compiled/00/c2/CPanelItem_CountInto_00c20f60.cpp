// CPanelItem::CountInto(CPanelItemStats* stats) -- check the item state (any state accepted), then bump the per-state counter of the caller's statistics record for item states 2..6
// Leaf: CountInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C20F60, 100 bytes (`ret 4`, five-entry embedded jump table at 0x00C20FB0).
//   Descriptive names (no PDB covers this middleware library): the manifest's "Weapon_IncrementSlotCounter" is
//   propagated. Its only caller is CPanel::GetItemStatistics 0x00C12300 (`item.CountInto(stats)` over the 0xB4-byte
//   items of the panel's item array, after IsActive 0x00C20840); the item's 16-bit state word sits at +0xAC.
// Callees: CPanelItem::ChangeState @ 0x00C20720 (the state helper every item method calls first; -1 = no change).
// Shape: `push -1 ; call` and then `movzx eax,[ecx+0ACh]` WITHOUT reloading ecx: VC7.1 knows the helper leaves ecx
//   alone because its body is compiled earlier in the same TU (the stand-in body below, `auto_inline(off)`).
//   The switch is biased by 2 (`add eax,-2 ; cmp eax,4 ; ja`), every case returns on its own `ret 4`, and the case
//   bodies are laid out in source order: retail's table (2 -> 0xC20FA4, 3 -> 0xC20F87, 4 -> 0xC20F9A,
//   5 -> 0xC20F90, 6 -> 0xC20F7D) puts case 6 first, then 3, 5, 4, 2. The counters live at
//   state 3 -> +0, 2 -> +4, 4 -> +8, 6 -> +0xC, 5 -> +0x10 (verified case by case against the table).

struct CPanelItemStats {                        // 0x18-byte statistics record filled by CPanel::GetItemStatistics
    long m_count0;                              // +0x00  items in state 3
    long m_count1;                              // +0x04  items in state 2
    long m_count2;                              // +0x08  items in state 4
    long m_count3;                              // +0x0c  items in state 6
    long m_count4;                              // +0x10  items in state 5
    long m_total;                               // +0x14
};

struct CPanelItem {                             // 0xB4-byte panel item
    unsigned char  _pad00[0xac];
    unsigned short m_state;                     // +0xAC
    void ChangeState(long state);               // 0x00C20720
    void CountInto(CPanelItemStats* stats);
};

#pragma auto_inline(off)
// Same-TU stand-in for the retail state helper 0x00C20720 (retail: a byte-indexed jump table over the state word
// whose every case is empty; it touches only eax): compiled above the caller so VC7.1 knows it preserves ecx.
// An EMPTY stand-in is worse: VC7.1 then drops the call altogether.
extern long g_panelItemRequestedState;          // stand-in only (not a retail global)
__declspec(noinline) void CPanelItem::ChangeState(long state)
{
    g_panelItemRequestedState = state;
}
#pragma auto_inline(on)

void CPanelItem::CountInto(CPanelItemStats* stats)
{
    ChangeState(-1);
    switch (m_state) {
    case 6: ++stats->m_count3; return;
    case 3: ++stats->m_count0; return;
    case 5: ++stats->m_count4; return;
    case 4: ++stats->m_count2; return;
    case 2: ++stats->m_count1; return;
    }
}