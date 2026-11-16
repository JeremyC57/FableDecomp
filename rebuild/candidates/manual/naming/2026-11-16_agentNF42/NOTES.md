# NF42 round 5 notes (slice land1115ga..land1116p23, 157 entries)

## Outcome (counted from APPLIED.tsv)
done 24, done-already 1, rejected 2, partial 8, blocked 4, deferred 0, carry 118 (total 157).
Rows changed: 47 (STATUS.tsv). Final recheck (recheck_final.log, one run, all 47): 6 MATCH + 41 RELOCATION_MATCH, 47 tests PASS.
Grades used: 59 in work/grades.log (54 on the 47 changed rows, max 3 on one row; 4 on the abandoned 0x0065F6A6; 1 on the
abandoned 0x00B54310). No changed row is included by an EH funclet row (grep of rebuild/src/compiled/01/2*: none).
Compiles not counted as grades (no row changed by them, private dirs pO/ and pS/probe, since deleted): 47 compiles of the ORIGINAL
landed sources (work/orig/) to read the old self symbols and relocations for RENAMES.tsv, 11 fresh compiles of the landed callers
of self-renamed rows (work/probe_self.tsv), and the start-of-round binder probe (work/probe.tsv).

## Status vocabulary used
- `partial`: some binders or the self row fixed, the rest not ported this round (20, 38, 42, 50, 98, 110, 111, 112). Each note
  lists the rows left and why.
- `blocked`: 60 and 152 (port, 4 real grades on 0x0065F6A6: VC7.1 C2785 on every explicit instantiation of the dependent-return
  count_if); 75 and 127 (authoring: the only wrong binder of each, 0x008631A0 / 0x00C2DA00, is a jmp-thunk row whose whole body
  is `extern "C" void Forward() { ForwardTarget_<addr>(); }` under an unrelated catalogue name, so binding the claim means
  authoring the thunk as a real member forwarder).
- `carry`: never fixed this round. 23 of them (1 8 13 14 57 58 59 62 63 70 72 76 78 97 107 113 114 115 116 121 123 125 151)
  were triaged (rows read, reason recorded) but no port was attempted, so they stay carry, not blocked/port. The other 95 carry
  the fresh probe counts (ok/bad/unv) and the wrong binders with their sizes (lines / local classes) from work/badrows.tsv.
- Strict reading of rule 4: entries 44, 45 and 109 are `done` (their claims are bound exactly), but NF42 also corrected the own
  leaf of their binder rows 0x0042D5B1, 0x0042B6B8 and 0x00C4B010, whose landed callers still bind other spellings (next
  section). Those leaf fixes are in RENAMES.tsv section 1; the callers are follow-ups.
- Entry 108 (??1CString@@QAE@XZ): 441 CString-only binder rows were not probed (work/cstr_only.txt).

## Descriptive names (original found in no PDB / SDK)
- `PKCBufferedReader::ReturnAllBuffers` (0x00C4B010): libpk has no PDB or SDK symbol; the name is the landed row's own method
  name, moved from the address-named class CBufferOwner_00c4b010 onto libpk/PKCBufferedReader.h. Descriptive.
