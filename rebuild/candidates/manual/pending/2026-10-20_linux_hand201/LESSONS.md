# Chunk 201 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Script wait conditions with a CCharString conversion temporary: a NAMED bool (`bool waiting = !(a("") || b || c);
  if (!waiting) break;`) puts the destruction flag in ebx and the result in a frame byte (0x00E1F2D0, 0x00D17090
  shape); the same chain written straight into `while (...)` keeps the flag in a frame word (`or eax,1` load/store) and
  the condition in bl (0x00F02030). Read which of the two retail uses before choosing the loop form.
- A `||` chain over several conversion temporaries (`f("A") || f("B") || f("C")`) gives VC7.1's constant-folded
  destruction mask (`mov ebx,1/3/7`, cleared bit by bit, last-built first) with the result in a frame byte
  (0x00F0B890); no hand-written flags are needed.
- Entity-script thread exits: a discarded `IsActiveThreadTerminating();` as the last statement is the tail `jmp`.
  When two paths share that tail but retail jumps past `mov ecx,esi` (ecx still holds `this`), write each path as its
  own `IsActiveThreadTerminating(); return;` block instead of a `break` into a shared tail (0x00D17DA0).
- A loop whose normal exit is a plain `ret` while a `break` path still re-tests termination is `for (;;) { ...; if
  (IsActiveThreadTerminating()) return; }` entered through `if (IsActiveThreadTerminating()) return;`, not
  `while (!IsActiveThreadTerminating())` followed by code (0x00F035E0).
- VC7.1 hoists an identical leading call out of both arms of an if/else when a value computed before the branch is
  live in a callee-saved register (0x00D38310: one IsActiveThreadTerminating call, `test al,al` duplicated per arm);
  the twin without such a value (0x00D38AA0) keeps a call per arm from the same source shape.
- A difference used twice in one condition (`x - y == 2 && z > x - y`) must be written out twice: a named local
  rotates eax/ecx in the compare and in the following block (0x00D38310, 0x00D38AA0).
- A float compare whose base pointer retail keeps in a callee-saved register across the call (`mov ebx,eax ; call ;
  fcomp [ebx+N]`) is not reproduced by a pointer local or `limit <= ratio` (the lane keeps the field ADDRESS: `lea
  ebx,[eax+N] ; fcomp [ebx]`) (0x00E24100, parked).
- Retail STLport `vector<bool>` is a plain 12-byte byte vector (one byte per element, `mov cl,[eax+2]`); with every
  container 12 bytes the CQ_FireHeartScript members line up exactly with Ego_r.pdb less 0x14 and 4 per container
  (StartingPattern +0xBC, CommentGivenThisRound +0xF8, Followers +0x11C, FollowerHandles +0x128, Pattern +0x134,
  MissionHistory +0x184).
- CSpawnedFunc0<T> spawns (`new CSpawnedFunc0<T>("Name", 0, this, &T::Fn)` passed to
  CScriptBase::AddSpawnedFunction with `CCharString("", -1)`): the name is an implicit conversion temporary guarded by
  a flag bit (it exists only when operator new succeeded); the region name is an explicit temporary (its ctor's eax
  is pushed); the thread object is 0x3C bytes (base 0x34, Func +0x34, PClass +0x38) (0x00D3AD60).
- A by-value CCharString built in the argument slot (`push ecx ; mov ecx,esp ; push -1 ; push lit ; call ctor`) is
  `f(CCharString("LIT", -1))` with a declared copy constructor (0x00D3AD60).
- A one-argument callee taking `ecx` plus one stack word (`ret 4`) is a thiscall member: modelling it as a free
  `__fastcall` with a dummy edx argument makes the caller load edx (`or edx,-1`) instead of pushing (0x00C37840 vs the
  landed 0x00C20440 definition).
- A by-value CScriptThing parameter destroyed at two exits: retail expands the inline destructor at the first exit and
  calls the out-of-line ~CScriptThing (0x004AA840) at the second; the lane expands both whatever the inline spelling
  (in-class, out-of-class inline, `__forceinline` member release) (0x00D17090, parked as an inline-budget lane gap).

Review at landing:
- Already-landed 0x00D3A920 (CQ_FireHeartScript::UpdateHistory) names its +0x184 member `Guest`; with retail's 12-byte
  containers (vector<bool> included) +0x184 is `MissionHistory` (EMissionHistory) and Guest is +0x140.
- Already-landed 0x00C20440 is a free `__fastcall bind2nd_greater(Obj*, int, long)`; the body is a thiscall member
  with one stack argument (0x00C37840 calls it with `push -1 ; lea ecx`). Its 0x00C13140 definition is labelled
  GetBlockParryReward but is the sound system's play request.
- Local base models: every new quest-script row models CScriptBase locally (no engine header for the script classes);
  0x00D17090 (parked) models CScriptThing locally because engine/CScriptThing.h is a flat view without virtuals.
- Callee names chosen by PDB where landed spellings disagree: 0x00CB7E50 CScriptBase::AddSpawnedFunction (landed
  `CQ_AmbushTradersScript::Add` / `CQR_EscortTraderScript::Add`), 0x00CDD450 CSpawnedFuncBase ctor (landed
  `CSpawnedFuncBaseRuntime`), 0x0099AED0 CWideString default ctor (manifest CCharString).
- No row needed more than 4 grades; 0x00C37840 carries `// Leaf: Update@CSoundLoop_00c37840` and 0x00DBE2E0
  `// Leaf: WatchForGotGold` because their names differ from the catalog's.
- Catalog row 0x00449B60 holds three functions (GetMultiplayerColour with its jump table, then 0x00449BF0 and
  0x00449C10).
