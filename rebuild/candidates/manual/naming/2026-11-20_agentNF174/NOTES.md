# NF174 notes (round 10: script / UI / creature-mode naming)

## Descriptive names
- No made-up names so far. The scaffolding helper `ReferenceArcheryCompetitionScriptDtor` (0x00E37680, anonymous namespace,
  never called) is descriptive scaffolding, not a retail name.

## q4102 0x00E32350 CV_ArcheryCompetitionScript ctor
- Self row ported onto engine/CV_ArcheryCompetitionScript.h with the overlay opt-in FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CTOR
  (the landed 0x00E2AB60 CV_AmbushScamScript recipe). RELOCATION_MATCH in one grade; the vtable 0x012E2EE4 now binds
  `??_7CV_ArcheryCompetitionScript@NScript@@6B@` (exe scan: this row is the vtable's only landed referencer; the ctor has no landed caller,
  the Alloc 0x00E37630 is not landed).
- **One definition (definer census, not in REFS):** the landed dtor row 0x00E37680 defined the same PDB ctor strongly (selection 1).
  Link of the renamed 0x00E32350 + HEAD 0x00E37680 with `LINK=` cleared: LNK2005 (link/ctl_00e32350.log). Claimed 0x00E37680 and
  dropped the ctor definition (declaration kept); ??1 is emitted by an anonymous-namespace qualified-destructor helper
  (`// Fidelity: scaffolding, not retail`, GOTCHAS nf86 / nf88). f1 (inline ctor + local object) still emitted the ctor as a pick-any
  COMDAT and gave LNK2005 against the strong owner (selection 1 vs 2), so the owner cannot be strong while any includer emits the ctor.
  f2: RELOCATION_MATCH, link of both new objects rc=0, LNK4088 only (link/new_pair.log).
- Same HEAD clash elsewhere (not in my locks, not touched): the AmbushScam pair 0x00E2AB60 (ctor row) + 0x00E32310 (dtor row defines
  the same PDB ctor). The f2 recipe above would fix 0x00E32310.
- Side finding (NAME_FIXES): 0x00E37660 (slot 0 of 0x012E2EE4) is landed as the free stand-in
  `CLandscapeBackgroundPatch_vector_deleting_destructor`, binding 0x00E37680 as `CLandscapeBackgroundPatch_dtor_00e37680`; it is
  `??_GCV_ArcheryCompetitionScript@NScript@@UAEPAXI@Z` (FableWin.pdb). Existing split on 0x00E37680, untouched. The new 0x00E32350
  object emits that ??_G pick-any, so a re-author of 0x00E37660 can reuse the ctor TU.
- Defined-symbol diff (objdump -t, external defined symbols), 0x00E32350 HEAD -> new: removed the local stand-ins
  `??0CScriptObject@@...`, `??1CScriptObject@@UAE@XZ`, the global-namespace `??0CV_ArcheryCompetitionScript@@QAE@PAXPAVCGameScriptInterface@@@Z`,
  `??_7CV_ArcheryCompetitionScript@@6B@`, `??_GCV_ArcheryCompetitionScript@@UAEPAXI@Z`, `??1CV_ArcheryCompetitionScript@@UAE@XZ`; added
  the row's own leaf `??0CV_ArcheryCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z`, the implicit
  `??_7` / `??_G` / `??1CV_ArcheryCompetitionScript@NScript@@...` (pick-any; ??1 is also emitted pick-any, selection 2, by its owner
  0x00E37680) and the shared-header CLevelScriptBase<CQ_SunnyvaleMasterData> inlines (??0 / ??1 / ??_7 / ??_G, pick-any, as every
  landed archery includer). `??0CTimer@@QAE@XZ` / `??_H` / `??_I` unchanged. The pair links clean (link/new_pair.log).
- 0x00E37680 defined-symbol diff HEAD -> new: removed the strong ctor, `??_7` / `??_G` of CV_ArcheryCompetitionScript, `??0CTimer@@QAE@XZ`,
  `??_H`, the CLevelScriptBase ctor / vtable / ??_G; added only the anonymous-namespace helper (`?A0x...`, TU-local). Own leaf unchanged.

