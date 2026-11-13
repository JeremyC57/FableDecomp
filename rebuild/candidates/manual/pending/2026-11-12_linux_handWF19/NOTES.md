## 0x006B3FF0

## 0x006B3FF0 CEnvironment::Update -- RELOCATION_MATCH (grade 6 of the row)
- What made it match: (1) GetRenderCamera(1.0f).PeekWorldPos() through CMainGameComponent::Get()->GetWorld().GetGameCamera() -- the
  by-value CCamera temp is the missing 0x34 frame bytes; (2) the threshold is `extern const float DELTA_NOTIONAL_ZERO` (0x0129BA3C),
  not a 0.0001f literal: with the literal VC7.1 threads the `rain = 0` path into the kill branch and the frame slots shift.
- Headers/flags: no flags. Overlay opt-ins CENVIRONMENT_UPDATE, CENVIRONMENTSPELLEFFECTINTERPOLATOR_FRAMEUPDATE,
  CGAMEDEFINITIONMANAGER_ENVIRONMENTDEF (hdr/README.md); plus existing CTCDPARTICLEEMITTER_FULL, CINTELLIGENTPOINTER(+_ASSIGN,
  _COMPARE_NULL_INT), IS_THING_ALIVE, CTHING_GETTCDRIVER, STLP_VECTOR_ASSIGN, CENVIRONMENTDEF_PARENT.
- Descriptive name: CGameDefinitionManager::PEnvironmentDef (+0xd8; accessor PeekEnvironmentDef is the PDB name).
- NAME_FIX (from dossier, not re-proved here): 0x006B5D80 is ??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ (landed as CBufferPairTriple_006b5d80).
  0x006B2CA0 is ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ (manifest CGameCamera_UpdateViewMatrix wrong).
## Repair (verifier finding 1: two GetRainStrength spellings in one retail TU)
- Reconciled on ONE spelling: `return GameTickState.BlendedTheme[0].Scalers[15];` (PDB member chain, typed by the
  FABLE_RETAIL_CENVIRONMENT_UPDATE overlay; +0x54 + 0x134 + 15*4 = +0x1C4). Update row unchanged in code, still RELOCATION_MATCH
  (also strict headers); test PASS.
- Proof the other rows take the same spelling under this overlay ($W/lf/):
  - lf/lf_respell.cpp = landed CEnvironment_LightingFlash_006b3b80.cpp with WEATHERSCALERS -> CENVIRONMENT_UPDATE, RainStrength ->
    Scalers[15], LightningFrequency (+0x1CC) -> Scalers[17]: 0x006B3B80 RELOCATION_MATCH (also strict). Coordinator: land this
    respelling together with the overlay so both TU rows share one body text.
  - lf/CEnvironment_GetRainStrength_006b2bd0.cpp = real row for the placeholder `C006b2bd0::M` (filed DrawGetSeaLevel):
    ?GetRainStrength@CEnvironment@@QBEMXZ, 0x006B2BD0 MATCH (strict). NAME_FIXES.tsv line added.

## 0x00727D00

## 0x00727D00 CTCRandomAppearanceMorph::OnAppearanceDraw -- RELOCATION_MATCH (grades: 7)
- Made it match: port onto shared headers with the dossier's corrected callee identities and ctor argument list (v1 764v760, 64 hunks);
  then advancing the index explicitly at the end of both loop paths (`++i; continue;` and `++i` after SetPrimitiveGraphics) instead of
  in the for-increment (760v760, 8 hunks); then CIVCountedPointer release opt-ins RELEASE_MEMBER + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES.
- Headers: overlay hdr/engine/NGraphicAppearance_CTCGraphicAppearance.h opt-in FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH (GetAlpha
  inline, PeekLightingChannel inline when _LIGHTINGCHANNEL is off, GetTintColour/GetPMeshEffect/RefreshPrimitiveAnimationAndPos decls); the row also defines the
  existing FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE for GetFullScale.
  _LIGHTINGCHANNEL cannot be combined with _MOVEMENT (LightingChannel redefinition). Flags: none.
- Local models (no shared header): CGraphicAppearanceDrawInfo, CEnginePrimitiveAnimatingMesh (0x90), CEnginePrimitiveMorphedAnimatingMesh (0xB4),
  CTCAppearanceMorphBase (+CMeshMorphSet/CMeshMorphSetParts), CTCRandomAppearanceMorph.
- NAME_FIXES candidate (from dossier, consistent with body + Ego_r): 0x004BBFF0 landed as CMap_DrawGetSeaLevel / called GetScale in 0x0075A530
  -> NGraphicAppearance::CTCGraphicAppearance::GetFullScale (?GetFullScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ).

- Repair (verifier findings 1-6): overlay rebased on the current repo header (diff now additions only; _REMOVETINT row 0x007E6450 and
  _FULLSCALE row 0x00724630 kept); RANDOMMORPH no longer declares GetFullScale, the row defines _FULLSCALE instead; NAME_FIXES adds the
  0x00747330 CTCStealth::InitialiseSneakSpeed caller and cites FableWin.pdb 0x01D0A3C0 + the header's inline GetScale (plain Scale read).
  Regraded RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1. hdrcheck2.py is not present under $S/tools, so not rerun here.

