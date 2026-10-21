# Chunk 259 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A lone inline `IS_THING_ALIVE(p)` whose result is the function's final `return true` compiles to VC's constant-select
  (`mov edx,eax ; mov cl,[edx+91h] ; mov al,1 ; test al,cl ; je`) with one merged epilogue; naming the item before the
  dead-bit test (`if (p.GetPItem() != 0) { CThing* item = p.GetPItem(); if (!(item->flags & 1)) return true; } return false;`)
  gives retail's `test byte ptr [eax+91h],1 ; jne` and the two tail-duplicated epilogues (0x00936210). A second inline
  IS_THING_ALIVE earlier in the same body also suppresses the select (why the landed 0x00947280 matched).
- A same-TU noinline stand-in for a callee that fills a local (vector, search out-param) must NOT pass the local's
  address on to an external sink: write the members from external values (`v._M_start = Sink(this, 0); ...`) or read
  the arguments' members. Then VC knows the address does not escape: it keeps vector begin/end in callee-saved
  registers across later calls, reuses the vector's frame slot for later temporaries and even drops the null test from
  the success path's `free` (0x0095D590); filter/comparator temporaries get retail's store scheduling (0x00936210).
- A long `if (!x) return false;` chain shrink-wraps (partial pushes, several epilogues); retail's all-registers-up-front
  prologue with one shared false epilogue (and 0 kept in ebx) is ONE nested `if` chain ending in a single `return false`
  (0x00986510).
- A by-value 64-bit UID argument whose two loads precede the `lea` of the next argument is an inline by-value getter
  (`GetUID()`), not the member read directly (0x00986510).
- An `if (A) x = GetTC(); else { CCharString name("..."); x = AddTC(name, true, 0); }` -- the named string in the else
  block shares the key slot (frame `push ecx`) and is built before the AddTC pushes; a `GetTC<T>()` returning 0 on a
  miss lets VC thread the 0 past the following null test (0x009162F0).
- An argument pushed after the object expression's call (`call GetThing ; mov ecx,[eax+60h] ; push 1 ; call`) is a
  component read through an inline const getter (`GetThing()->GetPhysicsTC()->SetAsMoving(true)`) (0x009162F0).
- The thing-search templates (GetPFirstThing 0x0056D050, GetPBestThing 0x009107B0) want a same-TU noinline stand-in too,
  even though they are distinct retail functions: declared extern, the out-pointer's address escapes and VC (a) keeps the
  out slot alive across later IP temporaries (frame +4, words shifted), (b) loads the out value early across destructors,
  (c) stores a by-reference comparator's x/y right after each load. A stand-in that reads `filter.Name.PStringData` /
  `compare.Pos.x` and writes `*out` from an external sink fixes all three (0x0097B900, 0x00963640, 0x00942D40).
- A search result tested AFTER the filter/name destructors (`neg bl ; sbb bl,bl ; inc bl`) is a negated bool held across a
  block: `bool missing; { CCharString name(..); Filter f(name); missing = !Get(f, &out); } T* t = missing ? a : out;`
  (0x00963640).
- A list scan whose iterator lives in a frame word (`mov ecx,[esp+x] ; add ecx,8 ; call GetPItem` with the sentinel in
  ebp, reloaded after the body) comes from a NAMED creature local inside the loop body (`CThing* creature = PBrain->PCreature;`
  before the GetPosition pair) taking ebp; written inline in the expression the iterator gets a register (0x00944E80).
- `HasTC(n)` + entry lookup vs the out-parameter `GetInterface(&p, n)` inside an `&&` chain compile to the same branches
  but rotate the scratch registers (eax vs edx after the lookup); try the GetInterface spelling when only eax/ecx/edx
  choices differ (0x00944E80).
- `shr eax,6 ; test al,1` on a virtual's unsigned-long result is `(unsigned char)(x >> 6) & 1`; `(x >> 6) & 1` folds to
  `test al,40h` (0x00944E80).
- A copied local STLport list (copy ctor 0x00618DA0, clear 0x004B8050 + free of the sentinel) needs same-TU stand-ins for
  the copy ctor and clear() so the sentinel stays in a register across clear() for the free (0x00944E80).
- Two success reasons sharing a tail are two separate `if`s each with its own `PTarget = enemy; return true;` (VC merges the
  tails); a single `||` condition or a bool flag rotates eax/edx in the second reason's block (0x0096FA90).
- Early-exit tests before the first callee-saved push get their own epilogues (`pop esi ; xor al,al ...` twice) only as
  early `return false;` statements, the thing test through an inline `IS_THING_ALIVE(CThing*)` (0x0096FA90).
- A virtual call on a stack-local action (`mov eax,[esp+68h] ; call [eax+2Ch]`) needs the action used through a base
  pointer (`CCreatureActionBase* pAction = &action;`); `action.Clone()` on the object is devirtualised (0x00969AE0).
- A literal passed to a `const CCharString&` parameter of an out-of-line ctor whose CCharString is built FIRST (before
  the other argument pushes) is the implicit conversion, not an explicit `CCharString("...")` temporary (0x00969AE0).
- The fast-rsqrt Normalise rows need C3DVector's DECLARED memberwise copy ctor (already a GOTCHA for small value
  structs): without it the copies go through memory and even the estimate's integer ops schedule differently (0x009633E0).
