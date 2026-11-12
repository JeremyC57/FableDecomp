## 0x008DA410

## 0x008DA410 NCreatureMode::CModeScreamerAggressive::OnActivate -- RELOCATION_MATCH (6 grades)
- Matched by porting onto shared headers (Drained 0x008DA7A0 macro set + CTCCombat/CDefinitionManager/CSpecialEffectsDef GETEFFECT_BODY/GetSubDef NOINLINE opt-ins), `CTCCombat* pcombat;` declared uninitialised right before its GetTC (an `=0` at the top added a frame slot), and implicit literal->const CCharString& conversion for both names (explicit `CCharString("...")` temps pushed the other args before constructing the string).
- Headers: overlay hdr/ (new NCreatureMode_CModeScreamerAggressive.h; opt-ins CMODESTANDBASE_ACTIVATE, CTCDPARTICLEEMITTER_CREATE, CTCCOMBAT_READYFORMELEE; see hdr/README.md). Flags: none.
- Old draft names proven wrong: CreateEffectThing -> CTCDParticleEmitter::Create (Ego_r), GetPosition -> CThing::GetPos, StandActivate -> CModeStandBase::OnActivate; literals "AGGRESSIVE"/"HIT_LOCATION_CYLINDER_01". The landed sibling 0x008DA690 still uses local models (ThingNative etc.); not touched.

## 0x0079CC80

## 0x0079CC80 CTCBulletTime::PlayEffect -- RELOCATION_MATCH (wf11, 2026-11-12, 20 grades)
- What made it match: port onto shared headers (GetSubDef<T> template body in the row as 0x007941D0, GETEFFECT_BODY, GETTC_EXPANDED,
  ThingAlive.h + CINTELLIGENTPOINTER_COMPARE_NULL_INT), then the real structure: `if (effect == 0)` is the OUTER test and each arm calls
  `PDef->GetSubDef(&effects) && effects->GetEffectFromString(...)` itself -- VC7.1 hoists the identical GetSubDef call above the branch,
  which is where the "zero-register gap" (xor ebx,ebx / cmp esi,ebx / push ebx) came from. Arm 2 passes CreateParticleEffect straight
  into the CIntelligentPointer temporary (a named `thing` local moves the temp onto the index slot).
- Headers/flags: overlay engine/CTCBulletTime.h opt-in FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT (typed PDef + PlayEffect decl); no flags.
- Lesson: a call that appears BEFORE a branch yet whose result is re-tested in both arms may be the same call written in both arms, hoisted.
- Repair (verifier): added an in-row comment attributing the GetSubDef<T> template body to landed row 0x007941D0 (CTCUnholyPower_CTCUnholyPower_007941d0.cpp lines 86-95, same text as the 0x00669580 specialization) and the retail fact that the body was visible but called out of line; still RELOCATION_MATCH (also strict headers), test PASS.

## 0x00AABD50

## 0x00AABD50 CSoundEmitterLH::PlaySoundEvent -- RELOCATION_MATCH (strict headers), 16 grades
- Made it match: port onto shared headers (CASoundEmitter/CASoundManager ENABLEFLAGS+SIZE40/CSoundInit/LHA_CEventPlayOptions/CCountedPointer/
  FABLE_RETAIL_CCHARSTRING GetAsCharArray/STLP list push_back noinline), then three shape facts: head-relative Pos copied through a named
  `const C3DVector& head = PManager->m_vector` componentwise; `options.EmitterId = (init.OverrideID == -1) ? EmitterID : init.OverrideID`;
  bank handle via a small inline accessor `CSoundBankLH::GetEngineBank()` (made-up, descriptive: no audio PDB; without it edi/ebx swap).
- Header overlay: hdr/engine/LHA_CEventPlayOptions.h opt-in FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_SENDFADERS (SetFlag16/SetFlag decls
  0x00C0A8A0/0x00C0A8C0, landed under CBitMaskPair_00c0a8c0 -- these are really LHA::CEventPlayOptions members by offsets; names descriptive).
