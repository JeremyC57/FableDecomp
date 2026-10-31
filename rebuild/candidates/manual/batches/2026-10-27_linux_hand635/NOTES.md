# Hand chunk 635 (CPhysicsMesh .. NUISystem::CMouseCursor): 3 of 10 byte-exact

## What matched (3)
- NScript::CQ_FireHeartScript::CFH_Scythe::Main (0x00D3AE40, 264 B, RELOCATION_MATCH): takes scripting control of the
  scythe entity (it first releases any handle it already holds), then waits until the hero is near and plays the cutscene once.
  Modelled on the retail headers (CEntityScriptBase, CGameScriptInterface, CScriptThing with FABLE_RETAIL_NSCRIPT /
  FABLE_RETAIL_BASE_OBJECT). Shape lesson: a named `CQ_FireHeartScript* parent = ParentClass;` local before the
  IsValid/Release pair gives retail's `mov eax,[esi+14h] ; push edi ; lea edi,[eax+164h]`. Inside the loop
  ParentClass is read again.
- GetActionAnimSetList (0x00693140, 269 B, RELOCATION_MATCH): collects the creature's anim sets for an action and
  skips combination anims. Shape lesson: put the STLport `vector::_M_insert_overflow(__true_type)` body (landed
  0x0066C3F0) in the same TU, noinline, before the caller. VC7.1 then knows the vector does not escape, and push_back's
  value is homed in the dead `action` argument slot as in retail. Function in an anonymous namespace plus a `_Use` caller.
- C3DVector::GetRandomPointWithinUnitSphere (0x00A146C0, 278 B, RELOCATION_MATCH): rejection-samples X/Y/Z with
  GFFloatRandom until the point lies in the unit sphere. Shape lesson: inside the inline GFFloatRandom, give the fmod
  result its own `double result` local. This schedules the integer seed update before the fmul. The same body still
  grades RELOCATION_MATCH at the landed 0x004978A0 GFFloatRandom.

## Parked for Fable 5.1 (7)
- 00a17690 CPhysicsMesh::HasBoundingBoxIntersectedFromTo DIFFER(262v262): centre X lane `fld [ecx+84h] ; fadd [ecx+90h]` (retail MinCoord.X first) while the Y lane loads MaxCoord.Y first; VC orders both lanes alike | tried: operand swap in operator+ (inert), named-return operator+ (314), box reference, inline GetCentre/GetHalfSize, by-value operand (281), friend operator+ (flips both lanes), friend + Y swapped (inert) | next: lane gap: x87 operand commutation (perhaps retail inlined two different helpers for X and Y; try a C3DBoundingBox::GetCentre returning C2DVector)
- 0099ba70 CWideString::Sprintf DIFFER(265v265): resize count `lea eax,[esi+0FA0h]` (ours edx) and the _vsnwprintf argument loads (retail eax=format/edx=args/ecx=count; ours rotated) | tried: same-TU STLport vector<unsigned short> bodies (closed the zero stores), STLport two-branch resize (269v265: args right, ebx/ebp swapped), resize(const T&), erase returning an iterator, declaration orders, begin() for &v[0], inline operator= (199) | next: two-branch resize with the vector declared before the buffer / count local; else lane gap: register allocation
- 00a65480 CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea DIFFER(265v265): ours preloads size into edx before `push esi` and builds the header in esi (retail header in eax, `mov ecx,[size] ; lea esi,[eax+ecx]`), late size re-read for the second Create, allocated-list loads rotate eax/ecx/edx | tried: unsigned needed, 64-bit check first (263), size + header (242), inline GetAlignedSize helper, PDB inline Create / InsertAreaIntoAllocatedList | next: model the free/allocated list links as a PDB inline helper on CMemoryArea (Unlink/LinkAfter) to change live ranges
- 008c7280 CCreatureAction_TrollGroundPound::Initialise DIFFER(268v268): the `range` float is homed at frame+0xc (the key/name word); retail homes it at +0x10 (the inserted pair's first word) | tried: const-ref pair ctor (inert), unnamed range (262), range declared early (inert), unnamed CCharString temporary (264), const GetSubDef<T>(CDefPointer<const T>*) respell (inert), PDB renames (inert) | next: use the real landed GetSubDef<CTrollBattleDef> header (FABLE_RETAIL_DEF_CLASS_BASE) and a std::pair<void* const,float> value_type; else lane gap: frame-slot assignment
- 00a65720 CMemoryAllocatorVariableSize::NewChunk DIFFER(269v269): minSize temp `mov ebp,[esp+28h] ; add esi,ebp` before `push esi` (retail `mov edi,[esp+30h] ; add esi,edi` after `push edi`) | tried: size expression orders, long size, minSize copy, declaration orders (the loop-top reset, named protect and declaration order closed everything else) | next: lane gap: register allocation (one register)
- 00b84470 CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer DIFFER(270v270): VC loads eax=2 once and groups the six RenderPass=2 stores (and the byte flags); retail keeps source order and clears eax after the float stores | tried: #pragma optimize("s") (242), inline SetMaxSortDistance(10000.0f) (kept, same residue), counted loop instead of memset (inert) | next: per-list inline setter (CSubPrimitiveList::SetRenderPass) called in source order; else lane gap: store scheduling
- 0055c6f0 NUISystem::CMouseCursor::ProcessEvent DIFFER(276v276): VC overlays the four GetRenderTargetDimensions hidden-result temporaries and the movement local in one slot (sub esp,10h); retail gives each its own slot (sub esp,30h) | tried: inline dims helper (346), const-ref temporaries (282: distinct slots but read from the slot), non-POD C2DExtentsI under /EHsc (inert) | next: C2DExtentsI with a user dtor (EH frame state keeps the slots apart); else lane gap: frame-slot packing

## Skipped
- none

## Label corrections
- 0x0099BA70 catalogued CCharString::SetFromFormatV: it is CWideString::Sprintf (wide, Ego_r order); 0x0099BB80 is SprintfDynamic.
- 0x00B84470 catalogued NHeroInformationScreens::CBase::CBase: it is the CEngineSubPrimitiveRenderer ctor (its vtables 0x012A3A60/0x012A3A28).
- 0x0055C6F0 catalogued NUISystem::CMouseCursor::vfunc_1: it is ProcessEvent(EEvent) (slot 1 of the observer-base vtable 0x0124C208, `this` adjusted +4).
- 0x00B59710 manifest label CNavigationLayer: it is CEngineComponent::CEngineComponent.
- 0x006A4E40 manifest labels GetNormalisedValue / CParticleSystem_GetNormalisedTime: it is CThingPhysical::GetHealthPercentage() const.
- 0x00A146C0 catalogued as global: static member C3DVector::GetRandomPointWithinUnitSphere (__fastcall).
- 0x00A65480 / 0x00A65720 family: the classes are nested (CMemoryAllocatorVariableSize::CMemoryAreaChunk / ::CMemoryArea).
- 0x013D28D4 / 0x013D28D8 landed as g_LastAllocatorUsage / g_LastAllocatorChunkCounter: they hold the current alignment and its shift.
