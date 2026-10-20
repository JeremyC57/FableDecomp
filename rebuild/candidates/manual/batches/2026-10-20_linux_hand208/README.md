# Hand chunk 208 (CTCHeroAttachableAppearanceModifiers .. CTCInventoryAbilities): 26 of 40 byte-exact

All 26 are RELOCATION_MATCH under the Wine lane (gall.sh re-run at the end), each with a behaviour test that
passes under g++ and under VC7.1/Wine (tall.sh + tall.py). Eight of the 40 catalog labels were other classes or
other functions (see Label corrections); those rows are named after the real owner.

## What matched (26)

- **CTCHeroStats** (frame-pointer size TU, `optimize("y",off)` + `("s",on)`)
  - 0x0057BAD4 GetRandomSuitName: the landed GetRandomHairName shape with nine names (`names = nameList.begin()` local).
  - 0x0057B327 AddMoney: the purse clamp is an inline `GFLimit(long&, const long&, const long&)` that must be
    `__forceinline` (else it is emitted out of line); `Max(MaxMoney, amount)` of the argument; the scoreboard (0x10D)
    is the unchecked out-parameter GetInterface whose key lands in the dead bool slot.
  - 0x0057BB8D AddNewMarriage: first try -- both UIDs as named 64-bit locals, `GetSex() == 1` for the bool,
    the marriage record a temporary to push_back, two scoreboard updates through one unchecked GetInterface.
  - 0x0057D8BE AddRenown: reading the def through a named `def` local and indexing `m_pFirst[RenownLevel]`
    directly fixed both the load order AND the zero-register CSE of the two ClampMin calls.
- **CTCInGameMenu**
  - 0x0047D483 IsAnySliderReceivingInput / 0x0047D648 IsLoadMenuReceivingInput: copies of the landed 0x0047D537 /
    0x0047D6F2 shapes (by-value CCharString FindChild; list FindChildrenByName), state `== 5 || == 3`.
  - 0x0047D012 ConfirmedVideoOptionsChanged: components handed to the NUISystem manager as their CObserver
    sub-object (+4, converting null tests); ObserveEvent through the +4 base; the timer = -1.0.
- **CTCHeroOnlineScoreboard**
  - 0x0055EE20 OnOkClicked: raw HasTC dword test (interface 0x11) + inline GetTC; same manager shape as above.
  - 0x0055F3B0 UpdateScoreForActivity: the find() iterator must be block-scoped (it shares its frame word with the
    later GFIntToCharString temporary); `!(score < best)` / `!(score > best)` early returns; the debug message
    (`CCharString(...) + GFIntToCharString(activity)`) is built and dropped.
- **CTCInventory**
  - 0x00584157 SetThingAsQuickAccessItem: HasTC + the one-ternary GetTC (component straight into ecx).
  - 0x00584F15 UnAssignThingPC: an action pair {1, empty list} passed by value (out-of-line ctor + copy ctor), a
    CRGBColour local (user ctor => hidden-pointer GetColour) whose alpha is cleared, the manager as a named local
    fetched before the implicit `"..."` -> CCharString conversion temporary.