## q4356 0x009C9070 GetPTextEntry: done-already
- HEAD probe of all 43 REFS rows (work/probe_databank.tsv): every one binds the PDB spelling, including 0x0065C781 (the wave-34
  leftover). Self row defines it. No landed object binds the old `?GetPGroupEntry@CTextBank@@...` spelling.

## q4639 0x00EF7AB0 ~CIsThingConscious: blocked (port), 1 row left
- HEAD probe (work/probe_q4639.tsv): 14 of 15 REFS rows bind `??1CIsThingConscious@NScript@@UAE@XZ`. Left: 0x00EF7800
  COpinionMan::Main binds `??1CIsThingStored@NScript@@UAE@XZ`. That row models the one retail class CIsThingConscious (vtable
  0x012C2FE8) as two local shadow classes, `CIsThing` (temporary, inline dtor under an inline_depth(0) holder) and `CIsThingStored`
  (the And's member, out-of-line dtor), to get retail's inline / out-of-line split. It also binds 0x012C2FE8 as `??_7CIsThing@NScript@@6B@`
  (that vtable already has 4 spellings), 0x012F6818 as `??_7CPredicate_And@NScript@@6B@` (0x00EF7B10 binds the template spelling) and the
  ctor 0x00EF7B50 as `??0CPredicate_And@NScript@@QAE@ABUCIsThing@1@ABVCIsThingAlive@1@@Z`. A port onto CIsThingConscious.h +
  CPredicate_And.h fixes all four. Three forms graded (STATUS); best DIFFER(628v627). Drafts: work/00ef7800_f1..f3.cpp. f2 / f3 define
  ~CIsThingConscious inline, so their objects emit ??1CIsThingConscious@NScript@@UAE@XZ pick-any against the strong owner 0x00EF7AB0
  (LNK2005, the q4102 case): a landable form needs the q11224 pattern first (header-inline dtor opt-in + pick-any scaffolding owner).

## q5066 0x00CBC130 vector<pair<u64,CPersistInfo>>::_M_insert_overflow: done
- Census (HEAD probe work/probe_q5066.tsv): 0x00CBC7B0 binds the owner spelling; 0x00CB8440 bound the local
  `CVector<CEntry>` stand-in. Self row 0x00CBC130 defines the entry spelling (HEAD compile). The reloc_bindings line for the correct
  spelling has an empty witness column (not used as evidence).
- 0x00CB8440 ported (form 3; f1 DIFFER(560v582), f2 / f3 RELOCATION_MATCH): STLport vector / pair / list from
  stlport/fable_containers.h, CCharString from fable_string.h, CStringParser from engine/CStringParser.h (READASUHUGE /
  SKIPUNTILWHITESPACE opt-ins), CKeyPairCompareLess from engine/CVectorMap.h, FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE and
  FABLE_RETAIL_STLP_SORT_FASTCALL. The owner class stays the row's pre-existing local `struct NScript::CRuntimeScriptedEntityPersist` (with
  its nested `CMap`; both tokens were defined at HEAD; vland warns SHARED_HEADER_AVAILABLE for both, pre-existing). The shared header
  declares no PersistScripts member and no PDB name exists for the map's Clear, so a full port onto CRuntimeScriptedEntityPersist.h /
  CVectorMap.h needs a header claim (PersistScripts as CVectorMap<unsigned __int64, CPersistInfo>, plus a proven Clear name).
- Callee spellings moved (count never grows): 0x00CBBDA0 erase onto the owner row's exact defined symbol (HEAD compile of 0x00CBBDA0);
  0x00CBD370 sort onto the owner row's exact defined symbol (HEAD compile of 0x00CBD370); 0x004046B0 from `??1?$list@...` to
  `??1?$_List_base@...` (the folded STLport body; both spellings were already bound, 0x00998A30 / 0x00998570; GOTCHAS nf104).
  0x00CBC660 keeps `?Clear@CMap@CRuntimeScriptedEntityPersist@NScript@@QAEXXZ`.
- One definition: the new object emits the vector instances (push_back, _M_insert_overflow, ...) pick-any; link of the new 0x00CB8440
  with the HEAD 0x00CBC130 and 0x00CBC7B0 objects: rc=0, no LNK2005 (link/q5066_link.log).
