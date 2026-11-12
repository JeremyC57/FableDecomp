# Independent evidence verdicts (Opus verifiers, one per row; workflow verify-handback-evidence)

## 00407cd0 -- ok=False, name proven=True, test can fail=True, issues=2

- ISSUE: hdr/engine/CUserProfileManager.h was copied from an older repo header and is now stale: it drops the existing `FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES` block (GetManualSaveFileName 0x00406610 / GetManualSaveDisplayName 0x00406AE0). The landed row rebuild/src/compiled/00/40/CUserProfileManager_GetManualSaveDisplayName_00406ae0.cpp defines that macro, so this block is still needed. Rebase the overlay on the current header and add only the PASSIVEINPUTS block.
- ISSUE: The callee CUserProfileManager::ApplyValues 0x00407370 is protected in both PDBs: Ego_r 0x00405900 and FableWin 0x017ED3E9 / 0x01882CF0 both spell it `?ApplyValues@CUserProfileManager@@IAEXXZ`. QAEXXZ appears in neither _fablewin_api.tsv nor egor_pdb_names.tsv. The PASSIVEINPUTS block declares it public, and its comment cites `QAEXXZ`. Declare it under `protected:`, fix the comment, and add a NAME_FIXES.tsv line for the landed row CUserProfileManager_ApplyValues_00407370.cpp, whose `// Leaf:` reads ?ApplyValues@CUserProfileManager@@QAEXXZ (correct: IAEXXZ).

1. Name and signature: ?RedefineControlPassive@CUserProfileManager@@QAEXABVCActionInputControl@@0J@Z appears in Ego_r (egor_pdb_names / egor_signatures 0x00405C70) and in FableWin (_fablewin_api 0x017F6A39 / 0x018893F0, plus the stragglers signatures file). It is public, non-virtual, returns void, takes two const CActionInputControl& and a long, and ends with `ret 0Ch`; the source matches. The manifest already has 0x00407CD0 accepted under this name. Its only caller is at 0x00557E40, inside NUISystem::CKeyRedefiner::Redefine 0x00557D20, which fits.
2. Callees: GetDirection 0x0040ED10 is called twice and spelled as the PDB names it (?GetDirection@CActionInputControl@@QBE?AW4EActionDirection@1@XZ). ApplyValues 0x00407370 has the right name and address, but its access is wrong (see issues).
3. Members checked against Ego_r: GameAction +0x00, ControllerType +0x04, KeyboardKey +0x08, XboxButton +0x0C, MouseButton +0x10, ControlDirection +0x14, sizeof 0x1C (the size()/28 multiply 0x92492493 and `rep movsd` of 7 dwords agree). AssignedInputsPassive, a vector<CActionInputControl>, sits at +0x54 (retail reads [ebx+54h] / [ebx+58h]). The switch maps type 1 to +0xC (XboxButton), 2 to +8 (KeyboardKey) and 3 to +0x10 (MouseButton), matching the source.
4. Local classes: none. CActionInputControl is a new shared header (no rebuild/include/engine/CActionInputControl.h exists; other headers only forward-declare it) and uses PDB member names.
5. Bodies: the GetDirection body in the overlay matches the landed 0x0040ED10 row in code (only enum casts on the returns). It is kept visible as `inline __declspec(noinline)`, and the cited retail fact is that old_control.GameAction stays in esi across both calls (reused at 0x00407D61 with no reload). IsSameControl is a descriptive helper and NOTES.md lists it as made up. Its use instead of the real IsSameActionAndButton 0x0040EDA0 (which has no caller) holds up against the bytes. Retail calls the entry's GetDirection first and then switches on the entry's ControllerType ([edi+4]). The real IsSameActionAndButton calls other.GetDirection first and IsSameButton switches on this->ControllerType, so neither receiver order of the real pair produces both facts.
6. Test: it runs a model copy that checks plain-action replacement with the index ignored, quick-access replacement only at count==index (counter starts at 0), context-sensitive replacement with the counter starting at -1, controller-type button comparison, and an ApplyValues call per invocation. It fails if the counters' start values are swapped or if the wrong button field is compared.
7. Faithfulness against the disassembly: the -1 / 0 counter starts ([esp+0xC]=-1 is the 56 counter, the 55 counter is 0), the 55/56/default switch on old_control.GameAction, the second switch on the entry's action after the copy, size() recomputed every pass, the unsigned `jb` loop and the early exit when empty all match retail.
8. LABELS.tsv rows for 0x00407CD0, 0x0040ED10 and 0x0040EDA0 hold up against the PDB. Neither NAME_FIXES entry concerns this row. Minor: the new CActionInputControl.h forward-declares the enums without the `#ifdef _MSC_VER` portable twin that CGamePlayerInterface.h uses. That only matters if a non-MSVC build includes it; the test does not include it.

