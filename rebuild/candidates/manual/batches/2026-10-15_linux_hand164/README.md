# Hand chunk 164 (CTCGraphicAppearance .. CTCHeroStats): 27 of 40 byte-exact

## What matched (27)
- **CTCLight** (row labelled ~CTCGraphicAppearanceMultipleStaticMeshes) 0x640F80 `~CTCLight`: unregister from the owner appearance, member dtors in reverse (vector frees, CDefPointer, CCountedPointer). The CDefPointer dtor needs the object in a local (`p = Object; if (p) {...; Object = 0;}`) or it re-reads the member after the decrement.
- **CTCGuard** 0x7B2BF0 PlayerIsCriminalAndCanApologise: unset-local `GetTC<T>(0x23)` accessor feeding the village getter, nested ifs with one `xor al,al`. 0x7B2C70 ReportCrime: `this` is never read, the opinion component is looked up on the VILLAGE's parent thing. 0x7B2DB0 (label CTCHeroRangedAttacks ctor) is the CTCGuard ctor: PDef + STLport deque in the init list (deque `_M_map` as an `_STLP_alloc_proxy` fixes the `push 0` placement), counters as body assignments.
- **CTCHearabilityManager** 0x7475C0 GetSoundRadiusWithModifiers: two templated `__forceinline` GetInterface lookups, multiplier local preset to 1.0f.
- **CTCInventoryClothing** (label CTCHero ctor) 0x4E7CC3 ctor: `#pragma optimize("s", on)` (push [esp+10h]), secondary listener base at +0x130, the menu vector as `_Vector_base` + alloc proxy.
- **NInventory::CCategory** (label CTCHero::Copy) 0x6255B0 `operator=`: intelligent-pointer assign, out-of-line vector assign, CDefPointer assign, dword/byte copies.
- **CTCCreatureOpinionOfHero** (label CTCHero::ClearMarriageAndRelationshipData) 0x681A70 ClearAllKnownDeeds: inline STLport list clear of PendingDeeds and the 92 KnownDeeds lists (count-down loop).
- **CTCHero** 0x6CA0A0 GetTitleGuiText: reading the title tag twice from the def (test, then argument) gives retail's `mov eax,ecx` copy; the entry's CIVCountedPointer dtor null-tests and calls the inline ReleaseTemporary (ref-count base at +4).
- **CTCHeroAttachableAppearanceModifiers** 0x707970 OnAppearanceClear (secondary-base virtual, `this` at +0xc): the loaded-arm statements go through `CTCHeroAttachableAppearanceModifiers* self = this;`, which keeps the full-object pointer in esi for that arm as retail does.
- **CTCHeroCentre** 0x77D400 RemoveQuestCard: inline IsThingAlive, inline STLport erase with `random_access_iterator_tag()` in the dead `card` slot, virtual-dtor tail jump. 0x77DBC0 GetQuestCards: `if (!empty()) { resize; index loop; return true; } return false;`.
- **CTCTrespasser** (label CTCHeroCentre ctor) 0x7BAE00 ctor: packed class (intelligent pointer at +0x2d), list head allocation, counters as body assignments.
- **CTCHeroCentreDoorMarker** 0x77F5C0 dtor and 0x77F750 OnInitialActivate: `p = CIntelligentPointer<CThing>(x)` temporaries; CIntelligentPointer must have NO user dtor (an empty one stores the vtable before each base-dtor call).
- **CTCHeroExperience** 0x6D66E0 GetTotalStatMaxLevel (0x48-byte ability entries, `if (n < 0) n = 0;`), 0x6D6EE0 OnCreate (slot 4; CDefPointer operator= then a null-tested ReleaseTemporary dtor), 0x6D8B50 SetExperienceStatus (one running index stepped in the for header after `++i`).
- **CTCHeroMorph** 0x71C1F0 KillIdleParticles: the test reads the last node's data directly while the Kill goes through `back()` = `*--end()`; 0x71CD90 Copy: CCountedPointer operator= (source loaded first) + memberwise copies.
- **CThingPlayerCreature** (label CTCHeroOnlineScoreboard::Close) 0x6AA100 OnRegionChange: chained `X = Y = Z = 0.0f` (descending stores, shared integer zero register) and inline map::clear().
- **CTCHeroOpinionDeedLog** 0x6C5680 OnInitialActivate (slot 6): STLport map walk with the static `_Rb_global<bool>::_M_increment(node)` returning the next node and `end()` in a local, then the list walk.
- **CTCHeroPodium** 0x771BD0 OnInitialActivate: `facing * -1.0f` as a literal (an extern constant loads first), `E_X = E_Y.GetPerpendicularXY()` inline const member (reads through the Normalise `this` register).
- **CTCHeroRangedAttacks** 0x789B30 ResetProjectileWeaponMode (four GetPlayer calls, mode pushed before the object call), 0x789C20 OnCreate (slot 4).
- **CTCHeroStats** 0x57A6B8 GetMoralityCategory (`optimize("s")`, `if (m < k/7) return n;` chain), 0x57A7B9 GetDrinkResistance (`optimize("s")`, clamp, `"TEXT_GUI_STATS_LABEL_DRINKERCLASS" + GFIntToCharString(level)` named local, GetTextBySymbol).

