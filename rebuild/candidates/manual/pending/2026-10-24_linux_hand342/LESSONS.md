# Chunk 342 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Callee that never reloads ecx (LTCG):** when retail calls a same-class member in a loop without reloading ecx, put the
  callee's real body in the same TU under `#pragma auto_inline(off)`. VC7.1 then sees that the callee keeps ecx and drops
  the reload too (0x00AC7A90 ContainsAllVertices over HasVertex 0x00AC7990). Try this before parking "ecx kept across a
  call" as a lane gap.
- **STLport tree walk register pairing:** a static `_M_increment(node)` that returns the next node, used as
  `_M_node = _M_increment(_M_node)`, gives retail's copy-in-eax / `mov esi,eax` walk (0x00AC7A90). An in-place member
  walk swaps eax/esi.
- **STLport `pop_front()`:** `pop_front()` = an inline `erase(begin())` into `erase(const_iterator)` with a converting
  ctor. It builds the iterator in its push slot and keeps a preceding member store before the slot push. A direct
  `erase(begin())` in the body lets VC sink the store past the push (0x007152F0).
- **`list.push_back(x)` = `insert(end(), x)`:** the iterator needs a user copy ctor for the in-slot build
  (`push ecx ; mov eax,esp ; mov [eax],edx`), and the dead argument slot then receives the discarded hidden result
  (0x00540D70 / 0x00540DA0).
- **Filter temporary after a pointer chain:** to get `push ; mov [esp+N],K` (filter stored after the push), put the
  manager pointer in a named local before the call, rather than building the inline filter temporary inside the
  `a->b->c->Find(Filter(K))` chain (0x007173D0).
- **Accessor vs `.Data`:** an inline `CCountedPointer::Get()` instead of a raw `.Data` read moves a new-expression's
  argument into the reused register (esi rather than edx) (0x00714D30). Also, a pointer compared after a virtual call
  must be read into a local before the call when retail loads it first (ebx).
- **Inline helper for a branch body:** a branch that does "front-action cleanup + clear" on another object
  (`script->...` twice) matches as an inline member of that object (`PTCScript->ClearActions()`). Spelled out in the
  caller, VC hoists the list address (0x00713660). A signed `long` count from an inline `GetNoActions()` gives `jg`;
  STLport `size()` alone gives `jbe`.
- **Slot-2 shot bodies:** a TC body that sits in slot 2 of every derived vtable is the base class's override. Read the
  slots straight from the exe (base vtable 0x0126E1A4) before trusting the manifest's first-hit class (0x007BF350 =
  CTCShotDefinitionBase::OnRemove).
- **Multiple-inheritance NUISystem handlers:** a ProcessEvent in the CObserver vtable (+4) takes `this` = the CObserver
  subobject. The RTTI class hierarchy (CDraggableInto 0x0133AE90) gives the subobject offsets and the base chain. Model it
  like the landed CDraggable 0x0055A510 (0x00558D90).
- **Retail CSubtitleRenderer bools (from the landed 0x0048DB46 layout):** InCutscene +0xDE, CutsceneSkipped +0xDF,
  DisplayingText +0xE5. Seen through g_pPlayerGui (+0x18) they are +0xF6 / +0xF7 / +0xFD (0x00717920, 0x007173D0).
- **Retail NTCScriptedControl::CActionTalkBase is 0x90 bytes:** Ego_r.pdb 0x98, because the debug CArray is 4 bytes
  larger and the tail pads to 8. So: CurrentMessageIndex +0x3C, Conversation +0x40, GameTurnDuration +0x48,
  MakeTargetListen +0x6E, SkippedCurrentLine +0x8F, and CActionTalkToThing::AddedDuringCutscene +0x90.

Review at landing:
- Rows checked with several q.py probe variants beyond the g.sh grades: 0x00714D30 (three accessor spellings),
  0x007152F0 (five erase spellings), 0x007173D0 (three). Each final form is in src/ and grades RELOCATION_MATCH.
- 0x00AC7A90 compiles the stand-in HasVertex body (retail 0x00AC7990) in the same TU. Under the accepted same-TU
  stand-in route the TU holds a second retail address's body, so check that the landing tool keys on the
  ContainsAllVertices leaf only.
- Local models where engine headers exist with PDB offsets that disagree with retail:
  - engine/CThingCreatureBase.h has PCurrentAction at +0x11C; retail is +0x100 (0x007152F0).
  - engine/CScriptThing.h has no virtuals; the slot 11/71/75/76/77 calls need the local virtual model, as in the other
    landed NTCScriptedControl rows.
  - engine/CPlayerGui.h is the PDB view; retail embedded subtitle-renderer bools are used locally.
- Catalog row 0x00644263 holds three 25-byte functions; split it into 0x00644263 / 0x0064427C / 0x00644295.
- Landed 0x0041A4C0 is labelled CUserProfileManager::GetAutoSaveCheckPointFileName but is
  NResourceDirectoryNames::GetUserDataDir. It also binds 0x0041A430 as CUserProfileManager::GetDocumentsFolder. Fixture
  copies of 0x0041A430 / 0x0041A540 sit under rebuild/src/compiled/00/41/; the GetSaveDir one is an inline-assembly oracle, so the new
  C++ should replace it.
- The 0x00AC79xx..0x00AC7Bxx cluster (forge autonames "CList_*", "CList::SetDefaultChild") is QuadricOptimiserInternals,
  not NUISystem.
- Parked 0x00C69A70 used all 4 grades.
