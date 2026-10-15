# Hand chunk 154 (CQ_HangingTreeGoodScript .. CQuestManager): 33 of 40 byte-exact

All 33 src/ files graded MATCH/RELOCATION_MATCH in the final gall.sh run (grades.txt). Every one has a test that prints PASS under g++ (tall_gcc.txt) and under VC7.1 (tall_vc.txt).

## What matched (33)
- **Quest-script constructors** (0xD83400 HeroSoulsMother, 0xD80C30 HeroSoulsBriar, 0xE1A800 WizardBattle, 0xD25410 NewOakValeIntro): the landed 0xE299F0/0xD2F6B0 model. The two argument words come from an inline `__declspec(novtable)` intermediate base. Maps are STLport `_Rb_tree` (header node malloc'd, then `_M_empty_initialize`). Timers use `CTimer() : Index(g_pGameScriptInterface->RegisterTimer())`. Counted-pointer zero words come from an embedded `{Data, Info}` member object. NewOakVale's six zero counters needed an embedded member object with its own ctor: as init-list zeros, the `xor ebx,ebx` moves above the vtable store. The retail map is 12 bytes (PDB 16), so offsets after a map shift by a further 4.
- **Quest-script Inits** (0xD87B20 Nostro, 0xD83470 Mother, 0xD84BB0 Guildmaster, 0xE0B020 UndeadRising): the landed 0xE1A870 form, with two implicit CCharString conversion temporaries built right to left. Nostro's timer reset is an inline `CTimer::Set(t)`. UndeadRising's `bool LightsActivated[8]` clear is an element `for` loop: it gives `lea eax ; xor ecx,ecx ; two dword stores`, while memset stores through esi with the shared zero.
- **QuadricOptimiserInternals::CQuadric** (0xAC6E80 `operator+=`, 0xAC7050 `Initialise`): `m += o.m` per double. Initialise uses locals a, b, c, d with `d = -normal.DotProduct(point)` (the landed Z+Y+X dot order), and every term is written `first * second`.
- **CLocalDetailCacheMap::CQuadTreeElement**: 0xBDDB90 ClearCache inlines RemoveObjectCollections, and its two bit clears merge into `and 0FAh`. 0xBDDC00 is the fuller reset: its three bit clears must be written after the three zero stores to get `and al,0F8h` hoisted with a late byte store. 0xBDDFB0 is the dtor: RemoveObjectCollections out of line, four separate deletes, and an inline empty CMovableResource base dtor (vtable 0x129B860).
- **CQ_HangingTreeGoodScript::RemoveDregs** (0xD6FD00): `while (!empty()) { if (back().IsAlive()) FadeOutAndKillEntity(back(), 0.5f, true, true); pop_back(); }`. pop_back is STLport's `--_M_finish; _M_finish->~T();`.
- **CQ_OpeningGraveyardSecretPassageScript::InitializeVariables** (0xDC3CF0): four `CCharString::operator=(const char*)` calls, then flag stores in retail order.
- **0xDCF4C0** (catalog "FadeObjectUp"): Main of entity-script vtable 0x12DB4B8, in the landed CHTE_Prisoner::Main shape: a scoped CIsThingAlive filter, then one `&&` chain re-testing IsDone.
- **CQ_TraderConflictGoodScript::WatchForKilledPeople** (0xDFC290): a local `vector<unsigned long>` declared first, and every terminate check is an early return. The check after the loop keeps its call but loses its test.
- **CQ_MinionCampScript::GetRandomSpawnPoint** (0xDA0E40): `rand() % points.size()` with an out-of-line size() at 0xDA0ED0, and an indexed list walk that re-reads end().
- **CQ_OakValeBanditRaidScript::CanThingSeeAnythingInVector** (0xDC2170): STLport size()/operator[] recomputed every pass.
- **CQuestInfoElementBar ctors** (0x656B1B, 0x64A450): a size TU (`optimize("s", on)` + `("y", off)`) with `__forceinline` CRGBColour copy ctor and Set. PThing (CIntelligentPointer) is byte-packed at +0x39, while `unsigned __int64 ThingUID` keeps 8-byte alignment at +0x48.
- **CQuestManager**:
  - GetBoast 0x4AF7A0, IsBoastTaken 0x4AF810: `||` of inline `operator==` forms; IsBoastTaken compares node string first.
  - SetBoastAsCompleted 0x4AF8F0, SetBoastAsFailed 0x4AF880: the search must be a `__forceinline bool Find(id, name, CBoast*& out)`. Only that form pushes ebx on the non-empty path and stores straight on the hit.
  - GetDayOrNightOnlyStringFromFullSectionName 0x4AF350: GetLength written `p == 0 ? 0 : p->len`.
  - SetQuestWorldMapOffset 0x4AF6D0: the same forceinline bool-finder idea over ActiveQuests.
  - RemoveAcceptedBoastsForScript 0x4B0030, RemoveBoastsForScript 0x4AFFA0: these needed Compare's real body (landed 0x411570, 15 lines) defined above under `auto_inline(off)`, so the key stays in edi across the call.
  - GetFullSectionNameForThing 0x4B1450: `return name + tc->GetSectionNameSuffix("%");`.
  - IsBetween...IgnoringVignettes 0x4B1110: an `&&` chain over the inlined HasTC/GetTC.
  - IsAnyQuestActive 0x4B1080: an unset-local component accessor, plus a counted pointer with inline AddRef/DecRef (`inc/dec [ecx+4]`) and `<= -1`.
  - OnActivateEntity 0x4AFA60: the CScriptThing must be a named local, because retail pushes its frame address.

## Parked for Fable 5.1 (7)
- 00d80cc0 CQ_HeroSoulsBriarScript::Init DIFFER(130v130): retail `mov ecx,[g_pGameScriptInterface] ; mov edx,[esi+68h]`, ours loads the timer index first | tried: CTimer::Set inline, direct call, by-value CTimer arg, interface local in Set | next: scheduling only (the same Set form matches Nostro 0xD87B20); try `PlayedBriarCS = false` after the timers or a two-timer inline helper
- 00d9ec50 CQ_MinionCampScript::IsAssignedAsHeroAttacker DIFFER(147v147): the IsAlive / second IsNull vtable loads use edx/eax where retail uses eax/edx | tried: flat `||` of `&&` triples, inline IsLiveAttacker(attacker, thing) helper (worse) | next: lane gap: register allocation
- 00bddf30 CQuadTreeElement::CQuadTreeElement DIFFER(119v119): retail loads `mov dl,[eax+48h]` before the child zero stores and stores +0x2c after the flag byte; ours loads after +0x04 and stores +0x2c first | tried: flags after/before the children, inline ClearStateFlags() before/after the collection reset | next: scheduling only; maybe the flags are a separate bit-field struct member with its own ctor
- 00987bf0 CQuaternion::operator* (really a 3x4 row-matrix Transform(in, out, w)) DIFFER(156v156): rows 1-2 evaluate the terms z,y,w,x where retail has z,y,x,w (row 0 matches when written y,z,w,x) | tried: written z,y,x,w / y,z,x,w / x,y,z,w, inline Dot3(t) + T*w | next: VC7.1 reassociates the float sums; try the remaining permutations (x,z,y,w / z,x,y,w / y,x,z,w) of rows 1-2
- 00addfe0 CQuaternion::operator* (in-place twin Transform(v, w)) DIFFER(157v157): same rows 1-2 order | tried: Dot3 + T*w, T*w + Dot3, written z,y,w,x | next: as 00987bf0 (fix one, apply to both)
- 004b00c0 CQuestManager::IsQuestRegistered DIFFER(159v154): retail keeps the quest-name local in the dead argument slot and passes __find's empty random_access_iterator_tag at that same slot; ours opens a push-ecx frame word, and the inline `operator==(const char*)` arms are laid out differently | tried: ternary-initialised local, inline by-value GetQuestNameFromSectionName helper | next: lane gap: local + empty tag temporary in a dead argument slot
- 004b1960 CQuestManager::TransferBoastToUI DIFFER(144v141): retail pushes edi at entry, ours delays the push into the non-empty path (everything else matches with the inline CCharString::operator== form) | tried: SameString(block, block) form, operator== form | next: lane gap: callee-saved push placement

## Skipped
None: every row holds one function.

## Label corrections
- 0x00D83470 "CQ_HeroSoulsGuildmasterScript::Init" is **CQ_HeroSoulsMotherScript::Init**: vtable 0x12D45F0 slot 3, registers "Q_HeroSoulsMother" (the Guildmaster Init is 0xD84BB0).
- 0x00DCF4C0 "CQ_OpeningGraveyardSecretPassageScript::FadeObjectUp" is slot 1 (Main) of entity-script vtable 0x12DB4B8. Its factory is 0xDCF3D0 (0x1c bytes). There is no RTTI or name string, so it is named CVtable_012DB4B8::Main.
- 0x004AF880 "SetBoastAsCompleted" is **SetBoastAsFailed** (it sets CBoast::Failed +0x19). 0x004AF8F0 is the real SetBoastAsCompleted.
- 0x004AFFA0 "RemoveAcceptedBoastsForScript" is **RemoveBoastsForScript** (it walks Boasts +0x68). 0x4B0030 is the AcceptedBoasts one.
- 0x00987BF0 / 0x00ADDFE0 "CQuaternion::operator*" are 3x4 row-matrix vector transforms (`this` holds 12 floats, and the w component comes from a float argument).
- Callees:
  - 0x00CB8110 "CHeroMorphDef" is CScriptBase::CScriptBase.
  - 0x00656795 "CVsyncCallbackTimerInfo" is CQuestInfoElement::CQuestInfoElement(float).
  - 0x00DA0ED0 "CGuiControlTreePane::SortTreeRecursively" is list<CScriptThing>::size().
  - 0x004B8C00 "SortTreeRecursively" is list<CBoast>::clear().
  - 0x0099E900 "less<CCharString>::operator()" is CCharString::operator!=(const CCharString&).
  - 0x007022E0 "CTCShotDefinitionBase::CanBeHitByMeleeStrike" is the quest-card vignette getter (Data(+0xc)->+0x59).
  - 0x004B8FF0 is STLport `__find` over CCharString (`__fastcall`).
  - 0x00F35B10 / 0x00F35B30 are the entity script's RegisterFilter / IsDone (as in landed CHTE_Prisoner::Main).