## 00801920 -- ok=True, name proven=True, test can fail=True, issues=0


NAME: ?RegisterIdleStateGroup@CTCIdleScheduler@@QAEXPAVCAIStateGroupBase@@W4EIdleStateGroup@@@Z appears in egor_pdb_names.tsv / egor_signatures.tsv (Ego_r 0x00805c50) and in the FableWin signature tables (_fablewin_api.tsv, FableWin.exe.signatures.tsv). The retail 0x00801920 entry has signature __thiscall void (CTCIdleScheduler*, CAIStateGroupBase*, EIdleStateGroup) and is marked accepted. Retail ends in `ret 8`, so it is public non-virtual thiscall with two arguments. Ten callers in the CAIStateGroup range (0x914c01, 0x936621, ...) call 0x801920, and the landed callers already spell it with this name. The source's Leaf line matches the PDB symbol exactly.
CALLEES: all 9 call targets in the disassembly match the source comments, in order: 0x802110 IdleStates[] (landed STLmap_operator), 0x802190 IdleStatesWeight[] twice (landed), 0x686DA0 GetPWorldSeed twice, 0x497890 GFROR13 (GFRandom inlined at the first use: imul 0x24a1 / add 0x24df / div), 0x4CDAE0 GFRandom, 0x6869D0 GetDefinitionManager, 0x42AF3C GetDef, 0x801670 GetSubDef<CIdleSchedulerDef>, 0x8014F0 const find twice. All of these come from shared-header bodies or landed rows. Two smaller observations, neither blocking: the landed row for 0x8014F0 is named CRetailTree_longkey::find, and the comment that GetDef's template argument is CThingBaseDef has no PDB proof (an earlier attempt flagged 0x42AF3C as a folded CThingBaseDef/CDefClassBase instantiation), but the bytes are identical either way.
GLOBAL: 0x013BAF1C has exactly one xref (0x00801943), no PDB or landed name, and is declared as made-up in NOTES.md and LABELS.tsv. Older pending drafts called it g_defaultIdleWeightRange, but nothing landed uses that name, so the "one name per address" rule holds.
MEMBERS: checked five against the layouts and headers.
- ParentThing at +4 (CTCBase).
- IdleStates at +0x0C and IdleStatesWeight at +0x18; PDB +0x0C/+0x1C with 16-byte debug maps, retail uses 12-byte STLport maps.
- CThing::DefGlobalIndex is a ushort read with `word ptr [eax+8Ch]`, through the CThing.h GetDefGlobalIndex() accessor.
- CIdleSchedulerDef::Period is map<EIdleStateGroup,long> at retail +0x3C (PDB +64).
- After the find, `[ecx+14h]` is the node's second.
LOCAL CLASS: CIdleSchedulerDef has no rebuild/include/engine/CIdleSchedulerDef.h. It copies the landed 0x00801670 model (StatGetTypeName, Transfer, Copy, GetSizeofClass) and adds the Ego_r member Period. The overlay CTCIdleScheduler.h is new: the class moved unchanged out of campfire_types.h, with PDB member names.
REAL BODIES: the only bodies other than the row's own are the inline StatGetTypeName (same as the landed model) and shared-header opt-ins: GetDef global body, GetSubDef body, STLP_TREE_CONSTFIND. No stand-in bodies.
TEST: a model copy of the real LCG (0x24a1 / 0x24df / ror13, range 0 gives 0) checks four things, any of which fails the test if the logic is wrong:
- group storage
- the default draw when there is no def index
- the def-period redraw that replaces the first draw, including the advance of the seed state
- the default draw kept for a type missing from the def, and for a zero range
FAITHFULNESS: declaring the def pointers before the thing pointer (so the def index is read again), and doing the period find twice, are both explained by comments that cite the retail instructions. The disassembly confirms both finds, both GetPWorldSeed calls, and the release order (ebp def first, then the idle def).
NAME_FIXES row 3 concerns 0x00801A50 (ChooseNewIdle), not this row; its Ego_r Period / DefGlobalIndex evidence holds. The LABELS entries for 0x00801920 and 0x013BAF1C are correct.

