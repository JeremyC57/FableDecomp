## 0x008A7A70

## 0x008A7A70 CGameScriptInterface::TeleportAllFollowersToHeroPosition -- RELOCATION_MATCH (wf18)
- Matched from the 1012c shared-header draft + three facts: (1) the minion loop is a `while` (site frequency 9x lets push_back expand;
  the summon-spell push_back stays a call to COMDAT 0x00436C80, per the c1*(1/f+6)/7<74 rule documented in 0x0056AD40);
  (2) CTCSummonSpell::GetSummonedCreature expanded via new overlay opt-in FABLE_RETAIL_CTCSUMMONSPELL_GETSUMMONEDCREATURE_FORCEINLINE;
  (3) the 2D distance written with float locals (dx, dy; squared = dy*dy; squared += dx*dx) -- a C2DVector difference temp got its
  own 8-byte frame slot (sub esp,38h, hero/key slots swapped); the scalar spill shares the TC-key slot like retail (0x30 frame).
- Headers/flags: no // Flags; overlay hdr/engine/CTCSummonSpell.h (new opt-in, needs promotion to rebuild/include at landing);
  defines FABLE_RETAIL_STLP_LIST (repo CTCFollowed.h uses list::size()) and FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE.
  FABLE_RETAIL_CTCFOLLOWED define is now a no-op (repo header is unconditional). Strict-header regrade: RELOCATION_MATCH.
- Tried and rejected: /d2inlT60..120 (push_back never expands at a `for` site), nested-ifs/continue/scope braces, ToC2DVector,
  named offset/Dot/C3DVector heroPos copies (all keep the 0x38 frame).
- Names: no new name fixes beyond the dossier's (landed GetFollowers / GetNearbyWanderPosition spellings already noted by 1012c).
- repair (verifier): GetTC/PeekTC now use the PDB-attested type-deduced single-argument forms (hero->GetTC(&followed), PeekTC(&summon/&sword),
  follower->PeekTC(&navigation)) + #define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE; still RELOCATION_MATCH (also strict headers).

## 0x00724630


## 0x00724630 CTCPhysicsStandard::SetPhysicsMeshBankIndex -- DIFFER(742v741), 1 residue (author wf18, 2026-11-12)
Best: differ/00724630.cpp (fully ported onto shared headers + hdr/ overlay, /EHsc). Residue: box centre X add only --
retail `fld [eax+0x90] ; add eax,0x84 ; fadd [eax]` (Max.X first) vs ours Min.X first (`fld [eax+0x84] ... fadd [eax+0xc]`).
Key findings:
- Source operand order of the box sum and the two radius products has NO effect (64-variant grid): VC7.1 picks the fadd/fmul
  operand order from global state. Each of these flips it: zero-branch offset form (plain stores / Initialise / ref local /
  Clear), bank form (direct PMainMeshBank_Data / local / PeekMeshBank), sphere/circle radius access (GetRadius() vs .Radius),
  GetTC form (TCI vs StatGetInterfaceType), even deleting the tail or the post-box code.
- `and byte [esi+0x85],0xef` (no hoist) needs the offset zeroed through an inline/ref (Initialise(0,0,0) matches order X,Y,Z);
  plain member stores hoist the bitfield load. Bank code (`mov eax,[eax+3Ch]; push edi; mov ecx,eax`) needs a bank local or
  PeekMeshBank(); direct access is wrong.
- Every state that gets bitfield+bank right has box Min.X first; states with box right (plain stores, or direct bank) break
  the bitfield or bank. Best: Initialise + const bank local + GetBoundingSphere().Radius + GetBoundingCircle().Radius.
Tried and no effect on box: MIN_FIRST/ADD_XY_GROUP/SCALE_X_PAREN/accum/sum/component GetCenter spellings, Peek accessors,
operator*, centre direct-init/early decl, meshInfo/bank refs, ToCentre spellings, nav literal, appearance accessor,
if/else forms, casts, /Ox /Ow /GX, no /EHsc, dtor variants, scoped mesh local, opaque box accessor, neighbour SetScaleFromOwner
in TU, 0.01f const local.
Name evidence: 0x004BBFF0 is NOT GetScale (inline `fld [+34h]`): its body `fld [ecx+4Ch]; fmul [ecx+34h]` + Ego_r 0x004BF740 /
FableWin 0x01D0A3C0 ?GetFullScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ (out of line in both) -> GetFullScale (the landed
RB row 0x0075A530 and CMap_DrawGetSeaLevel_004bbff0.cpp spell it wrong). 0x0071A730 = CTCMapwho::RemoveFromMapwho (landed file says OnDie).
Next idea: find a construct that changes the box choice without touching bitfield/bank: e.g. the real C3DBoundingBox::GetCenter
body (FableWin 0x01D... debug bytes) or an inline CPhysicsMesh helper Lionhead used; or diff the VC7.1 /FAs listing of
plain-store vs Initialise variants to learn which tuple decides the X order.

