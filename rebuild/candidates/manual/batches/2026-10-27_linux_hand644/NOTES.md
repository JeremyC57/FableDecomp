# Hand chunk 644 (CTCShop .. global WordWrap): 1 of 10 byte-exact

All 10 rows were parked same-length rows from the 2026-10-30/31 retry sweep. Every best attempt was reproduced at its recorded
grade first (probe/<addr>_b0.cpp), then changed one lever per grade. Grades ran with QTAG=a644.

## What matched (1)
- CBusyResourceManager (global catalog row "Deque_Process_And_Rebalance"): 0x00A618D0 `CBusyResourceManager::OnPostSwapScreens()`.
  It retires finished GPU event queries (or the oldest while more than 8 are in flight) into the free list, recording the finished
  frame, then issues a recycled or new D3D event query for this frame. Lever: STLport 4.6 `list::size()` written as
  `distance(begin(), end())` through the input-iterator `__distance` that copies the first iterator (the agent637 0x00AEB060 lesson).
  The raw node-counting loop of the best attempt swapped edi/ebx between the free-list sentinel and the node payload in the move
  block. The STLport form gives retail's colouring at the first grade (RELOCATION_MATCH).
  Names: Ego_r.pdb `?OnPostSwapScreens@CBusyResourceManager@@QAEXXZ`. Its donor order (SetDevice, ctor, OnPostSwapScreens,
  ClearAllQueries, dtor) matches the retail order (ctor 0x00A61880, this row, ClearAllQueries 0x00A61A80, dtor 0x00A61B10).
  The members are the PDB's (FreeQueoryObjects, PendingQueories, PendingFrame, FinishedFrame, D3DDevice). The placeholder
  `Slot000..Slot117` D3D device slots of the best attempt are replaced by the real d3d9.h method names. CreateQuery is slot 118 (+0x1D8).

## Parked for Fable 5.1 (9)
- 007751a0 CTCShop::SetShopKeeper DIFFER(419v419): `mov eax,[ebp+4] ; mov ecx,[eax+34h] ; test ch,8` (ParentThing->HasTC(CTCTavern 0xAB)) v ours `mov ecx,[ebp+4] ; mov eax,[ecx+34h] ; test ah,8`, then lea ecx/edx for the 0xBD key | tried: bool `/ %` HasTC (436, shr/test), std::bitset reference operator[] (registers right + dead `add eax,34h`, 422), STLport-exact reference ctor, no ~reference, mem-initialiser reference ctor (all 422), const `_Unchecked_test` HasTC (419, same residue), `if (bits) return true; return false;` HasTC (same), PDB respell of the class (CThingCreatureBase*, const IsPermanentlyStocked, ESTOCK_STATE, CBaseObject* GetPItem: byte-neutral) | next: find the form that computes the bitset word address without leaving the dead add (an inline accessor returning `const std::bitset<274>&`, or PeekTC/GetTC<CTCTavern> spelled as in the PDB), probe/007751a0_p4.cpp is the register-correct variant
- 00b4b8b0 CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace DIFFER(421v421): side.Y `fld st(2) ; fmul [dir.Z]` v `fld [dir.Z] ; fmul st(3)`, plus the result-copy setup scheduled two stores earlier | tried: PDB `C3DVector::operator^` (const, user copy ctor) for both cross products (more operand flips) | next: lane gap: x87 canonical operand order (park on sight); PDB signature is private AAE returning class CMatrix3x4 (respell at landing)
- 009a3460 ReadTextFileIntoArray DIFFER(424v424): big-endian branch `lea eax ; lea ecx,[esi+1]` and one-byte branch `lea eax` v ours edx/eax in every branch | tried: reuse `length` for the count, single default-argument `resize(n, const T& = T())` | next: lane gap: register colouring tie
- 006e94d0 CCombatManager::IsCreatureInAttackRingSector DIFFER(426v426): frame 0x1c v 0x24, the second find key at frame +0xc instead of sharing +4 with the first | tried: wheel reference straight from GetCombatantInfo (425), innerRadius declared first, attackedRadius declared first (both 426, same frame) | next: an inline helper whose parameters are the radii (inline-parameter temporaries reuse dead slots), or the key built in a named scope; the PDB lists no such helper
- 0057aeb4 CTCHeroStats::GetFavouriteWeapon DIFFER(421v427): retail caches the thing def in edi (`mov edi,[ebp-8]`, `test` compares, `xor edi,edi` at the loop tail), ours re-reads [ebp-8] and keeps edi as the zero register. Also retail re-takes `lea eax,[ebp+0Ch]` for the CCharString argument | tried: same-TU stand-in GetDef/GetSubDef bodies (frame 0x2c -> 0x28, the escape is gone), CDefPointer locals with destructors (404), thing-def copy before the zero store, text-bank accessor + explicit CCharString temporary (object load now first), named symbol local (slot moves to the frame), const/class respell (neutral) | next: the real landed GetDef/GetSubDef bodies (shared engine/CDefPointer.h + CSubDefClassBase.h) in the TU in place of the stand-ins
- 00b9d730 CEnginePrimitiveManagerStaticMeshesBase::IsInsideFustrum DIFFER(429v429): `fld [primitive+18h] ; mov ecx,camera ; fmul [mesh+58h]` homed in [esp+1Ch] v ours scale first, homed in [esp+20h] | tried: PDB signature (const C3DPrimitive2*, const CEngineInternalPrimitiveStaticMesh&, private) with C3DBoundingSphere members (neutral) | next: lane gap: x87 canonical operand order
- 006aeea0 CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions DIFFER(429v429): `lea eax,[esp+0Ch]` (ours ecx) for the GetDefName hidden pointer; table position loaded after `push ebx` into ecx (ours edx before it) | tried: named table position in operator== (worse), PDB CDefString::GetAsCharArray / EqualsCaseInsensitive / operator== chain (neutral, kept), user copy ctor + named result (427), PDB reference signature `const CThingAICreature&` (neutral, kept) | next: lane gap: scratch rotation tie (the agent641 copy-ctor lever does not apply here)
- 00b81820 CEngineSimulationManager::FrameStart DIFFER(433v433): `mov [esp+N],edx ; mov edx,[ecx+368h]` after `push ebx` (ours) v after `push ebx ; push esi` (retail) | tried: `g_twoPi` global for the 2pi constant (neutral, kept) | next: lane gap: scheduling tie
- 00a10560 WordWrap_FindNextLineA DIFFER(433v433): `cmp eax,esi` v `test eax,eax` after MultiByteToWideChar | tried: named `int converted` result | next: lane gap: zero-register CSE of one compare