- Write every failure/success exit out in full (`FreeBarPoint(); SetPushableByHero(true); return false;` repeated)
  instead of a goto to one shared label: VC7.1 cross-jumps the copies and keeps the first as the fall-through, as
  retail does; a shared label gets the block sunk below the loop or `push 1`/`mov ecx,esi` swapped (0x00967B40,
  0x009689C0). Not enough for 0x0096AF60 / 0x009692C0 (block-placement gap).
- Implicit `const char*` -> `const CCharString&` arguments build every temporary before the first push; explicit
  `CCharString("..")` temporaries interleave with the pushes (0x009689C0, 0x009261F0, 0x00969CB0).
- An inline creature getter (`GetCreature()` returning `(CCreature*)PBrain->PCreature`) loads the creature early
  (`mov eax,[esi+4] ; mov eax,[eax+18h]`) and picks retail's registers; a named local loads it late (0x00967B40,
  0x009689C0, 0x00968E00: also `lea ecx,[eax+170h]` vs `add ecx,170h`).
- The frames-per-second global (0x01375550) is read into a local BEFORE the GetWorldFrame call:
  `long fps = g_FramesPerSecond; long end = CWorld::GetWorldFrame() + fps * N;` (0x00967B40, 0x009692C0).
- A thiscall object read into a named local before the argument locals (`CCombatSystem* c = GetPWorld()->PCombatSystem;`)
  evaluates `this` before the pushes (0x0093C030).
- A by-value IP argument built from an IP member (base ctor, vtable, GetPItem, SetPItem all inline) is a converting
  ctor taking `const CBaseIntelligentPointer&` and calling `SetPItem(o.GetPItem())`, with the member passed directly
  (0x0093C030, 0x00940310).
- A member IP reached through an inline accessor (`GetTask()->GetCrop()` returning `PCrop.GetPItem()`) gives
  `lea ecx,[eax+8]`; the direct member chain gives `mov ecx,eax ; add ecx,8` (0x00940310).
- A retained but unused `setne bl` before a temporary's dtor = `if (Perform(&T(..),false) != 2) Terminate(); else
  Terminate(); return;` (0x00940310).
- Conditionally destroyed CCountedPointer temporaries in an `&&` chain (ebx flag mask 1/2/4) need a `__forceinline`
  copy ctor and dtor so they stay in registers (0x00984D90).
- Action sizes come from the frame: the ReceivePayment CCreatureAction_PlayAnimation is 0xac, the FillTankard actions
  0xa8, CCreatureAction_PlayIntoLoopOutOfAnimation 0xc4 (CCharStrings at +0xbc/+0xc0 destroyed inline).
- GFFloatRandom 0x004978A0 is `float __fastcall GFFloatRandom(float range, unsigned long* seed)` (seed in ecx);
  WorldRandomRange is inline `seed = seed*0x24A1+0x24DF; seed = GFROR13(seed); range ? seed%range : 0` with
  `__fastcall GFROR13` 0x00497890.
- Reuse landed models first: the CTCContainer::AddItemToInventory (0x008ED090) HasTC/GetTC + range-checked
  bitset<274>::test model matched 0x009261F0 on the first grade.
- Lane gap (LTCG-like): retail keeps escaped locals (a list sentinel, a CDefPointer target) in callee-saved
  registers across later calls, and std::for_each 0x0091B890 takes its return slot in ecx (`ret 0Ch`); VC7.1 RTM
  reloads them, /Oa only makes it worse (it also caches PBrain) (0x0091B570, 0x00966AF0, 0x0096C4F0).
- Lane gap: the inlined STLport lower_bound loads `first` into eax and copies it to esi/edi in retail; ours loads
  the target register directly (0x009363D0, 0x0093C030).
- Lane gap: all locals homed in dead argument slots with no frame (0x0094D780, partly 0x0091C220 / 0x009385F0).
- Lane gap (asm_bake family): a bare dword `fistp` + `fild`/`fsub 1.0` ceil helper (0x00986210).

Review at landing:
- Rows that needed more than 4 grades: 0x009689C0 (6), plus heavy probe series behind earlier matches 0x00936210,
  0x00942D40, 0x00944E80, 0x0096FA90, 0x00963640, 0x0097B900, 0x00969AE0 (see probe/); 0x00967B40 and 0x0093C030
  used exactly 4.
- Local CAIStateGroupBase-derived models sit next to existing engine headers (CAIStateGroup_TavernServer.h,
  _TavernKeeper.h, _WifeGiveYouGift.h, _SeekScriptTarget.h, _Thunder.h, _MinionWatchFires.h, _MoveGoodsBase.h,
  _OpinionReactionRoot.h, ...): the generated headers flatten the base into pads, so the srcs carry local models
  with the header member names; the shared-header verifier will warn.
- differ/0091b3a0.cpp passes a wrong 3.5f argument to GetPerceivedThing (0x007446A0 takes none); fix before reuse.
- Several matched srcs use same-TU noinline stand-ins for search/GetFirePointList/GetPFirstThing/GetSubDef callees
  (relocation-only); check they are acceptable to verify_and_land.py.
- No errors noticed in already-landed sources; no catalog row holds several functions.
