# Hand chunk 73 (CScriptGameResourceObjectScriptedThing .. CScriptThing): 38 of 40 byte-exact

All 38 sources in `src/` grade MATCH (4) or RELOCATION_MATCH (34) with the leaf in their `// Leaf:` line and no
flags or pragmas; every test in `test/` prints PASS under g++ (`tall.sh`) and the lane's VC7.1 cl.exe (`tall.py`).
`payload.json` / `oracle.tsv` written by `finish.py`. Most of the chunk's manifest labels are propagated
(see Label corrections); the sources are named after what the bytes do.

## What matched (38)

- **CRetailVector push_back family (8: 0081d620, 00be4380, 00f3b420, 004d18e0, 00543c90, 00bf5fe0, 00abed70,
  00b31c10; labelled CScriptModuleFullTransition / CScriptModuleTransitionBetweenPoints::OnInitLeaf)** -- `this`
  in ecx, `if (last != end) { new (last) T(value); ++last; } else InsertGrow(last, value, SScalarTag(), 1, true);`
  (the landed PushBack family, tag homed in the dead argument slot). SHAPE: the element decides the copy:
  six/seven/eight straight dword moves = an explicit memberwise copy constructor of N longs; a copy that moves
  dword 0, then re-bases both pointers (`add edx,4 ; add eax,4`) and moves the rest = a pair-style record
  `{long key; POD value}` with `first(o.first), second(o.second)` where the value is a PLAIN POD (a memberwise
  value copy constructor folds the offsets instead).
- **CScriptThing (4: GetDefName 004aa8d0, GetDataString 004aa900, GetFocalPos 004aa940, GetHomePos 004aa9a0)** --
  pimpl forwarders: `if (PImp_Data == 0) return g_static; return PImp_Data->Get();` through a local vtable view
  (slots 2/3/5/7). The CCharString default is a copy of the static empty string 0x0143E8EC through the
  out-of-line copy constructor 0x0099EC30; the C3DVector default is the static 0x0143E8E0 copied by an inline
  memberwise copy constructor (x through the x87, y/z through integer registers -- reproduced as is).
- **CScriptGameResourceObjectScriptedThingBase (7 rows labelled so)** -- 00905330 is the real copy constructor
  (CBaseObject copy 0x0099A390 + CCountedPointer member copy with `inc [Info]`); 006a0c50
  (CCreatureAction_SheatheItemToInventory, PFollowUpAction at +0xa8) and 009775f0 (CCreatureAction_JackScream,
  PModeCountedPointer at +0x114) are the same copy-constructor shape over CCreatureActionBase / CCombatActionBase;
  00cb9c50 is a `{CCharString, CCountedPointer}` pair copy constructor (probably the script-binding map's
  value_type). SHAPE: the counted pointer is a member object with its own inline copy constructor
  (`Data(o.Data), Info(o.Info) { if (Info) ++Info->m_count; }`), so it is scheduled after the vtable store.
  007e7490 GetScriptThing returns a CScriptThing by hidden pointer: the null arm is the inline default
  constructor (out-of-line CBaseClass base, the CScriptThing vtable, PImp zeroed by its member constructor).
- **CScriptGameResourceObjectScriptedThing (5 + the two 0066aexx twins)** -- engine header for `this`, the opaque
  `PThing` reached through a local `CIntelligentPointer_Retail*` view (byte-identical to a member model).
  IsFollowingThing: `if (flag && GetCurrentStateGroupType() == 2) return true; return false;`; GetScriptThing:
  nested null/dead ifs, `return thing->GetScriptThing();` / `return CScriptThing();` (out-of-line 0x008CFF40);
  ClearAllActions / ...IncludingLoopingAnimations: nested null/dead/creature-bit ifs, the creature fetched once
  more into a local (esi) for the calls, the last a tail jump. The constructor 00903710: the implementation
  derives from its own handle class (inline base constructor: CBaseObject call + PImp zeroed, dead vtable store),
  then ScriptInterface, PThing (`SetPItem(0)`), ScriptID / ActionPriority / NullResource=true.
  0066ae10 / 0066ae60 (CGameScriptThing::IsSamplePlaying / IsSampleLoadedAndPlaying, engine header): the
  landed SetToKillOnLevelUnload guard chain with a bool result; `push esi` hoisted above the global test.
- **CScriptInfoManager (00cb5c70, MATCH)** -- containers modelled at retail size (12-byte vectors; CVectorMap =
  vector + empty Compare + bool Dirty). SHAPE: `CurrentScriptID = 1` is the LAST store, an immediate: a body
  assignment (in the initialiser list it is stored first).
- **CScriptManager::FindScriptBinding (00cb7d60)** -- `return m_bindings.Find(name) != m_bindings.end();`
  (`xor eax,eax ; setne al` = long result). SHAPE: the inline Find caches `end()` in a local right after
  LowerBound (`mov esi,[esi+4]`); calling end() at each use compares against memory and forks the tails.