- Existing splits seen, not in my entries (for the coordinator): 0x00CBBDA0 also bound as `?Assign@Holder@@...` (0x00CB80F0) and
  `?erase@?$CRetailVector@U?$SPair...` (0x00CBC660); 0x00CBC660 as `?Finalize@Holder@@QAEXXZ` (0x00CB80F0); 0x00CBD370 as
  `?GuiVarBool_Apply@@YIXPAXH_N@Z` (0x00CBD5B0). The 0x00CBBDA0 owner row's catalog name is `_global::Container_MoveAndConstruct`
  although it defines vector::erase.
- Made-up helper: `SkipPastText` (anonymous namespace in 0x00CB8440, descriptive; was the row's local CStringParser member).

## q5211..q5214 (CCS_OakValeRevisitedScript / CQR_EscortTraderScript, parked rows)
- None of the four addresses is a landed row and the exe scan finds no landed referencer. q5211 / q5213: catalog fixes in
  NAME_FIXES (two sources each: both PDB name sets + the exe call / member-pointer evidence). q5212 / q5214: catalog already right.
- q5214's queued "wrong reloc bindings" for 0x00E20210: reloc_bindings.tsv has no line targeting 0x00E20210 (a first awk listing
  compared numerically and matched every 00eDDDDD target; the string compare gives 0) (the only absolute
  reference in Fable.exe is 0x00E1F97B inside the parked 0x00E1F7E0).

## q5222 0x00403750 ~CCountedPointer<NGameText::CDataBank>: done
- Census: 0x00418B7A / 0x0042F50B bind the PDB spelling; 0x004175E5 CMainGameComponent::Shutdown bound the local
  `?Release@CCountedPointer403750@@QAEXXZ`. Respelled in place: the member at +0x14 is now the shared
  `CCountedPointer<NGameText::CDataBank>` (engine/CCountedPointer.h, FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY), released by an
  explicit destructor call then `Data = 0` (`// Inferred:` line; the HEAD row's Clear() sequence).
- Rejected form: `Reset(0)` under FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR matched but emitted a pick-any
  `?Reset@?$CCountedPointer@VCDataBank...` with a body that is not retail's Reset<CDataBank> 0x00403E40 (the landed 0x00416832 calls that
  one out of line): two objects would carry different bodies under one pick-any name. (The vector instances 0x00CB8440 emits
  and the implicit members 0x00E32350 emits are the usual header-inline pick-any COMDATs every includer emits; the difference here is
  that a retail out-of-line copy with another body exists.)
- Same row, not my entries: the other counted-pointer releases are still stand-ins (`?Release@CCountedPointer4190b6@@` 0x004190B6
  ~CCountedPointer<CLUA>, `...4057a0` 0x004057A0 <CFontBank>, `...419108` 0x00419108 <CGraphicDataBank>, `g_GlobalCountedPointer`).
- Side finding: 0x00403E40's own row defines a `CCountedPointer_CIPhysicsFacesSegmentBase_const::Reset` stand-in while the landed 0x00416832
  binds 0x00403E40 as `?Reset@?$CCountedPointer@VCDataBank@NGameText@@@@QAEXPAVCDataBank@NGameText@@@Z` (existing split, one source; not
  in NAME_FIXES).

## q6494 / q6519 0x00E7E020 CPredicate_And<CIsThingAlive,CIsVariableFalse> ctor: done-already
- HEAD probe (work/probe_q6494.tsv): all 4 REFS rows bind the FableWin.pdb spelling (0x03B39C60); the self row defines it. The
  CAndPredicate / CIsFlagFalse stand-ins are gone from every landed object.

## q6496 0x012ECBA8 vtable: done-already (ExposeMayor reading rejected)
- The vtable is CSpawnedFunc<CV_HauntedHouseScript>'s (REJECTED.tsv has the evidence). HEAD probe: both binders bind it. The
  reloc_bindings.tsv witness line `??_7?$CSpawnedFunc@VCV_ExposeMayorScript@NScript@@@@6B@ 012ecba8 00e95f10` is stale.

## q11869 0x009C8FB0 BuildText: done
- Access-only self rename QBE -> private ABE (in place, as the twin 0x009C8700); the sole referencer 0x009C9070 respelled in place
  (private const declaration + `friend class CDataBank;` in its pre-existing local NGameText::CDataBankTextEntry model).