## 00877720 -- ok=True, name proven=True, test can fail=True, issues=0


1. Name: the `// Leaf:` mangled name `?GetStandardThings@CTools@NCameraModes@@QBEXAAV?$list@PBVCThing@@...@std@@ABVC3DVector@@M@Z` appears in ghidra_out/egor_pdb_names.tsv and egor_signatures.tsv (Ego_r 0x0085E160) and in naming_stragglers/FableWin.exe.signatures.tsv (0x0179DC86 / 0x029C1D60). The retail address 0x00877720 is tied to GetStandardThings by naming_stragglers/Fable.exe.signatures.tsv, engine_api.tsv and rebuild/manifest/functions.tsv (thiscall, void, CTools*, list<const CThing*>*, C3DVector*, float). The PDB spelling decodes to public, non-virtual, const, (list<const CThing*>&, const C3DVector&, float), returning void. The overlay declaration matches that, and `ret 0Ch` agrees. The xref tool finds one caller, 0x008FAFC1.
2. Callees: 0xBFEA14 (free) is the inline STLport list clear loop at 0x00877737-0x00877754. 0xBFEA70 (__ftol2) is called three times, for ToC2DCoordI and (long)radius. 0x00878430 is called twice (0x008777DA, 0x00877826) and is the GetAllThingsInRoughArea<CThing, list<const CThing*>, CFilter_Bind2<IsOfType, OverlappingSphere>> instance. Its PDB mangled name is in egor_pdb_names (Ego_r 0x0085EFD0) and in FableWin signatures. Its only two callers are in this row. The manifest's GetPBestThingInRoughArea<HasTrackName,...> label is a twin label copied from a byte-identical body and is wrong here. The NAME_FIXES.tsv / LABELS.tsv entry for 0x00878430 has the address, the correct symbol and evidence (FableWin instance, two call sites, three arguments: box, filter, list), and that evidence holds. GFFilter_Bind2 comes from the shared header CFilter_Bind2.h and is not made up.
3. Members: [this+0] = CTools::CameraManager (+0, Egor). [mgr+0x74] = CGameCameraManager::World (+116, Egor, CWorld*). [world+0x20] = CWorld::PThingSearchTools (+32, Egor scoped_ptr; the header asserts offsetof == 0x20). [origin+0/4/8] = C3DVector X/Y/Z. The filter temporaries are CThingFilter_IsOfType::Type (+0, ulong; immediates 3 and 5 at 0x008777C6 / 0x0087780F) and CThingFilter_OverlappingSphere SpherePos (+0) / Radius (+12). C2DBoxI is TLX/TLY/BRX/BRY: x-r, y-r, x+r, y+r, which matches the sub/add sequence. In the header, CameraManager is a const reference under an opt-in, while Egor has it as a pointer. That opt-in is already in the shared header with a PDB comment and has the same layout.
4. Local classes: none. The source only includes shared headers.
5. Real bodies: the row has only its own body. The other bodies come from shared headers: GetWorld / GetCameraManager / PeekThingSearchTools inlines, and the GetAllThingsInRoughArea declaration under ORIGINAL_ROUGHAREA. The source has the same shape as the landed sibling 0x00878390.
6. Test: a model copy clears a list that holds a stale entry, then searches type 3 and then type 5 over the truncated square around the origin with a sphere-overlap filter. It checks the resulting order (type-3 hits before type-5 hits), the size, the two box extents and that there were two searches. It fails if the clear, the type order, the filters or the box arithmetic were wrong.
7. Faithfulness: no compiler-steering constructs. The type word is stored in the dead `things` argument slot, as the comment says and as retail does (mov [esp+44h],3 / [esp+50h],5). Retail re-reads origin after the first search, which matches building a fresh OverlappingSphere for each call.