- **CScriptModuleBase-labelled engine rows (5)** -- 00851d30 is the real NCameraModes::CScriptModuleBase::
  SetCoordAxis and 006cb960 is CTCCreatureNavigation::SetNavigationTarget (both MATCH): a whole plain-POD
  record assignment is `add ecx,N` + dword moves through edx. 00c10a30 is the 0x50-byte SNavigationRequest
  constructor (init list around two out-of-line member constructors, 1.0f pair sharing eax); 00c266e0 a
  no-RTTI task object (vtable 0x012B3FDC, 0x78 bytes) embedding that request with its constructor inlined;
  00c1e5a0 a search-client constructor (vtable 0x012B33F0) copying a 20-byte POD and registering with the
  owner's host registry (virtual slot 9; the landed 0x00C1E260 is its destructor).
- **NCameraModes script modules (7)** -- SelfTerminateBase ctor (00852490: WFDuration in the init list,
  `EndFrame = GFGetCurrentFrame() + WFDuration` in the body re-reading the member); OpenGate ctor (00853190:
  the SelfTerminateBase ctor inlined, its vtable store kept, then PGate's converting constructor);
  TransitionBetweenPoints ctors (008524c0 forwards the first argument twice, 00852510 all six, PThing
  default-constructed with no SetPItem); TransitionBetweenPoints::IsFinished (00852090: `if (a && b && c)
  return true; return false;`); ConversationBase::IsThingsOk (008527f0: bool `&&` chain -> full-register 0/1);
  GoToLookInDirection::Update (00851e70, MATCH: POD assignment of GoTo, then `operator+` building a NAMED local
  field by field x/y/z -- a value constructor would evaluate z first).

## Parked for Fable 5.1 (1)

- 00853260 NCameraModes::CScriptModuleSpline::IsFinished DIFFER(38v38): retail `je 85327Ch` after the dead-bit test with the shared `mov al,1 ; pop esi ; ret` exit as its fall-through and the `mov al,[esi+58h] ; test al,al ; setne al` tail last; the lane emits `jne` to a `mov al,1` exit placed after the setne tail (same instructions, block order only) | tried: `if (!p || p->m_dead) return true; return RF != 0;`, `if (p && !p->m_dead) return RF != 0; return true;`, `return !p || p->m_dead || RF;` (bool: full-register 0/1, 42 bytes), nested `if (p) { if (dead) return true; return RF != 0; } return true;` | next: the lander's optimize("s"/"t") sweep, then lane gap: block placement of a shared constant-return exit

## Skipped

- 009cbd30 (CScriptGameResourceObjectScriptedThingBase::AskHeroQuestion): the 96-byte row holds THREE functions
  (0x009CBD30, 0x009CBD50, 0x009CBD70 -- `if (m_pImp at +0xec) jmp [vtable+4 / +0 / +8]` forwarders separated by
  int3 padding); not authored.

## Label corrections

- 0066ae10 / 0066ae60 "CScriptGameResourceObjectScriptedThing::GetScriptThing" -> CGameScriptThing::IsSamplePlaying
  / IsSampleLoadedAndPlaying (forward to CThing 0x00836870 / 0x008368A0, PThing at +0xc).
- 006a0c50 "CScriptGameResourceObjectScriptedThingBase" -> CCreatureAction_SheatheItemToInventory copy constructor
  (vtable 0x0125C83C); 009775f0 -> CCreatureAction_JackScream copy constructor (vtable 0x01297CB4); 00cb9c50 ->
  copy constructor of a {CCharString, CCountedPointer<CEntityScriptBindingBase>} pair (CScriptManager map).
- 0081d620 / 00be4380 / 00f3b420 "CScriptModuleFullTransition::OnInitLeaf" and 004d18e0 / 00543c90 / 00bf5fe0 /
  00abed70 / 00b31c10 "CScriptModuleTransitionBetweenPoints::OnInitLeaf" -> retail vector push_back of 24/28/32-byte
  records (their InsertGrow callees 0x0081C890, 0x00B6D0B0, 0x00F3B330, 0x004D1360, 0x00543B70, 0x00BF5EE0,
  0x00A96B30, 0x00B31B10 carry unrelated labels too).
- 006cb960 "CScriptModuleBase::SetCoordAxis" -> CTCCreatureNavigation::SetNavigationTarget (NavigationTarget +0x1c).
- 00c1e5a0 "CScriptModuleBase::SetCoordAxis" -> constructor of the vtable-0x012B33F0 search client;
  00c10a30 / 00c266e0 "CScriptModuleBase::CScriptModuleBase" -> SNavigationRequest constructor / the
  vtable-0x012B3FDC task constructor (module 0x00C0..; the landed 0x00C1E260 "PeekThingSearchTools" is the
  search client's destructor).
- Callees: 0x0049D870 "GFGetPreMainMemoryUsed" is GFGetCurrentFrame; 0x006D5AE0 / 0x00663600 "_Cons_val<...>" are
  the creature's state-group getter and queued-action clear; 0x006633A0 "_Dest_val<...>" releases the looping
  animation; 0x004C7E50 "GFPredicateAnd<...>" is CThing's CScriptThing factory; 0x008CFF40 "operator&&<...>" is the
  out-of-line CScriptThing default constructor; 0x0099A390 / 0x0099A380 "CBaseIntelligentPointer" are CBaseObject's
  copy / default constructors.