## q11945 / q11946 (NUISystem::CDeletion ctors): open, census done; q11947 0x0054EAC0 ~CViewport: done
- HEAD probe work/probe_ui.tsv (90 lines): every REFS row binds the PDB CDeletion / CViewport spellings except the family
  0x0059DB82 (RefreshEntriesAmountsAndAvailabilities) + 0x0059E6BD (RecurseRefreshEntriesAmountsAndAvailabilitiesPC), which bind
  `??0CAction@N_0042bed4@NUISystem@@QAE@XZ` / `..@ABU012@@Z`. Both rows model NUISystem::CComponent locally (vfunc placeholders);
  engine/NUISystem_CDeletion.h includes engine/NUISystem_CComponent.h, so the port moves CComponent and CDeletion together
  (slot names in APPLIED). Sibling recipe: 0x005A887D RefreshBuyPossibilityPC (FABLE_RETAIL_CDELETION + _BODIES, CTCInventoryTrade.h).
- 0x0054EAC0 is parked; only manifest-gaps.tsv still says ~CMesh (NAME_FIXES).

## Resume pointers (entries still open, queue order)
- q11945 / q11946: port 0x0059DB82 + 0x0059E6BD as above (0x0059E6BD lock claimed NF174; was `released`).
- q11950 0x013B8A98 CList::GPreviousSelected (data, protected static `1V`): 34 REFS rows, 12 stand-in spellings in WORKLIST; probe first.
- q11954 / q11959 0x008E3860 + q11956 0x008D8620 (dragon family): NF157 work/008e3860_f1.cpp; the landed 0x008E31A0 defines the
  FireBeam ctor strongly (one-definition, same class of problem as q4102: the qualified-dtor helper recipe of 0x00E37680 applies);
  claim 0x008D86E0 / 0x008E31A0 first.
- q12007 0x00E7FD50, q12008 0x00E9E350, q12017 0x00696340, q12018 0x00696900: small censuses (2 / 1 / 1 / 5 REFS rows).
- q12011..q12014 CTeamSpawn family (NF163 held/): the ??_G owner 0x00DCF6E0 clash is the q4102 pattern; do the ctor + ??_G together.

## Flags lines
- None added.

## Header opt-ins
- engine/CV_ArcheryCompetitionScript.h: FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CTOR (declaration only; hdr/README.md).
- No other overlay. Rows use existing repo opt-ins only (FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY, FABLE_RETAIL_STLP_SORT_FASTCALL,
  FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, FABLE_RETAIL_CSTRINGPARSER_READASUHUGE / _SKIPUNTILWHITESPACE).

## Decisions for the coordinator
- q4102: the dtor row 0x00E37680 no longer defines the ctor (one definition); the pair links clean. The AmbushScam pair
  0x00E2AB60 / 0x00E32310 has the same LNK2005 at HEAD (not touched).
- q5222 0x004175E5: `m_14.~CCountedPointer(); m_14.Data = 0;` carries an `// Inferred:` line (the source spelling of
  "release then null" is unproven; Reset(0) was rejected because it emits a non-retail Reset<CDataBank> COMDAT).
- No stand-in type was cast to a real one; no new local class model was added (0x00CB8440 keeps its HEAD tokens
  CRuntimeScriptedEntityPersist / CPersistInfo / CMap; CVector / CEntry / CCompare / local CStringParser / CCharString / std::list
  were removed in favour of the shared headers).

## Rule slips
- One HEAD compile was first run without sourcing env.sh (q.py traceback, no grade); rerun through tools/hd.sh.
- The first selfcheck call ran outside the repo root (traceback); rerun from /home/user/FableDecomp.
- While writing the 0x00CB8440 comment I first wrote a retail address range I had not measured; removed before any grade or
  hand-back (the row carries no unmeasured address).
- q4102 definer census: `grep -rnE '^[^/]*CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript\('` over rebuild/src/compiled finds only
  0x00E32350 and 0x00E37680; no in-class inline definition of the ctor exists in rebuild/src/compiled or rebuild/include.
- 0x004046B0 (`~list<CStringParserCommentBoundaries>`) is a landed owner row; 0x00CB8440 now emits `??1?$_List_base@...` pick-any as the
  other shared-header includers do (not added to the q5066 link proof).

## Locks outside rows.txt
- 0x00E37680 (claimed, changed), 0x0059E6BD (was `released`; claimed NF174, untouched).
