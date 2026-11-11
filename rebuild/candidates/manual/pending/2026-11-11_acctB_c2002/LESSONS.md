# Chunk 2002 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2002, 0x007B9150) A thing argument loaded into a callee-saved register BEFORE a by-value CCharString argument is built in its slot
  (`mov esi,[ebp+4]` at the loop head, `push esi` after the ctor) is the inline accessor `GetParentThing()`; `*ParentThing` is read late into
  a scratch register and flips the whole loop's colouring (action_points esi instead of edi).
- (chunk 2002, 0x007B92FF) `Create(def, pos, PeekPlayerManager().GetMainPlayer().GetNumber(), 0, 0, "")` builds the "" temporary before the
  player chain; retail's order (player number in edi, then the temporary) is a named `long player = ...;` before the call. An explicit
  `CCharString("")` argument fixes the order but pushes eax and moves the temporary's slot (all frame slots off by 4).
- (chunk 2002, 0x007B933F / 0x007B9150) With FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND VC7.1 itself reproduces retail's split: GetTC's
  LowerBound is called at the function entry (0x0040F020) and expanded at the two in-loop sites -- try the inline opt-in before any device.
- (chunk 2002, 0x007D8600) `C3DVector offset = a - b;` lets VC7.1 build the result in place; retail's temporary-then-copy (fstp to
  [esp+40h..48h], mov copies to [esp+34h..3Ch]) is `C3DVector offset; offset = a - b;`. A three-argument C3DVector constructor fed two dot
  products evaluates the Y argument first; field-by-field stores keep retail's X-first order.
- (chunk 2002, 0x007D8A09) A one-step eax/ecx/edx rotation at a by-value CWideString argument chain (SetStateAfterYesNo) closed by passing
  the state through the inline PDB accessor `GetGameState()` instead of reading TavernGameState: c2prio --rotation shows the accessor
  removes one rotating temporary before the chain (7 -> 6 temporaries on that line).
- (chunk 2002, 0x007D8570) A tavern game calls the protected CInputProcessGameBase::IsEventButtonPressed (IBE) through its
  CInputProcessTavernGame*; a `using` declaration in the derived class (opt-in) compiles and keeps the base symbol.
- (chunk 2002, 0x006224C0) A local vector passed by const& to a same-TU, earlier-compiled callee (ConstructQuestListPC 0x0061E6D0) keeps
  its _M_finish in a callee-saved register across calls in retail; declaration-only, the vector escapes (memory reloads, different frame
  order). Proven with a stand-in body (RELOCATION_MATCH); the honest fix is landing the pair with the callee's real body first in the TU.
- (chunk 2002, 0x006224C0) A CreateComponent result pushed with a fresh temporary (`mov [esp+1Ch],esi` right before push_back) means the
  local was typed as a derived class (CList*): push_back(const CComponent*&) then copies the converted pointer; a CComponent* local is itself
  address-taken and lives in memory.
- (chunk 2002, 0x00622744) `frame->AddPositionChild(list->FindChild("..."))` loads frame's vtable before the argument call and spills
  frame; retail's order (argument first, `mov edx,[ebp]` after) is a named child local.

Review at landing:
- 0x0074BC30 is a fragment of 0x0074BC20 (SPLITS.tsv): drop it from the queue; the true row is 0x0074BC20.
- 0x006224C0 needs 0x0061E6D0's real body in its TU (pair landing, like the owner's 0x00626E40 / 0x006276F0 decision).
- The landed 0x007D7C40 (FindTarget) and 0x007D8E20 (FrameUpdate) spell FindTarget / PositionMesh wrongly (NAME_FIXES.tsv).
- CInputProcessTavernGame PRESSED_ACCESS: the original access grant is unproven (using-declaration chosen; a friend declaration was not tried).
