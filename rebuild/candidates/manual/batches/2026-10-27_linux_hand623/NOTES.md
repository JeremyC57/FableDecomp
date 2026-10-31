# Hand chunk 623 (CEngineTreeFilerBase .. CThingManager): 2 of 10 byte-exact

Chunk 623 is a parked-row retry chunk: 10 rows, each with an earlier best attempt of retail length. Every row got a
real attempt with new levers (at most 4 grades per body). Each new lever is recorded in the differ file's third line.

## What matched (2)
- **NUISystem::CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D** (RELOCATION_MATCH, /Os + `optimize("y", off)`).
  It looks up the def index from the startup definitions, the manager's own definition manager (+0x64) or the game
  definitions, creates the component from it and names it through vtable slot 83.
  Shape lesson: write three whole `component = CreateComponent(<defs>.GetDefGlobalIndexFromName(name));` statements,
  one per arm. VC7.1 cross-jumps the identical tails into one shared GetDefGlobalIndexFromName + CreateComponent(long)
  sequence, and that keeps retail's `mov ecx,esi ; push eax`. A defIndex local assigned in the arms, or a ternary
  argument, gives `push eax ; mov ecx,esi`.
  Namespace proven: Ego_r.pdb `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` and the
  long overload `...@J@Z` (0x0041D21B). engine/CManager.h is a global `struct CManager`, so the class is local (header
  debt). CDefinitionManager now derives from engine/CDefinitionManager.h, which clears vland's SHARED_HEADER warning.
- **CTCHeroExperience::GetRunningNumHits() const 0x006D63A0** (RELOCATION_MATCH). It returns the fractional part of the
  combo multiplier (param / perUnit + 1), clamped at 0, divided by perUnit and multiplied by 4.
  Shape lesson: retail's `fld [def+64h] ; fdivr 1.0` reciprocal is VC7.1's own CSE of TWO DIVISIONS by one float held
  in a named local. A hand-written reciprocal (`1.0f / x`, inline helper, named or reference local), or two divisions
  by the member read in place, all give `fld 1.0 ; fdiv [def+64h]`. `this` uses the engine/CTCHeroExperience.h layout
  through the `#define` pattern of the landed 0x006D72D0.

## Parked for Fable 5.1 (8)
- 00bc9b30 CEngineTreeFilerBase::FindNode DIFFER(66v66): retail key=esi / index=edx (`mov esi,[esp+10h] ; mov edx,[esi+ecx*4] ; shr edx,cl ; and edx,edi ; mov eax,[eax+edx*4+1Ch]`), ours key=edx / index=esi | tried: earlier loop shapes, recursion, operand order and locals; this wave the unsigned `1UL << KeyBitCount` mask (landed AddNodeAutoExpandTree GetIndex) and the `return child ? FindNode(child, key) : 0` recursion (not tail-converted, longer) | next: lane gap: register colouring of a loop-invariant parameter against a loop temporary
- 00c73830 CRegistryKey::QueryDWord DIFFER(72v72): retail lays the shared `xor eax,eax ; pop ecx ; ret 8` straight after the RegQueryValueExA `test ; je`, ours after the type `cmp ; je` | tried: nested if, || chain, return &&, ternary, single-case switch, inline bool QueryValue helper, optimize("s") / ("sy") | next: `#pragma optimize("s")` reproduces retail's block order but adds an ebp frame; try a global `--flags /Os` or /O1 with frame omission (not graded: 4-attempt cap)
- 009e1960 GFStringToInt(const char*) DIFFER(74v74): zero-length guard `je` threaded to `mov eax,edi` (retail lands on `cmp al,1`) | tried: earlier hoisted-read forms and the ternary; this wave strlen in the for condition (not hoisted), a char flag with `== 1`, a `long sign` multiply | next: lane gap: jump threading over a constant-known flag (no source form keeps the unthreaded edge)
- 009e1a80 GFStringToInt(const wchar_t*) DIFFER(75v75): same unthreaded `jbe` to `cmp bl,1` | tried: twin levers of 009e1960 (inert there) | next: the same lever as 009e1960 once one is found
- 00c52c40 CCpuInfo::GetCyrixModelName DIFFER(75v75): family-5 block `mov ecx,[ecx+0Ch] ; cmp ecx,2/4` vs retail eax | tried: case order, nested switch (eax but sub/je), explicit else-return, ternary chain, unsigned Model, explicit `return "<unknown>"` closing case 5 | next: compile the sibling AMD namer 0x00C52BB0 (all-eax if-chains with `||` groups) to learn which source form picks eax
- 00be1e20 CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager DIFFER(77v77): zero-temp block copy alternates edx/eax and the `lea ecx,[esi+14h]` comes before `mov edx,eax`; retail sends all four dwords through edx | tried: copy-construct, user operator=, implicit assign; this wave the PDB ctor signature, a pack(1) C4DVector, /Oa, /Ow | next: lane gap: copy-register choice of VC's 16-byte struct assignment
- 007447e0 CTCPerceiveThing::UpdateDistanceFromPerceivedThing DIFFER(77v77): epilogue `pop esi ; fstp st(0) ; pop ebx ; fstp ; fstp` vs ours pops first | tried: four temporary/local shapes earlier; this wave the PDB spellings (same residue) | next: lane gap: epilogue scheduling (scheduling only)
- 00521a90 CThingManager::SetNameForThing DIFFER(78v78): operator[] argument registers are rotated (retail eax / ecx,edx with `mov ecx,esi` between the pushes; ours ecx / edx,eax) | tried: map reference, find in condition, entry/name/key references; this wave the STLport std::map model, a named iterator, the real operator[] body as same-TU noinline, an inline DoesNPCNameEntryExistForThing | next: lane gap: register colouring of argument temporaries

