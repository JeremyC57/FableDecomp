# Chunk 227 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A reference local bound to a member vector reverses the first indexed access's address operands (`[edx+edi]` vs retail
  `[edi+edx]`); using the member directly everywhere matches (00f060d0). A reference local was right for the float vector of
  00d766c0, so try both.
- CScriptThing::operator= written as `{ PImp = other.PImp; return *this; }` over the counted pointer's own inline operator=
  reproduces retail's dead `add eax,4` on a returned temporary (00d74e80) and the `[this+0x7c]` member addressing (00da0b30);
  a flat operator= body over the two words does not.
- When retail expands an inline operator= at one call site and calls the out-of-line copy at the other, write the expanded
  site through a `__forceinline Assign()` helper and the other through the out-of-line operator= (00da0b30; chunk 225 rule
  for predicates applies to assignment too).
- A float local that the source returns through `goto done` to a shared final `return alpha` stays a merged value: VC spills
  it once at entry (`fst [esp]` into the `push ecx` word) and re-loads it on the call path; a direct `return alpha` on that
  path is re-folded to `fld const` and assigning the call result to it moves the reload block (00439ab0).
- `if (!a->F()) { if (a->G() == b) return true; }` keeps a by-value temporary unconditional; the `&&` form adds a
  construction-flag word (`or [esp+N],1` / conditional dtor) (004b1340). The opposite holds where retail HAS the flag
  (004b0d30: `changed = a != b || (r != "" && GetRegionName() != r)`).
- An inline CCharString== whose length+Compare tail is a nested inline `IsEqual(const CCharStringData&)` gives retail's
  separate `xor al,al ; mov bl,al` false exit next to the null-test `xor bl,bl` (004b1340).
- A short-lived callee-saved local (`long number = Number;` read just before a call and compared after it) makes VC push/pop
  ebx around that range only (0048a070); sunk `push ebx`/`push edi` also reproduced in e1d690 and df20f0, so "callee-saved
  pushes are never sunk" is not universal under RTM 3077.
- GetPlayer-style lookups inlined at a call site need the element count cached before the loop (`long count = size();`) or
  VC re-reads finish/start every iteration (00449e60).
- Script waits: `if (!x) { if (term) return; while (!x) { yield; if (term) return; } if (term) return; ... }` gives retail's
  materialised `!x` (neg/sbb/inc) tests and the duplicated rotated condition (00e3a030); a `for (;;) { if (term) return; if
  (x) break; yield; }` drops one test.
- An intrusive slot `operator=(T*)` that releases through the pointee's inline `Release() { if (--RefCount == 0)
  DeleteThis(); }` gives `dec [ecx+4] ; jne` with the object in ecx; open-coding the decrement re-reads the count (00a6a540).
- STLport list `clear()` inlined as `cur = head->next; while (cur != head) { tmp = cur; cur = cur->next; ~T(tmp); free(tmp); }
  head->next = head->prev = head;` with the head member re-read each pass matches (004b4490).
- A chained `member + thing.GetX() + "_" + param` of `__fastcall` operator+ lands its last temporary in the dead argument slot
  of a reference parameter held in a register (00dcda80); block-scoped named string locals share the earlier temporaries'
  word (00dbe3c0).
- CRGBColour by value is built in its argument slot (`push ecx ; mov eax,esp`) with a ctor body `R = r; G = g; B = b; A = a;`
  in D3DCOLOR layout (00df0a50); as a const-reference temporary the same ctor gives the R,G,B,A stores with 0xff in al (00f07a80).

Review at landing:
- Grades used: 00da0b30, 00f060d0, 00439ab0 needed the full 4; 004b1340 3; others 1-2. No row exceeded 4.
- Local models next to engine headers (headers carry PDB offsets): engine/CQuestManager.h (retail ActiveQuests +0x38 ...
  HeroMapNumber +0x90; used by 004b4490, 004b0d30, 004b1340), engine/CPlayerManager.h (16-byte PDB vector; 00449e60, 0044a2a0),
  engine/CPlayerGui.h (retail is 8 bytes lower after the narrower CSubtitleRenderer and 4 per vector; 00439ab0). engine/CPlayer.h
  is used through the CPlayer_Methods carrier (006366b0, 00636e90, 0048a070).
- Module/leaf differs from the catalog: 00e3a030 (CV_BordelloScript::WatchForBordelloStatus, Leaf line), 00bde290
  (CLocalDetailCacheMap::CQuadTreeElement, Leaf line), 00dc25b0 keeps the doubtful catalog class.
- 00439ab0 uses a `goto` to the shared final return (plain C++; the shape needs it).
- 00da0b30 and 00449e60 carry an inline copy of an out-of-line function's body (Assign / FindPlayer) for the expanded sites.
- Errors in ALREADY-LANDED sources: rebuild/src/compiled/00/44/CPlayerManager_GetPlayer_004498c0.cpp and
  CPlayerManager_GetPlayer_00449910.cpp are naked `_emit` bakes (asm_bake purity violation; the body is the FindPlayer loop
  in 00449e60's source); rebuild/src/compiled/00/4b/CQuestManager_DeactivateAllQuests_004b4450.cpp is a naked `__asm` bake;
  global_Catch0040d959_00449970.cpp mislabels CPlayerManager::GetMainPlayer (`GetPlayer(MainPlayer)` tail call). Landed
  sources declaring 0x00487510 as `GetPlayerNumber` should say GetJoystickDeviceNumber.
- 00d80ae0's catalog row holds three things (GetJackSpeech, its jump table, the factory 0x00D80BA0).
