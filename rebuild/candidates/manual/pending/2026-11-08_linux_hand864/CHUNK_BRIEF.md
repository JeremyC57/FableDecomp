# Chunk 864 -- Fable retry WITH THE NEW RESEARCH RULES: twenty parked near misses (overrides PROMPT.md where they differ)

These rows were parked after 50-350 blind spellings each (chunks 856, 858, 859, 860). Since then the compiler was researched and its
register allocator can be read directly (c2prio.py). Your job: apply the research rules below row by row -- diagnose each residue with the
probe first, then apply the matching rule -- and match at least half of the rows. Read each row's earlier log before your first grade
and never repeat a logged spelling; the start/ sources carry their overlays' requirements (copy only the headers a row needs into your
hdr/, diff each against rebuild/include first, keep opt-ins).

## Research rules (2026-11-08; read docs/pipeline/VC71_CODEGEN_RESEARCH.md sections 2, 4-7 and docs/pipeline/C2_ALLOCATOR_PROBE.md in full)
These are proven on lane probes. Use them BEFORE spending grades on blind spellings:
- **Read the allocator, don't guess it.** For any register difference run
  `python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py <src.cpp> --trace` (25-50 s; `--func` / `--flags` default from
  `// Leaf:` / `// Flags:`; set FABLE_HDR_OVERLAY as for a grade; `--rotation` for eax/ecx/edx temporaries, `--blocks` for per-block
  priority shares). A probe run counts as one of your 2 concurrent jobs.
- **Callee-saved roles (esi/edi/ebx/ebp)** = priority colouring in table order eax,ecx,edx,esi,edi,ebx,ebp: priority = sum over blocks
  of w*K*2 per reference minus w*K per live-through block (w = 1<<loop depth, K = candidates referenced in the block); ties go to the
  larger tie key (last writing tuple); a copy relation gives a -1..-3 preference for the copied value's register. Move a role by
  adding/removing ONE real reference, changing which value is defined first, or breaking / creating a copy relation (`--trace` shows it).
- **Scratch rotation (eax/ecx/edx)** comes from a per-function rotating pointer, not the allocator: add or remove exactly one expression
  temporary BEFORE the site (a named value local, an inline accessor at one site, a folded cast); `--rotation` shows the pointer.
  A named local that becomes a callee-saved candidate is not a temporary (it moves other things).
- **Address-taken locals have no candidate** (they live in memory): to get a value into a register, pass a COPY by reference
  (`const T key = v; map[key]`) so the original is never address-exposed.
- **Zero in a register:** 3 zero stores -> `xor` scratch, 5 with calls -> `xor` callee-saved; cross the threshold by writing stores
  per arm or merging them.
- **Frame-slot sharing / dead stores / reloads:** give each callee that receives a local's address a same-TU body under
  `#pragma auto_inline(off)` that only READS through the pointer (landed bodies where they exist); a writing or declared-only callee
  keeps the slot and the stores (the reverse lever reproduces a retail dead store). Store->load forwarding of a member needs the same
  base expression on both sides (store through `T* p = &m;`, read through the member, or the reverse, to block it).
- **x87 order:** with parameter/member bases the larger displacement is loaded first (textual order irrelevant), equal -> earlier
  parameter; direct operands (globals, by-value, frame) keep textual order (right operand first) and beat indirect ones; sums of
  products are re-sorted regardless of parentheses / named locals / casts and pinned only by a store or a call; `float& r = x; f(r)`
  gives push-then-lea; naming a float call result before adding it lets an unrelated integer load schedule ahead of the `fadd`.
- **Flags:** /G5 = /G6 = /GB = default and retail is not /G7 -- never spend grades on them; a flag that matches one row must not break
  the TU's landed rows (`rebuild/candidates/manual/pending/2026-11-08_research861/tools/tu_flag_check.py <lo> <hi> <flag>`).
- **Pads / block layout:** an alignment-pad difference always follows an earlier byte difference: fix the first byte first.
- c2 is function-local: declarations, path length and neighbouring functions do not change a function's bytes (only visible bodies of
  callees do).

Levers to know (GOTCHAS, newest at the end; read the 2026-11-07 lines first):
- chunks 857-860 (2/8, 7/10, 4/20, 0/6): same-TU `__declspec(noinline)` / `auto_inline(off)` bodies for callees that receive a local's
  address (alias / slot sharing); the store-forwarding base-expression rule; a named float temporary before `+=`; `float& r` for
  push-then-lea; split `X*X+Y*Y` into two statements (VC7.1 re-sorts one expression). Read pending/2026-11-08_linux_hand85[7-9]/LESSONS.md.
- chunk 855 (3 of 8, all RELOCATION_MATCH): compare the call/ret sequence first (pending/2026-11-08_linux_hand855/tools/calls.sh);
  VC7.1 silently ignores a `template<> ~map()` declaration of an implicit dtor (a bodiless declared `~map()` gets the COMDAT call);
  retail's STLport `__destroy_aux` in the CWorldMap TU is `__fastcall`; a small inline helper retail expands (CRGBFloatColour::Clamp)
  may need `__forceinline`; never grade one address twice at once.