## Parked for Fable 5.1 (13)
- 004c04c0 CTCGraphicAppearance::SetAsHighlighted DIFFER(155v155): retail `mov eax,[eax+70h] ... mov ecx,eax` and `mov ebp,[esp+10h]`; ours `mov ecx,[eax+70h]`, `lea edx`, def through ecx then `mov ebp,ecx`; arg loads ecx/edx swapped | tried: out pointer first, mode locals first, CDefPointer with dtor | next: GetSubDef as a template member / thing def through an inline GetDef() accessor
- 004c1060 CTCGraphicAppearance::GetFocalPos DIFFER(126v126): second GetHelperPoint `lea ecx` (retail eax); fallback copy interleaved vs retail's three loads then three stores | tried: nested ifs, early return, Set(x,y,z), copy-ctor temp | next: real GetHelperPoint 0x4C0560 body above under auto_inline(off) (clobber knowledge)
- 004c12d0 CTCGraphicAppearance::GetCameraFocalPos DIFFER(116v116): second GetHelperPoint `lea ecx` (retail eax), copy-out Y/Z via edx/ecx (retail ecx/edx) | tried: `||` + return, `&&` + return GetFocalPos, bool found chain | next: same as 0x4C1060
- 006ca380 CTCHero::SetWeaponsAsUsable DIFFER(105v115): retail tail-jumps (`pop esi ; mov [esp+4],0 ; jmp`) on both arms and re-reads ParentThing into esi; ours `push 0 ; call ; ret 4` with one tail | tried: void callee, `return void-call`, key as the inline's own parameter | next: lane gap: tail call reusing the argument slot after an inlined GetTC (0x77D400's virtual-dtor tail jump does form)
- 00706c90 CTCHeroAttachableAppearanceModifiers::GetMeshesUsed DIFFER(118v118): placement copy loads the third dword into edx before the first store (ours after the second) | tried: named CMeshInfo local, hand copy ctor | next: lane gap: copy scheduling
- 006d6660 CTCHeroExperience::GetTotalStatLevel DIFFER(115v115): retail `push ebx` at entry with the first begin in ebx; ours loads edx and pushes ebx in the non-empty branch | tried: break form, `&&` loop | next: lane gap: callee-saved push placement
- 006d74b0 CTCHeroExperience::GetNextFlourishLevel DIFFER(146v146): first temporary release `dec eax` after `push ebx ; mov bl,[ecx+0D0h]` in retail, before it in ours | tried: split decrement, named bool | next: lane gap: scheduling
- 00856f00 CCombatActionBase::IsThingInStruckThingList DIFFER(121v123): retail loads FramesRunning and the list head first, `push ebx` at entry, ebx as the argument-area pointer, 0 / 1000000 stored from registers | tried: iterator/frame locals, three-argument functor ctor | next: functor built by an inline helper / for_each declared with a by-reference functor
- 0071c2d0 CTCHeroMorph::HasAPermanentTattooCoveringArea DIFFER(158v140): out-pointer release registers swapped (retail ecx object, eax count hoisted above the branch) and ours re-reads the out pointer for the zero test | tried: hand release, Release() member, a no-null-test out-pointer dtor | next: out-pointer dtor through a local copy `o = Object; if (--o->m_refCount == 0) o->Delete();`
- 006c5920 CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses DIFFER(116v117): node read `mov ebx,[eax]` (returned hidden pointer) vs retail `mov ebx,[esp+0Ch]` (frame) | tried: named iterator, const& binding (both drop ebx) | next: find() declared returning the iterator through an explicit out parameter
- 006c5f80 CTCHeroOpinionDeedLog::RemoveCrimeFromVillage DIFFER(132v130): retail loads the village component straight into ecx (`mov ecx,[eax+4] ; mov al,[ecx+7Ch]`) and keeps it for RemoveCrime | tried: GetInterface out pointer, HasTC + check-free GetTC | next: lane gap: looked-up component used as `this` straight in ecx
- 007e0910 CTCHeroSpecialMovement::OnCreate DIFFER(105v105): `mov edi,[esp+8] ; mov ecx,edi` (retail) vs `mov ecx,[esp+8] ; mov edi,ecx` | tried: out->GetSubDef + re-read release, sub-def pointer local | next: lane gap: register choice
- 0057a60a CTCHeroStats::GetFatnessReducingFrequencyInFrames DIFFER(71v116): retail rounds with a bare dword `fistp` after storing/reloading the product and 0.5f/1.0f locals; ours calls __ftol2 | tried: plain C++ round helper | next: lane gap: inline-asm fistp round helper