## Skipped
None.

## Label corrections
- 0x00A618D0 "Deque_Process_And_Rebalance" is `CBusyResourceManager::OnPostSwapScreens()` (Ego_r.pdb; see above).
- 0x009A3460 "ReadTextFileIntoArray" is the FableWin.pdb name: `?ReadTextFileIntoArray@?A0xfe46a155@@YIKABVCWideString@@AAV?$CArray@G@@@Z`
  (anonymous-namespace __fastcall). FableWin also has a CCharString overload.
- 0x006E94D0 "Combat_CheckRingRangeToTarget" is `?IsCreatureInAttackRingSector@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z`.
- 0x00A10560 "WordWrap_FindNextLineA" is a real FableWin.pdb name: `?WordWrap_FindNextLineA@@YIPBDPBDIPAPBD@Z` (next to
  WordWrap_FindNextLineW `?WordWrap_FindNextLineW@@YIPB_WPB_WIPAPB_W@Z`, as in the retail neighbourhood).
- 0x00B9D730: the PDB really spells `IsInsideFustrum` (`?IsInsideFustrum@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NPBVC3DPrimitive2@@ABVCEngineInternalPrimitiveStaticMesh@@@Z`).
  The virtuals of the primitive classes are spelled IsInsideFrustum.
- 0x007751A0 takes `CThingCreatureBase*` (Ego_r `?SetShopKeeper@CTCShop@@QAEXPAVCThingCreatureBase@@@Z`). 0x0057AEB4 is const
  (`QBE`) and returns class CWideString. 0x006AEEA0 takes `const CThingAICreature&`.

## Header debt / names still descriptive in the differ sources
- 0x00B81820: the globals 0x01436E98 / 0x01436EA4 / 0x01436EA0 keep address-suffixed descriptive names. 0x01436EA0 is bound
  elsewhere as `g_pEngineCamera`; 0x01436E98 has five different bindings.
- 0x007751A0: the creature's +0x170 pointer is `PWorkplace` (descriptive). The Ego_r CThingCreatureBase offsets are shifted by
  Dinkumware container sizes.
- 0x0057AEB4: the GetDef/GetSubDef stand-in callees `FindDef_*` follow the landed 0x007F90B0 idiom.
