# NF234 notes (2026-11-22, round 10): the nf233 follow-ups (q12641-q12652)

Base: the repo tree with nf233 landed. Locks at hand-back: NF234 on the 6 changed rows only (0x00412450 0x00415930 0x004330D2 0x013B8760 released, unchanged). Locks NF234 during the pass: the pre-claimed 0x00495780 0x00412450 0x00415930 0x00418B7A 0x004330D2 0x00495620 0x00A0A110
0x0121A2DC 0x01228AF1 0x013B8760; claimed from `released` 0x00496CF0 0x0042EA8F (changed) and 0x00418DCA 0x00496070 (census only, released
again); claimed new 0x0040CEC0 0x00B260B0 (census only, released). 0x00495620 / 0x00A0A110 unchanged: released. rows.txt = the 6 changed rows.
No AUTH23 lock, 0x00B4D370, 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`, Codex pass 27 row or 0x0042BEC0 touched. Tools: NF233's (re-pathed)
+ tools/rtti.py (retail RTTI locator decoder). Flags lines: none. EH funclets / includers: no landed source or test includes any of the six changed row files (work/includers.txt). Descriptive names added: none.

## Entry 1 (q12650): 0x00495780 CFrontendGameComponent::Shutdown -- done (one form beyond the approved pair: decision 1)
- f3 = NF233's f2 + the approved CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY + DELETEDATA_DECL_ONLY: leaf RELOCATION_MATCH; ReleaseData<CGraphicDataBank>
  EQ 0x00419108 and <const CFontBank> EQ 0x0042F948; no DeleteData copy; but Reset<CGraphicDataBank> / <const CFontBank> pick-any copies NEQ their
  owners 0x004194D1 / 0x0042FC13 (ours 103 B /O2 with ReleaseData expanded; owners 59 B size bodies calling ReleaseData). (pk.py printed
  "103 v 103": it reads retail to max(catalog, mine); the owner is 59 B.) REJECTED.tsv.
- f4 = f3 + two add-only RESET size bodies (hdr/README.md): leaf RELOCATION_MATCH, all four CCountedPointer copies EQ with relocations
  (work/pk_f4.tsv); NO-OWNER: ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z, vector<CWideString> begin / end / clear.
- Self leaf QAE -> AAE (Ego_r / FableWin / Ego_d): its only binder 0x00496CF0 (three calls) respelled in place (`private:` on the local
  model's Shutdown); defined-symbol diff of 0x00496CF0: one UND. Table: the QAE line goes, an AAE line comes (TABLE_CHANGES).
- 0x00412450: the new object binds the STLport ?erase@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@... (sc.sh diff, x2); the
  allocator-less line (witness 0x00495780) goes. Census (work/census_00412450.tsv, hb.sh over the other 15 binder rows callers.py lists):
  all 15 bind the STLport spelling; 2 -> 1.
- Link (work/lnk1: 0x00495780 0x00496CF0 0x00419108 0x0042F948 0x004194D1 0x0042FC13 0x00412450 0x00419036 0x0042F864 0x00494840): 0 duplicates.

## Entry 2 (q12642-q12645): CGameComponent / DeviceResetCallback spellings -- q12642 done (in place), q12643-q12645 blocked: family units
Retail RTTI (tools/rtti.py): CMainGameComponent 0x0122F180 (off 0) / 0x0122F174 (off 4); CFrontendGameComponent 0x01238768 / 0x0123875C;
CMeshBankManager 0x0129C98C / 0x0129C980; each class: bases CGameComponent (or none) / CBaseClassNonCopyable / CBaseClass (mdisp 0) and
CDeviceResetCallback (mdisp 4). All three PDBs carry ??_7<C>@@6BCBaseClassNonCopyable@@@ and ??_7<C>@@6BCDeviceResetCallback@@@ for
C = CMainGameComponent, CFrontendGameComponent, CMeshBankManager (grep of the PDB publics, 1/1/1 each).
Emitters (absolute refs in retail code -> landed row; spelling the HEAD object binds):
- 0x00415930 SetQuit (vtable 0x0122ECB0 slot 4 only): MAE in 0x00415930 / 0x00415940 / 0x0042F50B / 0x0051D8E0; UAE in 0x00418B7A.
- 0x0122ECB0: PDB name in every emitter (0x00415940 0x00418B7A 0x0042F50B 0x00495620 0x0051D8E0).
- 0x0122ECA4: 6BCDeviceResetCallback in 0x00415940 0x00418B7A 0x0042F50B 0x0051D8E0; 6BCGameComponentInterface in 0x00495620.
- 0x0122DE50: ??_7CDeviceResetCallback@@6B@ in 0x0040CEC0 / 0x0051D8E0; ??_7CMeshBankListener@@6B@ in 0x00A0A110;
  ??_7NativeDeviceResetCallback@@6B@ in 0x00B260B0 (CEngine ctor; NF233's census missed it: 3 spellings, not 2).
- 0x0122F180 / 0x0122F174: PDB pair in 0x00418B7A; 6BCBaseClass@@@ / 6BCComponentListEntry@@@ in 0x00418DCA.
- 0x01238768: 6BCBaseClassNonCopyable@@@ in 0x00495620, 6BCBase@@@ in 0x00496070. 0x0123875C: 6BCGameComponentInterface@@@ in both
  (one stand-in spelling: porting one row alone grows it to 2, so the two move together).
- 0x0129C98C: PDB name in 0x00A0A110 / 0x00A0A150. 0x0129C980: 6BCMeshBankListener@@@ in both (move together). Slots: 0x0129C980
  binds ?v00@CMeshBankListener@@UAEXXZ (0x00A0A110) for retail OnPreDeviceReset 0x00A09D90 (landed, leaf ?OnPreDeviceReset@CMeshBankManager@@UAEXXZ);
  0x00A0A150 binds ?v00@CMeshBankManager@@UAEXXZ.
- Side split found: 0x009C0190 is bound ?AddComponent@CComponentList@@QAEXPAUCComponentListEntry@@@Z by 0x00418DCA and
  ?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z by 0x0040CEC0 (moves with the 0x00418DCA port).
Retail slots (tools/rvt.py): 0x01238768 = ??_G 0x00497720, Init 0x00415920, Run 0x00496CF0, ChangeTextureColourDepth 0x00495610, SetQuit
0x00494370; 0x0123875C = OnPreDeviceReset 0x00494350, OnPostDeviceReset 0x00494360. 0x0122F180 = ??_G 0x004197B3, Init 0x004184BD, Run 0x004189C2,
ChangeTextureColourDepth 0x0041799B, SetQuit 0x004197A0, Update, GetInputs, Render, PostInit, IsEditorActive; 0x0122F174 = 0x00415E83 / 0x004175DA.
Done:
- q12642 0x00415930: 0x00418B7A (its HEAD local CGameComponent model, slot 4) respelled in place (`protected:` on SetQuit): binds MAE; 2 -> 1.
  Not the header port the brief asks for (decision 2).
Blocked (family units; a family hands back whole):
- CFrontendGameComponent (q12643 + q12645 second half): 0x00495620 (dtor) + 0x00496070 (ctor) together (0x0123875C has one stand-in spelling
  in both; porting one grows it), on CFrontendGameComponent.h (a fully typed CTOR_DTOR layout: Connections / Sessions / String / TempBuffer ...,
  CTexture SplashScreen, three vector<CWideString>, two CCountedPointers) + CGameComponent.h DTOR_INLINE. A full header vtable also binds slot
  2 Run: 0x00496CF0's local model declares a non-virtual `bool Run(CGameComponent*&)` (?Run@..@@QAE_NAAPAVCGameComponent@@@Z) where the header has
  `virtual bool Run(CGameComponent**)`, so 0x00496CF0 joins the family; slot 4 needs a new SetQuit override opt-in (PDBs:
  ?SetQuit@CFrontendGameComponent@@UAEXXZ; the header declares none, so the slot would bind the base 0x00415930); slots 0x00495610 /
  0x00494350 / 0x00494360 / 0x00497720 must be checked against their self leaves.
- CMainGameComponent (q12645 first half): 0x00418DCA alone (0x00418B7A already emits the PDB pair); needs a typed CTOR layout opt-in on
  CMainGameComponent.h (469 includers: a typed-layout claim) whose ten slots match what 0x00418B7A emits, and moves 0x009C0190 off the
  AddComponent stand-in (NAME_FIXES).
- CMeshBankManager (q12644): 0x00A0A110 (ctor) + 0x00A0A150 (dtor) together on CMeshBankManager.h (a bases opt-in: CBaseClassNonCopyable +
  CDeviceResetCallback, OnPreDeviceReset / OnPostDeviceReset overrides (FableWin UAEXXZ / UAE_NXZ), the STLport list member; 0x00A0A150 also
  models CCountedPointer and the list locally); slot rows 0x00A09D90 (leaf ?OnPreDeviceReset@CMeshBankManager@@UAEXXZ), 0x00A09D80, 0x00A0A8A0.
  0x0122DE50 then still has 2 spellings: 0x00B260B0 (CEngine ctor, local EngineNative / NativeDeviceResetCallback; a CEngine.h port).
Which family first is decision 4.

## Entry 3 (q12647 / q12651): the CStartupDefinitionManager::PClass initializer / atexit pair -- done
- One TU: `CCountedPointer<CStartupDefinitionManager> CStartupDefinitionManager::PClass;` on CStartupDefinitionManager.h (_PCLASS) +
  CCountedPointer.h (RELEASEDATA_DECL_ONLY). Its _$E1 = 0x0121A2DC (push _$E2 ; call _atexit ; pop ecx ; ret: the inline default ctor's zero
  stores fold away) and _$E2 = 0x01228AF1 (mov ecx,PClass ; jmp ReleaseData 0x00433CC0), both RELOCATION_MATCH on the first form.
- Names: all three PDBs carry `dynamic initializer for 'CStartupDefinitionManager::PClass'` / `dynamic atexit destructor for ..` and the
  mangled PClass (catalog names in RENAMES section 1; the coordinator renames files).
- Spellings: 0x013B8760 2 -> 1 (the stand-in g_AutoTinyThunkObject_013b8760 goes), 0x00433CC0 2 -> 1 (Invoke_00433cc0 goes); no table line names a stand-in.
- Link (work/lnk3, + 0x00433CC0 0x0043368D 0x004336B0 0x00433D3A 0x004085F0): 1 duplicate, ?PClass@ defined by both pair objects (the
  static-pair debt: VC7.1 emits _$E2 only where the static is defined; NF219's NullString pair landed the same); 0 with one pair object.
  Each pair object also carries the other's file-local _$E1 / _$E2 and a .CRT$XCU entry (the same TU twice): with both linked into one
  image the atexit registration would run twice, so the TU-emitter rule must pick one object at link time (as for NullString).
- vland WARN RELOC_CONFLICT `_$E2 -> 01228af1 here, 01228930 in landed 01219f00`: a file-local symbol; the landed 0x01223EC0 shows the same WARN.
- Pick-any ??0 / ??1 CCountedPointer<CStartupDefinitionManager> (16 / 5 B): NO-OWNER (no table line, no retail copy known).

## Entry 4 (q12646 / q12649): CEngineInterpolationInfoSet nesting -- done (add-only header claim)
- Evidence (PDB strings, three PDBs): Ego_d + FableWin ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ and
  ??4CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAEABV01@ABVCInterpolationInfoSet@CInterpolationInfo@@@Z; the type name
  CEngineInterpolationInfo::CEngineInterpolationInfoSet in all three (6 / 6 / 2 hits); top-level VCEngineInterpolationInfoSet@@ in none.
- Opt-in FABLE_RETAIL_CENGINEINTERPOLATIONINFO_NESTED_SET (hdr/README.md) and its one user 0x0042EA8F: the inline set ctor copy moves from
  ??0CEngineInterpolationInfoSet@@QAE@XZ to the PDB nested name (defined-symbol diff: exactly that; RELOCATION_MATCH + PASS). No retail owner.
- Left (local top-level models of the set, each its own port): 0x0069ED40 (??4 operator=), 0x0069EDE0, 0x0069E980, 0x0042FA30, 0x00B25950,
  0x00B812F0, 0x00B662F0, 0x00BCEB40, 0x01224900. The operator= the PDBs record takes CInterpolationInfo::CInterpolationInfoSet (also nested).

## Entry 5 (q12652): tests -- partial
New header tests (shared headers, a case per condition, stubs for what retail keeps out of line; test-only `#define private/protected public` scoped to one header): 0x00495780 (three cases), 0x0121A2DC and 0x01228AF1 (the pair: PClass zero before main; ~CCountedPointer = ReleaseData once). 0x00496CF0, 0x00418B7A, 0x0042EA8F keep HEAD tests (PASS); the nf233 list is still owed.