## 0x007BF790

## 0x007BF790 NThingShot::CTCShotDefinitionBase::Init (target-position overload) -- RELOCATION_MATCH (7 grades)
- Matched by porting onto the shared headers (OnCollision 0x007BF510 define block + CTCSHOTDEFINITIONBASE_INIT/_INIT_POSITION, CTCSOLIDTOTHING, CDEFSTRINGTABLE, CTCGRAPHICAPPEARANCE_MOVEMENT) and the solid-to-thing block spelled `CTCSolidToThing* solid; if (!ParentThing->GetTC(&solid)) { solid = 0; { CCharString name("CTCSolidToThing"); ParentThing->AddTC(name, false, 0); } ParentThing->GetTC(&solid); }`; store order buildup/damage/accuracy.
- Key level-change: FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE / FABLE_RETAIL_AUGMENTATION_PARTICLES make StatGetInterfaceType out of line (and UPDATE materialises HasTC) -- avoided with overlay opt-ins (hdr/README.md): CTCWEAPONTRAIL_UPDATEWEAPONTRAILS, CTCOBJECTAUGMENTATIONS_COPYFROMTHING, CTCGRAPHICAPPEARANCE_SETFORCERESEND. No flags, speed (optimize("s") gives a frame pointer).
- Names corrected vs old draft: CopyAugmentationDataFromThing (not InitAbilitySlot), GetPos, CWorld::GetFrame via CTCBase::GetWorld(), ReturnToDefaultInteractionFlags, InitAttachedEffects; const CDefPointer types (as DOSSIER).
- Repair (verifier): hdr/engine/NGraphicAppearance_CTCGraphicAppearance.h rebased onto the current repo header -- it now differs only by the added SETFORCERESEND block (keeps REMOVETINT row 0x007E6450 and FULLSCALE row 0x00724630). Regrade RELOCATION_MATCH (also strict headers), test PASS.

## 0x006BFCD0


## 0x006BFCD0 CTCBuyableHouse::GetSignPostPos -- wf19 author pass 1 (DIFFER 766v764, 4 hunks; was 739v764 / 30 hunks)
Best source: differ/006bfcd0.cpp (ported onto shared headers; overlay opt-in FABLE_RETAIL_CTCBUYABLEHOUSE_GETSIGNPOSTPOS in hdr/engine/CTCBuyableHouse.h).
What fixed most of it:
- port: CThing GetTC(TCI_THING_OWNER,&owner) (GETTC_EXPANDED), CTCThingOwner::GetObjectList list walk, IsDead, HasTC(TCI_DOOR), GetDefGlobalIndex,
  GetPhysicsTC()->GetRHSet(), CRightHandedSet::Rotate (FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE), PeekWorldMap().GetGroundSizeZAt, GetDefinitionManager();
- list end cached in a local iterator (`end = things.end()`; retail keeps it in ebx);
- forward copy is a C3DVector (`C3DVector forward = set.GetForward();` -> the 12-byte slot +0x40 with Z dead, integer-copied), not C2DVector;
- two statements: `base = set.GetForward()*3.0f + door->GetPos();` then `candidate = base + C3DVector(-forward.Y, forward.X, 0.0f)*side;`
  (gives frame 0x6C, ahead +0x28, sum z +0x58, perp +0x4C, hoisted side*0.0f +0x54 -- all match).
Residue (only the final candidate add): retail = sum + perp (`fxch st(2) ; fadd st,st(2)`, `fstp st(1) ; fadd [perp.y]`, `fld [sum.z] ; fadd [perp.z]`),
ours = perp + sum (`fadd st,st(2)`, `fld [perp.y] ; fadd st(1)`, `fstp st(0)` x2, `fld [perp.z] ; fadd [sum.z]`). +2 bytes.
Tried without effect on that residue (all same 4 hunks): operand order both ways (base+perp, perp+base, one-expression perp+(sum)), += forms,
named offset / const-ref base / const-ref offset / const base / scoped base, componentwise ctor of base+offset, explicit scaled ctor, offset*=,
C3DVector ADD_XY_GROUP, SCALE_X_PAREN (both), header probes (operator+ fully parenthesised, non-const return, plain-inline vector ops),
GetPos()+ahead order, named ahead, forward*3, doorPos reference + one pure expression, float ground / map ref / parenthesised call,
GetTC one-arg arity, GetPItem direct, IsAlive (worse), loop spellings, door/signPost decl order (worse), const CThing*, physics local,
/Op (worse), CTCTARGETINGAI/UPDATE_LIST copy-ctor variants, trivial copy ctor probe (worse), CrossWithUp forms ((-F).CrossWithUp(): 764 bytes
but 16 hunks; base - forward.CrossWithUp()*side: fsub).
One-expression forms (v20/v3) DO give retail's sum-first add order, but evaluate perp (materialised) before the GetPos call (725 bytes).
Best next idea: find a spelling where the perp is evaluated after GetPos but stays in the same full-expression as the sum (VC canonicalises the
add to sum-first only inside one expression); e.g. a FableWin-named inline helper taking the forward by value, or an operator on the sum temp
whose argument evaluation is deferred (check FableWin C3DVector/C2DVector inline set: GetScaled, Interpolate, ToC2DVector, Rotate).
Probe header kept at probes/C3DVector.probe.h (not part of the overlay).
Names: no landed spelling proven wrong; made-up local names only (door, signPost, forward, base, candidate).

