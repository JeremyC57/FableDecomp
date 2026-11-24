# NF233 notes (2026-11-22, round 10): the nf232 follow-ups (q12635-q12640)

Base: the repo tree with nf232 landed. Locks NF233 (pre-claimed entry rows; claimed 0x00418DCA 0x00496070 0x004330D2 0x0042DC5C 0x0042DC5F
0x0042DC60 0x00415920 0x00415930 0x00415940; 0x00496CF0 claimed then released). Rows not changed are released again (rows.txt = the 13 held).
No AUTH22 lock, 0x00B4D370, 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t1/`, Codex pass 27 row or 0x0042BEC0 touched. Tools: NF232's (tools/,
re-pathed) + fam.sh (census: relocs at family targets, vtable slots, symbols), spell.py (spellings per address from census files), rvt.py
(retail vtable slots), mkobjs.sh (objects for a link check). Flags lines: none. EH funclets: none. Descriptive names added: none.

## Entry 1 (q12638): the CNewFrontendGameComponent vtable / ctor / dtor family -- done (one header unit, 12 rows)
Vtable names, proven first:
- Ego_r.pdb, FableWin.pdb and Ego_d.pdb publics each carry ??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@ and
  ??_7CNewFrontendGameComponent@@6BCDeviceResetCallback@@@ (and the same pair for CGameComponent and CFrontendGameComponent);
  FableWin.pdb ??_7CDeviceResetCallback@@6B@.
- Retail RTTI (work/rtti.txt): 0x01230CA0 COL 0x0132D8B8 offset 0 and 0x01230C94 COL 0x0132D864 offset 4, type CNewFrontendGameComponent,
  bases CNewFrontendGameComponent / CGameComponent / CBaseClassNonCopyable / CBaseClass (mdisp 0) and CDeviceResetCallback (mdisp 4);
  0x0122ECB0 / 0x0122ECA4 the same for CGameComponent; 0x0122DE50 COL 0x0132CAD8 type CDeviceResetCallback. Slots (tools/rvt.py):
  0x01230CA0 = 0x004330D2, Init 0x0042F75E, Run 0x0042EC7C, ChangeTextureColourDepth 0x0042DC5C, SetQuit 0x00430340; 0x01230C94 =
  0x0042DC5F, 0x0042DC60; 0x0122ECB0 = 0x00415940, Init 0x00415920, _purecall x2, SetQuit 0x00415930; 0x0122ECA4 / 0x0122DE50 = _purecall x2.
- The header builds emit exactly these names and every slot on its PDB spelling (work/census_new.txt).
Gaps closed (hdr/README.md): CTOR_DTOR (typed ctor-built members, ctor / dtor / Init / OnPreDeviceReset / OnPostDeviceReset declarations;
CurrentMovement typed as NGameComponentHelpers::ECursorMovement), CGAMECOMPONENT_DTOR_INLINE, CSTOPWATCH_CTOR (the header had no ctor, so
the three timers would not be constructed), CENGINEINTERPOLATIONINFOSET_CTOR (m_LastInterp() value-init gives `stosd`, retail has three
`fldz ; fstp`: the inline ctor Ego_d / FableWin record as ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ).
Inline ~CGameComponent measured at stock flags first: plain inline expands in the size TU (0x0042F50B form 1 RELOCATION_MATCH, no /d2inl flag,
no __forceinline); the HEAD row comment's "only when __forceinline" claim does not hold on the header (REJECTED). Retail also expands it in
??_GCGameComponent 0x00415940, which the 0x0051D8E0 port now reproduces (pick-any copy EQ, HEAD NEQ).
Rows (STATUS.tsv): 0x0042F50B and 0x0042EA8F (full header ports, every local model gone), 0x0051D8E0 (header port), the vtable-slot self rows
0x0042DC5C / 0x0042DC5F / 0x0042DC60 / 0x00415920 / 0x00415930 (ports off stand-in structs; required by no-growth once the vtables are full),
and in-place binder respells in 0x00412F90 (two ctor binders), 0x00418DCA / 0x00496070 (their own PDB ctor signatures + the base ctor
binder) and 0x004330D2 (?Dtor -> ??1). Both emitters define SETQUIT.
Spellings per address (work/sp_head.tsv v work/sp_new.tsv: code relocations, vtable slots, self leaves of the 20 family rows + the 5 other
vtable referencers found by absrefs, work/absrefs.tsv): 0x0042EA8F 3 -> 1, 0x0051D8E0 2 -> 1, 0x0042F50B 2 -> 1, 0x00418DCA 2 -> 1,
0x00496070 3 -> 1, 0x0042DC5F 2 -> 1, 0x00415920 2 -> 1, 0x00415930 3 -> 2 (0x00418B7A UAE stand-in), 0x01230CA0 2 -> 1, 0x01230C94 1 -> 1
(stand-in -> PDB), 0x0122ECA4 3 -> 2 (0x00495620), 0x0122DE50 3 -> 2 (0x00A0A110), 0x004330D2 2 -> 2 (??_E slot v the stand-in self leaf),
0x00415940 2 (??_E / ??_G), the rest 1 -> 1. 0x0122D70C: the ctor leaves the local `_g_emptyWide` for an `L""` literal (??_C@_11LOCGONAA@?$AA?$AA@; the landed users checked, 0x0047FE83 /
0x004091C0 / 0x00446720 / 0x0040BCA0, write `L""` too; the table has no line for the literal). No growth.
Exe scan of the changed slot functions (work/absrefs3.tsv, + callers.py): each is referenced only from the vtables above, plus 0x00415920 from
CFrontendGameComponent's vtable 0x01238768 slot 1, which no landed row emits beyond slot 0 (0x00496070 / 0x00495620 truncated models); 0x00433CB5 only
from 0x00433D3A.
Defined-symbol diffs HEAD v new (work/sc_e1.log, 0x00433D3A by sc.sh): the leaves, the vtable names, shared-header inline COMDATs
(??0C2DBoxI@@QAE@XZ, ??0CDeviceResetCallback@@QAE@XZ, CCountedPointer / vector / CDefPointer members, ??0CCPPointerInfo now FableWin's `K`) and the
HEAD local-model members that went (CRetailVector / CAllocProxy / CVectorBase / CDateAndTimeVector / CGameListener); 0x00412F90 / 0x004330D2 /
0x00418DCA / 0x00496070 define nothing new beyond their leaf.
Pick-any (work/pk_e1.tsv, HEAD and NEW objects): no new NEQ copy. Fixed: 0x0042EA8F's five ??1CCountedPointer copies (HEAD NEQ 44 B, now EQ
5-B thunks) and its 59-B local ~CNewFrontendGameComponent copy (gone), 0x0051D8E0's ??_GCGameComponent (EQ). Debt kept (same at HEAD):
0x0042F50B ??_GCGameComponent (41 v 43: the size TU; owner 0x00415940 is a speed TU), 0x00418DCA / 0x00496070 ??1CCountedPointer copies.
~vector<CWideString> copy EQ owner 0x00412130 thanks to the end-of-TU `#pragma optimize("", on)` (form 2 of 0x0042F50B; also in 0x0042EA8F).
??_GCNewFrontendGameComponent (both objects, 28 B, size mode, generated with the vtable at the dtor) EQ owner 0x004330D2.
Link (work/lnk/link_check.txt, 22 objects: the 12 rows + 0x0042EC7C 0x00496CF0 0x0049DCC0 0x00418B7A 0x00415940 0x00495620 0x0042F75E
0x00430340 0x0042EBB6 0x00412130 at HEAD): 0 duplicates. Header proof: hdr/README.md (pin 2, names 0 mismatches, equiv 0 of 57,022 rows differ).
Table: TABLE_CHANGES.tsv (the 0x01230C94 stand-in line respelled, base-ctor / Play / L"" stand-in lines gone, the 0x0122ECA4 stand-in line
re-witnessed to 0x00495620).
Left as is (NAME_FIXES): 0x004330D2's self leaf (?ScalarDeletingDestructor stand-in; ??_G needs the vtable, Decision 2); 0x00418DCA /
0x00496070 / 0x00495620 / 0x00A0A110 / 0x00418B7A local vtable and SetQuit stand-ins (each needs its own header port); the nested
CEngineInterpolationInfo::CEngineInterpolationInfoSet scope (header item).
Recheck (recheck_final.log): 13 rows RELOCATION_MATCH / MATCH + PASS, no RELOC_CONFLICT. WARNs: SHARED_HEADER_AVAILABLE on the in-place rows
0x00412F90 / 0x00418DCA / 0x00496070 / 0x004330D2 (their HEAD local models; in place only), STANDIN_LEAF on 0x004330D2, RELOC_NAME label notes.

