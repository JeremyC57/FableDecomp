# Hand chunk 502 (CV_SickChildScript::Init .. $E_GlobalConsoleCutsceneName): 30 of 33 byte-exact

33 single-function rows in 0x00EC5420..0x01224CA5: 14 NScript quest/entity-script virtuals, 4 `CPredicate_And::Clone`
instantiations, 2 def `Transfer` overrides, `CFractal::NewFractalData`, and 12 compiler dynamic initializers (`_$E1`).
8 MATCH, 22 RELOCATION_MATCH, 0 retail_asm; 2 DIFFER parked (the streaming-font cache buffer initializers); 1 SKIP (0x00F29F60,
two functions). 22 matched on the first grade, 7 on the second, 1 on the third. Every test passes under g++ and the lane's VC7.1.
The vland dry run reports 30 wins with no RELOC_IDENTITY / RELOC_DUPLICATE; its WARN lines are listed under "Review at landing" in
LESSONS.md (SHARED_HEADER_AVAILABLE for data-only headers, RELOC_CONFLICT on the TU-local `_$E2` atexit thunks, RELOC_NAME on junk labels).

## What matched (30)

- Entity scripts {vptr, Game +4, CScriptThing Me +8, ParentClass +0x14, PMasterData +0x18} (Ego_r.pdb offsets) and level scripts
  (CScriptBase 0x40, Game +0x40, members at Ego_r - 0x14): one shared model, interface slots from ghidra_out/cgsi_slots_aligned.tsv.
- `CPredicate_And<...>::Clone`: `new CPredicate_And(*this)` with a `__forceinline` copy ctor (a plain inline one is called out of line);
  the three-term copy ctor 0x00EF6F40 stays out of line through a declared explicit specialisation.
- Def Transfers: one `context.Transfer(name, member[, default])` per PDB member; the two-argument overload goes through the protected
  static `__fastcall CPersistContext::GetDefaultVal<T>` temporary.
- NewFractalData: the exponent is its own decrementing counter (`-i` folds the index into one variable).
- Dynamic initializers: Leaf `_$E1`, the object defined in the TU with its inline ctor; loops in inline ctors need `__forceinline`
  (0x01224340, 0x01224540); `++count` on a counter defined in the same TU with 0 folds to `mov [count],1` as retail (0x01223EC0);
  0x01224CA5 needs `#pragma optimize("s", on)`.