## 0x006BFCD0 -- wf19 author pass 2 (still DIFFER 766v764, 4 hunks; best source unchanged = differ/006bfcd0.cpp)
Residue unchanged: final candidate add. Retail tree = sum + perp for all three components (z is memory/memory: retail `fld [sum.z] ; fadd [perp.z]`,
ours `fld [perp.z] ; fadd [sum.z]`), so it is the operand order of the TREE, not an x87 stack choice. Retail's sum is computed BEFORE perp
(sum adds right after GetPos, then fchs/fmul perp) and still goes first, with fxch.
Facts established this pass (all graded, see ATTEMPTS.log a1..x3):
- VC7.1 here puts the most recently computed operand first whenever both are on the FP stack: two-statement base+perp, perp+base, perp+(sum)
  in one expression (g1), named offset after base (e1/e3/x1/x2/x3), const-ref base (e1), made-up inline helper probe with base param (r1/r3),
  older-symbol offset assigned later (f1, x1 via Initialise): all perp-first. Only forms where perp is evaluated FIRST give sum-first
  (g3/k1: perp before GetPos or before the sum adds) -- wrong schedule.
- operator+ body order is irrelevant (overlay probe probes/hdrA: reversed body m1 = same; copy+= / named result m2/m3 = 803).
- Not a symbol/inline counter: 1/2/3 dead C3DVector copies (o1-o3), HASTC_FORCEINLINE / TCDRIVER / CINTELLIGENTPOINTER opt-ins (j1-j3),
  ptr-ref list read, side if/else, direct-init forms (h1-h3): all identical.
- Flags: /G5 same, /G7 worse, /Ob2 /Ox /Oa same, /Oy- 675; QFE-4035 compiler (FABLE_VC71=$S/wine/vc71) same.
- C2DVector forward copy (scout's suggestion) = 755 two-stmt / 725 one-expr (the copy vanishes); C3DVector copy is right.
- Reference-valued perp arg ((offset *= side), (forward = ...), temp *= side): 725/776.
- base += perp then copy, candidate = candidate + perp, assignment-not-init candidate: 778-827.
Best next idea: the order needs sum to be the "newer" tree operand while its code is scheduled first. Look for a construct where the sum is
built in the same full expression AFTER the GetPos call but the perp temp is created by something that is not a pre-evaluated argument --
e.g. a real FableWin inline that takes (const C3DVector&, float) and returns *this-based sum (Interpolate? GetScaled?) whose body adds
`*this + arg` with arg built inside the body; or check whether retail's TU has the CRightHandedSet::GetRight-style helper on CRightHandedSet
(FableWin inline list) that returns (-Forward.Y, Forward.X, 0). Also try diffing this TU's sibling CTCBuyableHouse::GetRentMoneyPos (0x006C06D0,
not landed) once it is authored: it adds three vectors and may show the house TU's idiom.
Names: none proven wrong. Made-up local names only.

## 0x006BFCD0 -- wf19 author pass 3: RELOCATION_MATCH (final.cpp, strict-header regrade RELOCATION_MATCH, vt PASS, addmatch done)
What made it match: the final add's operand roles. Toy probes (toy/*.cpp, stand-alone V3 replica of the row's vector code) showed VC7.1
emits retail's `fxch st(2) ; fadd st,st(2) ; fstp st(1) ; fadd [perp.y] ; fld [sum.z] ; fadd [perp.z]` only when the position (ahead + GetPos)
is a BY-VALUE copy evaluated first and the sideways offset is bound by CONST REFERENCE (Off4(const V3& p, V3 s){return s+p;}). Both-by-ref,
both-by-value, named base passed by value, member op+ in either order, free op+, reversed op+ body, += on a copy, Shift-style member with a
local, assignment-expression forms and every two-statement / named-offset form give the offset-first add (ours) or move the perp before the
call. So the row uses a file-static inline `AddOffset(const C3DVector& offset, C3DVector position) { return position + offset; }`.
CAVEAT for the coordinator: AddOffset is a MADE-UP descriptive helper. No symbol exists in Ego_r, FableWin (debug build that lists other
anonymous-namespace C3DVector helpers but none like this), FSE or EgoCore. It is plain C++ and fully inlined, but whether a helper with this
shape is acceptable provenance is the owner's call; if not, the row stays DIFFER(766v764) with the 4-hunk residue described above, and the
next idea is to find the real construct that gives "position copied by value, evaluated first; offset by const reference".
Headers / flags: overlay hdr/engine/CTCBuyableHouse.h (FABLE_RETAIL_CTCBUYABLEHOUSE_GETSIGNPOSTPOS); default flags.
Also proven this pass: CThing::GetPos's body must NOT be visible (visible = 723 bytes, the forward copy disappears).
Names: no landed spelling proven wrong.