- **CTCInventoryWeapons** (catalog: CTCInventoryAbilities) 0x005C39F0 destructor: `PX = 0;` (implicit conversion
  temporary, re-addressed with `lea`) for both weapon pointers; `pack(1)` on the intelligent-pointer classes only
  (PActiveRangedWeapon at the PDB's odd +0x141); member dtors in reverse; the screen base's inline dtor re-stores
  0x0123A3E0 before the primary base dtor.
- **CTCInventoryAbilities** 0x005EE6BF GetAbilityMaximumLevelWithRenown: the morality local declared BEFORE the max
  (frame-slot order) and two separate `if (...) return max - 1;` tests (one `||` inverts the first block).
- **CTCHeroOpinionDeedLog** 0x006C5E90 IsDeedWitnessed: the GetAllThings template is instantiated in this TU, so a
  stand-in body above the caller (`auto_inline(off)`) is what keeps the list header in edi; the list's in-class
  destructor is inlined on the `return false` path and called out of line on the early `return true`.
- **CTCHeroExperience** 0x006D8E30 OnSerialise (catalog vfunc_1, vtable slot 1): three `Transfer(name, long&, 0)`,
  two TransferVector, size-guarded resizes, `if (!context.IsSaving()) UpdateAbilitiesForAllStats(true)`.
- **CTCCameraPointScriptedSpline** (catalog: CTCHeroRangedAttacks ctor) 0x0075DD10: STLport list/set header nodes
  built inline (`head = 0; node = malloc; link; head = node`, `_M_empty_initialize` re-reading the header), the
  remaining scalars and `SelectedKeyCamera = KeyCameras.end()` as body assignments.
- **CTCHeroPodium** 0x00771DE0 FrameUpdate: `dx*dx + dy*dy < radius` in one expression into a bool, the podium thing a
  named local (spilled), the message manager a named local; the radius is its own .rdata 1.0f (0x0126A88C).
- **CTCHighlightItem** 0x0077CBE0 DoStartRunning: GetSubDef stand-in body above the caller keeps the def in edi;
  the emitter stored through the landed stack-CIntelligentPointer assignment shape.
- **CTCHeroRangedAttacks** 0x0078A2C0 LoadRangedWeapon / 0x0078A5D0 AttemptToFireRangedWeapon: unchecked
  out-parameter GetInterface (carrying, 0x46), inline IS_THING_ALIVE, masked HasTC word (`test ah,1` / `test ch,1`)
  and the CONST lookup (0x004365B0) for the weapon; the load action is a temporary bound to SetCurrentAction's
  `const CCreatureActionBase&`.
- **CTCInfoDisplay** 0x007E1B10 OnSerialise: inline by-value `EmptyString()` defaults (build, copy, destroy) and
  `0.0f` literal defaults landing in the dead context slot.
- **CTCHitLocations** 0x008169B0 GetPCombatFirstHitLocationIntersectionWithTriangle: the record array filled with
  indexed `hits[count].X = ...` stores (a reference local moves the strength-reduced base by 4), the triangle's
  vertex pointer read into a named local after the sort; the SGI sort is `/Gr __fastcall` with the empty comparator
  value-initialised.
- **CEngineSkyRenderer** (catalog: CTCHeroRangedAttacks::ContinueAimingRangedWeapon) 0x00B61FF0 BuildStarFieldVB:
  engine/CEngineSkyRenderer.h through a methods struct; a local CDiskFileWin32 with its inline closing dtor; the byte
  count AND the locked pointer as named locals (keeps `StarCount*24` in esi across Lock).
- **AI-steering library** (catalog: CTCInventory::OnCreate) 0x00C21620: stand-in bodies of the three state helpers
  (same TU in retail) let the caller keep `this` in ecx across them; the queries return int (`test eax,eax`).
- **Library class 0x012B4F74 / 0x012B4F84** (catalog: CTCHeroStats) 0x00C3C4E0 SetupEventNotification (slot 2) and
  0x00C3CD00 CreateCombatAction (/EHsc): the class `operator new`/`operator delete` (0x00C3C360 / 0x00C3C370)
  serve both the buffer and the 0x40-byte object; CreateCombatAction's failures must jump to one label
  (`goto failed;`): early returns each get their own EH epilogue and a merged `if (object)` loses the per-branch
  null tests.

## Parked for Fable 5.1 (14)

- 0047dbb8 CTCInGameMenu::CloseInGameMenuPC DIFFER(181v177): this/vector registers swapped (retail edi=this, esi=&Components) and the delete-loop entry CSEs begin (`mov eax,[edi] ; mov ecx,[edi+4] ; sub ecx,eax`) | tried: pointer local, `__forceinline` DeleteAll member, iterator-object operator[] | next: the Max/def-release parts already match; try the loop inside an out-of-line-looking inline helper taking `CComponentVector&`, or the landed IsSaveComponentReceivingInput vector model verbatim
- 0057c239 CTCHeroStats::GetTradeSkill DIFFER(173v173): interface select register -- retail keeps `last` in esi and moves it into eax (`mov eax,esi ; mov ecx,[eax+4]`), ours `jg ; mov esi,eax ; mov ecx,[esi+4]` | tried: out-param GetInterface, HasTC + GetTC (if/ternary/if-else FindEntry selects) | next: the AddRenown lesson (def through a named local + `m_pFirst[i]`), or the const HasTC form of 0x0078A2C0
- 00581b33 CTCInventory::HasScreenshots DIFFER(194v170): retail's biased /GS frame `push ebp ; lea ebp,[esp-1ACh] ; sub esp,22Ch` (8-bit displacements); the lane emits `mov ebp,esp` with 32-bit displacements | tried: y-off+s-on, y-off alone, /O1, /Os (all with /GS) | next: lane gap: biased-EBP /GS frame
- 00582a16 CTCInventory::IsButtonHeldForExpression DIFFER(184v184): register swap only -- retail def in edi, thing / player interface in ebx | tried: plain CDefPointer, `__forceinline` helper, GetDef<T> stand-in body (fixed the def caching), named def object local | next: declare the button helper's thing local outside (function scope) or pass the thing as a helper parameter
- 005840a6 CTCInventory::UseQuickAccessItem DIFFER(177v177): only the IS_THING_ALIVE converted temporary's home -- retail [ebp+8] (dead `slot` argument), ours [ebp-4] (dead component/key word) | tried: GetTC helper, function-scope out pointer, CBaseObject* const& binding (fixed the register), both combined | next: lane gap: frame-slot choice of a converted temporary
- 00591e8c CTCInventory::GlobalUpdate DIFFER(214v214): scheduling only -- retail issues the def release `dec [ecx+4]` after `fmul [ecx+1DCh]`, ours before | tried: operand order, product/division split | next: def copy via a holder whose dtor is `--count` (no null test), or a `GetPDef()` by-value inline getter
- 005ee02e CTCInventoryAbilities::IsAbilityPotentiallyAvailable DIFFER(185v185): ecx/esi swap only (retail: def in ecx, track-vector pointer in esi) | tried: reference-local inner vector (hoists the size), direct `def->StatLevels[i].Abilities`, track reference, vector reference | next: lane gap: register allocation (all four forms give the same instructions)
- 005f4a86 CTCInventoryAbilities::OnCreate DIFFER(172v170): scheduling only -- retail `mov edi,[ebp-4] ; push [edi+84h]`, ours loads eax and copies to edi after the push | tried: named local, holder, index local, `&&` condition | next: GetSubDef stand-in body above (the template is instantiated here; it fixed 0x0077CBE0 and 0x006C5E90)
- 005f6a6f CTCInventoryAbilities::ResetAssignedAbilitiesPC DIFFER(223v216): only the final def release: retail `test ecx,ecx ; je ; dec [ecx+4] ; jne`, ours `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0` | tried: plain, `__forceinline`, local-copy dtor | next: lane gap: inlined null-tested release (GOTCHAS retry 8); or a GetMiscThingsDef stand-in body above
- 006d8300 CTCHeroExperience::AddTrainableStatExperience DIFFER(173v173): one operand order -- retail `lea eax,[ecx+edx]` (scaled index + begin), ours `lea eax,[edx+ecx]` | tried: pointer, reference-local and iterator-object operator[] | next: lane gap: lea operand order
- 006d8980 CTCHeroExperience::UpgradeTrainableStat DIFFER(182v183): retail loads the track's first pointer into ebx before `sub` and keeps the level vector base in edi; ours subtracts from memory and re-reads the base | tried: reference + size(), iterator operator-, named-first size() + levels pointer | next: one attempt left -- the ageing-def read as a named local before the level test, or a `GetMaxTrainableStatLevel(stat)` inline helper
- 007079e0 CTCHeroAttachableAppearanceModifiers::OnCreate DIFFER(234v234): loop allocation -- retail one def register (edi) for the loop and the release and a shrink-wrapped `push ebx ; xor ebx,ebx`; ours `add edi,54h` with a copy in ebp and the zero register hoisted | tried: holder, thing-def local, direct first-pointer index, hand release | next: GetSubDef<CCreatureDef> stand-in body above (template instantiated here; the def then needs no holder)
- 0071ce80 CTCHeroMorph::GetTattooNames DIFFER(238v238): frame only -- retail puts the GetText() string and the `__false_type` tag of _M_insert_overflow in the same dead `names` slot (tag over the live string), ours gives the string its own word | tried: rvalue push_back argument, named string local | next: lane gap: tag temporary over a live slot
- 0071e180 CTCHeroMorph::CTCHeroMorph DIFFER(220v220): scheduling only -- retail pushes the new CMeshMorphSet and sets `mov ecx,edi` before the sixth zero store of its inline ctor | tried: 6-word array, two vectors, body-assigned last word | next: `PMeshMorphSet.Reset(new CMeshMorphSet())` (value-initialising new) or a CMeshMorphSet with a real STLport vector pair

## Skipped

None: every row holds one function (the 0x00B61FF0, 0x00C3C4E0, 0x00C3CD00, 0x0055EE20 ... rows end in int3 padding only).

## Label corrections

- 0x005C39F0 is `CTCInventoryWeapons::~CTCInventoryWeapons` (stores vtables 0x01253FF4 / 0x01253FE0), not ~CTCInventoryAbilities.
- 0x0075DD10 is `CTCCameraPointScriptedSpline::CTCCameraPointScriptedSpline(CThing*)` (vtable 0x0126A0BC; base ctor 0x0075C180 = CTCCameraPointDefinitionBase), not the CTCHeroRangedAttacks ctor (0x00789BB0).
- 0x00B61FF0 is `CEngineSkyRenderer::BuildStarFieldVB()` (Ego_r.pdb; reads StarVBFilePath/StarCount/StarFieldVertexBuffer), not CTCHeroRangedAttacks::ContinueAimingRangedWeapon (that is 0x00789FD0). The landed CEngineSkyRenderer::BuildBuffers (0x00B620A0) declares it as "BuildSolidBaseBand" -- wrong name.
- 0x0047D648 is `CTCInGameMenu::IsLoadMenuReceivingInput() const` (PDB list; looks up UI_MENU_ENTRY_LOAD_CONFIRM), not IsQuitMenuReceivingInput (0x0047D537). The landed 0x0047D6F2 "IsSaveComponentReceivingInput" is probably the PDB's IsSaveMenuReceivingInput.
- 0x00C21620 is AI-steering-library code (callers 0x00C112AC..), not CTCInventory::OnCreate.
- 0x00C3C4E0 / 0x00C3CD00 are library code of the no-RTTI classes with vtables 0x012B4F74 (base; slot 2 = 0x00C3C4E0) and 0x012B4F84 (ctor 0x00C3C660), not CTCHeroStats members; real names unknown.
- 0x006D8E30 "vfunc_1" is CTCHeroExperience::OnSerialise (vtable 0x0125F6C4 slot 1); 0x007079E0 "vfunc_4" is CTCHeroAttachableAppearanceModifiers::OnCreate (slot 4).
- Callee labels: 0x006D6520 is CTCHeroExperience::GetTrainableStatLevel (not CTCInventoryAbilities::GetAbilityLevel); 0x00661FF0 CThingCreatureBase::GetSex; 0x00593666 / 0x005BADE1 / 0x00485815 are GetDef/GetSubDef instantiations (CExpressionDef / CInventoryItemDef / CCreatureDef); 0x0058056F list<CHeroMarriageInfo>::push_back; 0x00485D1A vector<long>::clear; 0x0078AB20 ~CCreatureAction_HeroLoadRangedWeapon; 0x005EE3B3 CTCInventoryAbilities::GetMiscThingsDef; 0x004AB4F0 the out-of-line list<CThing*> dtor; 0x00412330 vector<CWideString>::_M_insert_overflow; 0x00A22F80 is called on a CRightHandedSet (rotation about an axis), not CMatrix3x3::MultiplyByQuaternionRotation; CHeroStatsDef +0xF0 is TradeSkillForGuileLevel and +0xFC StatLevels; 0x006869D0's +0xDC is the CHeroStatsDef.

## Names the landing session must reconcile

- 0x006869D0: my rows call it `CTCBase::GetDefinitionManager()` (it forwards to CThing::GetDefinitionManager 0x004C7990, which landed sources already name so); 15 landed CTCHeroStats-family files call it `GetDefinitions()`, 6 call it GetDefinitionManager. One spelling should win in the sweep.
- 0x0040DCE0: declared `bool __fastcall IS_THING_ALIVE(CBaseObject* const&)` (parked 0x005840A6 only), as the landed CTCVillage::DetachFromDummyVillager does; another landed file says `IS_THING_ALIVE<CThing*>`.
- 0x005F64DD: declared `CycleAssignedAbilities(bool)` (Ego_r.pdb `_N`) in parked 0x005F6A6F; one landed file says `(long direction)`.
- 0x00440C40 `~CRetailHandleVector` (vs landed `vector<CConversation>::~vector`), 0x004DD1C4 `~CClothingItemMap`, 0x0042ABCA `~CComponentList` (vs `~CEntryList`): I used the existing landed spellings that fit the element type.
- 0x006644F0 SetCurrentAction: declared with `const CCreatureActionBase&` (the landed reference spelling); most landed files use the pointer form (same bytes, different mangling).
