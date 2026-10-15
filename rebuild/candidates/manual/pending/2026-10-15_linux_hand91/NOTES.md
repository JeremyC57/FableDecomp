# Hand chunk 91 (CTCHeroStats .. CTCInventory): 34 of 40 byte-exact

All 34 src/ files grade MATCH/RELOCATION_MATCH (grades.txt) and their tests print PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py). The CTCInventory TU is size-optimised (`#pragma optimize("s", on)`); its frame-pointer
bodies (`push ebp ; mov ebp,esp`/`leave`) add `#pragma optimize("y", off)`. Retail CTCInventory keeps its vectors as
12-byte {first,last,end} records (QuickAccessItems +0x148, ContextSensitiveItems +0x154, ContextSensitiveFadeTimes
+0x160, Components +0x0c), so engine/CTCInventory.h (Ego_r.pdb, 16-byte vectors) cannot model `this`; every
CTCInventory source carries a local descriptive model that says so.

## What matched (34)
- CTCHeroStats-labelled rows (all propagated labels, see Label corrections):
  - 00a75460 / 00a754a0 CContainmentVolume::IsContainingSphere / IsContainingPartOfSphere: build the 16-byte
    {C3DVector, float} functor in a `sub esp,10h` frame and call the IsContaining<> template on the unchanged `this`.
  - 00a89320 / 00a89350 C3DMesh2::GetVertexCount / GetTriangleCount: sum a per-primitive count over the 0xa0-byte
    retail C3DPrimitive2 records (retail layout 8/12 bytes below the PDB).
  - 00c3c450 CVtable_012B4F74::Close: slot 1 of a no-RTTI vtable; frees the owned buffer per the state bits.
  - 00c6acc0 CSoundMixer::MixBlocks: `div` of the zero-extended 16-bit volume by the block count, word test of the quotient.
  - 00c74450 / 00c74480 no-RTTI base 0x012BF050 destructor / scalar deleting destructor: shared-table refcount release;
    a non-virtual destructor in a polymorphic class keeps `??_G` out of the TU, a virtual one plus a `new T` factory emits it.
- CTCHitLocations:
  - 004e75d9 constructor ("s"): `fldz ; fstp` per float zero, member objects with inline constructors after the vtable store.
  - 008153c0 GetPCombatFirstHitLocationPosInside: best pointer/priority in ebx/ebp; `!best || prio < bestPrio`.
- CTCInGameMenu-labelled rows:
  - 0047d5f2 CTCInGameMenu::SetPDef and 005bdb4d CTCInventoryBase::SetPDef: GetDef arguments pushed before the
    definition-manager getter that yields `this`; dropped bool result.
  - 008574d0 CCombatActionBase::SetCombatAnimation: vector clear + per-animation lookup.
