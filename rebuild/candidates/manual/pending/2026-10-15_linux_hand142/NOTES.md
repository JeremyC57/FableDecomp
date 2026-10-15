# Hand chunk 142 (CGameScriptInterface .. CGuiVarTransferStruct): 34 of 40 byte-exact

All 34 sources in src/ grade MATCH / RELOCATION_MATCH (grades.txt) and every test prints PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py, tall_vc.txt). Shared model text for the CGameScriptThing family is in gst/ (prelude.txt +
per-row header/body files); every source is self-contained.

## What matched (34)
- CGameScriptThing component accessors (IsUsable, IsSneaking, GetCurrentScriptPriority, SetFriendsWithEverythingFlag,
  GetActivationTriggerStatus, SetActivationTriggerStatus): `if (PThing.GetPItem() && !PThing.GetPItem()->dead)` (three
  GetPItem calls on the intelligent pointer at +0x0c, no local), then the component through the out-parameter
  `GetInterface(&tc, kind)` (keeps it in eax: `mov eax,[eax+4] ; mov al,[eax+N]`) or, when retail reads the dead key slot
  on the absent path, the unset-local accessor. Get* methods are const (const LowerBound 0x004365B0), Set* non-const
  (0x0040F020). Flag members are `bool` (an `unsigned char` adds `test ; setne`).
- CGameScriptThing constructors (0x008CFF60 / 0x008D0250): the +4/+8 zero words stored BEFORE the class vtable are the
  member of a `__declspec(novtable)` CScriptThing base with an inline constructor; the intelligent pointer is
  `CIntelligentPointer<CThing>(T*)` (SetPItem(NULL)) in one, default-constructed in the other; name assignment and UID
  clear are body statements. GetDefName: a NAMED CCharString local returned by value (copy ctor + dtor, no NRVO).
- CGameScriptInterface::AddCreatureScriptedMode: `modes->AddMode(CCountedPointer<CModeBase>(new CModeScripted(..)))` --
  the by-value counted pointer is built in the argument area only once its copy constructor is DECLARED.
- 0x009E7C00 (catalog "AddGossipFactionToCategory"): CVectorMap<CCharString,CSymbolInfo,...>::Add -- STLport pair temp
  from (key, value) with the implicit memberwise CSymbolInfo copy, push_back, `Dirty = true` (+0x0d).
- CGameScriptThingManager::FrameUpdate: `it = erase(it)` / `++it` sweep over an STLport list with a null-safe
  `GetRefCount() == 1` test; erase runs the counted pointer's member-form destructor (re-read after the decrement).
- CGameTimeManager: FastForwardTimeTo (if/else-if clamp on the FPU parameter, PDB member names); Load(text) is the
  CStringParser + CPersistContext pair of the landed CAnimComponentFlags::Initialise shape.
- The two "IsDaybreak" rows are four-edge position clip tests (CTCInventoryMap 0x005FABA0, CMiniMapDisplay 0x00829D20):
  `>` = `test ah,41h ; je`, `<=` = `test ah,41h ; jnp`.
- CGenericVar / CGenericVarRef::ReadFromParserAsFloat: the landed SLONG item model plus a float accessor; the int arm
  converts through the dead parser argument slot by itself.
- CGetBestIciclePointForTarget ctor: `BestPosScore(-1)` init, TargetPos assigned from `pos + C3DVector(dx, dy, 0.0f) * t`
  -- the LITERAL 0.0f (not an extern constant) gives `fld t ; fmul [0.0f]`.
- CGetBestTargetInDirection ctor: init list + body `TestLine = to - *base; TestLine.Z = 0; if (lenSq > eps*eps) Normalise();`.
- CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: outer `while (!terminating)` around an inner
  `while (valid && !dead) { Pump(); if (terminating) return; }` plus `if (terminating) return; Pump();`.
- NUISystem::CObservable::RemoveAllObservers (catalog "CGraphicBankManager::ProcessLoading"): two inlined STLport
  list::clear loops and a count reset.
- CGraphicBankManager destructor: body loop of inline counted-pointer `Reset()` (Info then Data zeroed), list member
  destructor = out-of-line clear + null-tested free of the sentinel, base destructor tail-jumped.
- CGraphicBankStateBlock::Set: member-by-member assignment in source order, ResourceIsSprite assigned twice.
- CGraphicDataBank::GetGraphicMaxWidth/Height: inline bounds-checked getter (NULL out of range, no guard after),
  `if (max <= v) max = v` (the `>=` spelling swaps the compare operands).
- CGraphicFrame: 0x0041C0E0 is operator= (release, then share + inc); 0x00B2ED60 is the `??_G` of the non-polymorphic
  class -- needs a `__forceinline` destructor plus a `delete` helper.
- CGraphicInfo::Sizeof: frame-data Sizeof as `p ? p->Sizeof() + 4 : 4` (the `(p ? .. : 0) + 4` form folds the 4 away)
  and the frame record's 0x10 as a default-initialised local in a `__forceinline` helper.
- CGraphicInfoFrame::BeginLoading: rotated `while (FrameNum > 0 && info->flat) frame = info->Frames;`, the
  CIVCountedPointer assignment from `new` = inline Reset through a LOCAL pointer copy, then the store.
- CGroundNavigatorExternalInterface: GetPThingToNotAvoid (unset out-parameter for the creature, null-initialised one
  for the other thing declared before `if (other)`, then `if (!p) return 0;`); the copy constructor (out-parameter
  lookup in GetPNavigatorObstacles, inlined find + STLport push_back with a prev local).