## 0x00878BE0 -- ok=True, name proven=True, test can fail=True, issues=0


NAME: Ego_r.pdb (egor_pdb_names/egor_signatures 0x00862D90) and FableWin.pdb (_fablewin_api + FableWin.exe.signatures 0x017C6960/0x029C91F0) both spell ?GetPosFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z (private, const, non-virtual, C3DVector by value, float,float). This matches the source's Leaf line and declaration, and retail `ret 0Ch` (hidden result plus 2 floats). The PDB order (GetCamPosOffsetXY 0x862CE0, then GetPosFromParams 0x862D90) matches retail 0x878A80 -> 0x878BE0. xref shows all callers are CProjectileWeapon methods (GetLookDirectionFromParams 0x878DB0, GetHeightParamForPostionWithPhi 0x879070, Update 0x879900). functions.tsv and the landing/label files agree on the name.
CALLEES: 0x00878A80 is spelled GetCamPosOffsetXY (PDB ?GetCamPosOffsetXY@...@@ABE?AVC3DVector@@M@Z, landed) and 0x0084F1B0 is spelled NCameraModes::CBase::GetObservedThing (landed, declared in the shared header). Both addresses in the comment are correct. GetPhysicsTC, GetPosition, C3DVector operators and GFGetInterpolatedValue are header inlines.
MEMBERS: retail offsets sit 0x24 below Ego_r, and sizeof(CBase) is asserted 0xa4 in the header. ShotFiredTimer +0xa4 (PDB 0xc8), Finished +0xa8 (0xcc), XAxis +0xa9 (0xcd), YAxis +0xb5 (0xd9) and ZAxis +0xc1 (0xe5) all line up; retail does `fmul [esi+0xc1/c5/c9]`. CThing::PhysicsTC +0x60 (asserted; retail `mov eax,[eax+0x60]`) and CTCPhysicsBase::Position +0xc (PDB 12; retail `fld [eax+0xc]`) are right.
LOCAL CLASS: no NCameraModes_CProjectileWeapon.h or CProjectileWeapon.h exists, and the members carry their PDB names. pack(1) is justified by the odd +0xcd PDB offset.
REAL BODIES: the code of GetCamPosOffsetXY is identical to the landed rebuild/src/compiled/00/87/NCameraModesCProjectileWeapon_GetCamPosOffsetXY_00878a80.cpp. The same TU is plausible (adjacent addresses), and retail keeps the call, which matches auto_inline(off).
CONSTANTS: 0x12777F8 = 0.5, 0x122DEDC = 0.0, 0x122DED8 = 1.0, confirming h*2+0.5, (h-0.5)*0.5 and GFGetInterpolatedValue(1,1,t) = t*0+1 as kept by retail.
FAITHFULNESS: `pos / 1.0f` is a compiler-steering construct, but the comment cites the retail bytes it reproduces. I confirmed that retail stores pos.X at 0x878C90, then reloads pos.Z and pos.X at 0x878C94/98, so the claim holds. The NOTES say the divisor's source name is unknown.
TEST: a model copy checks three phi/height cases (cos term, sin term, -cos term, plus the height rescale into ZAxis), compares against expected positions, and returns FAIL otherwise. The interp scale is always 1, which is retail's own degenerate case.
Cosmetic: a stray line in the header comment ("// for an orbit angle and height parameter.") is out of place. No NAME_FIXES apply; the LABELS entry is consistent.

