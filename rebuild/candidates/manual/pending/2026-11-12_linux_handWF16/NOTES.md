## 0x007E4E30

## 0x007E4E30 CTCBetting::FrameUpdate -- RELOCATION_MATCH (2 grades + 1 compile fix)
- Matched by porting onto shared headers (CTCBetting, CBettingDef, CThing/CThingBaseDef PeekBaseDef, CPlayer/CPlayerManager refs,
  CPlayerGui/CSubtitleRenderer SetText, CMainGameComponent PeekMainFont/GetConstantFPS/out-of-line GetCurrentServerFrame, CGameEvent
  inline ctor + AddUBYTE, CEnginePrimitiveHandle::Clear over std::vector iterators) with the real GetSubDef<CBettingDef> body in the TU.
  Last residue (empty-string dtor after the event) fixed by putting `CWideString empty(L"")` + SetText in their own block.
- Flags: /GS. Opt-in: hdr/engine/CTCBetting.h FABLE_RETAIL_CTCBETTING_FRAMEUPDATE (virtual FrameUpdate decl, slot 9).
- Descriptive name: g_bettingStartDelaySeconds (0x01384050, found in no PDB/FSE/strings). Globals 0x013BAE1D/0x013BAE20 use
  g_bettingActive / g_bettingAmount (the GetBetText / CGameScriptInterface spellings); ProcessInput's g_BettingActive / g_CurrentBet
  and DrawPreparePrimitives' g_bBettingInputEnabled are the conflicting landed spellings (no PDB data names).
- NAME_FIXES candidate 0x00416225 (landed as `T::Get` in rebuild/src/compiled/00/41/CWorld_DrawGetEnvironment_00416225.cpp, label
  `_global::GetMember_dword0x48_00416225`) -> `?GetCurrentServerFrame@CMainGameComponent@@QBEJXZ`. Sources: (1) the mangled name is in
  both ghidra_out/egor_pdb_names.tsv (Ego_r) and ghidra_out/fablewin_pdb_names.tsv (FableWin); (2) Ego_r layout
  (struct_layouts_egor.tsv) has CMainGameComponent `long CurrentServerFrame` at +0x48 (72), exactly the `mov eax,[ecx+48h] ; ret`
  body; (3) rebuild/include/engine/CMainGameComponent.h line 139-142 already opts in an out-of-line GetCurrentServerFrame bound to
  0x00416225 (wf10 row 0x006E3580 CTCTalk::PlaySpeech), and this row's call to it grades RELOCATION_MATCH. Caveat: 0x00416225 is a folded
  one-line getter with 41 callers (identical-COMDAT folding), so the respell applies to that row's name only, not to every caller.
- Repair: the betting mode is spelled PLAYER_MODE_BETTING (=39, engine/EPlayerMode.h, both PDBs) instead of `(EPlayerMode)0x27`; bytes unchanged.

## 0x007E8590

## 0x007E8590 CTCOwnedEntity::SetPOwner -- RELOCATION_MATCH (strict headers), 8 grades + repair
- Made it match: port onto shared headers (CTCOwnedEntity/CThing/CTCThingOwner/fable_string) with GETTC_EXPANDED; the "Dress" test reads
  ScriptData +0x78 (GetScriptData, FableWin-only inline), not ScriptName +0x74; StartsWithCaseInsensitive forceinline with the prefix
  as an implicit "Dress" conversion; AddTC string args are named CCharString locals; the nav name is block-scoped so its dtor
  (0x007E8807) runs before the flag store (0x007E880C) -- unblocked grades DIFFER at those two, a temporary DIFFER(646v650);
  `GetParentThing().SetAsDrawable(GetPOwner()->DrawIsDrawable())` keeps the parent in ebp (0x007E871F `mov ebp,[esi+4]`);
  `ParentThing->` grades DIFFER(650v650) with ebx/ebp swapped. GetTC's result ignored as retail does (0x007E85E7 reads the unset local).
  All three are documented in the row header comment.
- Headers (overlay hdr/, see README): FABLE_RETAIL_CTHING_SENDMESSAGEW, FABLE_RETAIL_CTHING_GETSCRIPTDATA,
  FABLE_RETAIL_CTCTHINGOWNER_OWNEDOBJECTS (friend CTCOwnedEntity), FABLE_RETAIL_CCHARSTRING_STARTSWITHCASEINSENSITIVE. No flags.
