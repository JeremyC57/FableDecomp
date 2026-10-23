# Chunk 318 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Repeated interface copies (`if (HasTC(t)) dst->AddAndCopyTC(GetTC(t), ...)` x8): use ONE shared `const CTCBase* tc;` and an out-param `GetInterface(&tc, type)` with the presence-bit test inside. Then each `->component` load lands in eax (`mov eax,[eax+4]`). A value-returning inline GetTC rotates edx/ecx/eax across the copies, and so do the ternary, if-assign, `find()->component`, reference-return and per-call out-param forms. Seen at 0x0089F4A0; same idiom as the landed 005b37f7.
- Inline CScriptThing accessors with a global fallback (GetDataString -> empty string 0x0143E8EC, GetPos -> zero vector 0x0143E8E0): write `return !PImp.Data ? g_Global : PImp.Data->Slot();`. The `Data ? ... : global` spelling inverts the branch (`je` + call + `jmp`) (0x00D71B30).
- When the only residue is ONE jump target in an else-if chain, look at the block it targets in retail: a tail-merged jump into another arm's trailing store means that arm ends with the same store. In 00d71b30 the MoveToPosition arm also does `ReturningToParty = false`.
- A `CScriptThing exitPoint;` local whose default ctor calls the out-of-line CBase ctor 0x0099A2D0: SUB script.inc's inline `CBase() { }` to a declaration and add `CScriptThing() { }`. The vptr store plus the zeroed PImp then follow the call, as in retail (0x00D71B30).
- mk.py `//#SUB` replaces EVERY occurrence and runs in file order. To move C3DVector ahead of the generated CScriptThingImpl, first SUB the original line away, then insert it with a SUB anchored on `struct CScriptThingImpl {`.
- Arm order: `if (!flag) { term-check } else { ... }` puts a duplicated terminate check where retail has it (0x00D02B80).
- A canOpen flag with then = main path, else = locked chest gives retail's block order. `x = A || B;` keeps the bl store inside the temp scope (0x00EEC890).
- Write grouped loop exits as breaks, `while (!Succeeded) { if (MyID < 3) { if (G1) break; } else if (G2) break; ... }`, to fix the register and the exit-block order (0x00D37530).
- An inline `IsDead() { return !IsAlive(); }` gives retail's virtual IsAlive call. An inline CScriptThing copy ctor expands a by-value argument. Assign a flag before a call so its store is scheduled between the pushes (0x00D9F6C0).
- A named pointer local in a `{}` block (`long* comments = ParentClass->SizeComment;`) picks eax. An implicit handle dtor plus `inline_depth(0)` on the later returns mixes inline and out-of-line exits as retail does (0x00E7E640).
- Timer loops: make SetTime/GetTime `__forceinline` accessors over g_pGameScriptInterface. `while (!term)` versus `do {} while` chooses where the exit copy is placed (0x00E82950, still parked on exit-copy assignment).

Review at landing:
- 0x0089F4A0 took 8 grades; check the CThing retail layout comment: type byte +0x10, present bits +0x20, interface map +0x44, ScriptName +0x74, DefGlobalIndex +0x8c, PlayerNumber +0x90. Here type 1 = AI creature and 2 = player creature, but some landed comments say "1 = the player creature, 2 = an AI creature". One of the two is wrong; the Create callees here (CThingAICreatureInit 0x004FACD0 -> 0x00833800; SetControlledCreature after 0x006AC910) support 1 = AI.
- 0x00487B80 is called as a CPlayer member (`SetPlayerCharacter_00487B80` here). Its manifest label CVillageTaskCarryCrate::AssignCreatureToTask comes from a byte-identical body; rename it when the real CPlayer symbol is confirmed (PDB has CPlayer::SetPlayerCharacter at 0x00493400).
- 00da7d30 had redeclared `operator new(unsigned int)`. The declaration is removed and the row still grades RELOCATION_MATCH.
- Rows over 4 grades: 0089f4a0 (8, matched), 00d37530 (6, matched), 00e82950 (9, parked), 00eebdf0 (14, parked), 00d91960 (8, parked).
- Script-class sources use local `this` structs with `// Layout:` comments (the quest-parent 0x14 shift and the retail CThing shift), not the PDB-derived headers.
- 0x00D2988B is not a function start (catalog row error). The real CHookCoastTeleporter::Main is at 0x00D29880.