- STLport 4.6.2 instantiations, descriptive-by-library (both PDBs are Dinkumware builds; spelling from the STLport source + the
  shared fable_map.h / fable_containers.h / fable_algorithm.h text, template arguments only as far as the callers' bindings prove):
  0x0042955B _Rb_global<bool>::_Rotate_right; 0x00779B90 _Rb_tree_iterator<pair<const long,float>,_Const_traits<..>>::operator--;
  0x005B322B / 0x005B3100 / 0x0042D5B1 map<..>::operator[]; 0x005B31CF / 0x0042D4D3 / 0x0042D1A3 map<..>::insert;
  0x005B1DFE / 0x0042AD6C _Rb_tree<..>::_M_lower_bound; 0x0042CEE3 insert_unique; 0x0042CB08 _M_insert;
  0x005B1AFD std::__distance; 0x005B1FF4 std::distance; 0x0042A18F _List_base<CComponent*>::clear;
  0x0042B4C3 list<CComponent*>::_M_insert_dispatch<const_iterator>; 0x0042B6B8 list<CComponent*>::push_back;
  0x0066000A std::find_if / 0x0065F9AB std::__find_if (list<CIntelligentPointer<CThingAICreature>>::iterator, HasGuardProperty).

## Self renames whose landed callers still bind other spellings (RENAMES.tsv section 1; coordinator respell at landing)
Fresh compile of each caller (work/probe_self.tsv, work/selfcallers_rb.tsv); none of these callers was changed by NF42:
- 0x0042B6B8 list<CComponent*>::push_back: 0x0042BEAD binds ?Add_0042b6b8@Sub@@QAEXPAH@Z; 0x0059DE04 binds the 1-parameter
  ?push_back@?$list@PAVCComponent@NUISystem@@@std@@... (no allocator argument).
- 0x0042D5B1 map<unsigned long,CUIState>::operator[]: 7 callers bind local map-view spellings (0x005461D0, 0x0054BA20,
  0x00540980, 0x00545DF0, 0x005339B0, 0x005518E0, 0x00596A68).
- 0x00C4B010 PKCBufferedReader::ReturnAllBuffers: 0x00C4B040 binds ?helperA@@YIXPAX@Z (0x00C4B270 already binds the new name).
- 0x00BACD80 (entry 20, partial): 6 callers (0x00BA1A70, 0x00BABB50, 0x00BAD040, 0x00BB6E00, 0x00BBA480, 0x00BC7E90; this
  round's probe) bind SetName / RegisterEnable stand-ins.
- 0x00C25C50 / 0x00C25C70 / 0x00C25CB0 (entries 110-112, partial): 0x00C119D0, 0x00C21BA0, 0x00C21C20 bind CKey / Row00c21c20
  stand-ins whose layouts disagree with libpk/CSampleRef.h (0x00C11230 already binds all three).
- rebuild/oracles/reloc_bindings.tsv is stale for 0x0042971D (it lists a non-std _Rotate_right); the fresh probe shows both
  callers of 0x0042955B bind the new std:: spelling.

## Row notes
- 0x00B23C00 CEngine::SetSpotlightShadowMode: retail inlines FableWin's CEngineLightingManager::SetShadowMode (0x02C9E7C0 sits
  right after SetSpotlightShadowMode 0x02C9E790 in FableWin.pdb); the made-up CGlobalState / g_engineGlobal is gone.
- 0x00BAC0F0: the local "Flush" view was CEngineLightingManager::RestoreDefaultPrimitiveParameters (the shared header's inline).
- 0x00B54310 RenderMist (abandoned, row unchanged, lock released): the mist view is MistEffectColour +0xD0 (FableWin inline
  PeekGlobalMistEffectColour, 0x02D3D570), but the row's local C4DVector / CArray / CMatrix3x4 / CShaderRenderManager models
  collide with the shared lighting header (C2011 / C2953). 0x00B76BC0 has the same local models (not attempted).
- 0x0065F6A6 (abandoned, row unchanged, lock released): see entries 60 / 152.
- 0x00595356 keeps its local deque / stack models (only the list stand-ins moved onto fable_containers.h).
- 0x00B32230: the row keeps its pre-existing non-MSVC (host-only, `#ifndef _MSC_VER`) stub `class CEngineLightingManager`;
  NF42 added `void DisablePointLights();` to that stub so the host branch still parses. The MSVC branch uses the shared header.
- Data-global spellings in RENAMES.tsv section 2: 0x01396868 table (bound at +0x18) = `g_AllocatorInfo`, the shared
  engine/CMemoryAllocator.h spelling (descriptive there); 0x013CAA70 = `g_allocatorLock`, the spelling of the sibling allocator
  rows 0x009F2310 / 0x009F2610 (the tree has 5 spellings for that address; descriptive, no PDB name found).
- 0x00B32230 and 0x00BAC0F0 have self-contained behaviour tests (restated copies with their own fakes); they still PASS but do
  not exercise the new spellings.
- 0x005B1FF4 has a new test (rows/005b1ff4_test.cpp, hand-built 3-node tree, PassPattern kept); the old one linked a helper
  symbol the row no longer emits (LINK_FAIL in rc1).
- Locks: NF42 holds nflocks for exactly the 47 changed rows. No row needed was held by NF39/NF41; no rows deferred.

## Rule slips (work/slips.txt)
- One Bash call was refused by the harness (a foreground `sleep 60`); nothing ran.
- One Bash call exited 2 (an `ls` of a non-existent header without `; true`).
- The first grade of 0x008F84A0 timed out (240 s): the overlay LSocket.h's relative include of LHNetworkTypes.h did not resolve
  from hdr/engine; fixed by copying LHNetworkTypes.h unchanged into the overlay.