- 0x00EC5420 NScript::CV_SickChildScript::Init (MATCH): reset the sick-child quest state (OwnerAlive = true, the rest false/0)
- 0x00ECB050 NScript::CV_SickChildScript::CIngredientOwner::Init (MATCH): clear BoughtSpecialStuff/StoleSpecialStuff; if the parent's ingredient owner is dead, remove Me
- 0x00ECD380 NScript::CV_SickChildScript::CWomanToAttract::Init (MATCH): GivenObject = false, TimesMadeLaugh = 0
- 0x00EDAE20 NScript::CV_TempleOfLightScript::Init (MATCH): reset the Temple of Light donation, morality and quest state
- 0x00EE4C60 NScript::CV_TourGuideScript::CTourGuideGuide::Init (RELOCATION_MATCH): give the guide information, make it a permanent friend and reset its waypoint and speech indices to -1
- 0x00EE73F0 NScript::CV_TrophyDealerScript::CTrophyDealerInCave::Init (MATCH): clear Me's information flags
- 0x00EE74E0 NScript::CV_TrophyDealerScript::CDemonDoorFace::Init (MATCH): TimesSpoken = 0; give the door face information, make it persistent and undamageable
- 0x00EECF20 NScript::CGlobal_ToggleTimeDisplayScript::Main (RELOCATION_MATCH): toggle the time display and deactivate the active quest (this toggle script) at once
- 0x00EED0E0 NScript::CalextestScript::Main (RELOCATION_MATCH): test script: wait (one frame at a time) for a region pre-unload message, then idle until the thread terminates
- 0x00EF37F0 NScript::CPersonalScript_EmptyScript::CTestPos::Init (MATCH): CurrentActiveScript = 0, QuestCardNumber = 1; make Me persistent
- 0x00EF5C40 NScript::CPredicate_And::Clone (RELOCATION_MATCH): copy the 3-term `alive && alive && ...` script predicate into a new heap object (`new CPredicate_And(*this)`)
- 0x00EF70B0 NScript::CPredicate_And::Clone (RELOCATION_MATCH): copy the 4-term `alive && alive && ...` script predicate into a new heap object (`new CPredicate_And(*this)`)
- 0x00EF7100 NScript::CPredicate_And::Clone (RELOCATION_MATCH): copy the 5-term `alive && alive && ...` script predicate into a new heap object (`new CPredicate_And(*this)`)
- 0x00EF7160 NScript::CPredicate_And::Clone (RELOCATION_MATCH): copy the 6-term `alive && alive && ...` script predicate into a new heap object (`new CPredicate_And(*this)`)
- 0x00EFA160 NScript::Cguy_scriptScript::Chenchmen::Init (RELOCATION_MATCH): make the henchman an ally of the hero, follow the hero at distance 1 and join FACTION_HERO
- 0x00F03850 NScript::CQ_AmbushTradersScript::CAT_BanditAlly::OnPredicateFail (MATCH): count the kill in the master data and remove the bandit's health bar if it has one
- 0x00F04480 NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::OnPredicateFail (RELOCATION_MATCH): count the kill; if BowerstonePosh is loaded and the success speech is over, flag the spy as exited
- 0x00F1B1B0 NScript::CQ_ArenaScript::CSUMMONED_CREATURE::OnPredicateFail (RELOCATION_MATCH): one summoned creature fewer in the arena; if the hero killed it, 8 more crowd points
- 0x00F261F0 CArenaWaveDef::Transfer (RELOCATION_MATCH): persist the arena wave def: NumWaveCreatures (default 0), the Creatures sub-def vector, ShortWave (default false)
- 0x00F27100 CHeroSoulsCreatureDef::Transfer (RELOCATION_MATCH): persist the Hero Souls creature def: CreatureType and Location (default-valued strings), NumCreatures (default 0)
- 0x00F35DA0 NFractal::CFractal::NewFractalData (RELOCATION_MATCH): store the lacunarity and octave count and rebuild the 128 spectral weights ExponentArray[i] = lacunarity^(-i * fractalIncrement)
- 0x01223EC0 CCharString::$E_NullString (RELOCATION_MATCH): dynamic initializer of CCharString::NullString: the inline default constructor (null string data, one more live string), then its destructor is registered with atexit
- 0x01223FC0 NStreamingFontBankGlobals::$E_g_PendingLoadedGlyphs (RELOCATION_MATCH): dynamic initializer of the 512 pending-glyph records: clear each record's counted glyph pointer, then register the array destructor with atexit
- 0x01224100 NProfileTimer::$E_LogFile (RELOCATION_MATCH): dynamic initializer of the profile timer's log file: an inline CDiskFileWin32 default constructor (base, empty pathname, vtable, not opened), then its destructor is registered with atexit
- 0x01224340 CMatrix4x4::$E_Identity (RELOCATION_MATCH): dynamic initializer of CMatrix4x4::Identity: the inline SetIdentity loop (each column j of the four rows gets 1.0f on the diagonal, 0.0f elsewhere)
- 0x012243B0 _global::$E_DefaultUncompressedPFormat (RELOCATION_MATCH): dynamic initializer of DefaultUncompressedPFormat: the inline CPixelFormat(D3DFMT_A8R8G8B8) constructor, i.e. CPixelFormat::Initialise(21)
- 0x012243D0 CNavQuadTree::$E_BlockedNode (RELOCATION_MATCH): dynamic initializer of CNavQuadTree::BlockedNode: the inline CNavBlockedQuadTreeNode(0, 0) constructor (position (-1,-1), level and layer 0, a blocked leaf), then its destructor is registered with atexit
- 0x01224540 _global::$E_LOCAL_HEAP_CParticle (RELOCATION_MATCH): dynamic initializer of the particle object heap: the inline CDynamicObjectHeap<156,2100>(false) constructor (base, vtable, no growth, every embedded entry pushed onto the free list), then its destructor is registered with atexit
- 0x01224C20 _global::$E_CpuDetect_01224c20 (RELOCATION_MATCH): libpk dynamic initializer: run the CPU capability detection once at start-up unless it has already run
- 0x01224CA5 _global::$E_GlobalConsoleCutsceneName (RELOCATION_MATCH): dynamic initializer of GlobalConsoleCutsceneName: construct the empty CCharString, then register its destructor with atexit

