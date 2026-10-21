# Hand chunk 246 (CGraphicInfoFrame .. CMainGameComponent): 27 of 40 byte-exact

All 27 grade RELOCATION_MATCH under the RTM 13.10.3077 lane compiler (two need `/GS`); every test passes under g++ and the lane's VC7.1.

## What matched (27)
- CGraphicInfoFrame::GetVolumeTexture 009ff710 -- load/upload the frame, LRU-touch the volume and frame data once per frame stamp, return the texture. Shape: `__forceinline` CResource::Touch, the texture read through a full-expression counted-pointer copy (+1/-1), success path nested so the pushes stay at entry and both failures share one exit.
- CGuildSealTrigger::Main efbe20, CGuildSealWarning::Main e4b010, CHTG_AmbushPointCond::Main d73970, CHTG_AmbushPointRear::Main d73ae0, CM_Guard::Main da63f0, CM_BodyGuardRespawn::Main e5e0c0, CM_BanditLeader::Main e4a740, CM_TauntingBandit::Main e4b430 -- entity scripts. Shapes: rotated `while (!IsDone())` waits; string literals as implicit CCharString conversions; CScriptThing virtuals (GetPos, SetDataString, IsAlive) defined inline in the class so VC devirtualises them on by-value locals; BodyGuardRespawn is a 4-case jump table with default-constructed strings assigned per case; BanditLeader's `~CScriptThing` is inline at the first temporary only (inline `virtual ~CScriptThing() { }` over an out-of-line ~CBase) and the spawned leader is scoped in the `if` to share the temporary's frame words; TauntingBandit's local vector destructor is inline at the end and out of line at the IsDone exit with a virtual-dtor `_Destroy` walk.
- CHandleBasedMemoryPool::BeginShutdown a02220, Defragment a02730, Allocate a03180 -- local model with the critical section at retail +0x44C. Shapes: inline Begin/EndCritical; a scoped {pool, locked} lock whose dtor is inline except at the merged failure exit; handler waits as `while (largest < size) { if (!a) break; if (!b) break; }`; `(unsigned char)~(flags >> 1) & 1`; timeout `!(GFGetTime() <= start + timeout)`; GFFreeFromGlobalAllocator `__forceinline`; free lists cleared with memset after the pointer stores.
- NScript::CHeroBoastsScript::Main ce1af0 / BoastUseNoHealthPotions ce4600 -- the CSpawnedFunc2 thread object with a conditional by-value CCharString argument (flag bit), the boast vector scoped in its own block (dtor before the frame call); the health watch is a rotated loop with `health > lastHealth`.
- CInputProcess*::ProcessInput -- BoastUI 690e70, Fishing 690b20 (/GS), InventoryStatsScreen 689c40, OracleMinigame 691680 (/GS), ProjectileTargetingAnalogueZoom 68f320, QuickAccessMenu 690fd0, TavernGame 690830. Shapes: `Send(x)` arms cross-jumped into one virtual-call tail; `T* p = 0; if (HasTC) p = GetTC;` preloads; GS game-event payloads in the dead `event` slot; `y > 0.6f` / `y < -0.6f` compares; a `__forceinline` GetTC with preload at each of three zoom calls.
- NUISystem::CKeyRedefiner::Redefine 557d20 -- `CActionInputControl& current` reference (address hoisted to ebx), switch cases written `if (differs) break; return;`, C2DVector user copy ctor (memberwise copy-out vs `rep movsd` assignment), `long index = 0` placed between the copy and the assignment (frame word), the parent list named before the notify call.
- CLandscapeBackgroundPatch::RelocateData be65a0 -- derived shell over engine/CLandscapeBackgroundPatch.h with the landed RelocatePointer helper.
- CLandscapeBackgroundTreeNode ctor bf6e20 -- local layout 8 bytes below the PDB; novtable base zeroing +4/+8, LOD records and sphere by member ctors, body assignments in retail order, `Set(C3DVector(0,0,0), 0)` keeping the dead temporary stores.
- CLandscapeTesselatorBase::BuildBridgeVertexAndIndexBuffers be3660 -- locked pointers as named locals before memcpy (right-to-left argument evaluation), managers fetched before the pushes.
- CLinearSpline::GetPointWithParam a57f70 -- C3DVector user copy ctor, travelled reset before the first copy, `delta * (target - t) * (1.0f / length)`.
- NUISystem::CList::RestoreState 536c64 -- size TU (`optimize("s", on)`), one polymorphic component view.

