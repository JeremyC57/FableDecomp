# Hand chunk 237 (CThingFilter_OverlappingArea .. CV_AssassinAttacksScript): 20 of 40 byte-exact

All grades under the lane default RTM cl 13.10.3077, default flags (no /GS, /EHsc or pragma needed by any row).
Test runs: tall.sh (g++) and tall.py (VC7.1) both PASS for all 20.

## What matched (20)

- CThingManager (5)
  - 0051e100 AllocateNewThingUID: map-specific / global / plain 64-bit UIDs. The post-increment + high-tag add is two
    statements (`uid = counter++; uid += tag;`); the map prefix is an `unsigned __int64` local (8-byte frame, only the low
    word stored, `xor eax,eax` high); the world map is re-read into a named local after the first call.
  - 0051f340 CheckAndFixUID: 64-bit tag/counter locals computed before the zero test; on a clash the map number and the
    `global` bool are named locals (map number first, bool homed in the dead uid slot).
  - 0051eab0 PreparePrimitivesForEngine: CEnginePrimitivePrepareInfo copied implicitly (rep movsd + CCamera copy ctor);
    the bullet-time object through an inline `GetBulletTime()` (a direct `PWorld->m_pBulletTime` loads the argument first
    and lets VC cross-jump the two virtual calls).
  - 00520570 LoadHeroStateScript: terminators as implicit CCharString conversions (lea re-address), the "" file name an
    explicit temporary; CStringParser/CPersistContext models from the landed DoScriptDataPersist.
  - 00522600 SaveToScript: fastcall `"Version " + GFIntToCharString(v) + ";\r\n"`; inline push_back with same-TU stand-ins
    for InsertGrow (reads the const ref only, never escapes the vector) AND for the thing-list writer 0x521EB0 (only reads
    the vector) -- with both, `last` stays in esi through the loop and `first` in ebx across the writer call.
- NThingMap::CThingMap (1)
  - 0085cec0 Clear: inline `GetCell(unsigned char level, x, y)` gives the `movzx edx,bl` re-index; node unlink clears
    PPrevious then PNext.
- CThingMapManager (1)
  - 00637b10 GetCellsForThingLayerCellPositions (MATCH): each corner's map info is a named local (kept in a register for
    the neighbour compares) and the cell comes from an inline `GetMapCell(info, long layer, pos)` (layer loaded as long,
    narrowed with `movzx ecx,cl`).
- CThingPhysical (2)
  - 006a5950 ConstructFromParams: creators assigned through `CIntelligentPointer<CThingPhysical>(T*)` temporaries + inline
    operator=; `Health = MaxHealth = def->Health`; `if (base) {...; return true;} return false;`.
  - 006a5af0 SerialiseGameState: the same reset block (creators cleared through (0) temporaries); Transfer<float>; the
    1.0f default as a local homed in the dead context slot.
- CThingPlayerCreature (3)
  - 006a9dd0 ConstructFromParams: implicit string temporaries; the unused sub-def CDefPointer released through a pointee
    `Release()` with DecRefToZero, which only gives the short ecx form once GetSubDef 0x42B0A2 has a same-TU stand-in.
  - 006ab300 PreOnKill: three GetTC forms (plain entry pick used as `this`, two zero-preloaded `T* p = 0; if (HasTC) p =
    GetTC`), implicit wide/char strings, the main-player test wrapping the body so `return true` is last.
  - 006ad9d0 WorldUpdate: the collision vectors cleared with chained `X = Y = Z = 0.0f` -- this is what makes VC keep 0
    (and the flag mask 2) in ebx; the look component through the out-parameter GetInterface (eax then `mov ecx,eax`),
    the weapon trail through the plain pick.
- CThingSwitch (1)
  - 0083be90 TriggerSetEnvironment: C2DBoxF filled member by member (one GetPosition() per component), inline
    ContainsPoint, the world as a named local so PeekWorld runs before the argument pushes.
- CUserProfileManager (2)
  - 00408c90 GetAssignedInputForAction / 00408f50 GetAssignedInputForContextSensitiveItemAction: one loop per table arm
    over the 12-byte retail vectors (Passive +0x54, Aggressive +0x60), size() recomputed (signed /28, unsigned compare).