## Entry 2 (q12639): 0x00495780 CFrontendGameComponent::Shutdown -- blocked (pick-any copies), drafts in work/e2/
- Header opt-in drafted (work/e2/CFrontendGameComponent.h.shutdown_optin, not in hdr/): a SHUTDOWN layout branch with Ego_r.pdb's
  PTempGraphics +0x1DFC, MainFont +0x1E04, ProfileNames / SaveNames / WorldNames, the offsets, SaveToLoad, and the private
  ?Shutdown@CFrontendGameComponent@@AAEXXZ (Ego_r 0x0049E380 + FableWin 0x01A41790; the self leaf would move QAE -> AAE with 0x00496CF0).
- f1 (`MainFont.ReleaseData(); PTempGraphics.ReleaseData();` default Info-first body): RELOCATION_MATCH, erase binder STLport; but the
  object's pick-any ReleaseData<CGraphicDataBank> / <const CFontBank> copies NEQ their owners 0x00419108 / 0x0042F948 (Data-first, size).
- f2 (`Reset(0)` over the Data-first FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA + the landed CONSTCFONTBANK_RELEASEDATA_SIZE_BODY): also
  RELOCATION_MATCH (dead-store elimination gives retail's Info-then-Data order), and the semantics fit: one Data-first ReleaseData body for
  the inline site and the owners. <const CFontBank> ReleaseData now EQ, but ReleaseData<CGraphicDataBank> (owner 0x00419108 is a size body),
  DeleteData x2 (T incomplete: 8 v 11 B) and Reset x2 (owners 0x004194D1 / 0x0042FC13) NEQ.
- Next form (not tried; context): f2 + FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY + a new add-only CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY
  in CCountedPointer.h (mirroring the CONSTCFONTBANK one), then check the Reset copies. 0x00412450 stays at 2 spellings.

## Entry 3 (q12635-q12637): the self rows
- 0x00412450: nothing left but the 0x00495780 binder (entry 2).
- 0x00433D3A: done. Explicit instantiation of the header Reset<CStartupDefinitionManager> (the 0x004194D1 recipe), first form
  RELOCATION_MATCH; callees bind ?ReleaseData@?$CCountedPointer@VCStartupDefinitionManager@@@@QAEXXZ (0x00433CC0's self leaf) and FableWin
  ?DeleteData@..@@SIXPAX@Z (0x00433CB5). 2 -> 1 spelling. No pick-any copy with an owner. Link (work/lnk3) with 0x00433CC0 / 0x004336B0 /
  0x0042EBB6: 0 duplicates. Catalog CCountedPointer<CPolymorphicData_00433D3A>::Reset -> CCountedPointer<CStartupDefinitionManager>::Reset.
- 0x013B8760 PClass: open. Its last stand-in binder 0x01228AF1 (`mov ecx,0x013B8760 ; jmp 0x00433CC0`) is the dynamic atexit destructor of
  PClass, registered by 0x0121A2DC (`push 0x01228AF1 ; call atexit`): an initializer / atexit pair (EXTRA_RULES 2026-11-20). Both rows must
  move as one unit defining PClass on the header (PCLASS opt-in + RELEASEDATA_DECL_ONLY), with the pair's known LNK2005 recorded as debt
  unless the TU-emitter rule lands first. Not started (context).

## Entry 4 (q12640): tests -- open (context)
The 13 changed rows keep their HEAD tests (host models), all PASS. Header tests are still owed for them and for 0x00412450, 0x00412B70,
0x0041D98A (port the row first), 0x00446720, 0x00599D5C, 0x00542140, 0x0059A238.

## Decisions for the coordinator
1. 0x0042EA8F defines FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE nowhere while 0x0042F50B (same retail TU) does: with it the ctor object would
   emit CGameComponent's vtable and a size-mode ??_GCGameComponent (41 B) that differs from owner 0x00415940. Accept the per-row difference?
2. 0x004330D2's self leaf: ??_GCNewFrontendGameComponent@@UAEPAXI@Z is emitted only with the class's vtable (a TU defining its ctor or dtor),
   and the dtor TU's copy already equals the retail bytes. Options: land 0x004330D2 as a folded alias of the 0x0042F50B object (its ??_G is the
   retail body), or keep the stand-in leaf queued.
3. CEngineInterpolationInfoSet is nested in CEngineInterpolationInfo in both debug PDBs: queue the header rename (header claim)?
4. 0x00495780: approve a CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY opt-in in CCountedPointer.h (entry 2's next form), or keep it held?
5. 0x013B8760 / 0x01228AF1 / 0x0121A2DC: queue as an initializer / atexit pair unit.
6. In-place rule condition 2 on 0x00418DCA / 0x00496070: both objects define ??1CGameComponent@@UAE@XZ from their HEAD local models (unchanged
   by this pass, an empty virtual body), and the new DTOR_INLINE opt-in makes the shared header define that name inline too (0x0042F50B /
   0x0051D8E0 objects). Accept (same empty body, no retail owner), or hold the two rows? If held, 0x0051D8E0 ends at 2 spellings
   (PAUCMainGameComponentParent from those two + the PDB one; was 2: no growth) and 0x00418DCA / 0x00496070 keep their own splits; the
   rest of the family lands without them (0x00412F90 still moves: it binds the two ctors, not the base).