- NGameText::CGroupEntry ctor: size = local `sizeof(*this) + capacity*4` first, then `+ group.Sizeof() - sizeof(group)`
  with the counted pointer's Sizeof a ternary keeping +8 in both arms.
- CGuardToHelpDefend::Init: the village name is a NAMED CCharString in its own block (a temporary is built after the
  interface vtable load); nested `Interface->Attach(Me, Interface->GetThingWithScriptName(name))`.

## Parked for Fable 5.1 (6)
- 008d0190 CGameScriptThing::FrameUpdate DIFFER(112v112): register swap only -- retail keeps this in edi and &PThing in esi, ours the reverse | tried: direct member chain, inline CThing GetPosition/GetAngleXY accessors (fixed the vcall load), inline IsThingAlive member | next: lane gap: register allocation
- 008a7d60 CGameScriptInterface::GetThingWithScriptName DIFFER(135v151): retail keeps the vector start in edi with ebx as zero and gives each return its own epilogue (null path `cmp edi,ebx ; je` before free, found path frees without a test); ours merges both returns into one null-tested free | tried: if(empty) first, if(!empty) first, STLport _Vector_base split | next: a found-path form where VC can prove the start non-null (e.g. `CThing** first = things.begin(); if (first != things.end()) { CThing* t = *first; ... }`), or a dtor without the null test plus an explicit test on the empty path
- 0070d100 CGraphicBankManager::ProcessLoading DIFFER(159v158): retail homes both the pushed `&entry` value and the empty insert tag in the dead `out` argument slot (no frame); ours adds a `push ecx` frame word for the value, shifting the loop pad | tried: rvalue cast argument, __forceinline by-value helper, named local | next: lane gap: two temporaries sharing one dead argument slot (as 0x005383DD)
- 009fd700 CGraphicInfo::CGraphicInfo DIFFER(126v126): scheduling only -- retail emits `push ebp ; mov ebp,[esp+10h]` after the PixelFormat/bit-field loads and `push edi` after the bit-field store; ours hoists them | tried: init-list header ctor, chained body assignments (reverses the stores), separate body assignments | next: lane gap: prologue push scheduling
- 006cdd50 CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface DIFFER(176v155): body and loops match; retail pushes edi in the prologue (before the mask test) so every path shares one epilogue, ours pushes it after the null test and emits separate early-out epilogues | tried: STLport list::remove, find + while(next != end), for(;;) with an explicit next test, for(;;) with the single find | next: route the absent-component path to the same exit (drop the separate `if (obstacles)` split or move the sweep into an auto_inline(off) helper defined above)
- 00aeef40 CGuiVarTransferStruct::Add DIFFER(172v146): retail keeps the counted-pointer temporary in registers (Data edi, Info esi) through the in-place copy, the reference bump and the release although its address goes to the insert; ours re-reads both words from the frame everywhere | tried: temporary argument, named local, init-list counted-pointer ctor | next: define the insert (0x00AF0FA0) above under auto_inline(off) so VC knows it does not write the value, else lane gap: store-to-load forwarding of an address-taken local

## Skipped
None (no row spans two functions).

## Label corrections
- 0x009E7C00 "CGameScriptInterface::AddGossipFactionToCategory" (propagated): CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<..>>::Add (vector at +0, Dirty +0x0d); no direct callers.
- 0x009E7AB0 "Vector_Append_CCharString": push_back of the 16-byte pair<CCharString,CSymbolInfo>.
- 0x005FABA0 / 0x00829D20 "CGameTimeManager::IsDaybreak" (propagated): four-edge position clip tests of CTCInventoryMap and CMiniMapDisplay (Z edge 51.0, Y edge 4740.0).
- 0x0055C950 "CGraphicBankManager::ProcessLoading" (propagated): NUISystem::CObservable vtable 0x01230044 slot 6, clears both observer lists.
- 0x0041C0E0 "CGraphicFrame::UnInitialise": CGraphicFrame::operator=; 0x00B2ED60 "CGraphicFrame::UnInitialise": CGraphicFrame's `??_G` scalar deleting destructor.
- 0x00861E00 "CTCCreatureNavigation::CTCCreatureNavigation": NCreatureMode base-mode constructor (CThing*), used by CModeScripted (vtable 0x01267670); 0x0073E8F0 "CConsole::SendMessageToUser": the scripted-mode info constructor (+0x68).
- 0x009B9890 "CWorld::DrawGetEnvironment" / 0x009B98A0 "CMap::DrawGetSeaLevel": the parsed item's long / float accessors.
- 0x004C73D0 "CVertexBufferWin32::DoSizeof": CThing::GetPosition; 0x00A14440 "C3DVector::GetScaled": C3DVector::Normalise (in place, returns *this).
- 0x009D5620 "CVertexBufferWin32::DoSizeof": the NGameText group's Sizeof (`[ecx+24h] + 48h`).
- 0x005ED860 "GetAllThings<...>": called as the world's (+0x20) script-name search filling a vector<CThing*>.
- 0x009F89B0 "remove_if<...CIsEventOfType>": the CGraphicBankManager entry list's clear.
- 0x0066C3F0 "std::vector<uint>::_Ufill" / 0x00AF0FA0 "Vector_InsertN_uint64Entity_v3": 4-byte / 8-byte vector reallocating inserts (InsertGrow shape).