## Skipped
- none

## Label corrections
- 0x00640F80 "~CTCGraphicAppearanceMultipleStaticMeshes" is `CTCLight::~CTCLight` (vtables 0x0125A194 / 0x0125A208).
- 0x004C1060 "CTCGraphicAppearance::GetHelperPointOr" is `CTCGraphicAppearance::GetFocalPos(float) const` (GetCameraFocalPos calls it).
- 0x004E7CC3 "CTCHero::CTCHero" is the `CTCInventoryClothing(CThing*)` ctor (vtables 0x0124356C / 0x01243558).
- 0x006255B0 "CTCHero::Copy" is `NInventory::CCategory::operator=` (0x2C-byte record copy loop at 0x00625830).
- 0x00681A70 "CTCHero::ClearMarriageAndRelationshipData" is `CTCCreatureOpinionOfHero::ClearAllKnownDeeds`.
- 0x007BAE00 "CTCHeroCentre::CTCHeroCentre" is the `CTCTrespasser(CThing*)` ctor (vtable 0x0126DE14).
- 0x007B2DB0 "CTCHeroRangedAttacks::CTCHeroRangedAttacks" is the `CTCGuard(CThing*)` ctor (vtable 0x0126D80C).
- 0x00856F00 "CTCHeroExperience::GetRoughExperienceLevel" is `CCombatActionBase::IsThingInStruckThingList(const CThingGameObject&) const`.
- 0x006AA100 "CTCHeroOnlineScoreboard::Close" is a CThingPlayerCreature member (probably `OnRegionChange`; only caller 0x004A4DDF).
- vfunc_4 rows 0x006D6EE0 / 0x00789C20 / 0x007E0910 are OnCreate; vfunc_6 0x006C5680 is OnInitialActivate.
- Callees: 0x00663340 ("CEventKilledCreature" ctor) is the thing's hero-combat def getter (CDefPointer by hidden pointer); 0x007EE540 ("IS_THING_ALIVE") returns the village member's CTCVillage*; 0x00527910 ("BuildTreeArray") is an STLport `_Rb_tree::_M_erase`; 0x004AC0F0 is the out-of-line `vector<CIntelligentPointer<CThing>>::erase(first,last)`; 0x006D7430 is GetNextExperienceFallbackMultiplier; 0x006C77E0 is `map<long,COpinionDeed*>::find`; 0x007B4420 is the deque's `_M_initialize_map`; 0x0042AF3C is CDefinitionManager::GetDef(long, CDefPointer*).
