# NF6 notes (agentNF6, 2026-11-10)

Hand-back: `rows/` (200 row sources, 13 tests), `STATUS.tsv`, `RENAMES.tsv`, `NAME_FIXES.tsv`, `LESSONS.md`.
No header changed, so `hdr/` is empty and there is no `hdr/README.md`. Row opt-ins use existing `#ifdef` macros
(FABLE_RETAIL_CAIBRAIN_FORCERESET, FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_SCRIPTSUPERVISION, CTHINGCREATUREBASE opt-ins, etc.).
Every row in `rows/` was graded through verify_and_land parity plus reloc_identity, one compile at a time.
- Each is MATCH or RELOCATION_MATCH, with the target bindings OK and no new BAD/HARD binding against the landed scan.
- Each test passes under g++ -std=c++11 (64-bit) and VC7.1 /Od, or fails the same way on the landed source (PREFAIL, listed below).

## Job 1: /Zc:wchar_t (15 rows)
- **12 applied, 3 rejected** because they are in BUSY.txt: 0x0099BA70, 0x00A0CDD0, 0x007E5570. Each needs the same edit once released; the wanted CompilerFlags are in STATUS.
- **Catalog CompilerFlags to edit in build_candidates.ps1:** 0x0099B150, 0x00A10A60, 0x004917B5, 0x0049124E, 0x00A0CA20.
  - Each changes `'/O2 /Oy /W3 /Zc:wchar_t'` to `'/O2 /Oy /W3'`.
- **Source `// Flags:` line only, catalog unchanged:** 0x00E68DC0, 0x0042D4F7, 0x00E81B40, 0x0049EF50, 0x00492BAB, 0x00A3B9D0, 0x009E4C70.
  - The "(no /Zc:wchar_t ...)" comment had been parsed as a flag token. It now sits on its own `// Wide text:` line.
  - /GS is kept where it is real.
- The `_W` entries in reloc_bindings.tsv for these rows are now stale; they bind the G spellings.
- 0x0042D4F7 / 0x0099B150 / 0x00A0CA20 carry `#define __forceinline inline` portable shims inherited from the landed source. I did not add them.

## Job 2: QUEUE.tsv (149 entries)
- **No claim rejected outright.** The one `_W` entry, 0x009E4CB0 GetItemFromPath, was rejected as spelled and applied as the G twin `?GetItemFromPath@CDriveManager@@AAE_NPBGKPAG@Z`. Its landed binder 0x009E4DA0 already binds G.
- **Evidence by symbol type:**
  - Function symbols were checked against the Ego_r.pdb and FableWin.pdb name sets (work/egor.txt, work/fw.txt). Most are in both. Three are FableWin-only: 0x0042D9E0, 0x007180C0, 0x0042AF3C/0x00513160 (the GetDef templates).
  - Data globals and vtables rely on Codex's raw PDB record evidence plus the retail data contents. For example, the string globals were read back from Fable.exe: "WEAPON_FOCUS_01", "weapon_pos_a", "weapon_pos_b", "ACTION_POINT_1".
  - The CVectorMap::LowerBound / STLport template entries are the pointer twins of FableWin's Dinkumware spellings.
  - 0x00C462E0 CFloatQuad::Interpolate is descriptive (Codex: no original found).
- **Results:** 188 binder or self rows were re-authored across 31 targets, plus 2 found-on-the-way fixes, 0x009ADA90 and 0x009AD6E0 (NAME_FIXES.tsv).
  - STATUS.tsv: 37 applied and 112 partly. "Partly" means the claim was verified but some landed binders still use the wrong spelling, or the queue address's own row still self-spells it.
- **Binder discovery covered only part of the corpus.** I compile-scanned only the rows that bind the smaller queue targets (2,974 rows; `work/scan.out`).
  - The "left" lists in STATUS are therefore complete only for those targets.
  - For the big shared targets they are lower bounds: CCharString, CBaseIntelligentPointer, the CIntelligentPointer vtables, CCharString::NullChar, the CVectorMap LowerBound pair, QuestManager PClass, and GetDefinitionManager.
  - A full compile scan of all 12.9k binder rows is about 3.5 h at about 60 rows/min.