## 0x009E3B60 -- ok=False, name proven=True, test can fail=True, issues=1

- ISSUE: The local class CPFDColour names its only data member `Long`, but no source gives that name: struct_layouts_egor.tsv and fable_types.h show only an unnamed union `CPFDColour_u_0` at +0x0, and no other file in the repo names its members. Yet NOTES.md says CPFDColour is 'local, PDB members'. Either prove the union member's real name from FableWin.pdb/Ego_d.pdb, or list `Long` in NOTES.md as a made-up descriptive name (the GetLong/SetLong accessors suggest it) and correct the 'PDB members' claim.

NAME/SIGNATURE: two independent sources prove the name. ghidra_out/bsim_port_audit_final.tsv maps retail 9e3b60 to the Ego_r ?ConvertPFDToRGB@CPixelFormat@@QBEXABVCPFDColour@@AAVCRGBColour@@@Z (Ego_r 0x00B359E0, similarity 0.971). rebuild/manifest/functions.tsv and ghidra_out/engine_api.tsv list 009e3b60 as CPixelFormat::ConvertPFDToRGB, __thiscall, void, (CPixelFormat*, CPFDColour*, CRGBColour*). The neighbourhood fits too: ConvertPFDDataToRGB is at 0x009E3DF0 and ConvertRGBToPFDData at 0x009E3F60. FableWin.pdb (_fablewin_api.tsv 0x0308D9E0) has the same mangled name. The mangling QBEX ABV ... AAV ... decodes to public, const, void, (const CPFDColour&, CRGBColour&), non-virtual, which matches the source and the overlay declaration. The `ret 8` agrees. It is not a virtual, so it has no vtable slot. xref.py reports 0 callers.
CALLEES: the retail disassembly has no call instructions. Every helper is inline: GetNoXBits, GetXPosShift, GetXScaleShift, GetXMask, CRGBColour::Initialise and CPFDColour::GetLong. All of them are FableWin.pdb names (?GetRMask/?GetGMask/?GetBMask/?GetAMask, ?GetR/G/A/BPosShift, ?GetLong@CPFDColour@@QBEJXZ, ?Initialise@CRGBColour@@QAEXEEEE@Z).
MEMBERS: CD3DFormatInfo AlphaBits +0x0C, RBits +0x10, GBits +0x14 and BBits +0x18 (stride 36) match struct_layouts_egor.tsv and retail's disp32 0x129BA4C/50/54/58 from GD3DFormats 0x0129BA40. CPixelFormat::FormatIndex at +0 is the `mov eax,[ecx]` at 0x009E3B63. CRGBColour B/G/R/A at +0/+1/+2/+3 match the stores [esi+2]=R, [esi+1]=G, [esi]=B, [esi+3]=A.
LOCAL CLASSES: rebuild/include/engine/CPFDColour.h does not exist. GetLong's const and long return match the PDB. The member name is the issue above.
INLINE BODIES: GetRMask/GetGMask/GetBMask have the same FableWin debug size and hash as GetAMask (47 bytes, 20 instructions, 6d10ea6bcebbc18b), so the 'GetAMask shape' claim holds. The position-shift debug sizes are G 36/14, R 42/16 and A 48/18, a constant +6 bytes / +2 instructions per channel. That fits one extra field read per step through a local reference, as the header comment says. GetBPosShift is 13 bytes / 8 instructions, which fits returning 0.
RETAIL BEHAVIOUR: alpha is computed first and parked in a frame word ([esp+0x1c], stored to +3 at the end). Red, green and blue follow. Blue is done in byte arithmetic with no right shift because its position is 0. This matches the source's right-to-left evaluation of the Initialise arguments. The packed value is read once and kept in the argument slot. The overlay is gated behind the new macros FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF and FABLE_RETAIL_CPIXELFORMAT_PFDTORGB, and the comment citing 0x009E3B78 explains the local-reference spelling. I found no unexplained steering constructs.
TEST: it models the function on a copied format table and checks the masked, shifted and scaled channels for three formats: 8888, 565 with alpha bits 0, and 4444. I checked the expected values by hand: for example 0xFFF1 in 565 gives R=F8, G=FC, B=88. The test fails if a mask, position or scale is wrong.
LABELS.tsv: the 009e3b60 row carries the correct mangled name, with the BSim and manifest evidence above. NAME_FIXES.tsv has no entries for this row.