## 0x00724630 attempt 2 (wf18, 2026-11-12) -- still DIFFER; best stays differ/00724630.cpp (742v741, box X only)
NOTE: fg.py does NOT read the `// Flags:` line -- always pass `--flags /EHsc` (without it the radius hunk reappears).
New proven mechanism: the box-centre X operand order is decided by whether ANY member of `this` is address-exposed anywhere in the
function. Exposure (Initialise on OffsetVector3D, a `C3DVector&`/`float&` ref to any member -- OffsetVector3D.Z alone, Radius,
PPhysicsMeshInfo, in either branch) => Min.X first (wrong); no exposure => Max.X first (right). The bitfield RMW
(`and byte [esi+0x85],0xef` not hoisted above the three zero stores) needs exactly that exposure: plain stores let the scheduler
hoist the byte load (D1: only `float& z = OffsetVector3D.Z; z = 0` already blocks the hoist -- and flips the box).
State B (differ/00724630_stateB.cpp = plain zero stores + parens `Radius = (CalculateRadius());` in the zero branch, `centre *=
(RenderSizeX * 0.01f)`, `(sphere.GetRadius() * RenderSizeX) * 0.01f`, `(RenderSizeX * circle.GetRadius()) * 0.01f`): 748v741,
ONLY the bitfield hoist left (box, bank, radii all right). Redundant parens are no-code scheduler tuples that flip the radius
operand order but never the box (512-variant grids grid3/ grid4/).
Tried this attempt (no box/hoist fix): flags /Op /G6 /G7 /Gy- /GF /Ob1 /Zi /GR- /Oi- /Os /Oa /Ow; zero via ctor temp / chain
X=Y=Z / reversed chain Z=Y=X / int literals / zero local / local vector copy / Initialise(0,0,0); decl order of mapwho/meshInfo;
local renames (no hash effect); extra locals (thing/world/appearance/scale); dummy function before the row (TU numbering: no
effect); C3DVector copy-ctor body forms; C3DVector plain inline; GetCenter forceinline and `/ 2.0f`; operator-> via Get /
InternalGetObject; CCountedPointer view of PMainMeshBank; box copied to a C3DBoundingBox local (copy not removed); explicit
CThingPhysicsMeshInfo ctor (FableWin ??0CThingPhysicsMeshInfo@@QAE@XZ exists); SetRadius (FableWin inline) in either branch;
HasTC forceinline / const-bool& form; nav placement before stores (RMW stays but lands before the stores); paren grids (7+2
knobs) in both states; bank cast forms (static_cast / parens / world ref).
Best next idea: find a construct that blocks the scheduler hoisting `mov al,[esi+85h]` above the three ebx stores WITHOUT taking
the address of a `this` member (state B), or one that restores Max-first under exposure (state a0). Candidates not yet tried:
the real retail TU context (all CTCPhysicsStandard.cpp functions before 0x00724630 compiled ahead of it, since inline COMDATs are
compiled in TU order), a different CTCPhysicsStandard bitfield unit type (Ego_r says bool:1), or C3DVector::Initialise taking
`const float&`.