## Parked for Fable 5.1

- 01223f00 `anonymous namespace'::$E_g_StreamingFontCacheBufferData DIFFER(71v88): VC7.1 RTM folds the StreamingFontCacheBufferData_static<32,32> record into .data and emits no `_$E1` (71 bytes graded are the out-of-line ctor) | tried: reference accessor to keep the object, empty user ctors on the entry/base, external linkage, ctor defined after the object (probes fc1/fc2) | next: find what in retail's TU blocks constant folding (a non-inline ctor in another TU? a /Od-ish pragma? the QFE-4035 toolset), or model the record with a member the compiler cannot evaluate statically.
- 01223f60 `anonymous namespace'::$E_g_StreamingFontCacheBufferData2 DIFFER(71v93): same residue for the <64,64> record | tried: as 0x01223F00 (same TU, same probes) | next: as 0x01223F00; land both together.

## Skipped

- 00f29f60: two functions -- a 5-byte `jmp 0x00F29F70` thunk, int3 padding, then a second body at 0x00F29F70 (a def copy-assign that calls CParentDefClassBase::operator= 0x00431ED0). Needs re-cutting into two rows.

## Names

### Proven
Evidence per row is in the source's `// Identity (proven):` line (vtable slot + storing ctor, RTTI, or FableWin initializer order, checked against the Ego_r/FableWin PDB name sets).

0x00EC5420 NScript::CV_SickChildScript::Init
0x00ECB050 NScript::CV_SickChildScript::CIngredientOwner::Init
0x00ECD380 NScript::CV_SickChildScript::CWomanToAttract::Init
0x00EDAE20 NScript::CV_TempleOfLightScript::Init
0x00EE4C60 NScript::CV_TourGuideScript::CTourGuideGuide::Init
0x00EE73F0 NScript::CV_TrophyDealerScript::CTrophyDealerInCave::Init
0x00EE74E0 NScript::CV_TrophyDealerScript::CDemonDoorFace::Init
0x00EECF20 NScript::CGlobal_ToggleTimeDisplayScript::Main
0x00EED0E0 NScript::CalextestScript::Main
0x00EF37F0 NScript::CPersonalScript_EmptyScript::CTestPos::Init
0x00EF5C40 NScript::CPredicate_And::Clone -- 3-term, vtable 0x012F6628 (distinct mangled Leaf per instantiation)
0x00EF70B0 NScript::CPredicate_And::Clone -- 4-term, vtable 0x012F6780 (distinct mangled Leaf per instantiation)
0x00EF7100 NScript::CPredicate_And::Clone -- 5-term, vtable 0x012F678C (distinct mangled Leaf per instantiation)
0x00EF7160 NScript::CPredicate_And::Clone -- 6-term, vtable 0x012F6798 (distinct mangled Leaf per instantiation)
0x00EFA160 NScript::Cguy_scriptScript::Chenchmen::Init
0x00F03850 NScript::CQ_AmbushTradersScript::CAT_BanditAlly::OnPredicateFail
0x00F04480 NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::OnPredicateFail
0x00F1B1B0 NScript::CQ_ArenaScript::CSUMMONED_CREATURE::OnPredicateFail
0x00F261F0 CArenaWaveDef::Transfer
0x00F27100 CHeroSoulsCreatureDef::Transfer
0x00F35DA0 NFractal::CFractal::NewFractalData
0x01223EC0 CCharString::$E_NullString
0x01223FC0 NStreamingFontBankGlobals::$E_g_PendingLoadedGlyphs
0x01224100 NProfileTimer::$E_LogFile
0x01224340 CMatrix4x4::$E_Identity
0x012243B0 _global::$E_DefaultUncompressedPFormat
0x012243D0 CNavQuadTree::$E_BlockedNode
0x01224540 _global::$E_LOCAL_HEAP_CParticle
0x01224CA5 _global::$E_GlobalConsoleCutsceneName