## 0x004330D2 (q12641 / q12648): the ??_G self leaf -- write-up, nothing landed
- VC7.1 emits ??_GCNewFrontendGameComponent@@UAEPAXI@Z only beside the class's vtable, and the vtable only in a TU whose code stores the vptr
  (a defined ctor or dtor, or an inline one that is emitted). The dtor TU 0x0042F50B already emits a ??_G equal to 0x004330D2 (28 B, EQ).
- Plain-C++ devices that would make the row emit it itself, and why each fails: (a) define the dtor in the row: a second strong ??1 (LNK2005
  with 0x0042F50B); (b) a visible `__declspec(noinline)` inline dtor opt-in (the RELEASEDATA_NOINLINE precedent): its COMDAT and the vtable
  are emitted only if something in the TU references the dtor, and any such reference is a new function symbol in the row's object;
  (c) an inline ctor: the real ctor is out of line (0x0042EA8F), so an inline one is a borrowed body.
- What would work: the pending TU-emitter rule (one retail TU, several addresses: 0x004330D2's bytes come from the 0x0042F50B object, as the
  static pairs need), or the folded-alias ruling NF233 asked for. Until then the stand-in leaf stays (queued).

## Decisions for the coordinator
1. 0x00495780: the approved pair alone leaves the two Reset copies NEQ; f4 adds CGRAPHICDATABANK_RESET_SIZE_BODY / CONSTCFONTBANK_RESET_SIZE_BODY
   (same scaffolding, owner rows set optimize("s", on)). It is the fourth form on the row counting NF233's two. Accept f4, or hold the row?
