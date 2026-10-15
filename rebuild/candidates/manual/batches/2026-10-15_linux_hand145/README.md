# Hand chunk 145 (CLandscapeBackgroundPatch .. CMainGameComponent): 32 of 40 byte-exact

All matched sources are self-contained (the shared preludes `prelude_tn.txt` / `prelude_light.txt` were expanded
into each file by `ex.py`). Every matched row has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (32)
- CLandscapeBackgroundSubNodeSplitBranch::ProcessBackgroundLoading (0x00BF8430): merge of the two branch node
  statuses. Retail's three separate `mov eax,N ; ret` exits need NESTED blocks
  (`if (left != 3) { right = ..; if (right != 3) { if (l==2 || r==2) return 2; return 1; } } return 3;`); the flat
  early-return forms merge 1/2 into a `mov eax,1 ; jne` select. The retail tree node is 0xE8 bytes (RightBranch +0xEC).
- CLandscapeBackgroundTreeNode (8 rows: TestIfLoadTaskCanBeAborted, StartTreeLoadTask, StartPatchLoadTask,
  InvalidateEdgeTesellation, MarkTreeAsInvisibleLastFrame, UpdateParentChainVisibilityFlag, OnPostRelocation,
  TreeHasLoadTasks): one retail-offset model (PDB members from LODLoadInfo on sit 8 bytes lower: FileBlockPos +0x8c,
  AsyncLoadTask +0x98, Parent +0x9c, SubNode +0xa0, Map +0xa4, MapX..Height +0xd8..+0xde, FirstNonSplitBand +0xe1,
  TesselationValid +0xe3, VisibleLastFrame +0xe4) with the landed GetPatch/GetSplitBranch as inline members. Shapes:
  a patch pointer kept in a local (one IsPatch call instead of two); the load task is `new (this) Task(...)` through a
  class `operator new(size, CMovableResource*)` forwarding to AllocVirtual (0x00BD9AF0), with `return task->m_requested
  = true` for the shared `mov al,1 ; mov [ecx+0Eh],al` and two statements for the twin; the recursive right-branch
  calls become loops by themselves (tail recursion), including the inlined SetVisibilityFlagAndUpdateParentChain;
  `if (split) return L || R; return false;` puts the `xor al,al` leaf exit last.
- CLayer::SetAllPolys (0x00A5CB00): column-outer/row-inner element copies around D3DXMatrixInverse (0x00BFEFE0) with a
  zeroed determinant float; first try.
- CLevelFiles::~CLevelFiles (0x0050F980): three intrusive-count smart pointers (the CCountedPointer Reset family's
  {object, count block} with a `__fastcall` destroy function) as THREE separate members; an array member gives a loop.
- CLevelLoader::ProcessHoverAnimQueue (0x006C2710): DisplayProgress strings must be NAMED locals in their own blocks
  (the temporary form builds the string after the pushes); list erase(begin()) with a by-value iterator class.
- CLight x4 (0x00C631E0 SetLightValue, 0x00C63270/0x00C632E0/0x00C63350 SetIntegerProperty 1-3): an EAX-style voice
  property block (slots +0x94 x 0x1c, records +0xfc x 0x70, committed values +0x348.., dirty +0x3bc, error +0x3d4) in
  the landed CEntity::SetVectorProperty_XY style; a reference local to the record field; the float change test must be
  `stored != value` (argument loaded first).
- CLightningBranch::~CLightningBranch (0x007F2360): two CBaseIntelligentPointer members, GetPItem re-called for every
  use, Kill(true) / RemoveTC(0x10b) guarded by the +0x91 bit 0 dead flag.
- CLipSyncFrameData::FindPhonemeID (0x00A48A40): returns the 12-byte phoneme record by value (implicit memberwise
  copy), not-found = inline default ctor; CLipSyncSampleData::Sizeof (0x00A43960): nested array Sizeof template that
  must be `__forceinline` (else the loop stays out of line) and the leaf `?Sizeof@CLipSyncSampleData@@QBEKXZ` (the
  default leaf picks the template instantiation).
- NUISystem::CList ChangePosition (0x00537956) and AddScrollingChildDynamically (0x0053BEAC): the landed
  CList::OnActivate owner-pointer model (count at +4, volatile compare) plus a null test; rvalue push_back arguments
  are copied into the dead `child` slot (the `true` into its high byte at +0xb). Frame-pointer size TU.
- CTCWeaponTrail::CTCWeaponTrail (0x0076E620, catalog "CList::CList"): CTCBase(thing), NoTrails(-1), the
  CEnginePrimitiveHandle member of the landed CTCEmoteIcon, five shipped 12-byte vectors (vector<bool> is 12 too),
  the one-word list with its sentinel, WeaponTrailActive(false); first try.
- CLoadingMemoryFailureHandler::HandleFailedAllocation (0x009D5F10): CCriticalSectionLock local, per-iteration size.
- CLocalDetailPrimitiveBase::ClipPrimitive (0x00BF9AA0): `range * range < z*z + y*y + x*x` (range squared first and
  kept on the FPU); the clip result is an `unsigned char` bit field read as `Outside ? false : true` -- that ternary is
  what gives the 8-bit `shr al,6 ; not al ; and al,1` (`!Outside` and the shift/mask forms widen to eax).
- CLocalDetailPrimitiveMesh::~CLocalDetailPrimitiveMesh (0x00BDFD80): the handle model of the landed 0x004C4160 as a
  member at +0x6c of a `__declspec(novtable)` class over an inline CMovableResource destructor (0x0129B860).