- NTCMessages::CMessageBase / CMessageOnSetThingOwner modeled in the row (no shared header exists), now with the real vtable:
  CMessageBase declares `virtual unsigned long GetType() const` (FableWin ?GetType@CMessageBase@NTCMessages@@UBEKXZ), the derived
  class overrides it returning 4 (FableWin ?GetType@CMessageOnSetThingOwner@NTCMessages@@UBEKXZ; rtti-vtables 01270bf0 slot 0
  0x007E8350 dtor, slot 1 0x007E8330 `mov eax,4 ; ret`). Still RELOCATION_MATCH (normal + strict).
- NAME_FIXES.tsv (hand-back):
  - 0x007E8310: landed row spells a global `struct CMessageOnSetThingOwner(void*)` with `m_argument`; correct is
    NTCMessages::CMessageOnSetThingOwner(CThing*) with member Owner +0x04 (FableWin ??0/??1/PeekOwner@...@NTCMessages, Ego_r
    struct_layouts Owner +4, rtti-vtables 01270bf0). The two rows define the class differently until it is respelled.
  - 0x006C0330: its local CThing model calls +0x78 "ScriptName"; it is ScriptData. Evidence: shared CThing.h (retail SetScriptName
    `lea ecx,[eax+74h]` puts ScriptName at +0x74) and Ego_r struct_layouts CThing ScriptName +0x84 / ScriptData +0x88 (adjacent, in
    that order, so retail +0x74/+0x78 = ScriptName/ScriptData).

## 0x00922010

## 0x00922010 CAIStateGroup_MinionWalkToMarker::Update -- RELOCATION_MATCH (wf16, first authoring attempt, ~16 grades)
- What made it match: port onto the shared headers (CTCAIScratchpad ScriptingStateGroup/TargetName, GetCreature().PeekPos(),
  GFRandom, CIntelligentPointer, CThingFilter_IsNot/HasScriptName, CFilter_Bind3, same-TU GetPBestThing/GetAllThings bodies);
  named `CCharString name` + `filter(name)` copies in each search block; the group read into a local `EScriptingStateGroups group`
  for the 8/9 if (gives retail's re-test `cmp eax,8` and mode-8 fallthrough); and the LAST residue (Nearest Pos copy stores
  scheduled late in retail) was fixed by NOT defining FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE: with
  CThingCompare_Nearest::operator() visible the caller knows the Nearest temporaries are not captured (Q32 visibility lesson).