- **Self rows respelled:**
  - 0x009ADA40 (QBE + CDefPointer<const CDefClassBase>)
  - 0x009ADA90 (IAE + LoadDef IBE)
  - 0x00661F70 IsChild (QBE)
  - 0x00662000 GetCreatureType (was a CountReferences stand-in; catalog GetMovementMode, see RENAMES)
  - 0x0057AB14 (class CTCHeroStats, not CTCHeroStats_Retail)
  - 0x00860A50 GetNearestNode const
- **PREFAIL_TEST rows** fail the same way on the landed source: 0053ddd0 0055a510 00662c70 00662ef0 006e2a50 006eda80 0073a9b0 00745730 007f90b0 0080e5d0 00891720 0093e090 0096e700 00a25d20 00a42ea0.
  - The usual causes are engine headers asserting 32-bit pointers under 64-bit g++, or a non-standard PASS line.
  - New portable tests now PASS for 009ada40, 009ada90, 00416231, 00494380, 0042dbfa, 006a7530, 00661f70, 00662000.
- 0x00494380 still self-spells an address-suffixed class `CFrontendGameComponent_00494380`. This predates NF6 and is out of scope for this queue.

### Left for a later wave (needs a real rewrite, not a respell)
- **0x006622A0 IsPerformingAnyAction:** retail exits with `mov eax,1` / `xor eax,eax`, which VC7.1 does not emit for `bool ... return true`. The bool const respell gave DIFFER(23v26). Its 9 binders were left too.
- **0x007AED10 GetBossPhase:** `#include "engine/CTCBoss.h"` hung cl.exe for more than 400 s, twice (once in apply, once alone). Retail is `mov eax,[ecx+0x10]; ret` at CTCBoss+0x10 currentBossPhase; the rewrite is in work/selfsrc/007aed10.cpp. Its 7 binders were left.
- **Self stand-ins still DIFF**, each needing its class model rebuilt:
  - 0040d2a0, 0042af3c, 0042d9e0, 004874d0
  - 0048ca60 (SortTreeRecursively stand-in for `_List_base` dtor)
  - 005bf19c (CCategory_Retail), 005d97b0 (callee 005d94b0 and global 013b8c18 unnamed)
  - 006290e0, 0069f230, 008c7fd0 (ctor/dtor reference params)
  - 00694d10, 006cba20, 007180c0, 007525c0, 00744730, 00782e90, 008645e0, 0086b810
  - 0091a670, 0093e800, 0097dcb0, 009ac700, 009fc210, 009fc380 (CResourceBank_Retail, pointer param), 009fc440, 00bacdd0
  - 0088bb60 self (CAIBrain_0088bb60::ForceResetWithOutput)
- **Large binder groups (job 3, not reached):**
  - 013B89FC QuestManager PClass (72 scanned; CTBaseSingletonNoDefaultAlloc private static, needs friend/accessor modelling)
  - 0042AF3C GetDef<CThingBaseDef> (67)
  - 006869D0 CTCBase::GetDefinitionManager (56)
  - 01238C6C / 01237F1C CIntelligentPointer vtables (52 / 29)
  - 004365B0 / 0040F020 CVectorMap::LowerBound (45 / 29)
  - 004C7990 PeekDefinitionManager (39)
  - 0129AAF4 CCharString::NullChar (39)
  - 00A01B50 GetPItem (38), 00A01B90 SetPItem (21), 00A01C10 (13), 00A01B10 (9)
  - 0042B0A2 GetSubDef<CAppearanceDef> (38; same-TU stand-in bodies)
  - 0062E900 ~CCombatActionBase (30)
  - 0099EAE0 / 0099EBF0 / 0099EC30 CCharString dtor / const char* ctor / copy (26 / 8 / 8)
  - 0097DB70 (22), 004C9D60 AddTC (21), 009AD410 (20), 008FCF40 (17), 0097E200 (14), 0042D9E0 (13), 0097E380 (12)
  - 00486024 vector push_back (11; needs std::vector)
  - The 12 GetCurrentMode (0x0073A6E0) rows that EDITERR'd model CModeBase or the manager differently and need hand work.
  - SetCurrentAction rows with a stand-in CCombatActionBase without a base (24).
- 007ed060/007ed0a0/007ed170/007ed2c0/007ed560/007ed870, 0085ff10 and 008da890 failed to compile or edit in the SetCurrentAction / CThing batches: SRC_FAIL, or a test that was already g++-broken.
- **Bookkeeping:** the GetDef-by-name family rows (GetDef_xxxxxxxx placeholders) now bind 0x009ADA40 correctly. Their own GetDef<T> identity is still unresolved.
