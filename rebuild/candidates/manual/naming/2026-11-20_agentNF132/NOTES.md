# NF132 notes (PRIORITY slice 10 remainder, round 10)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 10 entries: 2 done (q4019, q4055), 5 blocked with rows left (q4100, q6186,
q6233, q6375, q6379), 3 open / not started this round (q4283, q4285, q4458; NF129's census in APPLIED stands). Rows: 26 ported
(rows/; final recheck 26 RELOCATION_MATCH + PASS in one vland run), 13 not-a-binder (listed in STATUS), 13 blocked. selfcheck: 0.

## Descriptive names
None new. Every name is a PDB spelling (Ego_r.pdb + FableWin.pdb), or an STLport 4.6 instance spelling (descriptive-by-library:
0x009AA7C0 `_Rb_tree<CCharString, pair<const CCharString, CBankFileManager::CBankInfo>, ...>::_M_erase`, 0x009AC310
`map<CCharString, CCharString>::~map`). The helper `DestroyMapOfCCharStringCCharString` in 0x009AC310 is not a retail function: it
only makes VC7.1 emit the implicit ~map COMDAT (the landed 0x007AEB70 recipe).

## Self renames (RENAMES section 1)
0x0099A360 (entry), 0x00412F60 CGame::Uninitialise, 0x004AE990 CNetworkClient::Uninitialise, 0x00A6C120 CSystemFont::Uninitialise
(the three `_Forward` jmp forwarders of q4019), 0x00A0A9D0 CRenderManagerCore::Uninit (leaf only), 0x00A3BDE0 ~CMovieSubtitles,
0x009AA7C0 (entry, leaf only), 0x009AC310 (entry), 0x0088FEE0 / 0x00890010 / 0x00898B30 (CGameScriptInterface members, leaf only).
Each new own leaf is either bound by every referencer already or its referencers are respelled in this hand-back:
- 0x00412F60: referencer 0x00401B80 already binds ?Uninitialise@CGame@@QAEXXZ.
- 0x004AE990: referencers 0x004166A8 / 0x004175E5 respelled here (both now bind ?Uninitialise@CNetworkClient@@QAEXXZ).
- 0x00A6C120: 0x00A0B060 already binds CSystemFont::Uninitialise; 0x00A0A9D0 / 0x00A0B590 respelled here.
- 0x00A0A9D0: referencer 0x009BF500 already binds ?Uninit@CRenderManagerCore@@QAEXXZ.
- 0x00A3BDE0, 0x0088FEE0, 0x00890010, 0x00898B30: no code referencer (Fable.exe scan; the last three are vtable slots; absolute
  dwords 0x00890010 in 0x00AF4E10.. are constants, no relocation).
- 0x009AA7C0, 0x009AC310: the claim spelling is what the other referencers bind; their stand-in spelling stays only on blocked rows.
Source file names of the self-renamed rows change class (e.g. CInitBaseClass_Uninitialise_00412f60.cpp is CGame's), for the coordinator.

## Spelling sets after this hand-back (no new split)
- 0x0099A360: one spelling (IAE) over all 8 REFS rows + self. 0x0099A310: one spelling over all 17. 0x00412F60 / 0x004AE990 /
  0x00A6C120 / 0x004AE940 / 0x00A0A9D0: one spelling each.
- 0x009BE060: 5 -> 3 (claim + the two list_T stand-ins of the blocked 0x009BE150 / 0x009BE190).
- 0x009AA7C0: 4 -> 3. 0x009AC310: 4 -> 3. 0x009AD410: 13 -> 4.
- Callees moved onto spellings already bound by landed rows: 0x0044C6B0 Get, 0x006B38D0 TransitionToThemeAllChannels, 0x009AA830
  _M_erase. 0x006B3680 SetThemeWeightAllChannels: the landed self row defines the PDB spelling and 0x00890010 was its only caller.
- 0x013BCA14 (the counter Uninitialise decrements; NF129's note said ...18, which is the ctor's) keeps the three spellings it had
  (g_count / g_initCount / g_initialisedCount_013bca14); not widened, not fixed (no PDB data name found).

## Header opt-ins
See hdr/README.md: CInitBaseClass.h (VALIDATE, UNINITIALISE), CGame.h (UNINITIALISE), CNetworkClient.h (UNINITIALISE,
INITIALISEASLOCAL), CMovieSubtitles.h (CSUBTITLE_STRUCT), CEnvironment.h (SETTHEMEWEIGHTALLCHANNELS).
Header debt seen: engine/CMovieSubtitles.h's default `class CSubTitle;` mangles V where both PDBs key it U (a class-key fix of the
default line would be a header claim; the opt-in covers my rows).

## Local models
No new local model. Local models dropped for shared headers: 0x0099A360 (S), 0x00412F60 / 0x004AE990 / 0x00A6C120 (extern "C"
forwarders), 0x00A0A9D0 (CInitBaseClass, CInitBaseClassCalls, CRenderManagerCore_Methods), 0x004166A8 (CInitBaseClass,
CNetworkClient_Methods), 0x004175E5 (CInitBaseClass; its local CMainGameComponent model now derives from engine/CGameComponent.h:
the typed 8-aligned CNetworkClient member moved every field +4 behind the model's own vptr, DIFFER(354v354), and the base restores
the retail layout), 0x00B260B0 (NativeInitBaseClass), 0x00A3BC20 (CVideoSys, CMovieSubtitles, CSubtitleList, CWideString,
CTextureRenderer), 0x00A3BDE0 (CRetailList_009be060, CNamedEntryList_00a3bde0), 0x009AA7C0 / 0x009A7990 / 0x009AC310 (tree / list
/ CBankFileManager models), the q6233 CDefinitionManager stand-ins. In-place respells only in 0x00487AF0, 0x009A60D0, 0x00B25D80
(access / virtual-ness of Uninitialise in their existing CInitBaseClass models) and 0x00A0B590 (Uninitialise moved from its local
CInitBaseClass model to its existing local CSystemFont model).

## Flags lines
None added. (Leaf lines added where the HEAD row had none: 0x00A3BC20, 0x00A3BDE0, 0x009AA7C0, 0x009A7990, 0x009AC310, 0x0088FEE0
family kept theirs.)

## Funclets
No EH funclet under rebuild/src/compiled/01/21 includes any changed row's file.

## Blocked rows and why
- 0x009BE150 / 0x009BE190 (q4100), 0x009AABE0 / 0x009AB640 and 0x009AB700 / 0x009AC200 (q6186): byte-identical, unreferenced pairs
  (list<CSubTitle> ~list / ~_List_base; the Banks tree's clear and ~_Rb_tree / ~map copies). Each binds the entry address through its
  own stand-in class, so a respell needs a real own leaf, and which copy is which has no second source (no caller, no PDB order;
  the 0x007AEB70 "member run" adjacency argument does not separate them here). A coordinator call could accept an adjacency rule.
- 0x009A7B70 ~CBankFileManager (q6186): a whole-class port; it moves 0x009AA880 (set<CWideString> _M_erase) onto a PDB spelling no
  landed row binds while 0x009AB7C0 / 0x009AAD20 / 0x009AC3C0 / self keep stand-ins (would grow that set).
- 0x00D7C6D0 / 0x00D84B20 / 0x00E2AB30 (q6375/6379): free-function AutoTiny stand-ins of NScript scalar deleting destructors;
  need the script classes' ??_G (implicit dtor + vtable in the TU); CQ_HeroSoulsThunderScript.h declares a user dtor, the other two
  classes have no header.
- 0x006E11F0, 0x008902E0, 0x008981C0 (q6233): see STATUS (namespaced CCharString stand-in; full re-port; CMainGameComponent port).

## Blocked rows by kind (13)
- Identity unprovable (6, no grade spent: no form can be graded without picking a name): 0x009BE150, 0x009BE190, 0x009AABE0,
  0x009AB640, 0x009AB700, 0x009AC200.
- Family / no-new-split (1, no form tried): 0x009A7B70.
- Ports with ZERO forms tried (6; not exhausted, a later pass can take them): 0x00D7C6D0, 0x00D84B20, 0x00E2AB30 (NScript ??_G),
  0x006E11F0, 0x008902E0, 0x008981C0.

## Old leaves in RENAMES
Every section-1 old_leaf was read from the HEAD object (pH compiles: 0x00A0A9D0, 0x00A3BDE0, 0x009AC310, 0x009AA7C0, 0x00898B30,
0x0088FEE0, 0x00890010) or the HEAD source's own Leaf line / NF129 probe (0x0099A360 ?M@S@@QAEXXZ; `_Forward` for the three
forwarders, extern "C"). Section 2 old symbols are the HEAD pairs printed by pdiff (binds.py).

## Decisions for the coordinator
(c) 0x004175E5 CMainGameComponent::Shutdown: its pre-existing local CMainGameComponent model now has a base list
    (`: public CGameComponent`, the shared header, Ego_r's real base) in place of its own `virtual v00(); Socket[0xC]`; this is what
    made the typed CNetworkClient member keep retail offsets. Accept, or hold the row (then 0x004AE990's spelling set keeps the
    stand-in Uninitialise_004ae990 of that row next to the PDB name).
(a) The unreferenced byte-identical STL pairs above: accept an adjacency rule (as 0x007AEB70 did) to name them, or leave blocked?
(b) engine/CMovieSubtitles.h `class CSubTitle;` -> `struct CSubTitle;` as the default (header claim; one landed includer).

## Rule slips
- Two Bash calls exited non-zero: the first orientation `wc` over naming-dir globs (one LESSONS file missing) and a two-row pdiff
  loop that ended on a `grep` with no match (0x0088FEE0 / 0x00890010; both grades were fine).
- pdiff.sh grades the HEAD side with my overlay set (FABLE_HDR_OVERLAY); harmless (opt-ins only) but noted.
- grades_used counts include the HEAD compile of each pdiff; max 4 (0x009A7990: two C2039 forms before FABLE_RETAIL_STLP_LIST).

## 0x00A3BDE0 form
Final form is the implicit-destructor COMDAT helper (recipe from 0x007AEB70): `void DestroyCMovieSubtitles(CMovieSubtitles* subtitles)
{ subtitles->~CMovieSubtitles(); }` on engine/CMovieSubtitles.h with CWIDESTRING + CSUBTITLE_STRUCT + STLP_LIST_CLEAR_OUTOFLINE; the
emitted ??1CMovieSubtitles@@QAE@XZ is the graded symbol. No header DTOR opt-in (removed). Its list-clear callee binds 0x009BE060 by
the claim spelling. Final recheck (2026-11-20, env sourced): 26/26 RELOCATION_MATCH + PASS.

## Final checks (2026-11-20)
- The final row objects for 0x00A3BDE0 and 0x00A3BC20 both bind 0x009BE060 as
  `?clear@?$_List_base@UCSubTitle@@V?$allocator@UCSubTitle@@@std@@@std@@QAEXXZ` (objdump -r), the claim spelling, so the two rows agree.
- Rule slip: the first final vland run was started without `source $S/wine/env.sh`. ROOT fell back to `D:\Documents\FableTLC`, it
  crashed on a read and overwrote recheck_final.log, and it was rerun at once with the env sourced. It wrote nothing under the repo.
  The directory `/home/user/FableDecomp/D:\Documents\FableTLC` dates from 2026-11-19 13:14, before this run, and has no newer files.
  README.md / rebuild/COVERAGE.* changed at 05:35:37, after my run ended; neither vland nor selfcheck writes them, so another process did.
