# Hand chunk 69 (CQ_MinionCampScript .. CQuickAccessItemButtonGuiDef): 37 of 40 byte-exact

All 37 sources in `src/` grade MATCH / RELOCATION_MATCH with the leaf finish.py writes into the payload, and their
`test/` programs print `<addr>_TEST PASS` under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). Several rows
are renamed (see Label corrections). Identities come from FableWin.pdb names (`ghidra_out/_fablewin_api.tsv`),
rtti-vtables.tsv and landed callers.

## What matched (37)

- **CQ_MinionCampScript::PauseChantTimer** (00da2490): PausedTime from the thing-manager global's slot 90, then the
  script WAIT macro `while (BriarState == 1) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`
  and its trailing terminate test. Retail-offset view: quest members 0x14 below the PDB's, spawn-point lists 4 bytes,
  so BriarState is +0xa8. The `mov ebx,1` shared by the compare and the `true` store is the compiler's.
- **CQ_PrisonRaceScript::PlaceHeroBackInCells** (00ddb450) and **CQ_WizardBattleScript::Init** (00e1a870): string
  literals passed to `const CCharString&` slots (GSI DeactivateQuestLater slot 281, AddQuestRegion slot 271) are
  implicit conversion temporaries. Init uses the same shape as the landed CChestHidingAssassin::Init.
  **EndMission** (00e1d640): plain member calls. The GSI slot names (565 SetTeleportingAsActive, 321
  DisplayQuestInfo, 128 GiveHeroExperience) come from rtti-vtables + manifest.
- **CDynamicObjectHeap<56,16>::Release** (00b48e10, relabelled): the free-block chain at StartBlock.NextEntry
  (+0x398). Written as `next = head->NextEntry; delete head; head = next;` over the member, then `FreeList = 0`.
- **CLocalDetailCacheMap::CQuadTreeElement** RemoveObjectCollections / OnPostRelocation / UnloadStaticContents
  (00bdda50, 00bddcb0, 00bde020): the PDB layout is retail-identical. Bool bitfields at +0x48 give the
  `test byte`/`and byte` forms. `for (i < 4)` over Children[] becomes the `ebx/ebp = 4 ; dec ; jne` counted loop.
  `delete` of a polymorphic pointer is the null-tested `call [vtbl]` with 1. The landscape memory manager is read once into a local.
- **QuadricOptimiserInternals::CQuadric::operator*=(double)** (00ac6ef0): eleven `m *= s` statements give the
  `fld st(0)` per member.
- **C3DVector** DotProduct (00c256a0) / operator+ (00c257e0) and **AddPS** (00c88990), all relabelled from CQuadric.
  DotProduct is `Z*o.Z + Y*o.Y + X*o.X` (the right operand is loaded first). operator+ is a free `__fastcall`
  whose ctor arguments are evaluated right to left. AddPS is the `addps` twin of the landed MulPS_00c889b0.
- **CQuaternion** operator*= (00a88c10): `*this = *this * q; return *this;`. Equals / IsIdentity (00a88c50,
  00a88cb0): `if (fabs(a) < eps && ...) return true; return false;` with `#pragma intrinsic(fabs)`. eps =
  0.0001f @ 0x0129BA3C.
- **CCompositeBlendDesc ctor** (006caba0, relabelled): each CRGBColour is copied by an inline `Set(r, g, b, a)`
  with `(c.R, c.G, c.B, c.A)` arguments. That reproduces the all-loads-then-stores byte copy (A, B spilled to the
  dead argument slot, then G, R) on the first try. The channels sit B, G, R, A in memory.
- **CEngineMeshEffectDistanceFade ctor** (0077c210, relabelled): inline root (RefCount = 1) and base
  (MeshEffect = 2) constructors drop their vtable stores. The float arguments are copied with integer moves.
- **NPlayerGui CQuestInfoElementMiniGameBar dtor** (0064a515, /Os): the size() loop calls RemoveElement on the
  player GUI's quest-info display. The retail vector<long> dtor is `if (first) free(first)` (no clearing stores).
  The inline base dtor must not re-store the base vtable, so the base is modelled `__declspec(novtable)`.
- **CQuestInfoElementTick ctor** (00656c3c, /Os): init list `CQuestInfoElement(scale), Graphic, Text(L""), Ticked`,
  the landed CounterList shape.
