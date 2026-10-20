# Hand chunk 236 (CTCWife .. CThingFilter_IsInFOV): 27 of 40 byte-exact

All 27 matched sources grade RELOCATION_MATCH under RTM cl 13.10.3077 with default flags (no /GS, /EHsc or /Oa); two need
`#pragma optimize("s", on)` (0x004EAD07, 0x004E7FD8). Every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (27)
- CTCWife: 0x006CFF10 Unmarry -- first try on the landed Died (0x006D0020) model; local model because the header leaves ParentThing as padding.
- CTCWillEnflame: 0x007A43A0 ctor -- twin of the landed CTCForcePushPower ctor 0x007A54A0; non-const then const GetTC.
- CTCWillLightning: 0x0079F810 SetLevel -- local model (retail layout is the PDB's minus 0xC); the def object loaded once.
- CTH_Trigger_Oakvale_Gossip / CTH_Trigger_Oakvale: 0x00EE06B0, 0x00EE0A20 Main -- landed CHHUndead::Main opening, IsDone() re-tested with a plain return per step; the CreateCreature result is a discarded temporary.
- CTavernGameShoveHaPennyDef / CTavernGameSpotTheAdditionDef: 0x004EAD07 operator= (memberwise assignment written out, floats through eax) and 0x004E7FD8 Transfer (no base transfer, `lea`-pushed member addresses) -- both under `#pragma optimize("s", on)`.
- CTestTheInventoryScript: 0x00EEED20 TestForLampOff -- FireHeart spawned-thread model; the script base needs its vptr; the message is an implicit-conversion temporary.
- NUISystem::CTextSlider: 0x00549A00 ~CTextSlider -- RevertValue's loop shape; an empty allocator tag needs a user ctor.
- CTexture: 0x009FA340 LoadFromBitmapFile (named device local, two separate `return false` tests, C2DExtentsI size copy), 0x009FA4E0 CopyFromTexture (inline GetSurfaceLevel, min by reference; replaces a naked-asm oracle), 0x009FBBD0 CalcAlphaHistogram (inline GetPixelFormat/GetDimensions; rect stored top, left, right, bottom).
- CTextureRenderer: 0x00A3B5F0 SetMediaType -- abs via the intrinsic (cdq/xor/sub); local CVideoSys model (retail Height/Width at +0x84/+0x88, the PDB's at +0x8c/+0x90); two D3DSURFACE_DESC locals; `new CTexture` with an inline zeroing ctor.
- CThing: 0x004C9560 OnCreate (inline GetDefinitionManager copy; 16-byte CVectorMap copied via a same-TU stand-in), 0x004C9690 InitialActivate (conditional-temporary bool).
- CThingAICreature: 0x00831F80 ctor (bit fields as mem-initializers, before the set's header malloc), 0x00830360 ConstructFromParams (first try), 0x008338D0 InitialiseFromDef and 0x00833A70 OnCreate (const GetCreatureDef accessor makes retail reload the def field; brain def through a same-TU stand-in GetDef).
- CThingCreatureBase: 0x00662A00 / 0x00662B10 GetAnimFromName and 0x00662FA0 GetRandomLoadedAnimFromName (releasing def holder, plain "DEFAULT" literal; sub-def/filter order differs per body), 0x00665E90 IsPickPocketable (single shared false exit), 0x00667C00 GetPrefferedCombatDistance (inline raw-pointer GetCombatTypeDef helper), 0x00667310 OnCollision (inline dot with the constant up vector compiles to `(Y + X) * 0 + Z`; knockdown temporary sized 0x13c from the frame).
- CThingFilter_CombatCanCollideOrInteractWithThing: 0x00857250 operator() -- the landed 0x006A8370 sibling body plus the two pass tests.
- CThingFilter_IsInFOV: 0x006A7150 ctor -- FOV assigned in the body; memberwise C3DVector copy ctor.

## Parked for Fable 5.1 (13)
- 0x00830150 CThingAICreature::DrawGetEditDrawInfo DIFFER(241v241): CDefString temporary gets frame 0xc (retail its own slot 0x10) + colour stored R,G,B,A (retail R,A via al then G,B) | tried: 3 source variants (probe/830150*) | next: an inline colour ctor taking (r, g, b, a) so the shared 0xff byte goes through al
- 0x0082D0D0 CThingBuilding::DrawGetEditDrawInfo DIFFER(241v241): twin of 0x00830150 (blue) | tried: the 0x00830150 forms | next: same as 0x00830150
- 0x006A7270 CThingFilter_IsInFOV::operator() DIFFER(260v262): x87 order of the CSE'd squares (retail ZZ+XX then +YY) + one fxch in the dot | tried: ~20 operand-order / inline-helper variants (probe/fov*) | next: lane gap: x87 scheduling
- 0x00978790 CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() DIFFER(286v286): second-thing load v first-score spill, pop esi v fnstsw | tried: 2 statement orders (probe/heur*) | next: lane gap: scheduling
- 0x0097ABF0 CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() DIFFER(286v286): twin of 0x00978790 | tried: the 0x00978790 source | next: lane gap: scheduling
- 0x00934F20 CThingCompare_NearestGroupByMember::operator()<CThing> DIFFER(281v281): final return-a/return-b folded into a select (retail jumps back to the earlier return-b block) | tried: 8 return-structure variants (probe/ngm*) | next: a single result variable returned at one exit
- 0x00664D40 CThingCreatureBase::GetPCombatTypeDef DIFFER(268v268): register allocation only (eax/ecx/edx roles in the name arm and the final copy-out release) | tried: 3 variants (probe/gpct*) | next: lane gap: register allocation
- 0x00665F90 CThingCreatureBase::PlayCreatureContinuousSounds DIFFER(258v246): key word not shared with the voice local + release as load/dec/store (retail `dec [ecx+4]`) | tried: four release forms (probe/pccs*) | next: CDefPointer release via a raw pointer held in a register (`if (--p->m_count == 0)`)
- 0x0082D9C0 CThingBuilding::IsDoorBlockedBy DIFFER(312v308): door component spilled into its own slot (retail shares the first key word) + squared distance zz+xx then yy | tried: 2 variants (probe/idb*) | next: inline C3DVector operator- then LengthSquared on the difference
- 0x0080DC90 CThingFilter_IsEvadeExplosion::operator() DIFFER(242v242): x87 keeps X/spills Y in retail, ours the reverse | tried: ctor, memberwise, operator- and reference forms (probe/iee*) | next: lane gap: x87 scheduling
- 0x00863280 CThingBodyReorienter::GradualReorientBody DIFFER(262v290): retail keeps the difference in a frame float, the delta on the x87 stack; /Op spills both (318) | tried: default and /Op (probe/grb*) | next: a function-scope `float` for the difference only through a same-TU helper taking it by reference
- 0x00A6AAA0 CTextureManager::Initialise DIFFER(304v306): register allocation only -- this/specs in edi/esi (retail esi/edi) and `xor`-zero register for BufferSize = 0 (retail immediate) | tried: named size locals, if-block body, direct specs[i] indexing (302, worse) | next: lane gap: register allocation
- 0x007E2DF0 CTCWobble::OnInitialActivate DIFFER(260v257): retail is ebp-framed with the x87 fistp rounding idiom using frame-slot 0.5/1.0 constants | tried: C++ rounding helper | next: lane gap: inline-asm (`__asm` rounding helper)

## Skipped
None (no row covers two functions).

## Label corrections
- 0x004EAD07: catalog "CTavernGameDef::operator=" is CTavernGameShoveHaPennyDef::operator= (slot-19 thunk 0x004EAD02, vtable 0x01242524).
- 0x00830150 / 0x0082D0D0: "vfunc_25" is DrawGetEditDrawInfo (CThingAICreature / CThingBuilding).
- 0x00EE06B0: catalog "CTH_Trigger_Oakvale::Main" is CTH_Trigger_Oakvale_Gossip::Main (factory 0x00EDF119, name "TH_Trigger_Oakvale_Gossip" 0x012F40D8, creator 0x00EE05C0, vtable 0x012F4304); 0x00EE0A20 is the real CTH_Trigger_Oakvale::Main (factory 0x00EDF0C0, vtable 0x012F4364).
- Callees: CGSI slot 477 = SetThingPersistent 0x0088E5A0 (cgsi_vtable_slots.tsv is shifted there); 0x006C1150 CTCBuyableHouse::SetRented; 0x005DCC00 / 0x005DCD10 random animation picks; 0x004CC290 CVectorMap copy ctor; 0x009F37E0 CSurface::CopyFromSurface; 0x0044C6B0 CGameDefinitionManager::Get; 0x00EEEC00 CTestTheInventoryScript::TestForLampOn; 0x0073A6E0 CTCCreatureModeManager::GetCurrentMode (manifest "CPixelShader::CountReferences" propagated); 0x008679C0 is the CCombatAction_GenericStrikeResponseKnockdown ctor.
- The landed 0x009FA4E0 copy was a naked-asm oracle; src/009fa4e0.cpp is the compiled replacement.