- Headers/flags: no overlay, no flags. Repo headers only (see the #define list in the source).
- Dead ends tried: switch / flipped ifs / else-if / for(;;)+break dispatch; CCharString temp-in-argument spellings; C3DVector copy
  ctor spellings, by-value pos local, Nearest body-assign ctor; GetPBestThing body inline vs noinline.
- NAME_FIXES candidate (same TU, landed IsRunnable 0x00921630): its local CTCWalkToMarker{Mode,MarkerName} is really
  CTCAIScratchpad{ScriptingStateGroup +0x38, TargetName +0x50} (struct_layouts_egor.tsv; TCI_AI_SCRATCHPAD = 0xAA = 170).

## 0x00AF5090


## 0x00AF5090 NParticleEngine::CParticleSystem::ClearParticle -- DIFFER(659v657), ~75 grades
- Best source: differ/00af5090.cpp (= ported twin of landed ClearDecal 0x00AF5430; overlay engine/CParticle.h opt-in
  FABLE_RETAIL_CPARTICLE_CLEARPARTICLE; defines as ClearDecal plus FABLE_RETAIL_SCOPED_PTR(_RESET), CQUATERNION_PARTICLE_UPDATE,
  C3DVECTOR_PACKED, CRIGHTHANDEDSET needed for CParticle.h to compile). Leaf = PDB mangled name (non-const iterator&).
- Exact residue: everything identical except the inline ClearParentHandle handle Clear: retail `xor ebx,ebx ; cmp eax,ebx` / `cmp ecx,ebx` /
  `mov [esi+4],ebx` x2 then `mov ebx,[esp+18h]`; ours (exactly the retail ClearDecal pattern) `test eax,eax` / `test ecx,ecx` / imm stores
  with the group*12 reload hoisted between them. FableWin.pdb debug bodies of the CParticle and CDecal accessors and of ClearParticle /
  ClearDecal have identical instruction counts (87/40/26/13/28) -> same source; the difference is a codegen context fact, not a spelling.
- Tried (all same residue unless noted): Release forms INNER/default(645)/MEMBER+RELEASE_DELETES/no-local(567)/!Release/pre-store(635);
  Reset ADOPT, ADOPT+FORCEINLINE; Clear/Release forceinline; Clear with zero via local; SetData(0)(689); assign temp(721);
  ClearParentHandle spellings (unsigned casts, local refs, refs first(627), early return, split ifs(625), begin()[i], front()[i],
  explicit erase(661)); plain inline ClearParentHandle (always called: 478) with any other forced; forceinline GetParentHandle/
  GetPrimID/ClearAll; SPRITE GetPrimID; call-site: unsigned group, id/group/index hoisted, explicit compares, particle reference,
  handle before particle(18 hunks), while loops(466); handle dtor forms; no FABLE_RETAIL_CARRAY(633); accessor definition order;
  ClearDecal body before/after in TU; explicit template instantiations before; flags /Ob1 /G5 /G6 /G7 /Oa /Ow /Ox /Os /Oy- /GF /Gy- /GX,
  pragma optimize s/t.
- Best next idea: the retail TU context (CParticleSystem.cpp compiles ClearParticle after the ctor/AllocateParticle etc.) -- try
  reconstructing the TU's preceding real landed bodies (0x00AF4E10 ctor, 0x00AF5010 AllocateParticle) ahead of the row, or the
  QFE-4035 compiler; or a VC7.1 probe of what makes the zero-constant register choice (xor reg) win over the spill reload.

## 0x00AF5090 ClearParticle -- RELOCATION_MATCH (attempt 2, 4 grades)
- What made it match: the "zero-register" residue was a CONTROL-FLOW model error hidden by fg.py's jump-target normalisation:
  retail's `PEngine && !handle.IsNull()` test jumps to 0xAF5256 (the ++group), so particle->ClearParentHandle(group) sits INSIDE
  that if, after the inner GetPrimID loop. Moving it there gave RELOCATION_MATCH. Lesson: when a register residue survives every
  spelling, check every jcc TARGET by hand (the differ hunks ignore them).
- Headers/flags: overlay hdr/engine/CParticle.h opt-in FABLE_RETAIL_CPARTICLE_CLEARPARTICLE (accessors, ClearParentHandle FABLE_FORCEINLINE);
  default flags. QFE-4035 compiler gives the same bytes. Source: final.cpp (= differ/00af5090.cpp), test: test.cpp (vt PASS).
- Possible follow-up: the landed twin ClearDecal 0x00AF5430 might have the same misplaced ClearParentHandle (check its retail jcc targets).

## 0x00EB2140


## 0x00EB2140 CMurderTwistSheriffGuard::Main -- RELOCATION_MATCH (5 variants graded, v8)
- Matched by porting onto engine/CV_MurderTwistFinalScript.h + TRetVoid/CIsThingAlive/CGameScriptInterfaceBase (Assassin Main 0x00EB3F40 idiom), fixing callee names (IsValid, DoRemindingHeroToKillAssassin 0x00EB2900, DoAskingHeroToSpeakToBossAgain 0x00EB2DC0, DoCongratulatingHero 0x00EB3280, AttackHero 0x00EB3BA0) and adding the reconstructed dead `bool dead=false; if (dead && false && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {}` (Inferred, GOTCHAS 230) right BEFORE the first SheriffState loop. At the top of the function or right before the Guards registration it produced the dead store but swapped ecx/edx in the Guards[ID] address computation.
- Opt-ins: FABLE_RETAIL_NSCRIPT, _CLEVELSCRIPTBASE, _BASE_OBJECT, _CCHARSTRING, _DEF_CLASS_BASE, _CDEFSTRINGTABLE, _CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN (all existing repo opt-ins; no overlay header). Flags: none. AssassinState compared with (EAssassinState)4 (header has no ASSASSIN_STATE_4).
- Old draft names proven wrong (vs landed files/Ego_r): IsScripting -> IsValid (0x00CD23B9); DoStateHandler2/3, DoFightHero placeholders.

## 0x009F3B00

## 0x009F3B00 CSurface::CopyFromMemory (4-arg) -- RELOCATION_MATCH (13 grades)
- Matched by: porting onto shared headers; default boxes as `C2DBoxI::Initialise(0,0,...)` (keeps both inline GetDimensions descs of
  the both-null arm alive in one full-expression -> frame 0x9C); same-TU real Lock body (0x009F33E0, from the landed row) under
  `#pragma auto_inline(off)`; bare `GetPixelFormat();` is NOT what matched -- `CPixelFormat format = GetPixelFormat();` named local;
  indexed row loop `memcpy(to + y*Pitch, from + y*sourcePitch, rowBytes)` (pointer-stepping form loses).
- Headers: overlay engine/CSurface.h new opt-in FABLE_RETAIL_CSURFACE_LOCK (Lock decl + inline Unlock + out-of-line GetByteLength
  decl); macros CPIXELFORMAT, C2DBOXI(+_CTORS), CSURFACE, _COPYFROMMEMORY, _COPYFROMMEMORY_BODY, _BITMAP, _DIMENSIONS_INLINE, _LOCK.
  No flags.
- Name fix candidate: landed 0x009F2FC0 leaf `?GetByteLength@CSurface_Methods@@QBEKXZ` should be `?GetByteLength@CSurface@@QBEKXZ`
  (Ego_r.pdb 0x00B46CA0 + FableWin.pdb).

## 0x005C71AA


## 0x005C71AA CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight -- RELOCATION_MATCH (strict headers too)
- What made it match: the 0x005C6B26 sibling's TU header and parts (plain `for` copy loop, `const std::vector<long>& augmentations`
  ref, "none" text in BOTH elses, else-branch `if (GetNumberOfItemsOfTypeInInventory(idx) == 0) return;`) PLUS
  `#define FABLE_RETAIL_STLP_VECTOR` (STLport's _M_fill_insert body visible). With resize's whole chain visible VC7.1 knows the
  local CPerItemData vector never escapes: it hoists data.size() out of the copy loop into the dead fromDef slot (retail
  0x005C727C) and the early return jumps past the vector free (retail 0x005C72C3 `je 0x5C740F`).
- Hidden residue warning: fg.py shows a jump-displacement-only difference as "DIFFER(680v680) hunks=0"; every earlier draft (old
  draft included) also had the early-return `je` landing on the `cmp [ebp-38h],ebx` free test (q.py full listing shows it).
- Dead ends (all ~120 grades in ATTEMPTS.log): do-while / count-scope / loop-shape spellings; none-flag join form (fixes the copy
  loop by chance but gives the wrong NONE push/lea schedule); outer `used = 0` (xor edx too early); iterator counting loop (fixes
  copy loop, not the early-out); bank/itemData/early-return spellings; /Oa, optimize("t").
- Headers/flags: no overlay; optimize("s",on); the 0x005C6B26 define set minus the CTCWEAPON/STOCKITEM/SHOP ones, plus STLP_VECTOR.
- Names (NAME_FIXES.tsv): 0x005BE8D3 is landed as CTCInventoryBase_GetBoughtPrice_005be8d3.cpp ('Leaf: GetBoughtPrice@CTCInventoryBase'),
  but Ego_r 0x00621169 and FableWin.pdb have ?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z (this row and
  landed siblings 0x005C6B26/0x005BE8F3/0x005C90FC use that name). Also flagged: the address-suffixed landed placeholders 0x0042B312
  'GetDef_0042b312' (= ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z)
  and 0x004428F0 'Resize_CRetailVector_004428f0' (= std::vector<long>::resize).
- Template callees in the row: GetSubDef<CObjectAugmentationsDef> 0x0042AF9E, GetSubDef<CWeaponDef> 0x0042B1A6 and GetDef<CUIMiscThingsDef>
  0x0042B312 are header member templates that retail instantiated as COMDATs in this TU (called out of line). The source comment
  names the landed rows whose code it repeats (00/42/CDefClassBase_GetSubDef*_0042af9e/_0042b1a6.cpp, CDefinitionManager_GetDef0042b312_0042b312.cpp).
- Repair (verifier findings 1+2): source header comment only + NAME_FIXES.tsv; still RELOCATION_MATCH (strict too), test PASS.

## 0x009735D0


## 0x009735D0 CAIStateGroup_JackOfBladesBase::RushToPos -- RELOCATION_MATCH (strict headers)
- What made it match: (1) port onto shared headers (CAIStateGroupBase/CAIBrain GetCreature re-read per use, CThing GETTC_EXPANDED
  `GetTC(TCI_SOLID_TO_THING,&solid)`, PTargetThing.Get(), CCountedPointer default inline ctor/dtor, AIAction_PerformActionAndWait
  const ref) fixed the frame; (2) both string args as implicit `"..."` -> CCharString temps (an explicit `CCharString("...")` temp
  makes VC7.1 load the creature before constructing the string); (3) the expanded Normalise sums `(Z*Z)+(Y*Y)+(X*X)` with each
  square parenthesised: VC7.1 then spills the three squares first and re-sums them for GetMagnitude's fsqrt instead of sharing the
  whole sum (unparenthesised / pair-parenthesised forms either merge with GetMagnitude's canonical (Y+Z)+X or reorder products).
- Headers (overlay $W/hdr, see hdr/README.md): C3DVector.h FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX; CAIStateGroup_JackOfBladesBase.h
  FABLE_RETAIL_JACKOFBLADESBASE_RUSHTOPOS (FABLE_RETAIL_JACK_STAGE1_UPDATE alone does not compile CCombatActionBase.h's Jack branch in
  this include set); CTCSolidToThing.h FABLE_RETAIL_CTCSOLIDTOTHING_REMOVETHINGNOTSOLIDTO; CCreatureAction_CastAssassinRush.h
  FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH (retail class over CCombatActionBase, #pragma pack(1), Dir +0x115, sizeof 0x138). Flags: none.
- Repair (verifier findings, both applied): CastAssassinRush overlay now pack(1) with Dir +0x115 and pad +0x121 (ctor 0x008CE1B0
  [esi+0x115/0x119/0x11D], copy ctor 0x008CEF50), offsets static-asserted; hdr/README.md explains the re-headed C3DVector #if/#elif
  chain. Regraded RELOCATION_MATCH (also strict headers); test PASS.
- Names: all from the dossier (two-source PDB); no made-up names. Draft's `GetPosition`/`CountedDestroy`/long return were wrong
  spellings in the old draft only (not landed).

## 0x00803D60

## 0x00803D60 CTCTargetingPlayer::UpdateProjectileTargeting -- RELOCATION_MATCH (17 grades)
- Matched by porting onto the sibling UpdateTargetList 0x00804020 prelude (same TU, /d2inlT68) with the dossier's real callees: GetWorld().GetGameCamera()->GetCameraManager(&pcam_man), GetCamera().PeekWorldPos()/GetFOV(), GetCameraMode()->GetLookAtPhase().Position, PeekThingSearchTools().GetAllThingsInRoughArea<const CThing>(C2DBoxI(eye.ToC2DCoordI(),(long)range), GFFilter_Bind5(fov_filter, IsNot, not1(In2DRange), not1(ShotFiredByThing), Viable), things), IsLineOfSight(things,...,TEST_ALL,false).
- Keys: named `float fov` (spill slot); `C3DVector offset=pos-eye` (3D, Z dead -- a C2DVector local gave the wrong slot layout); range = GFFastSqrt(C2DVector(offset).GetSquaredMagnitude()) with C2DVECTOR_SQUAREDMAGNITUDE_YX + C2DVECTOR_FROM3D (retail squares Y first; GetMagnitude2D squares X first); IS_THING_ALIVE(hit); a named CIntelligentPointer<CThing> local for the new target (retail re-takes its address for operator=).
- Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CGAMECAMERAMANAGER_GETCAMERA, FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKATPHASE_INLINE, FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_PROJECTILE. Flags: /d2inlT68.
- Made-up names: none (anon filters ShotFiredByThing{CThing* Thing} / ViableProjectileAimingTarget from Ego_r types + FableWin ctors). Dossier NAME_FIX lead (0x00809210 likely ShotFiredByThing::operator(), not NoiseCameFromThing) unverified here.

## 0x006D8F10 (not landed)

## 0x006D8F10 CHeroExperienceDef::Transfer -- DIFFER(693v692), wf16 author pass 1 (~90 grades/probes)
Ported onto shared headers (overlay opt-ins in hdr/README.md); PDB-true one-arg TransferVector/TransferMap/Transfer<float> (release
macros, CheckCRC("")) give the same bytes as the old draft. Residue is ONLY the ebp save placement: retail shrink-wraps `push ebp`
to 0x6D9064 / `pop ebp` to 0x6D91A3; ours saves ebp in the prologue (pop in both duplicated epilogues).
Tried (no change unless noted): pragma optimize s/y/t, flags /EHsc /Ob1 /G5 /G6 /G7 /Ox /Oa /GS /Ow /arch:SSE, QFE-4035 c2 (no wrap);
stats ref/pointer local (at first use, at top, in a block), block scopes, do{}while(0), for-once, forceinline helper over 8..13;
visible noinline GFSerialise map/vector bodies (stub probe); TransferVector/TransferMap/Transfer<T> as if/else, case order, explicit
default, `return` arms, unused traits locals; maps through Transfer<T> + CPersistTraits<map> partial spec; named 2-arg forms with "";
CPersistTraits<float> as explicit class spec (with/without DefaultVal, const float& out); traits temporary (value-init stores 0 -- wrong);
trailing return / empty for / opaque call; inline-defined Transfer; neighbour functions before/after; param rename; macro-expanded
bodies (lose the lea-before-CheckCRC: retail really uses inline functions).
Probe findings (VC7.1 RTM does shrink-wrap straight-line regions here): with our one-arg bodies the ebp push is sunk only when the
stat region is short (<=3-4 transfers) AND at least two transfers follow it (e.g. [prefix | S Inc S | Lock V] wraps, [.. | S Inc S | V]
never does; float transfers in the prefix help, a 4-map prefix or a CSE'd repeated last vector blocks it). Retail wraps a 6-transfer
region followed by ONE transfer, so retail's IR around the end/region must differ from ours at a level not yet found.
Best next idea: find a landed Def::Transfer with a mid-body shrink-wrapped push (grep catalog bytes for `55` + `68 0E D7 22 01` gave only
0x007846B0, not a Transfer) and copy its idiom; or reconstruct the real GFSerialiseMapBinaryIn/Out + VectorBinary bodies of this TU
(0x6DAE40..0x6DD580, landed) in the row file with #pragma auto_inline(off) (real bodies, not the stub probe); or test whether the
float transfers come from a different inline path (Ego_r also names ?TransferBinaryIn@?$CPersistTraits@M@@QAEMAAVCMemoryDataInputStream@@@Z).
Names: landed 0x006DD1C0 leaf "CHeroExperienceDef::ReadFromStream" and 0x006DC450 "WriteToStream" are wrong (dossier: GFSerialiseMapBinaryIn/Out<CHeroStatIncreaseData,EDamageAttribute,less<EDamageAttribute>>, Ego_r.pdb).

## 0x006D8F10 -- attempt 2 (wf16 author pass 2, ~30 grades/probes): still DIFFER(693v692), same residue (ebp save placement only)
New ideas tried, none moved the ebp save (all logged in ATTEMPTS.log; the overlay is back to its pass-1 state):
- flags /Op (worse: float save arm changes), /Zi /Ob2 /Oi /GX /GX- /EHa /Gy /GR- /Zp4 /Og /Ot /Gm (no change), /Oy- /O1 (worse);
- TransferVector body spelled with stream reference locals (685) or a 3-way switch with return arms (697);
- visible float CPersistTraits<float>::TransferBinaryIn body (inline noinline / auto_inline off; probe body via ReadFloat): no change;
- three-argument TransferMap(name, map, default): V() temporaries add code (931/1043); a null-reference default gives the same bytes;
- V transferred through a two-layer Transfer(name, vector&) -> TransferVector, or its own body, for V only or every vector: no change;
- empty user dtor on CPersistTraits (EH-edge idea): no change without EH; with /EHsc or /GX it puts Transfer<float> out of line (609).
Probe map of OUR compiler's shrink-wrap (q.py outputs w51..wa3.out): it never sinks the push when only ONE transfer follows the second
stat transfer, whatever that transfer is (V, M, F; any region length 2..6), and never for a 6-transfer region even with two transfers
after it ([S Inc Lock Lost Run S | Lock V], [.. | V V'], [.. | F V] all keep the prologue push). It does sink it for regions of 3..4
with two transfers after ([S Inc S | Lock V], [S Inc S | V Lock], [S Inc S | V V'], [S Inc Lock S | Lock V]). Prefix content
(no prefix, 4 floats, 10 floats) does not change that. Retail: region 6, one transfer after. So retail's IR differs in the
region AND/OR the tail at a level that is not inline-layering, flags, compiler build, EH, callee visibility or default arguments.
Only other retail `push ebp ; push ""` site in .text is 0x00784820 (an argument push in CTCCreatureGroupBoastingCrowd, not a save).
Best next idea: find in retail any other function (not only Transfer rows) whose callee-saved push sits mid-body with a long region
and a one-statement tail, and that is already landed (its source shows which spelling VC7.1 RTM wraps); or test whether the tail is a
real statement that compiled to nothing late (e.g. a dead store to a member/local removed after register allocation).

## 0x006D8F10 -- attempt 3 (wf16 author pass 3, ~45 grades/probes): still DIFFER(693v692), same residue (ebp save placement only)
Best source unchanged: differ/006d8f10.cpp (overlay unchanged). New ideas tried, none moved the ebp save:
- flags /GA /Gr /Gz /vd0 /J /QIfist (no change), /Za (src fail), /Os (178 bytes);
- last vector transfer written out at function level with return arms (g1/g2) or a reference local (g3): 689, push stays in prologue;
- visible SIDE-EFFECTING GFSerialise map/vector bodies as `template __declspec(noinline)` (j1..j3): no change
  (`#pragma auto_inline(off)` does NOT keep template instantiations out of line -- they inline; use __declspec(noinline) for probes);
- inline helper around the four combo maps (h1..h3): not inlined by /O2 (499/454);
- macro bodies with a reference local per transfer (n2): same residue -- so the inline-parameter temps are not the cause;
- body spellings grid (grid/: vector switch|if, map switch|switch+default|if-chain, float same): switch forms all give the same
  693 residue, if-forms change code; extra empty cases (MODE_CLEAR / text modes) and `case MODE_SAVE_BINARY: default:` -> no change;
- Transfer<float>: traits declared before CheckCRC / per-case block -> no change; traits temporaries -> 706;
- last transfer swapped to a lower-offset member (k1..k3: FallbackMultipliers / map / float): push stays in prologue.
Probe facts that correct attempt 2's map: our compiler DOES sink the ebp push with ONE transfer after the range when the ebp temp is a
different value: r1 (region interior = 4 floats; ebp = float +0x64 address, wrapped 0x6D8FFA..0x6D9145) and r2 (interior = 4 vectors;
ebp = FallbackMultipliers address). r3 ([S Inc S | V]) does not. So it is not "one transfer after never" -- it depends on which temp gets
ebp and what else is held in ebx across its range (in retail the 4 combo-map temps are held in ebx inside the ebp range).
Retail scan (capstone over manifest function starts, $W/sw_scan.txt): mid-body `push ebp` is common around LOOPS (e.g. landed
0x0082A2A0 RemoveAllDeadMarkers), but in straight-line Transfer code only 0x6D8F10 has it.
Best next idea: the remaining unexplained lever is c2's allocation order. Try (a) making the combo-map temps rematerialisable vs held
(e.g. combo maps transferred via a form whose arms recompute &member) to see whether ebx usage inside the range flips the decision, then
search for a spelling that keeps retail's bytes; (b) compile retail-neighbour TU content (the real GFSerialise* COMDAT bodies 0x6DAE40..
0x6DD580 with their real STLport map code, __declspec(noinline)) before Transfer -- c2 per-TU state is the last untested level.