- chunk 856 (0 of 10 parked near misses, 51-103 grades each): the `mov reg,[StreamPos]; add reg,N` v `lea` form at an inlined
  CDataInputStream::Read is a register-pressure tie; retail sinking an expanded GetAngleXY `fstp` past the next null test is an
  x87-order park; a row that matches only with `inline_depth(0)` or `/d2inlT*` is a structural park (record the probe and move on);
  check brace pairing before grading generated variants. Spend the budget where a spelling still moves the bytes.
- chunk 854 (4 of 6, landed 2026-11-08e): an agent grade that masks call targets can hide a RELOC_IDENTITY: `new char[n]` calls scalar
  `??2` unless the TU declares `void* __cdecl operator new[](unsigned int);`; a function ending in a noreturn `call _CxxThrowException`
  owns the 0xCC after it (grade against the N+1 row, EXTRA_RULES int3-after-noreturn).
- chunk 853 (1 of 6 + a recut): a row whose first function ends in `jmp +0` holds two functions; write `(*it)->` with no element
  local and hoist the loop count so VC7.1 unrolls; `& 0x80000000` gives retail's `jns`; `for (r = 1; r <= 3; )` with the step at the
  bottom fixes frame-slot order; an unclosed brace hangs cl.exe like `#error`.
- chunk 852 (0 of 6): a CPersistTraits<float> temporary retail never zeroes needs an empty user-declared ctor; a catalog name
  from a PersistError literal can be wrong (check the `ret` size and what the body allocates); VC7.1 expands a plain-inline
  CScriptThing::operator= at no site and forceinline expands all: park that split early.
- chunk 851 (4 of 6): inline header members the PDB lists (FableWin-only FlushRender, GetPixelShadersEnabled, ...) written as
  same-file `__forceinline` bodies of the shared-header declarations; CIndexBuffer::Lock takes its flags first; a pointee whose Release
  deletes itself fixes the last lines of a counted-pointer ctor; same-file bodies of GetAllLeafThings / GetDef<T> / map::operator[].
  Park a Codex rebase at once when it needs a polymorphic CBankFile (engine/CBankFile.h is on HOLD).