## 0x00724630 attempt 3 (wf18, 2026-11-12) -- RELOCATION_MATCH (strict headers), /EHsc
What made it match: the zero branch frees the info with `operator delete(PPhysicsMeshInfo);` instead of the delete-expression
`delete PPhysicsMeshInfo;` (same `push [esi+40h] ; call 0x00BFE9BC` bytes). Everything else is attempt 1's best source (Initialise
for the zero offset, const bank local, `.Radius` accessors). Found by ablation of a0: under Initialise the box-centre X order flipped
to retail's Max-first whenever one of {the delete, the sphere line, the circle line, the transform, the nav else} was removed, but not
{the second CalculateRadius, BeginLoading, RemoveFromMapwho, the offset copy, the zero-branch nav clear}; then the delete spellings.
Headers/flags: hdr/ overlay as listed in hdr/README.md (unchanged from attempts 1-2); `--flags /EHsc` (fg.py ignores `// Flags:`).
Also tried this attempt (no effect): a preceding TU function using GetCenter / Initialise / operator*=; /G3 /G4 /G5; bitfield unit
unsigned int / unsigned char (state B); branches as inline helper members (state B); a 63-variant zero-branch paren grid (state B);
a 127-variant neutral-knob grid on a0 (cast, null tests, `new T()`, `!index`, PeekMeshBank, const pointer, IsInMap()!=false);
nav-else spellings (parens, ==false, ?:, named mesh local, bool local).
Name notes: GetFullScale (0x004BBFF0) and RemoveFromMapwho (0x0071A730) as in attempt 1's NAME_FIXES.tsv.

