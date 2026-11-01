# Naming pass, wave 17 (one agent beside two chunk agents on the lane)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it** -- a proven wrong spelling in a landed source is respelled in this wave, not held. Resolved decisions:
`naming/2026-11-01_wave16/DECISIONS_W16_RESOLVED.tsv` (+ waves 13-15); canonical globals / members in `naming/ESTABLISHED_SPELLINGS.tsv`.
Rules, mechanics and hand-back as `briefs/BRIEF_W16.md` / `BRIEF_W15.md` (read both, the wave 16 README + AGENT_README, DEFERRED.tsv,
QUEUE_W16.tsv and the wave 16 `scripts/` -- compose16.py, fam_*.py, hand16/ -- reused as compose17.py etc.). RENAMES.tsv paths are
repo-rooted (`rebuild/src/compiled/...`, `rebuild/tests/...`); new_name is the catalog's bare spelling (no `class`/`struct` keywords).
Labels the chunks already proved are in `ghidra_out/labels_chunk675..678_2026-10-27.tsv` (catalog and manifest carry them; the SOURCES do not).

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family)
1. **The NAME_FIXES_QUEUE rows c675-c676** (`$S/naming/w17/QUEUE_W17_input.tsv`; the c677/c678 rows are appended by the coordinator as
   those chunks land -- re-read the file before scope 3): the ??_G stand-ins (0x004942A2 ~CSubtitleRenderer, 0x006C9C30, 0x00DBEFA0),
   the landed 0x00D25410 file `CQNewOakValeIntroScript_CQNewOakValeIntroScript_00d25410.cpp` -> `NScript::CQ_DragonBossFightScript` ctor
   (file rename + ps1 + the vtable binding 0x012CB70C -> ??_7CQ_DragonBossFightScript@NScript@@6B@), the STLport instantiations (two
   `_M_erase`, `_List_base<CCountedPointer<CCreatureActionBase>>::clear` 0x00669B50, `~deque<CSetTextInfo>` 0x004427D0, the `V?$allocator`
   class-key manglings 0x00451583 / 0x00450CCC, set<unsigned long> members), GetAllLeafThings<CThingBuilding,...> 0x00947780,
   CTCShop::GetNumShoppers, CAIStateGroupBase::PeekThingSearchTools / GetPWorldSeed, CThingFilter_HasTC::operator(), CCharString::ToWideString
   0x0099E480, ControlledMove protected, CountRemainingGroups ABEKXZ in 0x00AD8FE0 / 0x00ADA240, SelectRandomShop -> FindASimplyDelightfulShop
   in 0x00947280, the 12 anonymous-namespace `*_ORIENTATION` constants (lane TU hash), g_AudioContext unified at 0x0143BDB0, the
   alSourcef/alSourcei slot fix in 0x00C72AF0 (`pfnSourcei` at +0x50 is alSourcef; alSourcei is +0x5C -- name-only if the bytes hold).
2. **Wave 16 DEFERRED.tsv**: the partial rows' remaining stand-in callers (00510d40, 00692910, 006edc50, 009918f0, 0099a330, 00a555c0,
   00a5c600, 00ac1ee0, 00b866a0, 00bd9ce0, 00f37030), the 009aa780 sibling rows (009aaca0, 009ab6b0, 009ac0f0, 009ac130), the 0x00404720
   own row (fix its behaviour test, then respell) and 009ae150, 004bfac0 (NGraphicAppearance namespace), 008edff0 callers, the PClass
   singleton 013b89fc (~60 rows: `?PClass@?$CTBaseSingletonNoDefaultAlloc@V<T>@@@@2V?$CCountedPointer@V<T>@@@@A` per T), then the
   destructor families that need class-view rewrites where the bytes hold (0099a300 / 0099a2e0 / 0099a410 / 009fc550 / 00aa75c0 / 00f35b40).
3. **The mega families, biggest first** (name-only; one family commit each): ConstantFPS 0x01375550 (314 rows), `bitset::test` (239),
   `CBasicString::Compare` (138), GetPItem (903 left), ~CCharString (135 left), GetSubDef, GetInstance, `Transfer<>`, AddEvent.
Not in this wave: the byte-moving tcb/bip/cmb/cab/bcn rows (re-authoring), 0x00C28690 (libpk name unknown), VIRTUAL_SCREEN_* /
PMainGameComponent (gated), the engine header changes (migration wave 10). 0x00440730 stays on HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W17.tsv (every queue row with its outcome), the per-family rows
files, logs and scripts, as wave 16 did; leave no lv_* directory and no background job.