## Parked for Fable 5.1 (13)
- 00e5f6b0 CHenchmanRegionEntranceComment::Main DIFFER(335v335): HenchmanVector[i] built start+off in edx (retail off into edx, start into eax) and Me's vtable in eax for GetDataString (retail edx) | tried: early-exit + forceinline walk helper (fixed the pushes), polymorphic CBaseClass, subscript forms, named element reference | next: same-TU stand-in for MakeHenchmanComment/GetDataString or advisor
- 00f36a30 CHeteroTerrainFractal::GetFractalHeight DIFFER(361v361): octave loop schedules the counter load after the first fmul | tried: octaves local, separate increment statement (VC otherwise refactors to value*(n*w+1)), split multiply | next: lane gap: scheduling
- 00868ce0 CHitParameters::SetWeapon DIFFER(361v360): frame 0x1C v 0x18, the intelligent-pointer temporary keeps its own 8 bytes instead of overlaying the GetTC key / def out words | tried: nested if, same-TU LowerBound/GetDef/GetSubDef stand-ins (388->361), component local, block-scoped assignment | next: stand-in for the augmentations getter 0x766920 that reads the vector
- 00541fd0 NUISystem::CIconText::GetDataFromDef DIFFER(350v366): the lookup temporary's destructor zero stores are dead-store-eliminated | tried: user copy ctor, named/const& icon, single full expression, inline bank helpers | next: advisor (asked)
- 00a60050 CInputManagerDX::CInputManagerDX DIFFER(333v333): count-block delete operand reloaded into eax (retail ecx) | tried: operator delete call, empty count-block dtor, zeroing dtor | next: advisor (asked)
- 00688d40 CInputProcessZTarget::AddToInterface DIFFER(362v360): this/def registers swap once the PTarget temporary's ctor/op= are forced inline (out of line otherwise, 332v360) | tried: forceinline ctor/op=, template IP, plain-inline map helpers, LowerBound stand-in, Assign helper | next: lane gap: inline budget/register choice
- 00be7be0 CLandscapeBackgroundPatch::Render DIFFER(361v391): sub-patch pointer in esi (retail edx with constant 4 hoisted into esi), 3/4/5 arm laid out second | tried: branch inversion (fixed), switch, by-value push_back, same-TU push_back stand-ins (fixed the home slot), negated test | next: lane gap: register choice
- 00bf6f80 CLandscapeBackgroundTreeNode::BuildDefaultSection DIFFER(357v375): retail loads the patch ctor's float args first and spills them to dead arg slots; box tail scheduling | tried: store order (fixed prologue), member-built corners | next: same-TU stand-in for the patch ctor/operator new
- 0053822b NUISystem::CList::GetDataFromDef DIFFER(377v377): child-count load scheduled before the alpha fst/fstp | tried: named/block/const& child, unsigned index, same-TU find/push_back stand-ins (fixed the frame 381->377), early index | next: lane gap: scheduling
- 00bfa920 CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer DIFFER(334v346): retail ebp=0 and ebx=0xFF (colour bytes + pushed alpha), mip bias spilled; ours ebx=0, immediate 0xFF | tried: body-assigned colour (fixed byte order), `==` compare (fixed jp), named mip bias | next: same-TU AddStaticMesh stand-in
- 00bf9fa0 CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc DIFFER(325v325): esi/edi swap in the descriptor argument loads | tried: named fade-in local | next: lane gap: register swap
- 00df48f0 CM_Experience::Main DIFFER(348v348): Me pointer in ebx (retail edi) | tried: vector-addition orb position (fixed the x87 part) | next: lane gap: register swap
- 00416e78 CMainGameComponent::GetInputs DIFFER(393v393): frame 0x4C v 0x78, 0x2C bytes of unexplained retail local space; code identical | tried: non-POD C3DVector | next: find the missing local (an unused 0x2C-byte buffer?) in the PDB locals of GetInputs

## Skipped
None.

## Label corrections
- 0x00687B30 is CInputProcessGameBase::IsEventButtonReleased and 0x00687BD0 IsEventButtonHeld (manifest names both "IsEventButtonHeld"); 0x00687A90 IsEventButtonPressed.
- 0x00687D00 is CInputProcessGameBase::GetEventGameActionVector (manifest "_Cons_val<...>").
- 0x00789A10 is the projectile zoom component's AdjustZoom(float) (manifest "ZoomControl_Update").
- 0x007B0CC0 is a CTCFishing button handler (`Xxx(0)` forwarder; manifest "_Dest_val<..CFlash>").
- 0x007E5F70 CTCOracleMinigame::PushSymbolToQueue(long).
- 0x00663340 CThingPlayerCreature::GetHeroCombatDef (manifest "CEventKilledCreature"); 0x00487DD0 CPlayer::GetControlledCreature.
- 0x00F35B30 IsDone/IsActiveThreadTerminating (manifest "CCreatureAction_TrollWhackGroundBase::Initialise"); 0x004AA840 ~CScriptThing (manifest "~C3DClothPrimitive"); 0x00EF7AB0 is the CIsThingConscious filter destructor (vtable 0x012C2FE8).
- 0x00E5E240 CV_BodyGuardScript's index-from-code helper (manifest "GetAlignmentFromCode", global).
- 0x009E2C80 the graphic bank lookup (manifest "CMessageEventFilter_CanBeSeenOrHeard::operator()").
- 0x00A5FC30 CCountedPointer<CJoystick>(CJoystick*) (manifest names CFileInstaller::CFileInfo, a folded twin).
- 0x0053C8F8 std::find over unsigned longs (manifest "_Umove<EWeaponClass*>"); 0x00486024 vector<CComponent*>::push_back; 0x0053CB4F vector<bool>::push_back.
- 0x0074F4B0 is an STLport _STLP_alloc_proxy constructor (manifest "__EH_epilog3"); 0x004E34B2 vector<pair<long,CCharString>> destructor (manifest "CActiveFile::OnReadFinished").
- 0x00766920 the object-augmentations list getter (manifest "CParticleMorphs_InitFromDef"); 0x0042AF3C / 0x0042B248 GetDef / GetSubDef instantiations named after other def types.
- 0x00A57EF0 CLinearSpline::GetPathLength (manifest "CPSCSplineBase_GetPathLength", global).