### Probable
0x01223F00 `anonymous namespace'::$E_g_StreamingFontCacheBufferData -- the <32,32> record
0x01223F60 `anonymous namespace'::$E_g_StreamingFontCacheBufferData2 -- the <64,64> record

Both probable: the two StreamingFontCacheBufferData_static instantiations named in FableWin.pdb sit at these initializer positions; the objects are anonymous-namespace, so no PDB public symbol binds them to the address.

### Descriptive
0x01224C20 _global::$E_CpuDetect_01224c20 -- libpk CPU-detect static object

Descriptive because the initializer only runs `if (!g_cpuDetected) DetectCpu();`; no PDB name for the object.
Also descriptive: the counter 0x013BD800 (`g_CCharStringInstanceCount`) and the CFractal::NewFractalData parameter names.

## Label corrections

- 0x0122A5E0: the CCharString::NullString atexit dtor (`_$E2` of 0x01223EC0); labelled `std::_Cons_val<...CParticleMorphs::CEntry...>` (body-identical propagation).
- 0x0122AAD0: the CNavQuadTree::BlockedNode atexit dtor (`_$E2` of 0x012243D0); labelled `std::_Cons_val<...>` likewise.
- 0x0122AC90: the LOCAL_HEAP_CParticle atexit dtor (`_$E2` of 0x01224540); labelled `CTCInventoryItem::ClearCachedDefs`.
- 0x0122AE74: the GlobalConsoleCutsceneName atexit dtor; labelled `_global::AtexitDtor_CCharString_at_0143e91c_0122ae74` (fine as descriptive; it is `_$E2` of 0x01224CA5).
- 0x0040E1E0: landed as `_global::GetDefaultVal`; the PDB has `CPersistContext::GetDefaultVal<CCharString>`, protected static `__fastcall`.
- 0x00C52E40: manifest label `InitCPUCapabilities`; this chunk binds it as `?DetectCpu@@YAXXZ` (descriptive libpk name -- reconcile at landing).
- 0x00F35B30, 0x004ABE90, 0x0099E4B0, 0x0099EBF0: reloc_bindings.tsv holds many invented spellings per address; this chunk uses the PDB ones.
- 0x00A768C0 (landed): its CANavQuadTreeNode model differs from the PDB ctor signature `(const C2DVector&, unsigned char, unsigned char)`.

## Header debt

- engine/CScriptThing.h, engine/CDefClassBase.h, engine/CPixelFormat.h and engine/CNavQuadTree.h are data-only (`__vftable` as a data
  member, no ctors, no virtuals, no statics), so the Clone rows, the two Transfers and the 0x012243B0 / 0x012243D0 initializers keep local
  polymorphic models with the PDB offsets in `// Layout:` lines. vland prints SHARED_HEADER_AVAILABLE (warning, not rejection) for them.
- CDefClassBase.h is 0x25 bytes packed; the derived defs here start at +0x28 (Ego_r.pdb), so the header cannot be a base as written.

## Callees

operator new 0x00BFEA1A (scalar), CScriptThing copy ctor 0x004ABE90, CPredicate_And three-term copy ctor 0x00EF6F40,
CPersistContext::Transfer<long> 0x00410BE0 / <bool> 0x004045C0 / <CCharString> 0x004109A0, GetDefaultVal<CCharString> 0x0040E1E0,
TransferVectorOfSubComponents<CArenaCreatureDef> 0x00F26250, _CIpow 0x00BFEB40, CCharString::UnassignString 0x0099E9B0,
CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CWideString 0x0099AED0, CBaseClassNonCopyable 0x0099A2F0,
CDiskFileWin32::Close 0x0099A920, CPixelFormat::Initialise 0x009E3830, CDynamicObjectHeapBase 0x00F36C60, DetectCpu 0x00C52E40.