## 0x00AD9770 -- ok=True, name proven=True, test can fail=True, issues=0


NAME: ?RemoveAllSubsets@CVertexGroupBlockList@@QAEXXZ is in Ego_r.pdb (egor_pdb_names/egor_signatures 0x00C5D160) and FableWin.pdb (_fablewin_api / FableWin.exe.signatures 0x017CB4BF). naming_stragglers/Fable.exe.signatures.tsv and engine_api.tsv bind retail 0x00AD9770 to it. The only caller is 0x00AD9890 (call at 0x00AD9963). That function stores 0x2710, which fits the CVertexGroupBlockOptimiser(const CVertexGroupBlockList&, K) constructor, and in Ego_r that constructor sits next to RemoveAllSubsets. The signature (public, non-virtual, void, no parameters, not const) matches QAEXXZ. GetBlockCount matches ?GetBlockCount@CVertexGroupBlockList@@QBEKXZ (const, unsigned long).
CALLEES: every call target in the disassembly is spelled with its landed or PDB name and the right address. They are 0x00AD8CA0 IsProperSubsetOf, 0x00535470 vector<unsigned long> copy ctor, 0x00A96E40 map copy ctor (ecx=[ebp+0xC]), 0x00ADCE60 _M_insert_overflow, 0x00ADCA30 vector<CVertexGroupBlock>::operator=, 0x00ADC190 __destroy_aux (fastcall edx/ecx) and 0x00BFEA14 free. The source uses no made-up names.
MEMBERS: Blocks is at +0 (ebx). The element stride is 0x18 (0x2AAAAAAB / sar 2 gives /24, and add ebp,0x18). Groups is at +0 and GroupToIndexMap at +0xC (lea ecx,[ebp+0xC] before the map copy). These agree with the Ego_r member names and types in struct_layouts_egor.tsv; Ego_r's offsets differ because of its debug containers. The overlay header CVertexGroupBlock.h gives the PDB types at the same layout, and GetGroupCount, GetGroup and HasGroup are FableWin.pdb inline accessors.
LOCAL CLASS: CVertexGroupBlockList has no rebuild/include/engine/CVertexGroupBlockList.h, and the overlay adds none. Its single member, Blocks, is the PDB name with the PDB type CArray<CVertexGroupBlock>.
REAL BODIES: the IsProperSubsetOf body has the same logic as the landed 0x00AD8CA0 row. It is spelled with the PDB accessors (GetGroupCount / GetGroup / HasGroup) where the landed row's local models use Groups.size() / Groups[i]; the meaning is identical. Its visibility is argued from the same retail source file (address 0x00AD8CA0 comes before 0x00AD9770 in the CVertexGroup cluster) and from the inner loop: ebp walks the array without re-reading [ebx] across the call. The push_back that expands inline matches the landed CArray<CVertexGroupBlock>::push_back 0x00AD9720 (STLport: placement copy of the vector and map, else _M_insert_overflow).
TEST: it checks that proper subsets are dropped, that order is kept, that equal duplicates survive (proper subset, not plain subset) and that an empty list stays empty. It would fail if the logic used subset instead of proper subset, or got the order or the break wrong.
FAITHFULNESS: the pragma auto_inline(off) around IsProperSubsetOf, the local inner-bound count and the per-TU defines are all explained by comments that cite retail addresses (0x00AD9855 / 0x00AD986A). The disassembly confirms the count is recomputed only at the outer test and the subset flag folds into the two exits (jne 0xAD981D).
LABELS: both entries hold (0x00AD9770 RemoveAllSubsets; 0x00AD8CA0 IsProperSubsetOf, already landed). NAME_FIXES.tsv has nothing for this row.