- **CQuestManager** (14). The retail view has one-pointer lists, 3-word vectors and 16-byte CVectorMaps:
  ActiveQuests +0x38, CreatureGenerationDisabledQuests +0x40, BetweenStartAndEndScreenQuests +0x4c,
  AvailableQuestCardTypes +0x58, Boasts/AcceptedBoasts +0x68/+0x6c, World +0x7c, WorldDestructed +0x8e,
  NumberOfQuestsBetweenStartAndFinishScreens +0x98. engine/CQuestManager.h (PDB offsets) is not usable.
  - ActivateFeat / SetExpressionPerformed (004af300, 004af3f0): `C3DVector(0,0,0)` is a float temporary whose
    zero stores land after the eight pushes. A call result used as the last argument must be a named local
    first, or the temporary's stores are hoisted above the call.
  - GetAvailableQuestCardDefNameFromScriptName / AddQuestRegion (004af670, 004b3900): retail loads the key's
    string block once, after the empty-list test, and keeps it across the Compare calls. Only a local
    initialised inside an `if (node != head)` guard around a do/while reproduces this. A local before the loop
    is scheduled above the test; a read inside the loop is redone per iteration.
  - AreAllBoastsProcessed, GetMasterScriptDataForScript, OnRegionLoaded/Unloaded (004af9c0, 004afa10, 004afc00,
    004afc30): plain `for (node = head->next; node != head; ...)` walks.
  - OnQuestFinishScreenDisplayed (004b0160): SGI `__find` (`__fastcall`, empty tag by const reference homed in
    the dead quest slot). `last` is a local declared BEFORE the vector pointer (callee-saved registers go in
    reverse declaration order), and the found path is the `if` body so the `return false` tail comes last.
  - SetQuestCardGoldReward / SetQuestCardRenownReward (004b0e50, 004b0eb0): the out-parameter
    `GetInterface(&tc, 0x6c)` template with its bool ignored. The key temporary, the out pointer and the dead
    argument share one slot, so the absent path reads the uninitialised slot exactly as retail does.
  - CRuntimeQuestPersistence::GetQuestMasterDataPersistScript (004b27f0): inline map find
    `(it == last || key < it->first) ? last : it` with `last` a local, found return first. CCharString `<`
    compares blocks: equal false, null key true, null element false.
  - OnWorldDestruction (004b49e0) inlines RemoveAllBoasts. SaveMasterScriptDataToGameState (004b5030) has a
    CCharString plus a 0x2c-byte CPersistContext with no destructor call.
- **CBankFile::FindIndexBySymbol** (009ccdc0, relabelled): a map pointer local, `it != head` found-first.
- **CQuickAccessItemButtonGuiDef** copy ctor (0046ee2d) and virtual `void Copy(const CDefClassBase*)` (0046ef87):
  /Os, every dword moved through eax, the qualified base Copy is direct. **CCreatureAction_BlockBase copy ctor**
  (00855ee0, relabelled): C3DVector memberwise copy ctor (one register per dword).

## Parked for Fable 5.1 (3)

- 00bde080 CLocalDetailCacheMap::CQuadTreeElement::ValidationTest DIFFER(44v49): retail ends `mov ecx,[esi+0Ch] ; test ecx,ecx ; pop esi ; je ; jmp ValidationTest`, the lane emits `mov esi,[esi+0Ch] ; test esi,esi ; jne <body>` (self tail call turned into a loop) | tried: out-of-line member, in-class inline definition + forcing caller, `#pragma optimize("g",off)` | next: lane gap: tail-recursion elimination (retail keeps the self sibling-call jmp)
- 004afbb0 CQuestManager::OnEntityInterrupted DIFFER(72v72): retail keeps `this` in ebx and the thing in ebp (`mov ebp,[esp+0Ch] ; mov ebx,ecx`) and loads the id with `mov eax,[ebp+1Ch] ; mov ecx,[ebp+18h]`, ours swaps ebx/ebp and loads the id through ecx/edx | tried: plain member access (the thing is loaded in the preheader), a `CThing*` local (propagated away), an id-pointer local (entry load, registers still swapped), a pointer local for the list member | next: lane gap: register colouring (try a `CThing&` member-function helper for the two script calls, or the out-of-line `for` with an iterator object)
- 004b3810 CQuestManager::SetCreatureGenerationEnabledDuringScript DIFFER(76v76): `lea eax,[esp+14h]` (retail homes the empty find tag on the LIVE `enabled` slot) vs the lane's `lea eax,[esp+10h]` (the dead `script` slot) | tried: vector pointer declared before / after the cached `last` (the second fixes the esi/edi colouring) | next: lane gap: empty tag temporary over a live argument slot (wave 24l/24n)