- chunk 850 (2 of 6): a PDB member sub-object with its own ctor (CConsoleVariable's `CGenericVarRef Var`) fixes the store order
  round the vptr; a by-reference SetMultiply helper changes the x87 order of a matrix product; a compiler-generated dtor (PDB 0x103)
  stays implicit even when retail calls it out of line; VC7.1 rotates `for(;;){if(!c)break;}`, `while(c)` and goto loops alike.
- chunk 848 (4 of 6): a 64-bit UID written as `value` plus a copy `uid = value` fixes stack-slot packing; `X = true; Y = X;` gives a
  single store; a single-site do-while(0) round cast-map assignments expands the implicit STB operator= in a cutscene helper;
  GetAsCharArray as a preset pointer + `if` gives the `[ecx+eax]` index.
- chunk 849 (6 of 6 Codex rebases): an out-of-line filter `operator()` stops VC7.1 hoisting the filter temporary's store out of a
  loop; a visible STLport `insert_unique` body changes frame-slot sharing; STLport `list::size()` is `distance(begin(), end())`; a same-TU
  `__declspec(noinline)` body of a real retail template (GetDef<CUIDef>) fixes caller register choices; a named reference local can fix
  a spill-slot swap (note it as a shaping lever).
- chunk 847 (Fable, 5 of 15): one flattened condition can fix register colouring; `long& end = Member;` keeps a dead store;
  `(float)(double)` identity wraps (noted on line 2) move independent x87 instructions; textual fmul operand order has no effect.
- chunk 846 (2 of 6, both Codex rebases): the held Codex pass-12 sources rebase onto our headers with a handful of additive opt-ins
  (0x0051FD80 had 61 errors on our tree, 0x00520D00 14; both landed RELOCATION_MATCH with the Codex body nearly unchanged). Read the landed
  rebuild/src/compiled/00/51/*_0051fd80.cpp and 00/52/*_00520d00.cpp and their opt-ins first. Overlaying `fable_string.h` (`#pragma once`) needs
  every root header that includes it copied into hdr/ too, or landed includers see two definitions. Check a rebased source's
  member-template symbols (STLport `_M_find` etc.) against reloc_bindings.tsv.
- chunk 845: an entry zero store with no visible reader in a script Main is a conditional-temporary flag home; a spawned-function
  name is `CCharString("Parent.") + CCharString(name)` (a literal left operand picks another operator+); a region-name temporary built
  after the new-expression is a default argument; a row-local inline `~CScriptThing` can fix per-site dtors and slot order.
- chunk 844 (expert): three separate `return false;` merge into one `xor al,al` block where an `||` chain does not; a single-site
  `do { } while (0);` splits an explicit call's inline budget (VC7.1 inlines loop-nested sites first).
- chunk 843: a Main ending in `jmp` to the T test then the NSF loop is `if (T) return; do NSF(); while (!T);`; in a long Main CTimer's
  `=`, `GetTime` and `==` must be `__forceinline`; grade the /d2inlS15 meter once as a DIAGNOSTIC on a long Main (never in a parked or
  landed source): it shows in one compile whether everything except the inline budget is right; copy-then-`-=` gives retail's in-place
  subtract.
- chunk 842: an entity Main whose expanded ScriptedThingBase dtor pad sits at the main-loop exit wants `for (;; Game->NewScriptFrame())
  { if (T) return; ... }` (0x00D40CF0 matched; `for (; !T; NSF)` and do-while(0) wrappers miss it); a row-local `__forceinline
  CScriptThing::GetPos` expands inline marker.GetPos() sites; a CThing base def read in place is `PeekBaseDef()` (GetPBaseDef copies
  the CDefPointer and adds an inc / dec pair).
- chunk 841: `break;` rather than `return;` after a closing call can be the matching spelling (0x00E71F10); an inline ~CTimer keeps the
  timer index in ebp, an out-of-line one spills it; VC7.1 evaluates the right operand first (def member on the left of a product,
  `rand() % 100` in its own long local); `if (!(x >= 1.0f))` gives retail's `test ah,1; je`.
- chunk 838: an idle-forever loop must be `while (true) { NSF; if (T) return; }` where retail expands the exit dtor (`for (;;)` calls
  it, 0x00EB7B00 matched); MOVIEBASE_OUTOFLINE_CTOR where retail calls every movie ctor; a row-local `__forceinline CScriptThing::operator=`
  for if-chain CreateCreature assignments (0x00D9DAF0 matched); md.py hides jump-target differences, so check reloc-masked bytes with
  `pending/2026-11-07_linux_hand838/tools/bcheck.py` before calling a 0-line row matched.
- chunk 837: in a looped Main the outer `for (;; NSF) { if (T) return; ... }` form changes which movie-dtor exits expand (while / do-while
  expand all); a handle ctor retail expands at one site and calls at another has no per-site spelling (park it); q.py shares one work dir
  per address, so never grade the same address twice at once (set QTAG for parallel diagnostics); read binding flag arguments from the
  retail bytes, not from a sibling row.
- chunk 836: the chunk 817 else-wait idle-loop form `if (T) return; for (;;) { NSF; if (T) return; }` puts the return pad before the
  loop as retail does (0x00D328C0 matched); try the implicit movie ctor before parking a row on mixed called / inlined movie-ctor sites
  (0x00ECD9E0 matched); the chunk 808 `parent` reference release turns an `add` into retail's `lea`.
- chunk 834: one `do { } while (0);` round the TimesSpokenTo SAY switch gets retail's movie-dtor tail merge (0x00D1FD30 matched, line-2
  `// Inline-split:` note); the same round a single SAY made the movie split worse elsewhere; TRetVoid and plain `return;` grade the same.
- chunk 832 / 831: a VALUE local read through a same-type `reinterpret_cast<const CScriptThing&>(x).IsAlive()` (owner-accepted,
  landed CMapKeeper::Main 0x00DA91A0) hoists a wait loop's `lea ecx,[esp+N]` above its terminate jne as retail does; a same-TU
  `__declspec(noinline)` helper body and the established CTimer forceinline / noinline members are fine.
- chunk 833: a single `do { } while (0);` round a cast-map assignment expands the implicit STB operator= in a ~2.5 KB Main
  (0x00E19D80 matched; line-2/3 `// Inline-split:` note) but not in 2.7-3 KB ones; a Main that matches only under diagnostic
  /d2inlS15 is the caller-size budget: park it.
- chunk 830: a chained `X = Y = Z = 0.0f` gives retail's Z/Y/X zero stores; `size() != 0` keeps an emptiness test `!empty()` folds;
  `do spoken = Play(); while (spoken == 2 && DoNothing() == 4);` keeps an unrotated speak loop; a `bool x : 1` set gives mov/or/mov.
- chunk 829: a protected member mangles IAE (declare it `protected:` or the grader grades the wrong function: check the
  first grade's WARN line); `// Flags: /GS` where a local array gives the cookie frame; hoist `size()` above a loop; fill a member box
  with field stores; a folded `x * 1.0f` reorders x87 products and is not source evidence; grades are cached on identical text.
- chunk 828: the chunk 813/824 final-loop form `for (;; NSF) { if (T) return; ... }` also fixes a one-pad scheduling swap; where
  retail keeps two copies apart, write the code out in each branch rather than via one row-local inline helper; a `switch` helps
  where retail merges two copies; `if (!X) continue; {...} break;` stops loop rotation but costs epilogues.
- chunk 826: count calls retail has v ours: an inline helper called out of line (a CDefPointer release, CCharString::IsEmpty) is worth
  100+ diff lines; check every local model's padding against sizeof; a standalone bit test as `if (w & m) return true; return false;`;
  an explicit `CCharString("")` after a new-expression; a `rand()` argument as its own statement; a named bool for a break test keeps
  x87 products above the calls.
- chunk 827 / 824: when retail homes locals in dead argument slots, hoists default temporaries, keeps a constant in a register, or
  expands a helper ours calls, put the landed body of the callee above the function in the same TU under `#pragma auto_inline(off)`;
  a PDB-named user copy ctor plus `T value; return value;` gives a memory-order default.
- chunk 825: implicit conversion temporaries for reference arguments are built before the pushes, each in its own slot; explicit ones
  between the pushes; named locals share slots (dead parameter slots too); `end() - begin()` may expand where `size()` is called.
- chunk 821 / 823: sibling blocks and inner scopes for frame slots and dtor order; `/d2inlS15` / `/d2inlT72` are DIAGNOSTIC meters only
  (never keep a flag); x87-heavy rows: name operands as locals in retail's evaluation order (chunk 815, 820).

The rows (oracle $S/batch/chunk864_oracle.tsv, exact; start/<addr>.cpp + start/<addr>.test.cpp are the latest best sources; start/SOURCES.txt
names each one's pending dir (its NOTES / ATTEMPTS log / header overlay; earlier pending dirs of the same row hold older overlays); line 2 states the residue):
- 0x0047ED76 CTCInGameMenu::SetToSave (366 bytes; chunks 252,641,757,840,844,859; from 2026-11-08_linux_hand859)
- 0x00A22E10 CRightHandedSet::MakePerpendicular (358 bytes; chunks 249,640,757,847,859; from 2026-11-08_linux_hand859)
- 0x006AFE60 CTCPhysicsBase::RespondToCollision (145 bytes; chunks 168,605,847,859; from 2026-11-08_linux_hand859)
- 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate (2340 bytes; chunks 795,820,832,859; from 2026-11-08_linux_hand859)
- 0x00C52BB0 CCpuInfo::GetAMDModelName (140 bytes; chunks 440,605,840,844,859; from 2026-11-08_linux_hand859)
- 0x0065B142 CTCVillage::EnableVillagerDefTypes (386 bytes; chunks 254,642,757,847,859; from 2026-11-08_linux_hand859)
- 0x00B92300 CEngineSceneGridCell::CreateOcclusionGroup (335 bytes; chunks 244,639,757,847,859; from 2026-11-08_linux_hand859)
- 0x00F36A30 CHeteroTerrainFractal::GetFractalHeight (361 bytes; chunks 246,640,757,847,859; from 2026-11-08_linux_hand859)
- 0x0085C7D0 NCameraModes::CMoveOnCylinder::OnInit (332 bytes; chunks 247,612,757,847,859; from 2026-11-08_linux_hand859)
- 0x00CFE030 NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main (3575 bytes; chunks 789,821,832,859; from 2026-11-08_linux_hand859)
- 0x00E76E90 NScript::CV_ExposeMayorScript::HeroFlashesLamp (1612 bytes; chunks 839,844,859; from 2026-11-08_linux_hand859)
- 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter (2916 bytes; chunks 792,823,832,859; from 2026-11-08_linux_hand859)
- 0x00D33730 NScript::CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main (3292 bytes; chunks 838,856; from 2026-11-08_linux_hand856)
- 0x00DDAE40 NScript::CQ_PrisonRaceScript::Main (1180 bytes; chunks 837,856; from 2026-11-08_linux_hand856)
- 0x0050A650 CWorldMap::GetMapNavigationAreaInit (2233 bytes; chunks 850,853,856; from 2026-11-08_linux_hand856)
- 0x00D3A4A0 NScript::CQ_FireHeartScript::CFH_Prisoner::Main (1140 bytes; chunks 837,856; from 2026-11-08_linux_hand856)
- 0x005253F0 CThingManager::LoadGameState (2263 bytes; chunks 843,848,856; from 2026-11-08_linux_hand856)
- 0x009D8CF0 CRenderManager2D::ClearRenderTarget (3385 bytes; chunks 857,860; from 2026-11-08_linux_hand860)
- 0x00A8B990 _global::C3DMesh2_AddSubMeshBoneVertices (1624 bytes; chunks 857,860; from 2026-11-08_linux_hand860)
- 0x009D9C80 CRenderManager2D::DrawBufferedPolys (3435 bytes; chunks 858; from 2026-11-08_linux_hand858)

Probe diagnoses already made (docs/pipeline/C2_ALLOCATOR_PROBE.md section 6; outputs in start/c2probe_rows/, variants in start/c2probe_var/):
- 0x0085C7D0: the swapped pair are expression temporaries; the rotating pointer is at edx at the vptr load, retail needs eax. Variant C
  (`CGameCameraManager* mgr = CameraManager;`) makes the call site byte-identical but mgr becomes a callee-saved candidate and flips the
  cross-product x87 order: find a spelling that shifts the rotation by one before the vptr load WITHOUT creating a candidate (`--rotation`).
- 0x0050A650: candidate #37 (`region_exits._M_start`) takes ebx only through a -1 copy preference to `exit_it`; retail's free reload is a
  separate web (eax). An index loop proves the lever (eax) but changes the body: find the iterator loop whose begin() load is not shared
  with the free's reload (`--trace`: the -1 must disappear).
- 0x005253F0: `uid` is address-taken (`NPCNameMap[uid]`) so it has no candidate; build the map key from a copy so uid gets a register.
- 0x00A8B990: compare candidate priorities between spellings rather than guessing.

Known residues to recognise (do NOT park early in this chunk: the Attempts section governs) (GOTCHAS chunks 797-827, 826): the per-site CScriptGameResourceObjectMovieBase dtor expansion
(ours calls the COMDAT 0x006E7B80 where retail expands it); VC7.1's inline expansion budget is function-wide in long Mains (a row
that matches only under the diagnostic `/d2inlS20` is a park, not a landing: say so in NOTES.md, and never leave a `/d2` flag in a
row); per-exit ~CTimer / CScriptThing copy splits; the CIsThingAlive / CIsThingConscious copy-inline case. Fixes that worked:
- a declared-only `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (0x006E7AB0) placed before the
  movie header (chunk 804);
- a named per-site C3DVector built member-wise; `__forceinline` CScriptThing `operator=` over a declared-only `CCountedPointer`
  `operator=` for if-chain assignments; a visible CTimer ctor body (chunk 805, `pending/2026-11-06_linux_hand805/LESSONS.md`);
- a loop-top thread check that jumps to an expanded epilogue is the loop condition `for(;!IsActiveThreadTerminating();NSF)`;
  temporary-dtor flag bits follow source order (chunk 806, `pending/2026-11-06_linux_hand806/LESSONS.md`);
- a row-local inline `CScriptThing::IsAlive` body when retail expands IsAlive; `CCharString("")` written out as the spawn-name
  argument; nested ifs (not `&&`) around a thread check (chunk 807, `pending/2026-11-06_linux_hand807/LESSONS.md`);
- `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` for misplaced exit-block copies; a local `parent` reference for a parent handle
  release (`lea`); row-local `__forceinline` CTimer `operator=` / `GetTime`; both string-compare opt-ins for `== "TEXT"`; one local
  in its own block for a frame one object too large; never leave an `#error` or an unclosed brace in a graded source (cl.exe spins under Wine)
  (chunk 808, `pending/2026-11-06_linux_hand808/LESSONS.md`);
- `__forceinline` for a helper with two call sites and no retail copy; two colour temporaries in one call built right to left;
  `for (;; NSF) { if (T) return ...; ... }` for retail's zero / index registers; a row-local `inline CScriptThing::CScriptThing() {}`
  for a one-site-expanded, one-site-called default ctor; a row matching only under `/d2inlT72` is a park (chunk 809,
  `pending/2026-11-06_linux_hand809/LESSONS.md`);
- check binding flags against the retail store; a local retail never keeps live in its own `{}`; a cut-scene block as a nested
  `{}` when a store must follow its dtors (chunk 811, `pending/2026-11-06_linux_hand811/LESSONS.md`);
- a block scope to make two locals share a frame slot; the compare's left operand loaded first (`GetHero()` first); an offset summed
  in place (`offset += GetHero().GetPos()`); a `rand()` angle as its own statement (chunk 812, `pending/2026-11-06_linux_hand812/LESSONS.md`);
- `for (;; NSF) { if (T) return ...; }` when retail's expanded implicit ~seh_me serves the natural exit and the first wait pad and every
  return cross-jumps to one shared tail; the declared-only CCountedPointer<MovieBase> dtor also picks which cut-scene scope expands
  ~MovieBase; the same-TU noinline CBasicString<char>::Compare body also fixes frame slots; drop MOVIEBASE_OUTOFLINE_CTOR when retail
  expands the movie ctor at one site only (chunk 813, `pending/2026-11-06_linux_hand813/LESSONS.md`);
- start an entity Main from a landed sibling's defines, includes and declarations verbatim (both chunk 814 matches did); a single
  expanded `timer = N` site is `__forceinline CTimer::operator=` alone; retail CALLING 0x00704430 inside an otherwise expanded
  by-value CScriptThing copy is a declared-only `template<> CCountedPointer<CScriptThing>::CCountedPointer(const CCountedPointer<CScriptThing>&);`;
  with a FORCEINLINE_CTOR movie / widescreen header, declare the ~CCountedPointer<MovieBase> specialisation right after
  CCountedPointer.h or VC7.1 stops with C2908; rows where retail expands EVERY movie dtor site are gated by frame-slot sharing of the
  movies and exit-pad cross-jumping, not by the dtor spelling (chunk 814, `pending/2026-11-06_linux_hand814/LESSONS.md`);
- a row-local `inline CScriptThing::~CScriptThing() { }` reproduced retail's "first temporary expanded, later ones called" split in a
  long Main (0x00D58490: 534 -> 64 lines), though not in every row; name x87 operands as locals before a score expression; write a
  short-circuit with a pre-set bool as `(A && B) || C` (chunk 815, `pending/2026-11-06_linux_hand815/LESSONS.md`);
- a loop bottom `NSF; if (!T) continue; return;` stops VC7.1 rotating a top-tested loop; FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
  isolates the copy blocker; a forceinline `CScriptThing::operator==` for expanded thing compares; int declaration order does not move
  scalar spill slots; retail overlays later-declared CTimers on earlier nested temporaries (chunk 816,
  `pending/2026-11-06_linux_hand816/LESSONS.md`);
- an else-wait written `T; for(;;){NSF; T;}` lets VC7.1 merge the loop test into the exit test; `CDefPointer<const CScriptDef>
  pGlobalScriptDef` preloads the def pointer into esi; write a no-return arm second so it stays inline; one block per phase counter plus
  one function-scope variable every phase assigns; CTimer `operator<(long)` and single-expression distance compares; a row-local
  Info-then-Data ~CCountedPointer for a PImp zero-store order (chunk 817, `pending/2026-11-06_linux_hand817/LESSONS.md`);
- wrap an earlier CTimer and its state loop in a sibling block so later cut-scene objects reuse its slots (frame one dword too large);
  when retail hoists a thread check into every arm of an if / else-if chain, write it in each arm plus `else { if (T) return; }`
  (chunk 818, `pending/2026-11-06_linux_hand818/LESSONS.md`);
- a question's no-return arm written second (`if (play != 1) {YES} else {NO}`) puts VC7.1's out-of-line arm where retail has it; a
  bool retail stores from al comes from writing the store before the call; in a Main whose retail expands only the first N movie /
  STB ctors and dtors, drop the MOVIEBASE OUTOFLINE_CTOR/DTOR opt-ins and keep the implicit movie and STB special members plus the
  declared-only PImp dtor spec (chunk 819, `pending/2026-11-06_linux_hand819/LESSONS.md`);
- the else-wait idle loop `if (T) return; for(;;){NSF; if (T) return;}` for a duplicated-head idle loop; member-wise C3DVector stores
  for retail's Dot product order; per-site out-of-line ~CTimer at one exit is an inline-frequency park (chunk 820,
  `pending/2026-11-06_linux_hand820/LESSONS.md`; its same-type reinterpret_cast lever is HELD for owner review: do not use it);
- a sibling block for an earlier float and its loops so a later def pointer gets its slot; one `do { } while (0)` round a whole SAY
  block for a one-exit MovieBase dtor; `/d2inlS15` as a diagnostic meter; a `for (;;) { if (it == end()) break; ... }` iterator loop
  that does not leak frequency (chunk 821, `pending/2026-11-06_linux_hand821/LESSONS.md`);
- the final thread check written at the end of both arms of an if / else so VC7.1 merges them into one call; a
  semantics-changing probe to find the single return site with an extra pad (chunk 822, `pending/2026-11-06_linux_hand822/LESSONS.md`);
- an inner `{ }` where a store sits between two retail dtor calls; an implicit / plain-inline MovieBase ctor expands at every
  site at any inline setting (chunk 823, `pending/2026-11-06_linux_hand823/LESSONS.md`);
- a same-TU body of a called template under `#pragma auto_inline(off)` when retail homes locals in dead argument slots or hoists
  default temporaries; chunk 813's loop form for a return tail after the first pad (chunk 824, `pending/2026-11-06_linux_hand824/LESSONS.md`).

Method that works (chunks 786-824 matched 67 of 183 script rows; chunk 790 matched the 5.4 KB CQ_HeroSoulsScript::Main on its first grade):
- Quest / village Mains: start from the quest-Main pattern in `batches/2026-11-05_landing_n/`, `_o/` and `_q/` (README +
  agent_NOTES.md).
- Entity Mains: start from the landed entity Mains in `_landing_m/`, `_p/` and `_q/` (CEM_SuspiciousVillager, CMI_InfoKeeper,
  CM_HiddenBooty, chunk 797's CAT_ConvoyMember in `_landing_w/`, chunk 798's CQ_FireHeartScript::Main / PlayCutscene in `_landing_y/` and chunk 799's five Mains in `_landing_z/`, are the newest). Each entity Main has a single .rdata reference, at slot 1 of its entity vtable.
- Use the per-quest header engine/CQ_<Quest>.h or CV_<Village>.h. Member offsets are the Ego_r.pdb offset minus 0x14, then a further
  minus 4 after each STLport vector or map. Extend an existing header with an opt-in branch or additive declarations; if you change
  an existing opt-in macro's text, list its landed users (`grep -rlw FABLE_RETAIL_<X> rebuild/src/compiled`) and recheck them.
- The FSE Lua port (`refs/script_recovery/seed_corpus/sources/<Quest>/FSE/`) gives the call order and literals.
- reloc_identity skips string relocations: check the string-literal order by hand. Check that each relocation binds the right symbol
  (chunk 794's lesson; agent794/bin/relpair794.py).
- Known residues to recognise early, and park rather than chase: per-exit ~CScriptThing / ~vector<CScriptThing> / ~CTimer inline
  splits, the CIsThingAlive / CIsThingConscious copy-inline case. GOTCHAS chunk 790/791: a declared-only explicit specialization of
  a dtor forces the out-of-line call; a `bool done; while (!done)` loop avoids rotation; the loop-head terminate check in a
  do-while(0) heats only its own exit.
- Single-site `do { } while (0)` levers are allowed with the line-2 `// Inline-split:` note (docs/pipeline/INLINE_DEPTH_REVISIT.md).
- Never name a helper script after a Python stdlib module.

Lane: you share the 4-core lane with two other authoring agents (chunks 862 and 863, $S/w22/agent862 and $S/w22/agent863). Never run more than 2 grades at once
yourself, and never touch its files or processes.

Time budget: get a first compiled attempt for every row early, then iterate per the Attempts section (50+ per row). Keep STATUS.tsv and an attempts log current so a container restart loses
nothing. Put the landed rows' own labels in LABELS.tsv too.

q.py ignores `// Flags:` (pass --flags). Export FABLE_HDR_OVERLAY before every grade, and never point it at a missing directory.
Check that `$S/batch/chunk864_oracle.tsv` is the oracle q.py uses.

NOTE on headers: the repo carries plan phase 7 and the chunk 782-824 headers. Chunks 820-827 have landed; the other running chunk may land while you work: copy a repo header fresh when you edit it, and diff your overlay against the repo before you hand back. Another chunk
may also edit headers in its own overlay; if you both need the same header, keep your edit an opt-in branch and say so in
hdr/README.md.

## Who you are (owner, 2026-11-05)
An expert in reverse engineering and in decompiling retail x86 game code into clean, portable C++. You read the retail disassembly and
recover the C++ a Lionhead programmer wrote; you know how VC7.1 RTM 13.10.3077 turns each spelling into machine code; portability is a
main goal: no inline asm anywhere in a row source or test (`docs/pipeline/ASM_RULES.md` is the one canonical asm page; plain C++ first,
always; beyond it only where retail's bytes prove the original used asm, and then only through an existing shared helper header).

## Environment (every Bash call must start with these lines; shell state does not persist)
    export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
    source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
Your directory is $S/w22/agent864 (src/ test/ differ/ hdr/ tmp/ probe/). PROMPT.md in it is the generic wave-25 brief: this file
overrides it where they differ (its $S path is right; its "4 attempts" rule is NOT in force, see below). No git at all, not even
read-only. Never write under /home/user/FableDecomp. Never kill a process you did not start (kill your own hung cl.exe by explicit
PID only; never pkill / killall / wineserver -k). This agent shares the 4-core lane with the chunk 862 and 863 agents (the coordinator may also run a short landing); never run more than 2 grades at once; q.py's work dir is per address, so grade
each address sequentially. Disk is a fixed allowance: delete lv/ and tmp/ work dirs when done; keep your directory under 300 MB.

## Attempts (the user: "a fable retry agent with the new research rules"; earlier: "give huge budget for retries ... keep trying every
row until half or more match / pass tests")
GOAL: at least 10 of the 20 rows landed-quality (MATCH / RELOCATION_MATCH under the read-only vland check + a PASSing test). Work in rounds
over ALL rows, smallest residue first: each round, probe the row (c2prio.py --trace / --rotation / --blocks as the residue class needs),
write down the deciding quantity (which candidate, which priority / tie key / copy preference / rotation step / slot rule), apply the rule,
grade, and log it. No per-row cap; stop only when 10+ match, or every unmatched row has 100+ real grades in this chunk or a documented
structural blocker (matches only under /d2inl* / inline_depth / a forbidden device). Never stop early to "save the lane".
Verify each claimed match with bcheck.py (pending/2026-11-07_linux_hand838/tools/bcheck.py) and the call-target check. Plain C++ only; no
__asm / naked / _emit / volatile / inline_depth / /d2 flags in a row. A row still unmatched at the end goes to differ/ with line 2
`// Grade: DIFFER(NvM) -- <residue + the probe's deciding quantity>` and line 3 `// Next idea: ...`.

## Read first, in this order (large files in parts)
1. /home/user/FableDecomp/CLAUDE.md, docs/HANDOFF.md, docs/pipeline/ASM_RULES.md.
2. tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md (the whole file; the 2026-10-28 and 2026-11-0x owner
   sections override anything older), then tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/BRIEF.md.
3. docs/pipeline/GOTCHAS.md: all of it, the 2026-11-0x lines at the end are newest and most relevant.
4. The two latest expert-agent chunks: rebuild/candidates/manual/batches/2026-11-05_landing_e/README.md + agent_NOTES.md (chunk 778),
   batches/2026-11-05_landing_d/README.md (chunk 777), and pending/2026-11-05_linux_hand778/ (STATUS.tsv, SIBLINGS.tsv, NOTES.md).
5. Landed sibling sources of each row's class (rebuild/src/compiled/<aa>/<bb>/*_<addr>.cpp; `grep -rl "<Class>" rebuild/src/compiled`
   and `grep "<Class>::" rebuild/oracles/auto-re-candidates.tsv`): their models, includes, FABLE_RETAIL_ opt-ins and the callee spellings
   they bind are the starting model for your rows.

## Names right the first time (owner rule 2026-10-28, full text in EXTRA_RULES.md)
One retail address = one name, everywhere. Before declaring any callee or global grep its address in rebuild/oracles/reloc_bindings.tsv
and the landed sources; use a correct bound spelling exactly; a wrong one gets the right PDB spelling and a NAME_FIXES.tsv line
(address, correct_symbol, evidence, wrong_spellings_seen). Two sources per name (PDB name set ghidra_out/egor_pdb_names.tsv +
fablewin_pdb_names.tsv as name SETS, RTTI slot, caller argument types, strings, STLport 4.6 source). Exact PDB spelling: class key
(ghidra_out/pdb_class_keys.tsv), namespace, const, access, calling convention, types; confirm the emitted symbol with objdump -t.
No placeholders (vfunc_N, sub_, Helper, Wrapper, _Retail, address-suffixed classes) where a real name can be found. NScript script
classes live in namespace NScript and are class-keyed. Wide text is unsigned short. STLport in namespace std with retail's sizes.

## Shared headers: correct them while you work (owner 2026-11-02)
When a row needs a class whose `rebuild/include/engine/<Class>.h` is wrong for retail (vptr flattened into a data member, Ego_r-only members or
offsets, a missing base, a missing member function), fix the header instead of writing a local model, whenever you can:
- Copy the repo header to `$S/w22/agent864/hdr/engine/<Class>.h` (same relative path; `hdr/<file>.h` for a non-engine header) and edit the
  copy. Grade with `export FABLE_HDR_OVERLAY=$S/w22/agent864/hdr` set before g.sh / gall.sh / tall.py / the vland check: the verifier then
  searches hdr/ before rebuild/include. A repo header that includes the one you corrected still finds the repo copy (MSVC looks in the including
  file's own directory first), so copy that includer into hdr/ too. Confirm once that your copy is the one used
  with a deliberate byte change that must turn a row DIFFER (never `#error`: VC7.1 under Wine spins forever on it).
- Prefer an opt-in retail branch: `#ifdef FABLE_RETAIL_<CLASS>` (retail layout) `#else` (the existing text, unchanged) `#endif`, with a
  `// Retail opt-in (chunk 864): ...` evidence comment (RTTI bases, vtable address and slot count, member offsets from the PDB checked against
  retail bytes -- two sources). Your row `#define`s the macro before the include. Existing includers keep the default path, so their bytes cannot
  change. Precedents: FABLE_RETAIL_CTCBASE (engine/CTCBase.h), FABLE_RETAIL_CTCDOOR, FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM, FABLE_RETAIL_NSCRIPT.
- If you change the default path instead, list every landed includer (real `#include` lines, transitively through other headers) in
  `hdr/INCLUDERS_<Class>.txt` and recheck all of them read-only with the overlay (`vland.py --recheck-landed` on a payload for those rows, in
  parts of <= 800, LANDVERIFY_WORK under your directory and deleted after): every one MATCH / RELOCATION_MATCH + PASS, logs in tmp/. If any fails,
  use the opt-in branch.
- Generated headers keep a `// Retail opt-in` / `// Retail layout` line or a FABLE_RETAIL_ macro (gen_class_headers.py's RETAIL_MARK), or the
  generator overwrites the edit.
- Another chunk agent may correct the same header in parallel: keep each edit minimal and confined to what your rows need. Never edit
  engine/CBankFile.h (HOLD) or rebuild_abi.h.
- Hand back hdr/ with `hdr/README.md`: per header what changed, the evidence, opt-in or default path, includers rechecked. The coordinator
  copies hdr/ into the repo at landing and rechecks every includer again. A fix too large for this chunk goes under "Header debt" in NOTES.md
  with the exact change.

## Source format and checks
- LINE 1 of every source: `// <Module>::<Name>(<params>) -- <what it does>`; a `// Leaf:` naming the exact object symbol
  (confirmed with objdump -t); exactly one real `// Flags:` line when flags are needed. Tests standalone, printing exactly
  `<addr>_TEST PASS`. No `__asm`, `naked`, `_emit`, `volatile` anywhere in src/ or test/ (comments included).
- Trust the oracle $S/batch/chunk864_oracle.tsv (trailing int3 padding already stripped). Check every call target against retail
  (`objdump -r`; import thunks: 0x00BFEB22 is operator new[], 0x00BFEA1A scalar operator new).
- Before finish.py: read-only verifier run (writes nothing to the repo):
  `FABLE_HDR_OVERLAY=$S/w22/agent864/hdr LANDVERIFY_WORK=$S/w22/agent864/lv python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/w22/agent864/payload.json $S/batch/chunk864_oracle.tsv > $S/w22/agent864/tmp/vland.log 2>&1`
  Fix every RELOC_IDENTITY and real DIFFER; delete lv/. If you changed a shared header, also recheck every landed includer of it
  read-only (recheck.py + vland --recheck-landed, as the headers section says) and list them in SIBLINGS.tsv.

## Hand-over (all in $S/w22/agent864)
src/ + test/ (matched rows), differ/ (best source per parked row, lines 2-3 as above, plus its test), hdr/ (+ hdr/README.md),
STATUS.tsv (address, name, status, attempts, best, grade, difflines, file -- one line per row, every attempt in the notes),
SIBLINGS.tsv (address, source, grade_with_final_overlay, note), NOTES.md (per row: shape, key spellings, callee names with evidence,
labels to correct, header debt; descriptive names marked), NAME_FIXES.tsv, LABELS.tsv (address, Class::Name, evidence), LESSONS.md
(GOTCHAS one-liners). Run finish.py LAST, then reply with a short report: matched (MATCH vs RELOCATION_MATCH) / parked with residue,
names proven vs descriptive, label corrections, lessons. Leave no background job running when you report.