- Entity / quest scripts (5)
  - 00dd8990 CTortureDoorPassage::Main: wait loops, implicit string temporaries (use test result in bl).
  - 00d88f90 CUndeadSoldier::Main: scoped CIsThingAlive filter, fastcall IsDistanceBetweenThingsUnder (float pushed before
    GetHero()), endless frame loop.
  - 00e0cff0 CUndeadRisingZombieAttacker::Main: CScriptGameResourceObjectScriptedThingBase local with a POLYMORPHIC
    CBaseObject root (the base ctor 0x99A380 runs on offset 0).
  - 00e48bf0 CTollBandit::Init: CScriptThing operator= over the inline CCountedPointer operator= (source fields first).
  - 00e39540 CV_AssassinAttacksScript::Init: the landed CSpawnedFunc0 spawn idiom (flag-guarded implicit name),
    `resize(5, 0)` with the 0 temporary in the frame, the script def re-read after each store.

## Parked for Fable 5.1 (19)
- 005252b0 CThingManager::SaveThingsOnLevelReadyForUnloading DIFFER(314v312): mapNumber stays in its arg slot (ebx becomes a zero register) so the homed thing gets its own frame word; esi push sunk | tried: stand-ins for push_back 0x436C80 + InsertGrow, continue vs nested if, `long map` copy, function-scope thing, alloc_proxy vector | next: find what makes VC enregister mapNumber (a third use?)
- 00637fc0 CThingMapManager::PlaceThing DIFFER(251v243): each corner's cell loaded into ecx then `mov edx,ecx` (retail loads edx directly) | tried: cell/node member AddNode, null test inside the helper, pos-info argument, named C2DVector, stand-ins for the two neighbour members | next: lane gap: register choice
- 007037d0 CThingObject::OnCollision DIFFER(247v247): `push 1 ; mov ecx,esi` before Kill(true) where retail has `mov ecx,esi ; push 1` | tried: GetInterface out-param, forceinline const GetTC, LowerBound + GetSubDef stand-ins, inline Kill wrapper | next: lane gap: scheduling only
- 0083df20 CThingPhysicalSwitch::UntriggerMe DIFFER(247v247): CSoundPair temporary stores sink below the pushes and the constant 1 lands in ecx (retail eax) | tried: named pair (frame +4), single-arg pair ctor, forceinline GetTC | next: lane gap: scheduling only
- 00836de0 CThingTrackNode::FlipTrackDirection DIFFER(249v247): the forward walk's swap is folded into [ecx+d0h]/[ecx+d4h]; retail keeps lea'd member addresses in ecx/esi with the node in edi | tried: member FlipNode, local copies at the top, explicit backward swap, direct SwapValues call | next: a swap through two named references in the loop
- 0083c990 CThingSwitch::TriggerSetPlayerAreaName DIFFER(255v255): the C2DVector conversion's Y load sinks below the X store | tried: getter vs direct member, float-pair ctor, named position local, pointer ctor | next: lane gap: scheduling only (sibling 0083be90 matched)
- 0098e1e0 CThreadedFile::Open DIFFER(310v310): the sort-key counter store is emitted before the drive dtor's `lea ecx` | tried: ++g, split load/store, manager-member counter | next: lane gap: scheduling only
- 00407cd0 CUserProfileManager::RedefineControlPassive DIFFER(309v310): entry address `mov edi,begin ; add edi,ofs` vs retail `lea edi,[edx+ecx]` after the action load | tried: forceinline IsSameControl, if-form switch, GetDirection stand-in (puts the action in edx), per-iteration `oldAction` copy (best), no reference local, operator[] form | next: register choice only
- 00f3b970 CTriangleStripifier2::StripUnusedVerts DIFFER(223v243): retail keeps kept.last in ebx, i in the frame, re-reads the inner size each pass and homes the vertex at push time | tried: InsertGrow + operator= stand-ins, long vertex, walking pointer | next: drop the operator= stand-in / try the vertex as a homed `unsigned short` temp of a long
- 00f3f5a0 CTriangleStripifier2::ReorderTriangleListInPlace DIFFER(249v245): reordered.first in edx (reloaded after the first free) instead of esi; push_back part matches | tried: GenerateStrips stand-in (fixed the fill loop), source pointer copy loop | next: copy loop through a named walking pointer
- 00637c80 CThingMapManager::DebugDraw DIFFER(288v288): map-local subtraction order (ours X first) and the colour/rect store interleave | tried: member-store origin, C2DCoordI local (fixed the frame), ctor form | next: lane gap: scheduling
- 0083dc80 CThingPhysicalSwitch::DrawPreparePrimitivesForRendering (vfunc_36) DIFFER(296v309): frame 0x8c vs 0x80 (retail homes the UniqueID high word in the scaled vector's dead Y word), UniqueID low preloaded in ebx, x87 add order | tried: return-ctor operators, named-result + / * | next: lane gap: temp-slot overlap; then the x87 order of `(P-L)*t + L`
- 00837b30 CThingTrackNode::DrawPreparePrimitivesForRendering (vfunc_36) DIFFER(301v314): same shape as 0083dc80 (def graphic at def+0xe6) | tried: the 0083dc80 source | next: as 0083dc80
- 0083b6b0 CThingVillage::DrawPreparePrimitivesForRendering (vfunc_36) DIFFER(296v309): same shape as 0083dc80 | tried: the 0083dc80 source | next: as 0083dc80
- 00877720 CTools::GetStandardThings DIFFER(250v277): filter object layout / class words homed in dead arg slots | tried: one sphere+class filter model | next: decode the filter objects at [esp+14h]/[esp+2Ch] and the 0x878430 template signature
- 00406ae0 CUserProfileManager::GetManualSaveDisplayName DIFFER(342v314): our frame overlaps the string temporaries (0x0c) where retail keeps five words (0x14) | tried: forceinline vs inline number helper, named result | next: lane gap: frame packing (reverse of the usual direction)
- 0050ef30 CThingFilter_OverlappingArea::operator() DIFFER(219v242): bare dword `fistp` CeilToInt in an ebp frame | tried: C ceil | next: lane gap: inline-asm fistp (asm_bake family)
- 005115e0 CThingFilter_OverlappingArea::operator() DIFFER(303v299): same fistp helper + slot-55 predicate + second filter | tried: C ceil model | next: lane gap: inline-asm fistp (asm_bake family)
- 00511a10 CThingFilter_OverlappingArea::operator() DIFFER(241v252): same fistp helper after a first filter | tried: C ceil model | next: lane gap: inline-asm fistp (asm_bake family)

## Skipped
- 0083cd70 CThingSwitch::vfunc_19: the row holds the switch body, its 5-entry jump table and two more functions after
  int3 padding (`mov eax,ecx ; mov [eax],0 ; ret 4` and a counted-pointer copy ctor) -- ledger.

## Label corrections
- 0x004FCE00 "CEditWorldMap::GetEditMap" is the CWorldMap map-UID prefix getter (byte twin label).
- 0x00527D80 "BSTNode_FindLowerBound" is map<unsigned __int64, CThing*>::find (UIDToThingMap).
- 0x00521EB0 "Save_WriteEntityHierarchy" and 0x005239C0 "Thing_SavePersistentState" are CThingManager members
  (thing-list writer (script, mode, vector, names); SavePersistentState(map, vector, gameState)).
- 0x00410620 "Transfer<signed_char>" is used as Transfer<float>(name, float&, const float&).
- 0x0083D900 "CThingDeadCreature::PeekDefinition" is used as the CThingPhysicalSwitch def peek (byte twin).
- 0x006A4D00 / 0x006A4D10 "CVertexBufferWin32::DoSizeof" are CThing::GetPosition / the last-position getter (TU copies).
- 0x0099AED0 "CCharString::CCharString()" and 0x0099AF00 "CCharString::operator const char*" are used on CWideString
  (default ctor / operator const wchar_t*; byte twins).
- 0x004C79A0 "CViewBase::GetDisplayEngine" is a CThing def getter (CThingTrackNode graphic at def +0xE6).
- 0x007E74D0 "~CScriptGameResourceObjectMovieBase" is ~CScriptGameResourceObjectScriptedThingBase here.
- 0x00405F00 "CGuiControlNumber::SetSLongValue" is a `__fastcall` int-to-CWideString helper.
- 0x00F3B640 "std_vector_assign_with_realloc" is CArray<unsigned short>::operator=; 0x005135D0 is its InsertGrow.
- 0x00878430 "GetPBestThingInRoughArea<CThingFilter_HasTrackName,...>" is a different instantiation at this caller
  (area, filter, list*).
- The three "vfunc_36" rows (0x0083DC80, 0x00837B30, 0x0083B6B0) are DrawPreparePrimitivesForRendering (slot 36).