## Skipped

None. No row spans two functions.

## Label corrections

Rows:

- 00b48e10 "CQuadTreeElement::RemoveObjectCollections": the correct name is **CDynamicObjectHeap<56,16>::Release**
  (FableWin `?Release@?$CDynamicObjectHeap@$0DI@$0BA@@@EAEXXZ`). The siblings 0x00B48C70 (<12,16>, +0xd8/+0xc0)
  and 0x00B48D40 (<72,16>, +0x498/+0x480) carry the same wrong label.
- 00c256a0 "CQuadric::GetDeterminant" (BSim 0.52): the correct name is **C3DVector::DotProduct** (float).
- 00c257e0 "CQuadric::operator+=": the correct name is **operator+(const C3DVector&, const C3DVector&)**
  (`__fastcall`, as the landed 0x00C62460 uses it).
- 00c88990 "CQuadric::operator+=": the correct name is **AddPS_00c88990** (an SSE four-float add, the twin of
  landed MulPS_00c889b0).
- 006caba0 "CQuestInfoElementBar::CQuestInfoElementBar": the correct name is
  **CCompositeBlendDesc::CCompositeBlendDesc** (its only caller is CTCHeroMorph::AddCompositeTexture 0x0071DC95).
- 0077c210 "CQuestInfoElementCounterList::CQuestInfoElementCounterList": the correct name is
  **CEngineMeshEffectDistanceFade ctor** (vtable 0x0126ADE0).
- 00855ee0 "CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef": the correct name is
  **CCreatureAction_BlockBase copy ctor** (vtable 0x01275F3C).
- 004afc30 "CQuestManager::OnRegionLoaded": the correct name is **OnRegionUnloaded**.
- 004b0eb0 "CQuestManager::SetQuestCardGoldReward": the correct name is **SetQuestCardRenownReward** (+0x30).
- 004b27f0 "CQuestManager::LookupPersistentQuestInfo": the correct name is
  **NQuestManager::CRuntimeQuestPersistence::GetQuestMasterDataPersistScript**.
- 004b3810 "CQuestManager::SetCreatureGeneratorState": the correct name is
  **SetCreatureGenerationEnabledDuringScript**.
- 009ccdc0 "CQuestManager::OnQuestStartScreenDisplayed": the correct name is **CBankFile::FindIndexBySymbol**.

Callees:

- 0x00CB7D60 "CScriptManager::FindScriptBinding" is **CScriptBase::IsEntityScriptBound**.
- 0x00CB7BC0 "NavQuadTree_MarkNodeActive" is **CScriptBase::OnScriptedEntityInterrupted(unsigned __int64)**.
- 0x00CB7B00 / 0x00CB7B60 "CScriptBase::KillSpawnedFunction" are **CScriptBase::OnRegionLoaded /
  OnRegionUnloaded**.
- 0x0099E900 "less<CCharString>::operator()" is **CCharString::operator!=**.
- 0x004B8E40 "VectorFind" is the SGI `__find` for CCharString ranges.
- 0x004B8770 is vector<CCharString>::erase.
- 0x004B8C00 "CGuiControlTreePane::SortTreeRecursively" is list<CBoast>::clear.
- 0x0049D870 "GFGetPreMainMemoryUsed" is GFGetCurrentFrame.
- 0x00656795 "CVsyncCallbackTimerInfo" is the NPlayerGui CQuestInfoElement(float) constructor.
- 0x00431ED0 "CParentDefClassBase::operator=" is called as the base `Copy(const CDefClassBase*)` (no return
  value is used).
- 0x00E1D7B0 "DoMission_WizardBattleMazeDeath" is an unverified label, kept descriptive.