## 0x0055D180

## 0x0055D180 CTCPhotojournalCapture::TakeQuickScreenshot -- RELOCATION_MATCH (/GS), ~17 grades
- What made it match: (1) path = GetPhotojournalDir() + GetValidProfileName(CUserProfileManager::Get() ref .ProfileName) + L"\\" + name + L".jpg";
  (2) the "no room" case is an EARLY RETURN (`if (!CAFile::IsRoomForFile(10000000, path)) return;`) so the CDiskFileWin32 and the
  CCharString sound name are function-scope locals with their own frame slots (block-scoped they share the temporaries' slots: frame
  0x270 v 0x274); (3) surface via inline GPDisplayManager->GetBackBuffer().GetD3DSurface() (direct member read put it in ecx, retail eax);
  (4) _SYSTEMTIME with its 8 named WORD fields (an array model moved its frame slot).
- Headers/flags: `// Flags: /GS`; overlay opt-ins in hdr/README.md (CAFile ISROOMFORFILE, CDateAndTime SYSTEMTIME, CUserProfileManager
  PROFILENAME_MEMBER, CPlayerGui SCREENSHOTMESSAGE, CSurface GETD3DSURFACE) plus existing CDISPLAYMANAGER(+_FRONTEND), SCOPED_PTR, CTEXTURE, CPIXELFORMAT.
- Names: FormatDateTime 0x00A0D980 is ?FormatDateTime@CXBConfig@@SIXABU_FILETIME@@PA_W1H@Z in BOTH Ego_r (00b77fa0) and FableWin (0318fd40);
  landed global_FormatDateTime_00a0d980.cpp spells it global `?FormatDateTime@@YIXABU_FILETIME@@PAG1J@Z` (NAME_FIXES). 0x0055D4C0 landed as
  global_Destval_0055d4c0.cpp is really CTCPhotojournalCapture::TakeTheBloodyScreenshot (Ego_r). 0x0041A600 is GetPhotojournalDir (Ego_r
  0042be40 + FableWin 017cabd2/01913810), misnamed GetSaveDir in landed CTCInventory_HasScreenshots_00581b33.cpp.
- Made-up names: none (CTCPhotojournalCapture modelled locally as its siblings do: no engine header).
- Repair (2026-11-13): hdr/engine/CSurface.h re-based on the current repo header (keeps agentNF32's FABLE_RETAIL_CSURFACE_GETBYTELENGTH block; only the
  GETD3DSURFACE block added); hdr/README.md now documents the CDateAndTime line-4 and CUserProfileManager line-3 #if rewrites (default paths unchanged).
  Regraded RELOCATION_MATCH /GS (normal + LANDVERIFY_STRICT_HEADERS=1); vt.py PASS.

## 0x00A8F9E0


## 0x00A8F9E0 C3DPrimitive2::UploadToIndexBuffer(unsigned long) -- RELOCATION_MATCH (wf19, ~10 grades)
- Made it match: port onto shared headers (C3DPrimitive2 overlay with Data typed scoped_ptr<CDeviceIndependent>, scoped_ptr reset,
  CIndexBuffer, ANIMATEDBLOCKS); param is `unsigned long flags` (PDB _NK), InitIndexBuffer inline; first loop = empty peek loop over
  TriangleLists (dead GetTotalTriangleCount() vanishes entirely, wrong); index count loop written in the function (GetTotalIndexCount
  inline member was emitted as a call); `length` computed before the 2nd new; strip size read once into a local; triangles read as
  tri[j] (gives the +4-biased pointer walk); offset = rep * vertex_count (strength-reduced base; zero store lands inside reps>0 guard).
- Headers: overlay hdr/engine/C3DPrimitive2.h opt-ins FABLE_RETAIL_C3DPRIMITIVE2_DATA + FABLE_RETAIL_C3DPRIMITIVE2_UPLOADTOINDEXBUFFER;
  also FABLE_RETAIL_SCOPED_PTR(_RESET), FABLE_RETAIL_CINDEXBUFFER, FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER/_ANIMATEDBLOCKS,
  FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT (for operator*). No flags.
- Local models (no shared header): N3DPrimitive::CDeviceIndependent, N3DPrimitive::CTriangleList (Ego_r names, FableWin accessors).
- NAME_FIX 0x00AC1F00: landed rebuild/src/compiled/00/ac/CVertexBufferWin32_DoUnlock_00ac1f00.cpp is named CVertexBufferWin32::DoUnlock;
  correct name CIndexBuffer::Unlock. Evidence: (1) Ego_r.pdb ghidra_out/egor_pdb_names.tsv:35189 00bcec40 ?Unlock@CIndexBuffer@@QAEXXZ
  (Ego_r copy of the same method); (2) ghidra_out/labels_chunks718_725_2026-11-03.tsv:270 00ac1f00 CIndexBuffer::Unlock (Ego_r + FableWin
  ?Unlock@CIndexBuffer@@QAEXXZ, IDirect3DIndexBuffer9::Unlock slot 12 on +0x04; catalog DoUnlock name wrong) and
  ghidra_out/labels_naming_wave1_2026-10-28.tsv:530 00ac1f00 CIndexBuffer::Unlock; (3) this row calls it on IndexBuffer (a CIndexBuffer*).
  The coordinator should respell the landed file/symbol at landing.
