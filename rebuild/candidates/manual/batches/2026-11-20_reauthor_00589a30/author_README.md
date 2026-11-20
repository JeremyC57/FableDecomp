# reg589 (followup author, 2026-11-20): 0x00589A30 CTCInventory::OpenPCMapMenu re-authored -- READY (test caveat)

| Row | Grade (strict, --recheck-landed, no overlay) | Test | Mutants |
|---|---|---|---|
| 0x00589A30 CTCInventory::OpenPCMapMenu | RELOCATION_MATCH PASS (vland.log) | PASS: HOST MODEL (pre-existing kind, does not link the row) | none (a host model cannot kill row mutants) |

On today's tree the landed text grades DIFFER(549v549): one reload pair after the inline `repe cmpsb` swapped (ours
`mov edi,[esp+14h]` before `mov esi,[esp+18h]`, retail the other way round at 0x00589A97). That is a c2 symbol-number effect of
ce3b25d5d (GFDebugPDynamicCast in its own header). No header was changed back.

Fix: the row is now written in Ego_d.pdb's text (line table and locals in the source header): Ego_d names, the condition-form search
loop `count < Components.size() && !pmap_list`, `if (pmap_list) { Initialise; push_back; }` and the FindChild block inside a second
`if (pmap_list) { }` (Ego_d's block scope for the four child locals), and two scroll locals each reused for two buttons (Ego_d: four
child locals for six FindChild calls). Matches on the first compile. v_brk.cpp (the same text with the landed `if (list) break;` loop) also
matches; Ego_d's line table (7032 = 48 bytes, no break line) picks the condition form. landed.cpp = the old text (DIFFER today).
No `// Inferred:` lines; no header change; no NAME_FIXES. src.diff = against the landed file.

Open: the landed test (and this updated copy) is a host model. A real-object test needs these 10 externals of the row object:
??0CCharString@@QAE@PBDJ@Z, ??1CCharString@@QAE@XZ, ??8?$CBasicString@D@@QBE_NPBD@Z, ?ConstructPCMapList@CTCInventoryMap@@...,
?DeleteMenuComponents@CTCInventory@@QAEXXZ, ?GetInstance@CManager@NUISystem@@SIPAV12@XZ, ?test@?$bitset@$0BBC@@std@@QBE_NI@Z,
malloc / free / memmove, plus fake CComponent objects with hand-built vtables (GetName +0x150 returning CCharString by hidden pointer,
Initialise +0xAC, FindChild +0x0C taking CCharString by value, ChangeState slot 48, the CObserver base at +4 with ObserveEvent slot 3,
the manager's AddObserver slot 2) and a CThing whose TC bitset / CVectorMap give the map TC. A `__fastcall F(T* self, int edx, ...)`
stands in for each thiscall slot. Not written (wind-down).
