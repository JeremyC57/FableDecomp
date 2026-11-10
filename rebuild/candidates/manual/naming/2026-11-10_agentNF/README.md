# 2026-11-10 agentNF -- Codex fixes [5]-[10], missed-binder sweep, CCountedPointer ReleaseData

75 landed rows respelled or re-authored; all rechecked under VC7.1 (vland --recheck-landed): 74 RELOCATION_MATCH + 1 MATCH (0x005BA9BE),
tests PASS. Bindings recomputed (`rebind.py`), multi-name baseline unchanged (1). Agent notes, STATUS and NAME_FIXES:
`pending/2026-11-10_linux_handNF/`.

## Supersedes codex20nf fix [1]
0x004291DE is `?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ`, not the destructor; `??1` is the 5-byte `jmp` COMDAT
0x00429C53. Two sources: Ego_r.pdb `??1<CComponent>` 0x00438DDC is 5 bytes / 1 instruction (retail 0x00429C53 = `e9 86f5ffff`), and
FableWin.pdb's debug `??1<CComponent>` (17 bytes) wraps the 249-byte ReleaseData. Call sites agree: 0x00429C53 at local scope ends
(0x0054522E, 0x0056BBB9), 0x004291DE on live pointers (ClearBastardChild, Reset 0x00429C72, ShareData 0x00429217). New opt-ins
FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA / _RELEASEDATA_DECL_ONLY in engine/CCountedPointer.h. The fix-[1] opt-ins
(_DTOR_DECL_ONLY, _RESET_CALLS_DTOR) remain for their two remaining users.
Same body + jmp pairs (queued `land1011q`, not swept): CAction 0x00429160 / 0x00429B72 (about 332 call sites bind ??1<CAction> at
0x00429160), CMeshDataBank 0x0042918C / 0x00429BF9, CList 0x00429288 / 0x00429D8E, CText 0x0042925C.

## Renames (rerow.py)
0041d207 NUISystem::CManager::ClearActionParams; 00429c15 / 004291de / 0042920a / 00429c53 CCountedPointer<NUISystem::CComponent>
ctor / ReleaseData / ShareData / dtor; 00429e16 std::pair<const CCharString, CCountedPointer<NUISystem::CComponent>>::~pair;
005bb04f / 005babb8 / 005ba78b _Rb_tree<EClothingSuitPart,...> ~_Rb_tree / clear / _M_erase; 005bb0a7 / 005bb3ef / 005bac52
list<CDefPointer<const CAppearanceModifierDef> > insert / push_back / _M_create_node. Headers: FABLE_RETAIL_STLP_LIST_REMOVE_DECL
(stlport/fable_containers.h), FABLE_RETAIL_CTCINVENTORYCLOTHING_TOTALARMOUR (engine/CTCInventoryClothing.h); default paths unchanged.

## Parked
The three Codex pass-15 rows stay parked (`pending/2026-11-10_linux_handNF/differ/`): 0x00E5E350 DIFFER(584v585, one missing store at
0x00E5E355), 0x0064FDB4 DIFFER(2593v2593) and 0x00651273 DIFFER(1677v1681) (frame and local-home differences; need Codex's header work).
Debt: 005518e0 / 00551bc0 / 005fc2d0 / 005fc8a0 live in invented namespaces; 00443d70 CQuestInfoElement nesting; 0x0042ABCA bound
under 9 spellings; 0x0059A994 ReleaseData<CList> clashes with 0x00429288; SetStaticCast<CList> on 0x0042A2DB and 0x0042A2CA.