- Made-up names: CSoundBankLH::GetEngineBank, CSoundEventManager::PlayEvent (slot 16), CSampleQueryLH (landed 0x00A0BA40 spelling).
- Wrong landed spelling noticed: CActiveSound ctor 0x00AAB8B0 landed as (void*,void*,const char*,void*,bool,bool); real args are
  (long handle, CASoundBank&, const CCharString& name, float time, bool is3D, bool headRelative) (this caller's pushes + copy ctor 0x0099EC30).
- No flags/pragmas.

### Repair pass (verifier findings, 2026-11-12) -- still RELOCATION_MATCH (strict headers)
- One address, one name: NAME_FIXES.tsv hands back 0x00C0A8A0 / 0x00C0A8C0 -> LHA::CEventPlayOptions::SetFlag16 / SetFlag
  (class proven by Ego_r struct_layouts_egor offsets +0x2a/+0x4c and +0x2c/+0x4e = WhichGain/PitchSendFaders and
  Params.Gain/PitchSendFaders, and by both callers using the 0x00C0A790 ??0CEventPlayOptions@LHA@@ stack object); the method
  names stay the landed descriptive ones. The coordinator respells CBitMaskPair00c0a8c0_SetFlag16_00c0a8a0.cpp / _SetFlag_00c0a8c0.cpp.
- Made-up names (complete list): CSoundBankLH::GetEngineBank, CSoundEventManager::PlayEvent (slot 16), CSoundSystem::GetEventManager
  (slot 6, 0x00C0A590, landed body gapscan_sub00c0a590; same descriptive name as landed PlayAtmos 0x00AAC1D0), CSampleQueryLH
  (landed 0x00A0BA40 spelling), LHA::CEventPlayOptions::SetFlag16 / SetFlag (method names). Each is marked (descriptive) in the row.
- Test: now runs the pitch-fader branch as retail does it -- FindFirstSetBit hoisted on the exhausted gain mask (always -1),
  SetFlag ignoring index >= 8 (SetMaskBit 0x00C0AE70 `cmp ecx,count ; jae`), `1 << bit` as x86 shl (count mod 32), so with
  PitchFaderBits = 0x80000000 the loop runs once and sets nothing; any other non-zero pitch mask never terminates in retail
  (a retail bug: the loop xors the same hoisted bit 31), so the test cannot run those. No added guards.

## 0x009810B0


## 0x009810B0 CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage -- RELOCATION_MATCH (wf11 author, ~25 grades)
- What made it match: (1) port onto shared headers with correct identities (PeekPos, PeekThingSearchTools/PeekWorld, GetPBestLeafThing<CThingFilter_AICreatureHasProperty, CThingCompare_Nearest, CThingAICreature>, ShowFear(CThingAICreature&), Flee/PerformActionAndWait by const&); (2) the REAL body of the same-TU CCreatureAction_SendMessage ctor 0x00982210 as `__declspec(noinline)` -- with it visible c2 knows &message does not escape, so the by-value message handle is released in place (`dec [ecx+4] ; jne`) instead of the load/dec/store form (probe: an address-taken by-value CDefPointer param always gets load/dec/store; an `auto_inline(off)` pragma instead of noinline left ~SendMessage out of line); (3) `CIntelligentPointer<CThingAICreature> guardPointer; guardPointer.Set(guard);` (guard read after the base ctor); (4) `const CThingAICreature& creature = GetCreature(); CThingAICreature* guard = 0; const C3DVector& pos = creature.PeekPos();` (store scheduling of guard=0 and the compare copy).
- Headers: overlay opt-ins FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD (nested CAIFilterBase public, Flee + FindAGuard decls), FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF_DECL, FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE, FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR, FABLE_RETAIL_CCREATUREACTION_SHOWFEAR_DERIVED. No flags.
- Local classes (no engine header exists): CCreatureAction_SendMessage (members as landed 0x009822E0), CAIFilter_NoOtherYellForHelp (Ego_r: base CAIStateGroupBase::CAIFilterBase, World +4; ctor spelling `(const CWorld&)` and `const CWorld* World` are descriptive -- PDB has `CWorld* World`).
- Name notes: landed 0x00982210 row spells the ctor with stand-in types (CDefPointer<CDefBase>, SMessageData, CThing*) -- PDB ??0CCreatureAction_SendMessage@@QAE@AAVCThingCreatureBase@@ABV?$CDefPointer@$$CBVCMessageEventDef@@@@PAV1@ABVC3DVector@@J@Z; landed 0x008FCFA0 "GetPThingWorld" is PeekWorld (Ego_r ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ); caller 0x009378F0 declares this function with a wrong local signature. 0x006924B0 used as landed GetRandomAnimFromName (same mangled shape KIABVCAnimationEntry@@ABVCThingCreatureBase@@ABVCCharString@@@Z as Ego_r GetAnimFromName -- not re-proved).

## 0x00A44F40


## 0x00A44F40 C3DSkeletalMorphTarget::CombineMorph -- DIFFER(516v525), author wf11 (2026-11-12)
Best source: differ/00a44f40.cpp (ported onto fable_string.h / engine/C3DVector.h / engine/CArray.h with FABLE_RETAIL_CCHARSTRING,
FABLE_RETAIL_CARRAY, FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE; no overlay header needed; default flags). `i++` inside the insert
argument gives 14 hunks vs 20 for `++i;` after (same bytes count).
Residue (unchanged by ~55 grades): (1) miss arm: retail keeps the default ctor's dead `mov [morph+4],0` and emits CRC copy + the three
(1,1,1) temp->morph.Scaling dword copies strictly before the first `fld [ebx+8]`; ours drops the zero store and interleaves the source
float loads with those stores. (2) i=edi/&BoneMorphs=esi in retail, swapped in ours (prologue push order follows). (3) hit test: retail
`mov ecx,[ebx+4] ; ... cmp [eax+4],ecx`, ours `mov ecx,[eax+4] ; cmp ecx,[ebx+4]`.
Probes (facts): VC7.1 hoists `[ebx]` float loads above frame stores even when morph's address (whole, or &CRC/&Scaling fields) escaped to
an opaque call earlier, and even when source comes from an opaque call -> retail's strict order is NOT an aliasing effect of escape;
the zero store is removed by a local DSE in every spelling tried.
Tried (no change, 516 / 20 hunks unless noted): FABLE_RETAIL_CARRAY on/off; default CCharString stand-in; `source.CRC == elem.CRC`
(21 hunks); ctor body assignment; ctor defined after CombineMorph (inline) / non-inline in .cpp before/after (auto-inlined);
accessor spellings GetNameCRC/GetScaling/SetScaling (in class / defined after); CompareCRCs inline in the while; source as pointer /
no source ref (559); morph via pointer / reference (19 hunks); value-init `= C3DSkeletalBoneMorph()`; user copy ctor/op= decls;
named `delta` / named `one`; default ctor Scaling(1,1,1) (468, folds); placement copy (468); `Scaling += (src - Scaling)` (504);
`Scaling = Scaling + d` (540); SetScaling(GetScaling()+d) (540/600); SetScaling(one + d) (492, 14 hunks); i/t long / uint / decl
order / while form; ref local `morphs = BoneMorphs` (474); flags /Op (539) /Ow /Oa (510) /G5 /G6 /G7 (507) /EHsc (599);
pragma optimize s (415) / t off (415) / g off (897) / w on / p on (539); probe: C3DVector ops as plain inline; empty do-while(0).
Best next idea: find what puts a basic-block or memory barrier after `morph.CRC = 0` in retail -- a construct that leaves control flow
c2 only folds after scheduling/DSE (e.g. a null-tested pointer such as placement new / an inline returning `this`, or a CArray /
C3DSkeletalBoneMorph member that builds the inserted element in place), or a different compiler build (QFE 4035) for this TU.

## 0x00A44F40 C3DSkeletalMorphTarget::CombineMorph -- RELOCATION_MATCH, author wf11 attempt 2 (2026-11-12)
What made it match (source: final.cpp, also differ/00a44f40.cpp): (1) the miss arm re-binds the source element in its own scope
before the bone morph is constructed (`const C3DSkeletalBoneMorph& added = target.BoneMorphs[t];`). Probe fact: VC7.1's alias analysis
treats a pointer bound before a local's scope opens as unable to alias that local (even when its address is stored to a global), so
it hoisted the source float loads above morph's stores and dead-store-eliminated the ctor's `BoneNameCRC = 0`; binding the pointer
inside the scope keeps both (CSE still reuses ebx, no reload). (2) the source CRC is a named local `const unsigned long crc` at the top
of the loop body, used by the inner while and the hit test (not by the miss arm): this alone fixed the esi/edi swap and the hit-test
`cmp [eax+4],ecx` direction. (3) `++i;` as its own statement after the insert (with crc in place; `i++` inside the argument left 6 hunks).
Headers/flags: repo headers only (FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CARRAY, FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE), default
flags, no overlay (probe overlays parked in hdr_unused_probes/, not needed). Local names source/crc/added are descriptive (locals only).
Tried without effect this attempt: block-boundary tricks (do-while(0), if(true), switch, __assume), user dtor / full init list / copy
ctor, GetBoneMorphs/GetNameCRC accessors, insert body visible noinline / plain inline, /EHsc + nothrow callees, QFE 4035, /Ob1 /Ox,
volatile CRC (diagnostic), C3DVector user operator= (folds, 468), placement new, ctor assigning Scaling.

## 0x00D38D20

## 0x00D38D20 NScript::CQ_FireHeartScript::RunVillagerComments -- RELOCATION_MATCH (grades: 4 calls, 9 variants)
- Ported onto shared headers (StartRound's include set + FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE, CTimer inline defs over pDSTGame,
  GetSquaredDistanceBetweenThings 0x00CBE583 declared). That alone left only exit-tail layout (530v526): normal-exit cleanup placed last.
- What made it match: a local `TRetVoid _Fail_Val;` declared FIRST (before CTimer) and every `return;` spelled
  `return _Fail_Val.Return();` (the script macro form, as StartRound 0x00D39960). (Variant with it after the vector was graded in the same call; only the first-declared one was confirmed.)
- No header overlay, no flags. Old draft name GetDistanceBetweenThings was wrong -> GetSquaredDistanceBetweenThings (dossier evidence).

## 0x00704860

## 0x00704860 CGameScriptThingManager::CreateScriptThing -- RELOCATION_MATCH (repaired, still RELOCATION_MATCH incl. LANDVERIFY_STRICT_HEADERS=1)
- Made it match: port onto shared headers (CCountedPointer<CScriptThing>, CScriptThing under FABLE_RETAIL_NSCRIPT + IMPLICIT_COPY, CGameScriptThing(CWorld&, const CThing&), CMainGameComponent::Get()->GetWorld(), thing.GetUniqueID(), STLport list<CScriptThingInfo>); named `CScriptThingInfo info` (default ctor, uid assigned after Reset -> its +20h slot never stored); inline protected FindScriptThing; found/not-found as `if (...) {...} else {...}` so both CScriptThing locals share slot +14h (plain `if {return} ...` put the found one at +20h; `if(!Find)` first = 528v531). Reset (0x007045F0) and SetImplementation (0x007046D0) are plain out-of-line calls through the shared headers' declarations.
- Headers: overlay hdr/engine/CGameScriptThing.h (FABLE_RETAIL_CGAMESCRIPTTHING_MANAGER_FRIEND) only. No flags.
- Repair (verifier findings): dropped the CCountedPointer.h overlay (FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_RESET), its #define and the row-local `inline __declspec(noinline)` SetImplementation body -- none was needed (graded RELOCATION_MATCH without them, plain and strict); the source comment no longer claims body-visibility steering. Old overlay kept only for reference in $W/removed_overlay/ (not part of the hand-back).
- Name fixes (NAME_FIXES.tsv): 0x007046D0 CMeshDataBank_007046d0::SetAnimationMemoryPool -> ?SetImplementation@CScriptThing@@...; 0x007045F0 CCountedPointer_007044b0::Reset -> ?Reset@?$CCountedPointer@VCScriptThing@@@@QAEXPAVCScriptThing@@@Z (FableWin + PDB member type PScriptThing + sole caller 0x0070498D); 0x008D0250 landed as public CGameScriptThing(CWorld*, CThing*) -> ??0CGameScriptThing@@AAE@AAVCWorld@@ABVCThing@@@Z (Ego_r 0x009072A0 + FableWin); caller 0x004C7E50 landed as `Ctor_CPredicate_And_NScript` -> ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ (Ego_r order + FableWin + reloc_bindings + labels_*). CGameScriptThingManager has no engine header; modelled locally (ScriptThings, NullThing, nested CScriptThingInfo, FindScriptThing).

## 0x00A5B370


## 0x00A5B370 CCachedFile::Read -- RELOCATION_MATCH (first author attempt, ~12 grades)
- Made it match: ported onto shared engine/CAFile.h (FABLE_RETAIL_CAFILE), engine/CArray.h, engine/CCountedPointer.h
  (FABLE_RETAIL_CCOUNTEDPOINTER), fable_string.h CCharString (FABLE_RETAIL_CCHARSTRING); named `message` locals passed to the
  by-value CBBBFileException(CCharString, EFileExceptionType) ctor (an rvalue temp argument elides the copy ctor);
  refill 1 spelled `-GetPosition() + GetLength()` (VC evaluates the right operand of `a - b` first, so retail's
  GetLength-first refill needs the negated form; refill 2 is the plain `GetLength() - GetPosition()`);
  loop-local copies `unsigned char* out = destination` (top of function: gives `mov eax ; test ; mov ebp,eax`) and
  `long bytesLeft = size` (just before the outer loop: re-reads size from its slot); window test on
  `(unsigned long)(DataBuffer.end() - DataBuffer.begin())` (size() swapped the cmp operands).
- Headers/flags: no overlay, no flags (default /O2). Local models: CBBBFileException, CCachedFile, Min (no shared header).
  Descriptive (made-up) names: enumerators NullParameter=1 (reused from landed CMemoryFile::Read 0x009E9220; repair) / ReadPastEnd=3, locals out/bytesLeft/remaining/bufferSize/message.
- Landed CDiskFileWin32::Read 0x0099A960 spells CBBBFileException(long, const CCharString&) with `Error` -- PDB says
  (CCharString by value, EFileExceptionType) with member `Type` (dossier sect. 3); not respelled here.

## 0x00E36750


## 0x00E36750 NScript::CV_ArcheryCompetitionScript::DoCountDown(long) -- RELOCATION_MATCH (grade ~13 fg calls)
- What made it match: the generated-script return idiom `TRetVoid _Fail_Val;` (first local, before CTimer) + every exit `return _Fail_Val.Return();`.
  Plain `return;` gives identical code but VC7.1 moves the shared post-wait/in-loop exit block and the inline-dtor block to the
  function end (541 v 536). No inline_depth pragmas, no OUTOFLINE_DTOR: the implicit handle dtor is expanded once natively.
- Headers: overlay engine/CV_ArcheryCompetitionScript.h with FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_DOCOUNTDOWN (declares DoCountDown);
  engine/TRetVoid.h; defines FABLE_RETAIL_CCHARSTRING/_NSCRIPT/_BASE_OBJECT/_CLEVELSCRIPTBASE/_DEF_CLASS_BASE; row-local inline CTimer bodies. No flags.
- Tried and rejected: inline_depth(0/1/2) on various returns (breaks CTimer inlining / no effect), do-while(0) wrappers, if(!T){...}
  nesting, else-branch, for(;;) / if+do-while loop forms, optimize("s")/("y",off).
- Param name `count` is descriptive (PDB lists carry no parameter name). Old draft names fixed: IsScripting->IsValid, Destroy->implicit dtor,
  GameCalls/g_pGameScriptInterface->CGameScriptInterfaceBase/pDSTGame, CTimer::SetTime/Index->operator=/TimerIndex.

## 0x00773EE0

## 0x00773EE0 CTCShop::GetSaleItemsToPlace -- RELOCATION_MATCH (grade 2 of 2; strict regrade RELOCATION_MATCH; test PASS)
- What made it match: porting the old local-model draft onto the shared headers (CThing::GetPBaseDef by value + IsNull, const
  GetSubDef<T> templates, CTCBase::GetDefinitionManager() const&, const GetPDefFromGlobalIndex returning CDefPointer<const CDefClassBase>,
  std::map const_iterator loop, std::vector<long>, std::random_shuffle). The old "this spilled / epilogues merged" residue was a model error.
- Opt-ins (all existing repo opt-ins, no overlay headers needed): BASE_OBJECT, CTCBASE, DEF_CLASS_BASE, CTHING, CCHARSTRING, CDEFSTRINGTABLE,
  STLP_LIST, CDEFINITIONMANAGER, CDEFCLASSBASE_GETSUBDEF_NOINLINE, CGAMEDEFINITIONMANAGER, CRESOURCELIST, CRESOURCE, CTCSHOP, CSTOCKITEMDEF,
  STLP_RANDOM_SHUFFLE_2ARG, STLP_VECTOR_COPY_OUTOFLINE, STLP_VECTOR_PUSHBACK_NOINLINE. No flags.
- Name fix candidate: landed 0x00777080 leaf random_shuffle<CUnprovenElement_00777080**> -- this row calls it on vector<long>, i.e. std::random_shuffle<long*>.