2. 0x00418B7A: SetQuit fixed in place (`protected:` in its HEAD local CGameComponent model; nf233 ruling 6 shape, one UND changes) instead
   of a header port. Accept?
3. CEngineInterpolationInfoSet: the nesting lands as an opt-in (default path untouched); flip it to the default later, after the nine local
   models above are ported?
4. Entry 2 families: which goes first (CFrontendGameComponent with 0x00496CF0 and its slot rows; CMainGameComponent 0x00418DCA, a typed
   layout claim on a 469-includer header; CMeshBankManager 0x00A0A110 + 0x00A0A150)? And 0x00B260B0 (CEngine) for the last 0x0122DE50 stand-in.
5. 0x0121A2DC / 0x01228AF1: the PClass LNK2005 and the doubled .CRT$XCU entry are pair debt until the TU-emitter rule; land as for NullString?

## Header proof
pin_asserts 0 pinned (three headers), assert_names_check 0 mismatches, equiv_check 0 of 57,023 landed rows differ (hdr/README.md).
recheck_final.log: the 6 changed rows, all RELOCATION_MATCH + PASS (new header tests on 0x00495780 / 0x0121A2DC / 0x01228AF1), no RELOC_CONFLICT
beyond the file-local _$E2 WARN (as landed 0x01223EC0).
Pick-any, all six rows HEAD v NEW (work/pk_fin.tsv): no new NEQ copy; the one NEQ (0x00418B7A ??_GCGameComponent 41 v 43, the size TU) is identical at HEAD.
Link: 0x00418B7A and 0x0042EA8F change no defined symbol another object defines (their diffs: one UND; one pick-any ctor copy renamed to the PDB nested name).

## Rule slips
- Two intermediate vland runs (the first recheck_final and its rerun) used `timeout 1800`; superseded by the final recheck_final.log run under `timeout 900`. Every other grade ran under 900 or less.
- STATUS / APPLIED were written per entry, not after every row.