- Descriptive names (repair, made up, no PDB enumerator names exist in any extract): EIndexBufferFormat::IndexFormat16 = 101 and
  EIndexBufferPool::IndexPoolManaged = 1, spelled exactly as landed 0x00BBF380 / 0x00BC3E10 (types EIndexBufferFormat / EIndexBufferPool
  are PDB names from ?Initialise@CIndexBuffer@@). Pool value 2 is a cast (no landed name). C3DPrimitive2::EBufferStatus (PDB type, from
  ?SetIndexBufferStatus@C3DPrimitive2@@AAEXW4EBufferStatus@1@@Z) is declared with no enumerators; the row passes (EBufferStatus)1.
  Old made-up IBF_16 / IBP_STATIC / IBP_DYNAMIC / BUFFER_STATUS_UPLOADED dropped. Still RELOCATION_MATCH (also strict headers); test PASS.

## 0x00B206B0

## 0x00B206B0 CSplineRandomOffsetData::OffsetSplinePositions -- RELOCATION_MATCH (29 grades)
- Made it match: shared headers (CCardinalSpline/C3DVector/CArray) + IsNonZero via FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ;
  limit squares via GFSqr (engine/GFWorldDistance.h; FableWin ?GFSqr@@YIMM@Z) -> `fld; fld st0; fmul`; bind thisPoint then
  initialPoint refs (fixes load order + esi reuse); `change += offset` (frame 0x30, VC commutes fadd); ellipsoid terms
  parenthesised (m*m)/l2 AND summed as running statements `d = a; d += b;` -- a one-line sum is canonically reordered
  (Z term first, ly2/lz2 slots swapped) regardless of source order or declaration order.
- Needs: hdr/engine/CCardinalSpline.h opt-in FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT (GetControlPoint/SetControlPoint inline,
  FableWin names); defines FABLE_RETAIL_C3DVECTOR, _NONZERO_SQUARED_XYZ, FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_CARRAY. No flags.
- Descriptive name: SplineRandomSeed (0x013999C4, TU-local static = 42, no PDB symbol found).
- Repair (verifier): `C3DVector& moved = change;` alias now justified in the header Shape section -- without it (v30.cpp)
  the row grades DIFFER(773v773) only in the inlined IsNonZero x87 sequence at 0x00B206F2 (retail fld st0/fmul st1/fld st2/fmul st3/faddp).

## 0x007D46D0

## 0x007D46D0 CTCTavernGameCoinGolf::FadeOutTableOccupants -- RELOCATION_MATCH (wf19; repaired, also under LANDVERIFY_STRICT_HEADERS=1)
- Made it match: shared headers (retail CThing chain + CThingCreatureBase, CTCBase::PeekThingSearchTools, GETTC_EXPANDED GetTC,
  std::list/std::vector from stlport/fable_containers.h, real GetCamera(C3DVector&,C3DVector&)); a NAMED empty CThingFilter_IsHuman
  local; the GetAllThings body visible (noinline, list head stays in edi); lookAt in its own block; coin top through std::min.
- Headers/flags: overlay $W/hdr (engine/CThingSearchTools.h + nested opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_TYPEMASK,
  hdr/README.md); also FABLE_RETAIL_CTHING_STATTYPEFLAGS, FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS, FABLE_RETAIL_CTCTAVERNGAME. No Flags.
