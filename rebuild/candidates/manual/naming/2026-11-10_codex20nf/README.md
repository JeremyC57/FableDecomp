# Codex pass 15 name fixes, coordinator pass (2026-11-10)

Codex's 22 NAME_FIXES (queue lines `land1011m`) were checked against two sources each before any landed spelling changed.
71 landed rows were touched; all regraded RELOCATION_MATCH and pass their tests under `vland.py --recheck-landed`
(68 + 3 rechecked with their own pass patterns). `reloc_bindings.tsv` lines of those 71 rows were recomputed from the
objects (`reloc_identity.check`); `bindings_multi_check` stays at its baseline of 1.

| Fix | Target | Done |
|---|---|---|
| [0] | 0x004196B2 GetDef<CSoundDef> (const, `CDefPointer<const CSoundDef>*` out) | binders 006b1600, 006be300, 006b1960 |
| [1]/[2] | 0x004291DE ~CCountedPointer<NUISystem::CComponent>, 0x00429C6F Reset | binders 0041e641, 0041ded2, 00429e16, 0059db82, 0056f69f, 005a45f6, 005566a0 (the header was included inside `namespace NUISystem`); row 00429c6f re-authored |
| [3]/[4] | 0x0042AB58 / 0x0042AB73 `_List_base<long, allocator<long> >` ctor / dtor | rows re-authored; binders 0042be50, 0041e3f6, 0059eb0c, 0064f659, 0041e641, 005797fa, 00bba140, 006866f0, 0059ec48, 00443d70, 00ac8540 |
| [11] | 0x005EDFD9 GetAbilityPDef (const, `CDefPointer<const CHeroAbilityDef>`) | 005f4b30, 006ac690 (+ IsAbilityAvailable const there and in 005747a3, 0057238f) |
| [14] | 0x005F900A ~_List_base<CWideString> | row re-authored |
| [15] | 0x0074E140 GeneratePopoutVectorFromVars private (ABE) | 0074e6e0 |
| [16]-[20] | IsValidStockItem, CASoundManager::GetBank, AllocStringData, AccurateNormalise, push_back rebases | earlier in this pass |
| [21] | 0x01436E80 `g_pEnginePrimitiveRenderer` (descriptive; type proven by the CEngine forwarders) | 25 rows (member reads keep each row's local view through a cast: debt) |
| [12]/[13] | 0x005F8944 / 0x005F8F6F | already right |
| [5]-[10] | set<EClothingSuitPart> / map<EDamageAttribute,float> / list<CDefPointer<const CAppearanceModifierDef>> | open: handed to the agentNF pass with the missed-binder sweep |

Renames found on the way (two sources each, `ghidra_out/labels_codex20nf_2026-11-10.tsv`): 0x005797A4 is
SExpSpendUndo::SExpSpendUndo (Ego_r ctor + layout + adjacency to the landed destructor; the CHeroExperienceDef label was a
propagated twin); 0x00AC8540 is QuadricOptimiserInternals::COptimisedPrimitive::PaintSegments (private; work list is list<long>).

New header opt-ins (default paths preprocess identically to HEAD): engine/CCountedPointer.h
`FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY`, `FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR`; stlport/fable_containers.h
`FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE`.

Name debt left: 0x0099BDB0 binds `?SetSubstring@CWideString@@` (retail GetSubString by value does not match yet; queue line
codex20nf); 005518e0 RetailUI::Pointer; 00443d70 / quest-bar rows spell CQuestInfoElement at global scope (Ego_r:
NPlayerGui::CDrawQuestInfo::CQuestInfoElement); 0x0042ABCA is bound as both list<CComponent*> and list<CActionParam*>
destructors; 0x0042A168 clear binders 0041d207 / 005a30a1; other rows still binding the old spellings (reloc_bindings only
records a symbol's first caller): 00485524, 005a731f and the agentNF sweep list.
