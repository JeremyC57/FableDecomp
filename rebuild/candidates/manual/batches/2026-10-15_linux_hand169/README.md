# Hand chunk 169 (CTCQuestCard .. CTCSmashableBarrier): 25 of 40 byte-exact

## What matched (25)
- CTCQuestCard 007026a0 ctor: GetSubDef writes straight into the PDef member; rewards read back through PDef->.
- CTCQuickAccessMenu 00625950 DrawPreparePrimitivesForRendering (slot 11): CTCInventoryWeapons 0x5C2FE1 shape; the draw is the `if (!FirstRenderFrame)` arm.
- CTCRegionFollower 006aecf0 CullNullFollowers: per-node CIntelligentPointer copy (inline copy ctor) + STLport inline erase (virtual ~T via vtable, free).
- CTCRotate 0072a570 FrameUpdate: implicit 24-byte CRightHandedSet copy, `RotationSpeed * Timer`, `if (++Timer > TimerLimit)`.
- CTCScriptedControl 00712ce0 TerminateActionByName / 00712d70 IsActionRunning: CCharString == as two-level inline (outer pointer/null tests, inner length+Compare whose own `return false` gives `xor al,al ; mov bl,al`); GetName temp lands in the dead arg slot by itself.
- CTCScriptedControl 007137d0 AddAction: CCountedPointer temp from raw pointer (`new SCountBlock`, no item null test), STLport push_back = insert(end()) with `prev = pos->_M_prev` in a local.
- CTCScriptedControl 00713850 InterruptControl: inline GetCurrentAction() (`empty() ? 0 : front().Data`), `PScriptHandleImp = CIntelligentPointer(0)`; retail list is the 4-byte STLport sentinel pointer so Locked is +0x18.
- CTCScriptedControl 00713910 Update: erase(begin()) out of line with by-value iterator (user copy ctor) and hidden-pointer result; erase sits inside the action null test.
- CTCShapeManager 00760910 GetPrincipalVector: selected shape read twice through CCountedPointer operator-> (fixes the eax/edx rotation); operator- takes a by-value C3DVector.
- CTCShapeManager 00762d80 / 00762f50 / 00762fd0 EditUpdateDelete/Split/SnapPointToPos: `AddNewShape(CreateShape(type, points))` builds the by-value counted pointer in the argument area; points vector copied with an out-of-line copy ctor, freed inline.
- CTCShop 00772120 RemoveShopper: `v[i] = v[size()-1]; pop_back()` (the pop_back virtual dtor becomes the `mov [esp+4],0 ; jmp [eax]` tail call).
- CTCShopKeeper 00799350 GetFriendlynessIndicator: `difference < 0.2f / < 0.55f` returns of CCharString temporaries (shared `push -1`).
- CTCShotDefinitionBase 007bf3d0 HasPassedTarget: packed TargetPos at +0x51, GetPhysics out-pointer helper sharing the key word, natural dot order matched.
- CTCShotDefinitionMultiArrow 007bdcfc GetShotDamage: size+frame TU; IS_THING_ALIVE<CThing*>(GetPItem()) binds through a frame temporary that the interface key reuses (0x64B668 shape).
- NThingShot::CTCShotDefinitionProjectile 004dc9d2 and CTCStockItem 004de44d dtors: novtable class + empty dtor; CEnginePrimitiveHandle member dtor needs `__forceinline` GetRefCount() ternary / IsUnique / Release in the size TU.
- CTCChest 00750240 GetOpenAnimationForCreature: def copied out of the GetSubDef out pointer into a holder whose dtor releases it after the RVO string return.
- Pair-vector map Add 006bf500 / 00cb72a0: `push_back(Pair(key, value))` then needs-sort flag (AddKeyedChunk 0xA737B0 shape).
- CTCSkeletalMorph 007865e0 ctor and CLipSyncDataBank 00a42b80 ctor: init-list zeros on one register, CArray as _Vector_base + _STLP_alloc_proxy, `Reset(new T)`.
- CTCSleep 0092c3b0 GetFastTrack: `FastTrack = (fabs && fabs && fabs) ? true : false; return FastTrack;` (plain && widened to eax).