- Repair (verifier findings, all applied):
  1. The visible GetAllThings body now is the landed 0x007D4CE0 instance: scan test, then the THING_TYPE mask test via
     T::StatGetTypeFlags (0xF for CThingCreatureBase; `and edx,0Fh / cmp dl,0Fh` @0x007D4D1E), then the filter, push_back.
     Opt-in sub-macro so the CThing-instance rows that use ALLTHINGSBODY keep their default path. The source comment names 0x007D4CE0.
  2. `this` model: CTCTavernGameCoinGolf : CTCTavernGameCoinBase (local, no shared header exists) : CTCTavernGame (shared header,
     retail sizeof 0xF0). Chain confirmed by Ego_r and retail RTTI (ghidra_out/rtti_bases.tsv lines 1961-1962). CoinBase holds
     Arrow/Arrow1/Arrow2 +0xF0/F4/F8, Coins +0xFC, Held +0x108, BoardPos +0x10C. Note on offsets: Ego_r minus 0x18 up to Coins, but
     minus 0x1C after it (Ego_r's vector<Coin> is 16 bytes, retail STLport's 12): Held 292-0x1C = 0x108, BoardPos 296-0x1C = 0x10C
     (the verifier's "296 - 0x18" gives 0x110; the conclusion BoardPos = retail 0x10C is right).
  3. NAME_FIXES.tsv: landed GetCamera 0x007D3560 `PCoin` (+0x108) -> CTCTavernGameCoinBase::Held and descriptive `TableCentre`
     (+0x10C) -> CTCTavernGameCoinBase::BoardPos. Sources: Ego_r type info (struct_layouts_egor.tsv / fable_types.h) and the landed
     CTCTavernGameCoinBase rows AddNewCoin 0x008EFA00 / UpdateMeshes 0x008EF2F0 / ShoveHaPenny::NewRound 0x007CF7E0 (same offsets,
     same names; AddNewCoin's retail bytes set +0x108 = &Coins.back()), plus this row comparing +0x108 with &Coins[i].
     Caveat: member names exist only in Ego_r type info (FableWin.pdb extracts here carry no member records), so the landed rows are
     the second source for the offset<->name mapping, not an independent PDB.
## Verifier findings not applied
- none.

## 0x0094A3B0

## 0x0094A3B0 CAIStateGroup_GoToPub::ChooseAction(bool) -- DIFFER, first authoring pass (~75 grades)
Ported onto shared headers (no overlay needed): FreeSeat's opt-in set + FABLE_RETAIL_CTCVILLAGE, _CTCVILLAGE_TAVERNTIMES,
_CTCTAVERNTABLE_SEATS + _LAYOUT, _CTCTAVERNGAME; engine/GFRandom.h GFRandom; ThingAlive.h IS_THING_ALIVE (3 inline + 4th
out of line 0x4CC340 comes out by itself); CTCVillageMember::GetPVillage, CTCVillage::IsTavernTime, CTCTavernGame::IsBeingPlayed.
Frame (8 bytes, arg slot reused), every GetTC, every random and every test already match. Best = differ/0094a3b0.cpp (v3), 810v785.
Residue = only which duplicate `call X; epilogue` block survives VC7.1's tail merge:
- Drink: retail keeps the 1st copy (0x94A4EA, (a) Fullness!=0 path); ours keeps the last (rand%5 case 0).
- Laugh: retail keeps talking case 2 (0x94A5E1); ours keeps the laughing-path copy, so ours also lacks the `mov ecx,edi`
  hoists at 0x94A604 / 0x94A628. TellStory: retail merges default into the r2 tail (0x94A650); v3 leaves it unmerged.
Levers found (each fixes one part, breaks another):
- (a) as `return tankard->Fullness != 0.0f ? Drink() : false;` (v21, 792v785): Drink survivor right, but its false merges
  with ThankBarmaid's false (xor al,al moved to 0x94A525) and layout becomes jnp. `==0 ? false : Drink()` keeps the false
  inline but the survivor goes back to (b)/(d). A ternary then-arm call seems to win the merge; plain `return X()` = last wins.
- Single `return TellStory();` after `if (talking) {switch 0/1/2} else {laugh; r2==0 SitIdle}` (v50 795 / v51 777): Laugh
  survivor becomes case 2 (right) but then NO `mov ecx,edi` is hoisted in the cascade / laughing / r2 paths (11 hunks).
Tried, no effect: case order in both switches, default/break/case 3, else-if chains, nested ifs, `!GFRandom`, bool locals,
result-variable switch, ternary chains (incl. laugh as else-arm), choice variable + one switch (v46, 784v785, partial
threading), gotos so every call appears once (v54, 812), SitIdle once at the end (v52/v53/v61-63), full else-chain (v76),
(a)x(b) form matrix on v50 (c11..c44), flags /Ob1 /Ob2 /Oa /EHsc /GR /Zi.
Best next idea: find the IL shape where all `mov ecx,edi` hoists happen BEFORE merging (v3 hoists cascade + r2 but not
laughing) AND Drink(a)/Laugh(case2) win: e.g. try ternary-then-arm Laugh in the talking switch region combined with v21's (a)
and a (b) that cannot merge with (a)'s false; or read VC7.1's cross-jump survivor rule off a small probe (two identical
`if (c) return X();` returns, vary jump- vs fallthrough-reached) and spell the row to it.
No landed name proven wrong here (dossier's SlumpDrunk-for-0x94A200 note stands, not re-checked).

## 0x0094A3B0 CAIStateGroup_GoToPub::ChooseAction(bool) -- RELOCATION_MATCH (attempt 2)
What made it match: VC7.1 cross-jumps identical tails; the survivor/hoist pattern is a fact about the SOURCE shape, not a tie.
- The laughing branch repeats the 50/50 SitIdle/TellStory tail (`else if (laughing) { r2==0 Laugh; r2==0 SitIdle; TellStory }
  else { r2==0 SitIdle; TellStory }`): the compiler merges the two tails, which produces retail's `mov ecx,edi` hoists at
  0x94A604/0x94A628 and makes the laughing Laugh jump into talk case 2 (0x94A5E1).
- The talking switch stores into `bool result` + break, then `return result;` (default TellStory merges into the r2 copy).
- Drink survivor = first copy: ThankBarmaid as `if (!ThankBarmaid()) return false; return Drink();` AND rand%5 written
  `case 1: FallAsleep; case 0: if (alive) return Drink(); /*fall*/ default: return SitIdle();` (found by a 40+48 form grid).
Headers / flags: no overlay, no flags; FreeSeat's opt-in set + CTCVILLAGE(_TAVERNTIMES), CTCTAVERNTABLE_SEATS/_LAYOUT, CTCTAVERNGAME.
No landed name proven wrong (dossier's SlumpDrunk note for 0x94A200 stands, unchecked).

## 0x00D17A80 (not landed)

## 0x00D17A80 NScript::CQ_BountyHuntScript::WatchForBoasts -- DIFFER(705v785), 4 hunks (wf19 author, 2026-11-12)
Best source: differ/00d17a80.cpp (ported onto shared headers: CQ_BountyHuntScript.h + overlay opt-in FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS,
CLevelScriptBase Game/MasterScriptData, CGameScriptInterfaceBase, CQ_SunnyvaleMasterData::BountyHuntTimeLimitExceeded, stlport vector,
CSpawnedFunc; vector ctor 0x0074F680 declared as an explicit out-of-line specialization; CTimer::GetTime inline via g_pGameScriptInterface).
Fixed on the way: the spawn temps' frame slots (name at esp+0x18, region "" at esp+0x1c) need the IMPLICIT conversion for the name --
`new CSpawnedFunc<CQ_BountyHuntScript>("WatchForQuestInfoDisplayConditions", 0, this, &...)` with an explicit `CCharString("")` region arg.
Residue: only the boast-vector destruction at the 5 early returns. Retail: 5 separate exits; for-top return = inline elem loop calling
??_G pair 0x004DA68A(0) + ~_Vector_base 0x004D9C5D call; the two arm returns = inline ~CCharString loop + 0x004D9C5D call; expiry and
post-loop returns fully inline. Ours (default flags) inlines everything and tail-merges all exits (705 bytes).
Proven with /d2inlT sweeps (T1..T80): our compile spends the inline budget on the return sites in FORWARD source order (site1 most
inlined, last site first to fall back to a ~vector / __destroy_aux call; b1 while-loop form at T57 = sites 1-4 full, site 5 calls ~vector).
Retail's split is the exact REVERSE order (and retail even calls the trivial vector ctor out of line, the earliest call of all) -> the
retail inliner walked this function bottom-up. Also: ours never emits the ??_G(pair) call at any T (it rejects __destroy_aux before ~pair).
Tried (no match): /d2inlS 1..40 (no effect), /d2inlT 0..80 on for/while forms, T+S combos, /Ob1 /Ob2; vector ctor inline (always inlined,
retail calls it); forceinline overlays for ~vector / _Destroy(range) / __destroy / __destroy_aux / _Destroy(T*) (then everything inlines);
plain-inline CSpawnedFunc ctor; loop forms: for(++i / i++), while with ++i at body end, do-while with guard, `<` vs `!=`, int index,
boast declared outside the loop, outer for(;;)+check; switch for the 15/16 arms; nested ifs for the expiry check; inverted
term-check polarity (`if(!term){...}else return;`, normalised by c1).
Best next idea: find what makes VC7.1 process inline sites bottom-up (a probe: tiny function with 3 early returns of a vector at
/d2inlT~40 under candidate source shapes: function defined in-class / as a template member / after a `#pragma auto_inline`, local
class wrappers, the script macro shape). QFE-4035 checked (
probe done: QFE-4035 default build of v1 and b1 = same 705-byte all-inline result, so the compiler build is not it).

## attempt 2 (wf19 author, 2026-11-12) -- still DIFFER(705v785), same residue; best source unchanged (differ/00d17a80.cpp)
New facts (probes, logged in ATTEMPTS.log):
- `for (; !term; Game->NewScriptFrame()) { vector ...; if (term) return; }` (x1.cpp, vector in the loop-body scope) compiles to the SAME
  705 bytes as the while + inner-block form: an equally valid spelling, no gain.
- Per-statement `#pragma inline_depth(0)` round the for-top return makes that site call ~vector whole; inline_depth(1..7) there =
  fully expanded (the scoped pragma does not limit nested inlinee depth, so it cannot give the partial ??_G / ~_Vector_base mix).
- c2.dll 13.10.3077 inliner switches (from its option table): -inlT -inlS -inld -inlld -inlcrmax -inlnobr -inlnold -inlnoln -inlnild
  -inlniln -inlflcsw -inlfcsw -inlocsa1..4 -inlfcsa -inluserinl -isize. Swept /d2<opt>{0,1,4,16,100} x T{40,50,60}: only inld/inlld move
  anything; none gives a ??_G(pair) call. /d2inlS below default (S0..S5) = default (floor); S only RAISES the budget.
- Budget model in OUR compile (probes pr2/pr5): a cumulative, frequency-weighted budget spent in FORWARD order. 10 extra in-loop
  exits: the first ~6 in-loop exits expand fully, later ones call ~vector; 14 extra exits after the for loop: all expand (cheap,
  cold). Ours never rejects the inline vector ctor at any T, and never emits the ??_G(pair) leaf call at any T (54 generated
  structural variants x T20..80, gen.py / big1.txt: outer while/for/do, elem read via [] twice / reference / direct, for/while/`<`
  index loops, ctor inline or declared).
- Retail's split fits exactly a budget spent in REVERSE IL order with depth-first expansion inside each inlinee (normal-end,
  post-loop, expiry full; the two arms: chain incl. ~pair inlined, then ~_Vector_base out of budget; for-top: chain down to
  _Destroy(T*), ~pair (??_G 0) and ~_Vector_base out; ctor last, called). No flag tried changes our order: /Ob1 /Og- /Oi- /Ot- /Oy- /Gy
  /Ox /G5-7 /Oa /Ow /GF /GR /Op /arch:SSE /EHsc(adds EH frame) x T sweeps; an `inline` address-taken definition; a TU-filler function
  before/after (budget is not TU-wide).
Best next idea: find the source fact that makes VC7.1 spend the budget bottom-up -- most likely the ORIGINAL TU context (the function
compiled after the rest of CQ_BountyHuntScript.cpp: Main 0x00D13580 etc. instantiate the same vector/pair COMDATs first, so c2 may
already hold compiled bodies of ~vector / ~_Vector_base / ??_G pair and cost them differently) -- needs Main or another real same-TU
user of vector<pair<long,CCharString> > reconstructed into the TU (no stand-ins). Alternatively disassemble c2.dll inline.c's
site ordering (string "INL:\tInlining %s (%d instrs) into" marks the logger) to learn what orders call sites.

## attempt 3 (wf19 author, 2026-11-13) -- still DIFFER(705v785), same residue; best source unchanged (differ/00d17a80.cpp)
New facts (all graded, ATTEMPTS.log):
- TU context is NOT it: a same-type vector<pair<long,CCharString> > user before/after the row (z1-z3), and `delete pair*` so the
  ??_G pair COMDAT exists in the TU (z4/z5, also x T30/45/57): all identical to the plain row.
- Callee-chain shape is NOT it: real STLport allocator::deallocate -> __malloc_alloc<0>::deallocate level (y1, x T40/50/60);
  #pragma inline_depth(0) / auto_inline(off) round the in-class ~vector (in-class = explicit inline, auto_inline ignored) and
  round the whole _construct.h destroy chain (y_cd0, x T40/50): no change.
- /Zi /Z7 /Zd: no change. /Os /O1 /Ob2+/Os: 444 bytes (wrong code); /Os /Ot = default.
- FUNCTION-WIDE `#pragma inline_depth(N)` before the definition DOES limit nested depth (unlike the scoped in-body form):
  N=5 makes EVERY exit an inline loop of `push 0; call ??_G pair` (the first time ours emits retail's ??_G(0) call) with the
  free inline; N=4/5 = 722 bytes, N>=7 = default. Uniform per function, so it cannot give retail's per-site mix alone;
  N=5/6 x T30..80 swept: no match (best 710/722).
- Which sites lose the budget in OUR compile (read at /d2inlT50, sites: 1 for-top, 2 arm15, 3 arm16, 4 expiry, 5 post-loop,
  0 normal end): 1,2,5,0 full, 3 and 4 partial (`call _Destroy(range)`, free inline). That is exactly a DFS POSTORDER of the CFG
  that visits the return path FIRST at every term check (5,0,1,2,4,3). Retail's split (0,5,4 full; 2,3 partial; 1 least; vector
  ctor called) is the same postorder with the CONTINUE path visited first at the term checks (0/5, 4, 2/3, 1, ctor).
  This order is very robust in ours: unchanged by `if(!term){...}else return;` everywhere (inv1/inv2), `while(term) return;`,
  `if(term != false)`, `if(term){return;}else{}`, and `goto done;` with the label at function end (gd1): c1/c2 normalise all.
  A goto-reordered loop (g_rev: post-loop check placed textually before the loop body) DOES move the split (in-loop sites full,
  post-loop/normal partial) -- so block order/CFG shape is the lever, not budget size.
- /EHsc: all exits share one cleanup funclet path (593 bytes) -- not retail.
Best next idea: find the CFG shape that makes c2's (postorder) inline walk reach the expiry site before the arm returns and the
for-top return last: i.e. at each IsActiveThreadTerminating() check the CONTINUE successor must be the first-visited one. Since
polarity/goto spellings are normalised, try shapes that change the successor list itself: the term test as a `switch` (case
labels are jump targets), the check folded into a compound condition with the following statement, the arms' `if (!flag)` and
term test merged (`if (!BoastActiveTimeLimit) { if (IsActiveThreadTerminating()) return; ...}` vs `if (!BoastActiveTimeLimit &&
...)`), or a for-top check that is the inner loop's own condition with the post-loop return re-test; read each at /d2inlT50 with
the site map above (q.py --flags /d2inlT50, map jne targets of the 5 term calls) before grading at default.
- (attempt 3, last) `switch (IsActiveThreadTerminating()) { case true: return; default: break; }` at all 5 checks (sw1): the branch
  sense flips to `je` (retail has `jne`, so wrong anyway) and the inline order is UNCHANGED (sites 3,4 still least at T55: whole
  ~vector call). Struck from the best-next-idea list.