- CLocalDetailPrimitiveMeshZSpriteBatch::RelocateData (0x00BFA8A0, catalog "vfunc_1" = CMovableResource slot 1):
  inline IsInBlock + Rebase (the second null test), else-if for the second pointer, then the renderer update with
  SetupPrimitiveDesc's 0x50-byte by-value result.
- NCameraModes::CSimplePointBase::CSimplePointBase (0x00873A10, catalog "CLookAtFront"): memberwise C3DVector copy
  ctor; PDB members minus the 0x24-byte narrower retail root.
- CM_HeroDownstairs::Main (0x00E3B320): `while (!IsDone())` with an IsDone test before every step, the 3.0f pushed
  before the virtual GetHero (fastcall IsDistanceBetweenThingsUnder).
- CMainGameComponent UpdateRegular, SetExclusiveMode, GetGameTimeOfNextPresentCompletion, InitialiseFonts: all
  `optimize("s", on)` + `("y", off)` (frame pointer, `leave`, `fld ; push ecx ; fstp [esp]`). SetExclusiveMode: the
  width is a named local read before the call's other arguments (right to left). GetGameTimeOf..: the
  CVsyncCallbackTimerInfo is 0x30 bytes (frame 0x38 with the shared fild/result slot).

## Parked for Fable 5.1 (7)
- 00be6790 CLandscapeBackgroundPatch::PackVertexNormal DIFFER(75v106): retail `fstp [esp+10h] ; fld [esp+10h] ; fistp dword [esp+N]` per component (bare fistp rounding after a float store/reload in the dead argument slot); ours calls __ftol2 | tried: (long) cast through an inline helper, /QIfist (qword fistp, no reload) | next: lane gap: inline-asm FTOI helper (byte purity forbids it)
- 00be8b50 CLandscapeBackgroundPatch::PackVertexNormal DIFFER(81v108): same fistp rounding; this variant stores the packed word into `this` (+0) | tried: same as 00be6790 | next: lane gap: inline-asm FTOI helper
- 00be8df0 CLandscapeBackgroundPatch::PackVertexNormal DIFFER(81v111): same fistp rounding; `this` moved to eax first (`mov eax,ecx`), packed word stored through it | tried: same as 00be6790 | next: lane gap: inline-asm FTOI helper
- 00bfdf60 CLandscapeBackgroundPatch::PackVertexNormal DIFFER(75v106): byte-twin of 00be6790 | tried: same | next: lane gap: inline-asm FTOI helper
- 00a42df0 CLipSyncDataBank::NotifyEntryAdded DIFFER(119v119): retail `fld [esp+20h] ; push edi ; fstp [edx+edi*8+4] ; push eax`, ours `push edi ; push eax ; fstp` (scheduling only) | tried: begin()+index, m_first[index], float local punned through its address | next: lane gap: scheduling (the float store sunk past the base call's pushes)
- 004165e8 CMainGameComponent::CheckSync DIFFER(138v136): retail keeps the packet index in esi and re-reads the package argument `mov ecx,[ebp+8]` before every call; ours caches the package in esi and the index in edi (+push/pop edi) | tried: const reference, pointer, one-word handle class by value | next: lane gap: register allocation (or the argument's address escapes somewhere in the original, e.g. an inline debug compare compiled out)
- 0053852b NUISystem::CList::SaveState DIFFER(131v131): retail homes the list's allocator temporary in the high byte of the LIVE hidden-return slot (`lea eax,[ebp+0Bh]`, 0x10 frame, list [ebp-4], record [ebp-10h]) and walks nodes in edi / component in esi; ours gives the temporary [ebp-1] (0x14 frame) and swaps esi/edi | tried: template and plain allocator tag, component declared before the loop | next: lane gap: empty temporary over a live parameter slot (same as the 24n `find` tag rows)

## Skipped
- 00d9f1b0 CMC_Minion::OnPredicateFail: the 160-byte row holds OnPredicateFail (ends `ret` at 0x00D9F1FA, int3
  padding) and a second function at 0x00D9F200 (the tail-jump target: fild/fmul timer + virtual slot 89 call).

## Label corrections
- 0x0076E620 "CList::CList" is CTCWeaponTrail::CTCWeaponTrail(CThing*) (vtable 0x0126A5B4 = CTCWeaponTrail).
- 0x00873A10 "CLookAtFront::CLookAtFront" is NCameraModes::CSimplePointBase::CSimplePointBase (the only vtable stored
  is 0x01277644 = CSimplePointBase; CLookAtFront's is 0x01277714).
- 0x00BFA8A0 "vfunc_1" is CLocalDetailPrimitiveMeshZSpriteBatch::RelocateData (CMovableResource slot 1, 4 args).
- 0x00A5CB00 "CLayer::SetAllPolys" is a 4x4 matrix "set to inverse of" (copy in, D3DXMatrixInverse, copy back);
  kept under the catalog name -- the owning class is unverified.
- 0x006C2710 "CLevelLoader::ProcessHoverAnimQueue" is the level loader's update step (process + pop the front pending
  object between two "Level loader update" progress texts); kept under the catalog name.
- Callees: 0x00434A10 (manifest "CScriptThing::operator==") is a no-argument member of the display object at
  CMainGameComponent+0x28; 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite") is CTCBase::CTCBase(CThing*);
  0x00BD9AB0 ("$E2") is the landscape memory manager's rebind-owner (block, owner); 0x009F17A0 / 0x009F18F0
  ("CopyToBuffer" / "CircularBuffer_Read") are CEventPacket::CopyToBuffer(CReadBuffer*) and CReadBuffer::Read(size, out).