- CTCInventory (all "s"):
  - 005819aa OnItemHighlighted (slot 36): argument loaded once into edi, pushed before every call.
  - 00581a14 SetQuickAccessAssign: `cond ? assign : false` is the branch-free `neg ; sbb ; and` mask.
  - 00581a3b / 00581a54 ProcessInventoryInputLeft / Right (slots 40/41): SetQuickAccessAssign stays a call, the
    one-store remove setter inlines, the base call is a sibling `jmp`.
  - 00581a6d ProcessButtonAReleased / 00581b01 ProcessButtonYReleased: out-bool frame (`"y"` off); /Os cross-jumping.
  - 00581c1e StartGameRendering: global display chain virtual slot 76, then world frame stamp through a named local.
  - 00581c78 InitialActivate (slot 5): `for (i < size())` with size() re-evaluated, `= 0` is `and [..],0`.
  - 00581d4e GetContextSensitiveItemButtonIndex / 00581fd1 GetQuickAccessItemButtonIndex: counted loop strength-reduced
    to a walking pointer, `or eax,-1` not-found; 00581fd1 first runs the dropped GetWorld()->GetMainPlayer()->
    m_pItems->GetCount() chain (the landed ValidateIndex 0x005819F2 model).
  - 00581e33 GetQuickAccessItem: unsigned `index < CS.size()` (`jae`), shared element-read tail.
  - 00581ddb GetQuickAccessItemDef: needs `__forceinline` on GetQuickAccessItem (an /Os TU does not inline it
    otherwise) and a non-POD CDefPointer (user constructors) so the __fastcall GetPDef builds into the hidden result.
  - 00581e6f UpdateContextSensitiveItemFades: item-count bound, 0x1c-byte fade index strength-reduced to `add ebx,1Ch`;
    engine/CFadeInFadeOutBase.h used for the element.
  - 00582027 GetQuickAccessNextFreeSlot: `long result = -1;` before the loop + `break` (not early returns) gives the
    `or eax,-1` prologue and the shared exits.
  - 005826fa IsQuitMenuReceivingInput: by-value CCharString (user copy ctor) built in its stack slot for the virtual
    slot-3 lookup; receiver reached by the derived-to-base `lea ecx,[eax+4]`.
  - 005829cd RemoveContextSensitivePreferredItem: static 12-byte-record vector walked with first/last read once;
    name pushed before the getter that yields `this`.
  - 00583a6d Copy (slot 12): base Copy, then the out-of-line vector<long>::operator= on `other + 0x148`.
  - 00584af6 ClosePCScoreboard / 00584fe3 ClosePCMapMenu: the thing's GetInterface template (bitset test + map
    LowerBound) as a `__forceinline` member with a T** out parameter. Scoreboard ignores the bool and calls
    unconditionally (absent path reads the never-written out var, which shares the key's frame slot); MapMenu
    is `if (GetInterface(..) && p) p->Close()`.
  - 00584edc AssignThingSwappingPC: named local for the slot's previous item, loaded between the pushes.
  - 0058509f OnItemAdded: retail does not reload ecx after GetQuickAccessNextFreeSlot; defining that callee EARLIER
    IN THE SAME TU lets VC7.1 know it preserves ecx (`// Leaf: OnItemAdded`).

## Parked for Fable 5.1 (5)
- 0057de14 CTCHeroStats::ApplyPenaltyForDeath DIFFER(67v67): second def-object load `mov ecx,[eax+0DCh] ; mov eax,[ecx+58h]` where retail keeps eax (`mov eax,[eax+0DCh] ; mov eax,[eax+58h]`) | tried: inline `-abs(...)` args, `-abs` local, `abs` local, raw-value local | next: lane gap: register allocation (scheduling-only)
- 00581cfd CTCInventory::GetContextSensitiveItemGraphicIndex DIFFER(43v45): ours `lea eax,[eax+ecx*4] ; cmp [eax],0 ; mov edx,[eax]`, retail `shl ecx,2 ; cmp [ecx+eax],0 ; mov edx,[ecx+eax]` | tried: POD by-value return, explicit result pointer, pack(1)/pack(2), non-POD CEngineGraphic (fixed the hidden-ecx call and the ebp-0Eh slot) | next: an element accessor that scales the index by itself (Dinkumware `*(begin() + n)` through an iterator class, or byte-offset arithmetic)
- 00581fa4 CTCInventory::GetQuickAccessItemGraphicIndex DIFFER(43v45): same `lea` vs `shl ecx,2 ; [ecx+eax]` addressing as 00581cfd | tried: POD return, explicit result pointer, pack(2), non-POD without "s" | next: same as 00581cfd (one fix lands both)
- 00581d8e CTCInventory::IsContextSensitiveOpinionExpressionAvailable DIFFER(75v77): ours `mov ecx,[eax+edi*4] ; test ecx,ecx`, retail `cmp dword ptr [eax+edi*4],0 ; jle ; mov ecx,[eax+edi*4]` | tried: `&&`, nested if + signed index, no "s", volatile compare (gives the memory compare + reload but folds the address with `lea`, 78v77) | next: the same index-scaling accessor as 00581cfd combined with the volatile-free form
- 00584b42 CTCInventory::ClosePCInventoryMenu DIFFER(70v68): ours `mov eax,[eax+4] ; mov edx,[eax] ; mov ecx,eax ; call [edx+7Ch]`, retail `mov ecx,[eax+4] ; mov eax,[ecx] ; call [eax+7Ch]` | tried: T** out, T*& out, split HasInterface/FindInterface helpers, no "s" | next: lane gap: register choice at a virtual call (sibling rows 00584af6/00584fe3 with the same template match)

## Skipped
- 00581ab3 [CTCInventory::vfunc_59]: the row holds two functions, ProcessButtonBReleased (slot 59, 0x581ab3..0x581ae1) and
  ProcessButtonXReleased (slot 60, from 0x581ae1, `lea eax,[ecx+1CDh]`), with no padding between them (/Os TU).

## Label corrections
- 00a75460 / 00a754a0 "CTCHeroStats::AddHousePosition" -> CContainmentVolume::IsContainingSphere / IsContainingPartOfSphere.
- 00a89320 / 00a89350 "CTCHeroStats::GetNumberOfAbilityCasts" -> C3DMesh2::GetVertexCount / GetTriangleCount.
- 00c3c450 "CTCHeroStats::ProcessEventNotification" -> slot 1 (close/free) of no-RTTI vtable 0x012B4F74.
- 00c6acc0 "CTCHeroStats::ApplyVectorScale" -> the software sound mixer's block mixer (CSoundMixer::MixBlocks, descriptive).
- 00c74450 / 00c74480 "CTCHeroStats::~CTCHeroStats" -> destructor / scalar deleting destructor of no-RTTI base 0x012BF050.
- 005bdb4d "CTCInGameMenu::SetPDef" -> NInventory::CTCInventoryBase::SetPDef.
- 008574d0 "CTCInGameMenu::SetPDef" -> CCombatActionBase::SetCombatAnimation.
- 00581fa4 "GetContextSensitiveItemGraphicIndex" -> CTCInventory::GetQuickAccessItemGraphicIndex (reads QuickAccessItems +0x148).
- vfunc names: 00581c78 slot 5 = InitialActivate, 00583a6d slot 12 = Copy, 005819aa slot 36 = OnItemHighlighted,
  00581a3b / 00581a54 slots 40/41 = ProcessInventoryInputLeft / Right, 00581ab3 slot 59 = ProcessButtonBReleased.
- Callees: 0x00454886 "CActiveFile::operator=" = std::vector<long>::operator=; 0x00562E70 "~CParticleEffect" = the PC
  scoreboard's close routine; 0x005FAD30 "CWADFile::Close" = the map menu's close routine; 0x006869D0 "_Cons_val<...>" =
  CTCBase::GetDefinitions; 0x00686D50 "CViewBase::GetDisplayEngine" = free GetWorld; 0x00445950
  "CShaderPreParser::GetConstantCount" = the main player's 8-byte-record count; 0x00449960 "Catch@0040d959" =
  CWorld::GetMainPlayer; 0x0059310D "MedianCut::GetNewTreeNode" = vector<12-byte record>::erase(iterator).