## 0x00BC2480 -- ok=True, name proven=True, test can fail=True, issues=0


I found nothing that must be fixed before landing. I compiled nothing and changed no files.

1. Name and signature hold. `?Update@CEngineInternalPrimitiveParticleMesh@@UAEXABVCEnginePrimitive@@K@Z` (public virtual, void, takes `const CEnginePrimitive&, unsigned long`) is listed in three places:
   - ghidra_out/egor_pdb_names.tsv at 00a77770
   - ghidra_out/egor_signatures.tsv
   - FableWin.exe.signatures.tsv / _fablewin_api.tsv at 01854ad0 and 02eb6510

   rtti-vtables.tsv puts 00bc2480 in slot 5 of vtable 012a6184. That is the Update slot of every other CEngineInternalPrimitive* class, and it matches the base `virtual void Update` order. The Ego_r spacing agrees with retail: MeshGroup::Update is 0x80 before Mesh::Update in both (a776f0/a77770 vs bc2400/bc2480), and Ego_r GetWorldPosition 00a778e0 lines up with retail slot 15 at 00bc25b0. The only xref is that vtable entry in .rdata. The function ends with `ret 8`, which fits a thiscall with two arguments.

2. Callees: retail makes no call instructions. GetForward, GetUp, GetRight and `C3DVector::operator*=` are PDB-named inlines from the shared headers. GetRight has an out-of-line copy at 0x0049CE00, but here it is expanded inline (0x00BC24CF..0x00BC24FD). No made-up names.

3. Members match struct_layouts_egor.tsv and the retail offsets:
   - EnginePrimitiveFlags: Base +0x0C (`mov [ecx+0Ch]`)
   - Colour: +0x78, read from descriptor +0x3C
   - ObjectTransform: +0x48. E11..E43 sit at 0x48..0x74: rows 1–2 negated via `fchs`, row 3 is Up, row 4 is WorldPos read from descriptor +0x0C/+0x10/+0x14.
   - Descriptor: Size +0x18, RightHandedSet +0x24 (Up +0x24, Forward +0x30). The 0.01f constant is at 0x01230A5C.

4. No local classes. CEnginePrimitiveParticleMesh.h does not exist in rebuild/include/engine, so adding it as a new header is right. Its members use the Ego_r PDB names and sizeof is 0x40.

5. Other bodies:
   - The GetRight GROUPED variant matches the retail operand order. X = Fwd.Y*Up.Z - Fwd.Z*Up.Y; Y and Z follow the same pattern, which is Forward x Up, the same arithmetic as the landed 0x0049CE00.
   - GetForward, GetUp and operator*= are the existing shared-header inlines.

6. The test models Update. It checks that flags and colour are copied, and that the 12 matrix values equal -(F x U)*Size.X/100, -F*Size.Y/100, U*Size.Z/100 and the position, across two orientations. Any error in the cross product, the sign, the scaling or the row order would make it fail.

7. Two spellings that steer the compiler are each explained by retail bytes:
   - Three separate scale floats: z is homed in the flags argument slot at [esp+2Ch], and the frame is 0x24 bytes.
   - Element-by-element negated stores: `fld; fchs; fstp` at 0x00BC254E..0x00BC2582.

   I confirmed both against the disassembly.

8. LABELS.tsv has the correct symbol for 00bc2480. NAME_FIXES.tsv has nothing for this row.

Two small points in the header overlay, neither needed for this row:
- The new class keeps the generated header's flattened `RenderInfo_Lights` / `RenderInfo_Mesh` / `RenderInfo_LOD` at +0x7C. The PDB member is `CEngineRenderInfoMeshBase RenderInfo`, and engine/CEngineRenderInfoMeshBase.h exists.
- The declared constructor `CEngineInternalPrimitiveParticleMesh()` has no PDB name. It is only declared, never defined.