## Parked for Fable 5.1 (14)
- 005e3df0 CTCQuestCompletionUI::Deactivate DIFFER(123v119): ours `mov eax,ecx ; mov cl,[eax+0Ch]` after the cookie load, AddGameEvent arg `lea ecx` after the push | tried: plain, optimize("s"), named component local, static event helper | next: lane gap: /GS parameter caching
- 00639930 CTCRegionDisplay::FinishShowingRegionName DIFFER(115v115): fcompp operand load order (retail `fild elapsed` first) | tried: `>`, `!(<=)`, `!(<)`, `<` with the difference first | next: `elapsed < displayFrames - fadeFrames`
- 00713740 CTCScriptedControl::vfunc_13 DIFFER(144v135): flag store `mov cl,[eax+94h] ; add eax,94h ; or cl,1 ; mov [eax],cl` vs `or byte ptr [eax+94h],1` | tried: char |=, char/long bitfield, optimize("s") | next: pointer-returning GetAnimation or inline setter on CAnimateThing
- 00760870 CTCShapeManager::IsPosInAnyShape DIFFER(142v153): compares match; retail 8-byte frame (Y copy), early esi/edi pushes, loop `cmp edi,[ebx+10h]` re-read | tried: || chain, by-value C2DVector &&, (x,y) if-chain, (x,y) && with `<` | next: Contains(C2DVector p) by value with the &&/`<` form
- 00762580 CTCShapeManager::AddNewShape DIFFER(181v150): retail caches by-value param words in esi/ebx across calls taking their address | tried: default, /GS, /Oa | next: lane gap: parameter caching in callee-saved registers
- 007723f0 CTCShop::IsShopKeeperNearerThan DIFFER(153v153): hero edi / keeper ebp swapped; dx squared first in retail | tried: locals, __forceinline Distance2DSquared helper, both sum orders | next: lane gap: x87 sum-of-squares term order
- 00772610 CTCShop::GetName DIFFER(166v151): late zero stores of both out pointers, def straight into esi, hit-arm release `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax` | tried: raw pointers, CDefPointer outs | next: lane gap: mov/dec/mov release; try a by-value GetDef helper
- 007726b0 CTCShop::GetMaxStock (reads Quantity) DIFFER(93v101): bare dword `fistp [ebp+8]` rounding | tried: (long) cast | next: lane gap: inline-asm fistp helper
- 00772720 CTCShop::GetMaxStock DIFFER(93v101): same fistp rounding | tried: (long) cast | next: lane gap: inline-asm fistp helper
- 00773aa0 CTCShop::IsShopKeeperAvailable DIFFER(118v118): component in eax / flag in cl vs ecx / al | tried: direct, named local, inline GetTC, single condition | next: scheduling artefact
- 00773c00 CTCShop::SetStockState DIFFER(142v142): retail loads the component into ecx before `cmp edi,1 ; setne dl ; push edx` | tried: direct, named local, inline GetTC | next: lane gap: component `this` loaded before the argument
- 007bdac7 CTCShotDefinitionDrainLife::UpdateCollision DIFFER(157v159): retail target in dead arg slot, hit in edi + [ebp-4] copy, finished in ebx, kill arm first | tried: locals, declaration swap, explicit template args, reference temporary | next: hit as derived pointer passed to IS_THING_ALIVE<CThing*> (converted temporary, 0x64B668)
- 007be05c CTCShotDefinitionDrainLife::SetSoulEffect DIFFER(154v157): effect/emitter frame slots swapped ([ebp+8]/[ebp-4]) | tried: plain, top-declared emitter, named CCharString, nested blocks | next: lane gap: dead-argument-slot homing of a named local
- 007868a0 CTCSkeletalMorph::vfunc_4 DIFFER(164v159): retail shares one frame word between the out pointer and each name string, keeps def in esi for loop and release | tried: plain, scoped-out helper, block, string block | next: CDefHolder release-by-destructor form of 0x00750240

## Skipped
- 007eeb20 CTCSmashableBarrier::Smash: row holds Smash plus three more bodies after int3 padding (0x007EEB80 `mov eax,4Eh`, 0x007EEB90 setter +0x0C, 0x007EEBA0 getter +0x10).

## Label corrections
- 004de44d "~CTCShotDefinitionProjectile" is CTCStockItem::~CTCStockItem (only caller 0x004DE431 = slot 0 of CTCStockItem vtable 0x01240C1C).
- 00a42b80 "CTCSkeletalMorph::CTCSkeletalMorph" is CLipSyncDataBank::CLipSyncDataBank (vtable 0x0129D3D4; retail lacks the PDB SpeechAnalyser member).
- 00750240 "CTCSinglePersonOccupiable::IsOfType" is CTCChest::GetOpenAnimationForCreature (returns CCharString; reads CChestDef +0x30).
- 006bf500 / 00cb72a0 "CTCSkeletalMorph::AddSkeletalMorph" are pair-vector map Add bodies (owner unidentified, named by address).
- 007726b0 "CTCShop::GetMaxStock" reads CShopItem::Quantity (+0), not MaxStock; 00772720 is the MaxStock one.
- 00625950 "vfunc_11" = DrawPreparePrimitivesForRendering; 00713740 "vfunc_13" (base slot 0x006868E0 is an empty `ret`, manifest "$E2"); 007868a0 "vfunc_4" = OnCreate.
- Callee labels: 0x006CB6B0 OnStateGroupRemoved (not CScriptThing::operator==), 0x0070B460 CTCAnimationComplex::GetAnimation (not CWorld::DrawGetEnvironment), 0x00449960 CPlayerManager::GetMainPlayer (not Catch@), 0x00719280 list clear (not remove_if), 0x00719210 / 0x00764D10 list/vector erase, 0x007624E0 CTCShapeManager::CreateShape, 0x007BEE10 CTCShotDefinitionBase::GetShotDamage, 0x004378C0 CPlayerGui::Refresh.