## Skipped
None.

## Label corrections
- 0x00C52C40 "CEngineInternalPrimitiveParticleGroup::GetRenderLayerMask" is CCpuInfo::GetCyrixModelName (descriptive; Cyrix
  model strings, sits between the AMD and Centaur namers, and the landed CCpuInfo::GetIntelModelName / GetCentaurModelName
  share its layout).
- 0x00BE1E20 "CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline" is the
  ??0CEnginePrimitive2DViewportManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z constructor (vtable 0x012A7ED8 + PDB).
- 0x006D63A0 "CEngine::GetEditorAnimation" is CTCHeroExperience::GetRunningNumHits (caller
  CGameScriptInterface::GetPlayerCreatureCombatMultiplierRunningNumHits 0x00899D3E, PDB donor order).
- 0x00C73830 "Registry_QueryDWord" is the descriptive CRegistryKey::QueryDWord, the spelling of the landed CRegistryKey
  siblings 0x00C73790 / 0x00C737D0 / 0x00C73800 / 0x00C739A0. No PDB or upstream source names it: descriptive.
- 0x0041DB1D "CManager::CreateComponent" is NUISystem::CManager (PDB).

## Names and evidence
- Matched rows: both names come from the PDB (Ego_r mangled strings, checked against the object with objdump).
- Callee spellings follow the PDB: CStartupDefinitionManager::Get / CGameDefinitionManager::Get `SIAAV1@XZ`,
  CDefinitionManager::GetDefGlobalIndexFromName `QBEJABVCCharString@@@Z`, CManager::CreateComponent(long) `QAEPAVCComponent@2@J@Z`.
  The pad virtuals v000..v082 in front of CComponent::SetName (slot 83, `UAEXABVCCharString@@@Z` in Ego_r) emit no
  symbol.
- NAME_FIXES.tsv lists the wrong spellings this chunk found in landed code: 0x00BACDB0 (base ctor), 0x0051F010
  (DoesNPCNameEntryExistForThing), 0x0052B8B0 (map::operator[]), 0x00527E00 (_M_find), 0x004C73D0 (CThing::GetPos),
  and 0x00A01B50 (CBaseIntelligentPointer::GetPItem).

## Header debt
- engine/CManager.h: a global `struct CManager` with no namespace and no virtuals; retail is `NUISystem::CManager`.
- engine/CTCPerceiveThing.h: +0x04 `field4` is CTCBase::ParentThing (Ego_r).
- engine/CEnginePrimitive2DViewportManager.h: the base and C2DAlphaMaskInfo are flattened into pads.