## 0x00724630 repair (wf18, 2026-11-12) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), /EHsc
Applied all 7 verifier findings: dropped overlays C3DVector.h (stale, lost CROSS_XZ_GROUP / NORMALISE_PAREN_SQUARES_ZYX), C3DMeshObject.h
(repo's wf17 POSITIONMATRIXFROMRHSET block used as is), C3DBoundingBox.h, CIVCountedPointer.h, and the CTCPhysicsBase SETRADIUS block.
Row-local `class CCircle {float CentreX, CentreY; ...}` replaced by a new shared hdr/engine/CCircle.h (Ego_r: C2DVector Centre +0,
float Radius +8); CPhysicsMesh BOUNDS now types BoundingCircle as CCircle (no reinterpret_cast). The two landed local CCircle copies
(`float X, Y, Radius` in CPhysicsMesh_CPhysicsMesh_00a6d550.cpp / _00a6da40.cpp) should move onto engine/CCircle.h at landing (repo
is read-only for this agent). Backup of the pre-repair hand-back: repair_bak/.

## 0x009CE5E0

## 0x009CE5E0 CBankFile::AddEntryMultipleFiles -- RELOCATION_MATCH (13 grades)
- What made it match: the hit arm ends `InitEntry(...); return;` and the miss path's filename + InitEntry sit at FUNCTION scope (not an else block):
  the two filename locals then get different frame slots (+4 / +8; the resize zero temps share +4), so VC7.1 cross-jumps only the
  ~CCharString call instead of merging the whole InitEntry tail (old residue 726v741 + ebx/ebp swap vanished at once).
- Port: shared headers fable_string.h / CArray / CPackedUIntArray / CCountedPointer / fable_map (FABLE_RETAIL_STLP_TREE_MFIND_DECL,
  `OldSymbolIndices.find(symbol) != end()`), CBankFile.h with FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS (virtual slot 6) +
  FILENAMEINDICES + new ADDENTRYMULTIPLEFILES opt-in, CBankFileEntryUpdateData.h new RETAIL opt-in (0x18 bytes);
  FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (2-arg resize bodies visible, noinline) + an explicit-specialization DECLARATION of
  vector<CCharString>::resize(unsigned) (0x0049B760, out of line). Without the resize opt-in: 926v741 (inlined resizes). No Flags.
- Explicit blank CRuntimeData (field stores dword,dword,byte,byte) -- the STLport resize(n) value-init `_Tp()` zeroes 3 dwords (worse).
- Local models (no shared header): CBankStateBlock (FableWin: ~dtor slot 0, GetDefault slot 1, CreateCRC UAEKXZ slot 2 -- per
  CWADBankStateBlock vtable 0x0129CFD0 slot 2 = CreateCRC 0x00A3A2D0), CBankCreatorException (as landed siblings 0x009CB8F0/0x009D47E0).
- Name issues found: landed Equals 0x009CC750 calls slot 2 `GetCRC() const` -- real name is CBankStateBlock::CreateCRC() (non-const,
  ?CreateCRC@C*BankStateBlock@@UAEKXZ in FableWin.pdb); landed 0x0049B760 is spelled `?resize@CCharStringVector@@QAEXI@Z`, real is
  std::vector<CCharString>::resize(unsigned int). Landed Equals/SetFilenames/InitEntry are public; PDB has them private (no byte effect).
## Repair (verifier findings, 2026-11-12) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1)
- 0x009D47C0: NAME_FIXES row -> ??1CBankCreatorException@@QAE@XZ (its only xref is ThrowInfo 0x013693B4 pmfnUnwind; Ego_r dtor/copy-ctor pair
  0x00B19590/0x00B195B0 mirrors retail 0x009D47C0/0x009D47E0); landed made-up ?Destroy@Pair@@QAEXXZ must be respelled. Source comment cites it.
- 0x009D3DF0 / 0x00464931: NAME_FIXES rows with the exact PDB spellings (FableWin vector<CBankFileEntryUpdateData*>::resize(n,x);
  Ego_r/FableWin vector<unsigned long>::resize(n,x)); landed address-suffixed / Leaf-less spellings listed as wrong.
- Test: added a type-only refresh (same filenames + CRC, new type -> RequiresUpdate set, DataType rewritten) and a no-change refresh;
  removing `d.DataType != type ||` from the model now makes the test FAIL.

## 0x00627400

## 0x00627400 CTCQuickAccessMenu::ConstructExpressionCategoryList -- RELOCATION_MATCH (grade 1)
- Matched by copying sibling 0x626E40's exact include/macro set + auto_inline(off) list<CComponent*> specialisations + inline GFIntToWideString, with the dossier's real names/strings (no map, items.size() direct). Default flags.
- Needs overlay hdr/engine/CTCQuickAccessMenu.h opt-in FABLE_RETAIL_CTCQUICKACCESSMENU_EXPRESSIONS (declares the private method; repo header lacks it). Old draft names wrong (CFrontEndManager, Format, SUIActionData, SetAction, Disable, AddEntry, ...) -- see DOSSIER table.
- Repair (verifier findings 1-4): dropped the local `#pragma auto_inline(off)` ~list/_List_base::clear specialisations; now uses the shared-header opt-ins FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE + FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE (real STLport bodies: empty ~list + inline ~_List_base{clear(); deallocate}), commented with call 0x0042ABCA @0x0062758A and the landed 0x0042ABCA / 0x0042A18F rows. GFIntToWideString inline body now cites landed 0x00405F00 and the inlined Sprintf (push 0x0122DE44 @0x0062745B, call 0x0099BA70 @0x00627461). Still RELOCATION_MATCH (also strict headers); test PASS.

## 0x007EB6A0

## 0x007EB6A0 CTCActionUseSearch::OnActionUse(CThing&) -- RELOCATION_MATCH (wf18, 8 grades)
- Made it match: full port onto shared headers (draft's Use(CThing*) / shop / tutorial-flag model was wrong; see DOSSIER 7); named
  C3DVector edge local for the Z height; if/else animation assignment; CPlayer.h included BEFORE CThingPlayerCreature.h (else the
  inline CPlayer::GetPPlayerCharacter body is not defined and is emitted as a call); PostDeed thing argument spelled &GetParentThing()
  (ParentThing member read gave an eax/edx register swap through the rest of the function).
- Overlay opt-ins (hdr/README.md): CTCACTIONUSEBASE_ISTHISTHEFT, CPHYSICSMESHOBJECT_ZEDGE, CTCCONTAINERREWARDHERO_HASREWARD.
  CTCSearchableContainer has no shared header: declared in the row on its CTCContainerRewardHero base (Search(CThing&) 0x00812340).
- NAME_FIXES candidates (from DOSSIER, Ego_r): 0x00747030 is CTCStealth::SetActionPermittedToSneak (landed as
  CDrawEnvironmentUser_00747030::SetWorld); 0x00812340 Search takes CThing& (landed `Search(long)`); 0x0074E500 HasReward is const bool.

## 0x00959F80

## 0x00959F80 CAIStateGroup_HoverKeepInArea::Update -- RELOCATION_MATCH (wf18)
- Matched by: porting onto the sibling 0x00959BF0 overlay/spelling (GetWorld/GetWorldSeed seed hoisted, GetTC expanded, GFRandom,
  GFSin/GFCos, `dir = target - GetPos(); dir.Z = 0.0f;`, `perp = dir.CrossWithUp()`, ADD_XY_GROUP), plus row-specific shape:
  `if (LookAtPos(...) != 2) { Terminate(); return; }` (merged `mov ecx,esi` before jne), loop `diff = GetPos() - StartPos` (ebx = &StartPos
  reuse) and `while (result == 4 && (float)sqrt(dy*dy+dx*dx) >= 20.0f)` (test ah,1 form; `!(x<20)` gives test ah,5/jp).
- Needs: repo FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP + overlay FABLE_RETAIL_C3DVECTOR_CROSSWITHUP, _ENEMYCREATUREBASE_VSLOTS,
  _HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS (from 00959bf0/hdr) and new FABLE_RETAIL_CAISTATEGROUP_HOVERKEEPINAREA. Flags none. Strict: RELOCATION_MATCH.
- Ideas that failed first: local models with C3DVector(dX,dY,0) ctor + explicit perp (x87 residue c/-X popped vs consumed); operator* paren
  variants (ALL/XY/Y/X); float s/c locals; term reorderings.
- Draft name errors confirmed (dossier): GetPWorld->GetWorld, GetRandomSeed->GetWorldSeed, GetPosition->GetPos, Normalise->AccurateNormalise,
  AIAction_MoveToPosProximityAndFacePos->AIAction_MoveToPosAndFacePos (slot 22), Centre->StartPos.
- Repair (verifier): bare casts/literals replaced with the PDB enumerators from engine/EAIAction.h (ACTION_REPEAT_UNTIL_FINISHED,
  ACTION_REPEAT_CALLBACK, AI_ACTION_FINISHED, AI_ACTION_IN_PROGRESS); still RELOCATION_MATCH (also strict); test PASS; payload rewritten.

## 0x0094F180


## 0x0094F180 CAIStateGroup_BalverineCornered::IsRunnable -- RELOCATION_MATCH (wf18, 5 grades)
- Made it match: full port onto the shared headers (sibling 0x0094F480's #define list + GetTC(TCI_ENTITY_EVENTS) / PeekTC(TCI_NAVIGATION),
  real GetEvent<CEventHitBy, CFilter_Bind2<CIsHitEventHitByMelee,CIsHitEventBlocked>>, GetHitParams().GetPThingHitting(), GetDiameter,
  GetWorld().GetWorldMap(), GetCombatTypeDef().FrontMiddleLimit, nested single-false form). Last residue (744 v 754) was VC7.1 calling the
  inline CIntelligentPointer::operator= out of line (inline budget of this big function); retail expands it -> FORCEINLINE opt-in.
- Headers (overlay $W/hdr, see hdr/README.md): FABLE_RETAIL_CAISTATEGROUP_BALVERINECORNERED (new real class),
  FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBASE_GETHITPARAMS, FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE (FABLE_FORCEINLINE in the
  shared header, precedent FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR). Also uses FABLE_RETAIL_JACK_STAGE1_UPDATE only because it
  gates the GetCombatTypeDef() declaration in CThingCreatureBase.h. No flags.
- Landed names proven wrong (dossier): GetLandPos 0x0094F090 landed on a stand-in signature (real: C3DVector GetLandPos(const CThing&) const);
  0x00810F40 landed as global DrawGetEnvironment (ICF fold; Ego_r GetHitParams); 0x008645E0 landed as CMainGameComponent::BeginInputLoading (real NCreatureAI::IsAwareOfThing).
- Repair (verifier findings): dropped the unguarded `class CHitParameters;` from the NEntityEvents_CEventHitBase.h overlay (CHitParameters.h is already included; hdrcheck2 default=same now); restored CIntelligentPointer.h line 4 verbatim and gave the ASSIGN opt-in its own guarded fable_compiler.h include (overlay diff has no `<` lines). Regrade RELOCATION_MATCH, strict too; test PASS.

## 0x00AC53F0


## 0x00AC53F0 C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk&) -- RELOCATION_MATCH
- What made it match: (1) port onto shared headers with overlay opt-ins; (2) case values from the index table: 8 = Bump, 9 = Reflection
  (DOSSIER section 7.1 was wrong, the old draft was right), case blocks in retail code order (Transparent, Diffuse, Bump, Reflection,
  Illumination) -- this alone gave the `and esp,-8` frame; (3) named `ext` local for GetExtendedProperties; (4) the decisive one:
  CalculateMapFlags' REAL body (landed row 0x00AC5260) visible in the TU under `#pragma auto_inline(off)` fixed the register choice
  (material edi / map.Info ebx / Data spill) and the exit-block layout.
- Headers: overlay hdr/ (see hdr/README.md): C3DMeshFileEngineMaterialChunk.h (_FROM_MATERIAL ctor decl), C3DMeshFileMaterialChunk.h
  (_METHODS), C3DMeshFileMapChunk.h (_METHODS), C3DMeshFileMaterialExtChunk.h (_METHODS). No flags.
- Name fix: 0x00ABFDC0 is ?GetSelfIllumination@C3DMeshFileMaterialExtChunk@@QBEKXZ (Ego_r 00c3c8e0), landed as CWorld::DrawGetEnvironment (propagated label).
- Enumerator names of EMaterialMapType unknown: case labels are integer literals.

## 0x007E6450


## 0x007E6450 CTCOracleMinigame::UpdateTints -- RELOCATION_MATCH (flags /GS)
- What made it match: port onto the shared headers (CTCOracleMinigame.h CLASS, CThing GETTC_EXPANDED, CGameEvent INLINE+ADDMORE,
  CMainGameComponent PeekPlayerManager, CPlayerManager const GetMainPlayer, CThingPlayerCreature chain, GFSin.h, CRGBColour RGBA ctor),
  OutputsFiring's real body inline in the row (retail expands it), `long expression = -1;` before the switch (no default),
  `float s = GFSin(t*0.5f)` (float pool constants), and `CWorld& world = GetWorld(); long fps = CMainGameComponent::GetConstantFPS();
  ... world.GetFrame()` (retail calls GetWorld 0x00686A70 then reads ConstantFPS before GetFrame; no CWorld fps accessor exists in
  either PDB, so no made-up GetFramesPerSecond).
- Headers: overlay engine/NGraphicAppearance_CTCGraphicAppearance.h FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REMOVETINT (RemoveTintColour 0x004BFAE0).
- NAME_FIXES candidate (dossier): rebuild/src/compiled/00/4b/CParticleSystem_Kill_004bfae0.cpp is really
  NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour(void*) (Ego_r ?RemoveTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAX@Z, functions.tsv).
- Repair (verifier 1): the inline OutputsFiring body is now copied verbatim from the landed row
  rebuild/src/compiled/00/7e/CTCOracleMinigame_OutputsFiring_007e5d00.cpp (`int i` / `i++`, braces, `this->`), cited in a comment;
  still RELOCATION_MATCH (/GS), also with LANDVERIFY_STRICT_HEADERS=1; test spelling aligned, vt PASS.

## Coordinator fix at landing (2026-11-12): the switch mapped inputs to the wrong expressions
The hand-back graded RELOCATION_MATCH in fg.py and passed both verifiers, but the landing verifier (vland) rejected it:
RELOC_JUMPTABLE, three of the four jump-table entries pointed at the wrong case bodies. Retail table 0x007E6734: input 0 ->
0x007E64CB (expression 0), 1 -> 0x007E64DD (3), 2 -> 0x007E64CF (1), 3 -> 0x007E64D6 (2). The source had `case n: expression = n`.
It is now `case 0: 0; case 2: 1; case 3: 2; case 1: 3` (bodies in layout order). The test, which copied the wrong mapping,
is corrected too. fg.py does not compare jump tables; only vland does.

## 0x00959BF0 (not landed)

## 0x00959BF0 CAIStateGroup_HoverWander::Update -- RELOCATION_MATCH (wf18)
- Matched by: porting onto shared headers (GetWorld/GetWorldSeed, GetTC expanded, GFRandom/GFSin/GFCos headers, LookAtPos enums),
  `perp = dir.CrossWithUp()` (FableWin ?CrossWithUp@C3DVector@@QBE?AV1@XZ, inline) and `dir = target - here; dir.Z = 0.0f;`
  (the XY-constructor spelling left an x87 allocation residue: c/-X popped vs consumed, T.Y+Ysd operand order).
- Needs: FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP (repo) + overlay opt-ins FABLE_RETAIL_C3DVECTOR_CROSSWITHUP,
  FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS (slots 14..21; names = manifest labels, only GetCombatMinDistance proven in Ego_r),
  FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS. Flags none. Strict headers: RELOCATION_MATCH.
- Repair (2026-11-12): class now comes from the shared header engine/CAIStateGroup_HoverWander.h via overlay opt-in
  FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER (base CAIStateGroup_HoverMovementBase, public virtual Update; default flat layout unchanged);
  slot 20 IsCombatStateGroup made public in the VSLOTS overlay (UBE in both PDBs). The earlier "name note" on
  CTCCreatureNavigation::GetNearbyWanderPosition was wrong and is withdrawn: Ego_r 0x00701120 / FableWin 0x024787D0 both give
  ?GetNearbyWanderPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@_N@Z, matching the landed header. Still RELOCATION_MATCH (strict too).

