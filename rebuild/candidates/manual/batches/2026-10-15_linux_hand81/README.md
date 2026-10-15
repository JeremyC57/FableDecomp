# Hand chunk 81 (CTCAssassinRush .. CTCBoastingPodium): 36 of 40 byte-exact

All 36 sources in `src/` grade MATCH or RELOCATION_MATCH (`grades.txt`). Their tests in `test/` print
PASS under g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`). The four parked rows are in
`differ/`. Most chunk labels are propagated names (see the label corrections below). Classes were
named from the vtable that is stored, from the deleting destructor's vtable slot, or from Ego_r.pdb
layouts that fit every touched offset.

## What matched (36)

- **Destructors labelled "CTCAssassinRush::FinishMove" (5).**
  - 004e2365 `CQuestCardDef::~CQuestCardDef`: frees the map and the RewardObjects buffer, then the
    inline CDefPointeeBase destructor runs. Shape: a `__declspec(novtable)` def class over an inline
    empty `~CDefPointeeBase()`, under `optimize("s")`.
  - 007e4c70 `CTCCreditsUI::~CTCCreditsUI`: novtable over the out-of-line `~CTCBase`.
  - 00bba140 `CEngineInternalPrimitiveParticleGroup::~...`: novtable over an inline
    `~CIVCountedPointeeBase()`. Shape: the root's vtable store with no base call. The list's inline
    destructor is `clear(); free(node)`.
  - 00569c30, an owner destructor: the user body is one `Clear()` call. The members follow, and the
    out-of-line vector destructor is reached by tail jump.
  - 00a833b0, a non-virtual `??_G` of a plain record. It is emitted by a `delete` helper; the leaf is
    `??_G`.
- **CTCAssassinRush / CTCWillEnflame / CTCBattleCharge (5).**
  - SetPTarget x3 (007a4350, 007a7150, 007a9ab0) follow the landed AssignTarget family. The temporary
    CIntelligentPointer sits in an inner block, so its destructor runs before the IsZTargetting store.
    CTCWillEnflame is byte packed (PTarget at +0x21).
  - 007aa480 CancelMove: the parent is read into a local before the accessor call. The unlocked path
    must be the `if` arm, because the early-return form puts the locked exit first.
  - 007aa840 FinishMove(creature): written directly.
- **CTC constructors (11).**
  - 007e35a0 CTCAtmosPlayer and 00771140 CTCThingUIDDummyForSerialisation: zero initialisers stored
    before the vtable.
  - 00782770 CTCBoastingArea: constants as immediates after the vtable.
  - 007b6610 CTCCrateStack and 004d975c CTCAvoidRegionExit (`optimize("s")`): the CIntelligentPointer
    members are inline constructors after the vtable store.
  - 004dda37 CTCTargetingAI: `optimize("y",off)` + `("s",on)`. The list's allocator is a const-ref
    empty temporary in `[ebp+0Bh]`.
  - 004e7513 CTCBoastingPodium (`optimize("s")`): the vector map and the def pointer are member
    objects with inline constructors.
  - 007e57c0 CTCBetting: `Active = false` must be the FIRST BODY statement, because as an
    initialiser it is stored before the vectors.
  - 00781200 CFirePoint: the Ego_r.pdb layout fits exactly. The CCharString copy constructor is
    modelled locally (0x0099EC30).
  - 00656ba2 / 00656bd8 are the two NPlayerGui `CQuestInfoElementCounter` constructors, under
    `optimize("s")`.
  - 007554c0 is a stack record of `CTCLook::OnAppearanceDraw`. A `__forceinline` base constructor
    writes type 8 and initialises an STLport tree; the derived body then writes type 10.
- **CTCBase (1).** 00686aa0 Serialise: `(mode == 1 || mode == 3) && !ShouldBeSaved()` returns early,
  then the slot-1 transfer runs. It uses a local polymorphic model, because the engine header's vptr
  is a plain word.
- **CTCBerserk (3).**
  - 0078be70 GetSuperRumble: `def->x + member` loads the def first.
  - 0078bfc0 ShouldAttackDecapitate: the world accessor 0x00686A80 must be a thiscall MEMBER, because
    ecx stays live and the index goes to edx. The result is a full-register `long`.
  - 0078d210 CancelForCutscene: the parent is read into a local.
  - All three use local models: the retail PDef is at +0x68, not the header's +0x6c.
- **CTCBoastUI (2).**
  - 0056a880 Deactivate: nested ifs, and the GUI Close is a tail jump.
  - 0056a930 AllowUIToOpen: the landed RequestActivate is inlined.
  - Both use local models with the retail offsets (Active +0x18, flags +0x28..+0x2b).
- **CTCBoastingArea (1).** 00782a90 OnKill (slot 14): the state is tested twice, the hero is loaded
  through the player-manager chain, and the dead bit is checked.
- **CTCBoastingCrowdControl (7).**
  - 00783210 slot-1 transfer of "CrowdSizesSuppressed": the false default is homed in the dead flag
    slot.
  - 00783a10 slot 16.
  - Five GetCrowd forwarders (00783bc0, 00783bf0, 00783c20, 00783c50, 00783c80). The three
    no-create forwarders need the parent read into a local first: retail loads `[ecx+4]` before
    `sub esp,8` / `push 0`.

## Parked for Fable 5.1 (4)

- 007e93c0 CTCAvoidRegionExit::vfunc_2 DIFFER(78v78): `mov edx,[esp+8]` (range.first) is loaded after the second iterator's slot copy instead of before `push ecx` | tried: plain EqualRange+Erase, inline map forwarders (both loads hoisted, `mov ecx,esp` addressing), STLport base-class iterator copy constructor, a named first-iterator local | next: lane gap: argument-build scheduling of two by-value 4-byte iterators
- 007e9410 CTCAvoidRegionExit::OnKill DIFFER(78v78): same body as 007e93c0 (slot 14, base call 0x006868F0); the same single load is out of place | tried: plain form, inline forwarders | next: lane gap: argument-build scheduling (as 007e93c0)
- 007e9360 CTCAvoidRegionExit::vfunc_16 DIFFER(91v91): same removal under `if (flag)`, `__forceinline` helper; the key temporary already lands in the dead bool slot; only `mov edx,[esp+8]` is out of place | tried: plain helper (not inlined, 30 bytes), forceinline + forwarders, forceinline plain | next: lane gap: argument-build scheduling (as 007e93c0)
- 0064639c CPlayerGuiThingList_0064639c::RemoveThing DIFFER(69v69): the key temporary's store `mov [ebp+8],eax` is scheduled before `lea esi,[ecx+3Ch] ; mov ecx,[esi]` instead of after | tried: `const CThing* const&` key (binds directly, no copy), a derived-to-base key conversion (temporary and erase-result slot now exact), an inline list Find wrapper (out of line), the same wrapper `__forceinline` | next: lane gap: store scheduling of a conversion temporary homed in a dead argument slot

## Skipped

None. No row spans two functions.

## Label corrections

- 004e2365 "CTCAssassinRush::FinishMove" -> `CQuestCardDef::~CQuestCardDef` (the only caller is ??_G 0x004E2349, slot 0 of vtable 0x01241E44).
- 007e4c70 "CTCAssassinRush::FinishMove" -> `CTCCreditsUI::~CTCCreditsUI` (??_G 0x007E4C50 in vtable 0x012705AC).
- 00bba140 "CTCAssassinRush::FinishMove" -> `CEngineInternalPrimitiveParticleGroup::~...` (called by the ??_G of the particle group classes, e.g. 0x00BBA240 in vtable 0x012A5A54).
- 00569c30 and 00a833b0 "CTCAssassinRush::FinishMove" -> owner destructor / ??_G of unattributed records.
- 007a4350 "CTCAssassinRush::SetPTarget" -> `CTCWillEnflame::SetPTarget`.
- 007a7150 "CTCAssassinRush::SetPTarget" -> `CTCBattleCharge::SetPTarget`.
- 00656ba2 / 00656bd8 "CTCAtmosPlayer::CTCAtmosPlayer" -> `NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter` constructors (vtable 0x0125A4F0).
- 00771140 "CTCAtmosPlayer::CTCAtmosPlayer" -> `CTCThingUIDDummyForSerialisation` constructor (vtable 0x0126A724).
- 004dda37 "CTCAvoidRegionExit::CTCAvoidRegionExit" -> `CTCTargetingAI` constructor (vtable 0x012411FC).
- 00781200 "CTCAvoidRegionExit::CTCAvoidRegionExit" -> `CFirePoint` constructor (no vtable; Ego_r.pdb CFirePoint).
- 007b6610 "CTCAvoidRegionExit::CTCAvoidRegionExit" -> `CTCCrateStack` constructor (vtable 0x0126DA14).
- 00782770 "CTCBed::CTCBed" -> `CTCBoastingArea` constructor (vtable 0x0126B314).
- 007554c0 "CTCBoastingPodium::CTCBoastingPodium" -> a stack draw-request record built by `CTCLook::OnAppearanceDraw`.
- 0064639c "CTCBoastingCrowdControl::OnKillCrowd" -> a player-GUI draw member's list RemoveThing (thunk 0x00437900 `mov ecx,[ecx+1B8h]`).
- 00783bf0 "OnHeroApproachPodiumArea" -> OnHeroEnterPodiumArea; 00783c20 "OnHeroAttackCrowdMember" -> OnHeroLeavePodiumArea; 00783c50 "OnHeroAttackCrowdMember" -> OnHeroBoast (each named after the crowd callback it forwards).
- 007e93c0 "OnKill" is CTCAvoidRegionExit vtable slot 2; slot 14 (007e9410) is the OnKill twin.
- Callees:
  - 0x00686D40 "CViewBase::GetDisplayEngine" = the player-manager accessor (singleton 0x013B8A1C + 0x30).
  - 0x00686A80 = the world accessor (+0x24), a thiscall member.
  - 0x006867F0 / 0x006868F0 / 0x00686910 "$E2" = CTCBase's empty virtual slots 2 / 14 / 16.
  - 0x0083FBC0 "SetStaticCast<...>" = the 00569c30 owner's Clear.
  - 0x00834F90 "SortTreeRecursively" = list clear.
  - 0x007E9AE0 "std::map_Tree_Find_Node" = map equal_range (hidden pair).
  - 0x007832B0 "LTextTree_KillAllThings" = the crowd teardown.
  - 0x00655CDC = SGI `find` (__fastcall), not a vector range constructor.
