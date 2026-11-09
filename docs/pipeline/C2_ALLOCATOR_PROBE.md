# The c2.dll register-allocator probe (c2prio.py) -- reading VC7.1 13.10.3077's own colouring decisions

> Committed copy (2026-11-08). The tool lives in `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/` (`c2prio.py`, `c2dbg.py`; `cfg.py` / `explore*.py` are the static exploration
> scripts used to find the hooks). The probe micro-TUs, per-row outputs and STATUS are in `rebuild/candidates/manual/pending/2026-11-08_c2probe/`.
> Not committed: the agent's `web/` copies of the byte-tactics prior art (follow the cited URLs). Run it from the repo root after
> `source $S/wine/env.sh`; it never modifies the toolchain (it patches a COPY of c2.dll and compiles with `/B2<copy>`).

> Tool: `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py` (+ `c2dbg.py`). Written 2026-11-08 (chunk
> "c2probe"), after research861 (`docs/pipeline/VC71_CODEGEN_RESEARCH.md`, sections 0, 2 and 7) asked for it. The method is
> the Total Annihilation decomp's (`HectorBailey/byte-tactics`, PR #5132 `docs/c2-regalloc.md` and `tools/c2prio.py`,
> <https://github.com/HectorBailey/byte-tactics/pull/5132>, PR #1127 for the agent guide), adapted from MSVC 5's `C2.EXE`
> to our `c2.dll`, which is a DLL loaded into `cl.exe`. Every address, offset and formula below was read from the lane's
> `c2.dll` (sha256 `bcd28f39b179...6b81e`, exports `_InvokeCompilerPass@12`, image base `0x10700000`) and checked against the
> running compiler on research861's micro-TUs. Confidence: **high** = read in the disassembly AND reproduced by the probe
> on two or more TUs; **medium** = one of the two; **low** = inferred.

## 1. What it does, in one paragraph

`c2prio.py <src.cpp> [--func NAME] [--flags "..."] [--trace] [--blocks] [--rotation]` compiles the file exactly as
`verify_and_land.py` does (same BASE_FLAGS `/O2 /Oy /W3`, INCLUDE, `vc71()` rewrites, `FABLE_HDR_OVERLAY`), but through a
copy of `c2.dll` whose `_InvokeCompilerPass@12` entry is patched to `jmp $`. `cl.exe` loads the copy (`/B2<copy>`) and
spins there; the tool attaches `winedbg --gdb` to the spinning process, runs gdb with itself as the Python script, puts the
two entry bytes back, plants breakpoints inside `color.c`'s allocator and lets the compile finish. The `.text` of the object
produced is byte-identical to a plain compile (checked on 0085c7d0.cpp; only the COFF timestamp and the compiler-name string
in `.debug$S` differ). For each function it prints the allocator's own table: every register candidate with its **priority**,
**tie key**, **spill benefit**, **reference count**, **allowed set** and the **register** it got, in the order c2 coloured
them, plus (options) each colouring step with the chooser's per-register costs, each basic block's share of each priority,
and the expression temporaries' eax/ecx/edx rotation. A micro-TU takes ~6 s; a 2 KB Fable row 25-50 s (the front end
dominates). Runs are independent (own directory, own copies), so three can go at once; `$FABLE_VC71` is never modified.

## 2. Usage

```
source $S/wine/env.sh                                   # FABLE_VC71 (RTM 3077), WINEPREFIX
python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py rebuild/candidates/manual/pending/2026-11-08_research861/probe/prio.cpp                   # every function of a micro-TU
python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py rebuild/candidates/manual/pending/2026-11-08_research861/probe/prio.cpp --trace --blocks  # + colouring steps + per-block priority shares
python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py rebuild/candidates/manual/pending/2026-11-08_research861/probe/rotation.cpp --rotation    # + expression temporaries (scratch rotation)
FABLE_HDR_OVERLAY=$S/w22/research861/hdr856 \
python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py .../differ/0050a650.cpp --trace --blocks --json out.json
        # a lane row: --func defaults to its `// Leaf:` mangled name, --flags to its `// Flags:` line
python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py row.cpp --func Update --func '?Foo@Bar@@' ...   # substrings of the mangled name (repeatable)
python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py row.cpp --keep --obj out.obj      # keep the run dir (gdb.log, winedbg.log, the .obj)
```

Needs `gdb` with Python and `winedbg` (both on the lane). Run directories live under `$S/w22/c2probe/runs/<token>/`
(`C2PRIO_RUNS` overrides) and are deleted unless `--keep`. Do **not** run research861's `gr.py` grader on two files
concurrently (it stages into one shared `g_<addr>` directory); the probe itself is safe to run in parallel.

Output columns (one row per candidate, in colouring order):

| column | candidate field | meaning |
|---|---|---|
| `id` | +0x1c | c2's candidate id, restarts at 1 per function (counter `0x10882154`); a reused record keeps its old id (free list `0x108a92d8`) |
| `candidate` | leaf +0 / +4 | the local / parameter name (storage record +4), `temp` (compiler temporary, kind 3), `local` (unnamed front-end local, kind 4), `global` (kind 7), `const N` (kind 0xd; value node +0x18) |
| `lines` | tuple +0x14 | source lines of the tuples that read/write it (absolute when the definition line is found, else `+N` from the function's first line) |
| `prio` | +0x0c | the priority (section 4) |
| `key` | +0x44 | tie key: number of the last tuple that writes it (section 4) |
| `spill` | +0x40 | spill benefit, sum of w(b)*cost (no K); <= 0 means "not worth a register" |
| `refs` | +0x24 | reference count |
| `allowed` | +0x20 | allowed registers when the sorted list was built, in table order `a c d s i b p` = eax ecx edx esi edi ebx ebp |
| `register` | +0x10 | register chosen (descriptor pointer -> number), `memory`, `immediate`, `split: ...` (pieces' registers), or `-> immediates after codegen` for a constant whose register was given back |

`--trace` prints each step: `choose #id prio allowed`, `-> reg costs {...}` (the chooser's nonzero per-register costs:
negative = preference from a copy relation, positive = penalty, `ebp: 17800`-style = a neighbour with only that register
left), `skip` (spill <= 0), `no reg` (allowed set empty), `split pass over ids [...]`, `piece #n of #m`, `re-sort`, and the
post-codegen `given back` events. `--blocks` prints every block of the first priority pass with `w`, `K` and each
candidate's share; the shares add up to the table's priority (verified on prio.cpp p1-p7 and on the three rows below).
`--rotation` prints the expression temporaries in code-generation order with the rotating pointer's register before each
choice and the rule that chose it.

## 3. Where the allocator is in c2.dll (all **high**)

Function recovery: `c2.dll` was built with basic-block reordering (hot/cold splitting, no `int3` padding between
functions), so the assertion sites research861 listed (`probe/c2_assert_sites.txt`) are cold blocks far from their hot
code. A crude CFG walk from every `call` target (`work/cfg.py` in the chunk dir) owns 390k of 456k instructions and was
enough. The family (addresses are VAs at the preferred base, which Wine honours):

| address | role (MSVC 5 counterpart in byte-tactics' names) | how it was identified |
|---|---|---|
| `0x1077c08e` | per-function register-allocation pipeline: `eax` = function record; calls `0x1077dfd0`, `0x1077c367` (init: id counter = 1, class lists), `0x10780353`, then the driver | caller chain from the driver; name at `[[fn]+4]` (mangled) |
| `0x1077c10f` | candidate constructor (`FUN_0040ebb6`): `eax` = leaf, allocates 0x48 bytes, id from `0x10882154`, inserts in HASH; returns the candidate in `eax` at `0x1077c1de` | the only writer of `HASH` |
| `0x10780000` | tuple walk (`FUN_00416a8d`): numbers tuples block by block, each block from its LAST tuple to its first (`[esp+0x1c]`), links operands to candidates; **DEF** at `0x1077fae3` (`esi` = cand, `eax` = tuple number -> cand +0x44), **USE** at `0x1077fbf7` (`ecx` = cand); current line in `0x108a9114` (tuple +0x14, relative to the function's first line) | writes +0x44 |
| `0x107821a7` | allocator driver (`FUN_00416e6a`): loops register classes 6..0; per class: `0x107844a4`, priorities, drop pass, sort, colour loop, splits, re-sort | calls everything below |
| `0x10782473` | priority pass (`FUN_0040ee1d`), 7 `imul`s; block start `0x1078244d` (`ebx` = block), block end `0x107828eb` (`eax` = w*K, `ebp` = w, `ebx` = referenced-not-live list, `[esp+0x10]` = live list) | the imuls (section 4) |
| `0x10782978` | drop pass (`FUN_0041a6f8`): `0x10782ab8` drops a candidate with fewer than 2 references and spill <= 0; `0x10782b1d` calls `0x107820b7` on a 2-reference candidate (copy forwarding) | `cmp [cand+0x24],2` |
| `0x1078482f` | builds the sorted list for one class, head at `LIST_HEAD = 0x108807b4`; insertion `0x10782b26`: by +0x0c desc, then +0x44 desc, a new candidate goes **before** equal ones | listing of the insertion |
| `0x1078368e` | interference / cost builder (`FUN_0041a985`+`FUN_0041b785`): fills `COSTS = 0x10880680[reg]` | called at `0x10782a0d` with the head candidate in `ebp` |
| `0x107833f2` | chooser/assigner: walks the class's order table (`0x108a7b40[cls]` -> `0x108691a0` = 1,2,3,7,8,4,6,0 = eax ecx edx esi edi ebx ebp), keeps the allowed register with the smallest cost, **strict `<`** so ties go to the earlier table entry; `0x107834a4`: `ebx` = register, `[esp+0x20]` = candidate; stores the descriptor `0x108a97a4 + 0x60*reg` at cand +0x10 and removes the register from the neighbours' allowed sets | reads the table, writes +0x10 |
| `0x107896ae` | low-spill branch of the driver: head candidate (`ebp`) has +0x40 <= 0 | `jle` on +0x40 |
| `0x10722cbb` | head candidate (`ecx`) has no allowed register left -> stays in memory / is split | empty-set test on +0x20 |
| `0x1078b2aa` | live-range split pass (`FUN_00439385`): `[esp+8]` = bit set of candidate ids to split; pieces are made by the constructor from `0x1078b494` (parent in `esi`, in `0x1078b346`) and `0x1078bf46` (parent in `ebp`, in `0x1078b79c`/`0x1078b93a`), the piece copies the parent's +0x44 | constructor call sites |
| `0x1078abac` | re-sort after a split (priorities recomputed by `0x10782473`, list rebuilt by `0x1078ba60`) | driver listing |
| `0x10756957`, `0x10782328` | the driver's `ret` sites (end of allocation for the function) | |
| `0x10785300` | after code generation (`FUN_0042bf00`): a register whose descriptor +0x38 (constants-only benefit) is <= 2 is released at `0x10785366` (`esi` = descriptor) and its constants become immediates | `cmp [esi+0x38],2; jg` |
| `0x10775acc` | expression-temporary register assignment (regasg.c `FUN_00435c37`); `0x1077542e`/`0x1077687e` read/write the class's rotating pointer `0x108a7d0c` into the `.data` order table `0x108a9738` (1,2,3,7,8,4,6,0); `0x10775782` resets it once per function (`0x10775844`, `0x107dff94`); common exit `0x10775be8` with `eax` = 3*register | the pointer's only readers/writers |
| `0x10785179` | a second round-robin over the whole table with an index at `0x108b022c` (Pentium-pairing rename pass, `FUN_0042b2c4`) -- not traced | reads `0x108a9738` with a wrapping index |

Data structures (**high**, dumped live):

- **candidate** (0x48 bytes): +0 leaf, +4 kind tag (2), +5/+6 flag bytes (+5 bit 0x2 = uncolourable/spilled, bit 0x4 =
  live-through penalty applied; +6 bit 0x10 = referenced in the current block, 0x8 = seen), +0xc priority, +0x10 register
  descriptor pointer (0 = none), +0x14 next / +0x18 prev in the sorted list (+0x18 doubles as the per-block cost
  accumulator in the priority pass), +0x1c id, +0x20 allowed set (sparse bit set: header -> chunks {base, next, 32 bits}),
  +0x24 reference count, +0x28/+0x2c first/last referencing block, +0x30 hash chain, +0x34 byte counter, +0x38 per-block
  reference list {next, block, count}, +0x3c leaf opcode node, +0x40 spill benefit, +0x44 tie key.
- **leaf** (what the candidate stands for): +4 kind (3 temp, 4 local, 5 param, 7 global, 0xd const), +0x10 type word
  (`>> 12`: 1 = integer -> class 0 via the table at `0x10869140`, 4 = x87 -> class 1; `0x1001`/`0x1002`/`0x1004` = 8/16/32
  bit), +0 storage record whose +4 is the name (`_a`, `_this`...), +0x28 (constants) the value node: +4 opcode 0x24b, +0x18
  / +0x1c the value's low / high dword.
- **function record**: +0 symbol (+4 = mangled name).  **block**: +0x1c first tuple, +0x20 end tuple, +0xa6 loop depth
  (byte), +0x40/+0x44/+0x48/+0x50/+0x54 live sets.  **tuple**: +0x10 previous tuple, +0x14 line (word, relative),
  +0x28 operand list (operand +8 kind, +0x10 web number, +0x1c candidate link or the sentinel `0x108a92e4`).
- **register descriptor** (`0x108a97a4` + 0x60*n): +0x10 type, +0x1c register number, +0x38 constants-only benefit. Numbers
  are MSVC 5's: eax 1, ecx 2, edx 3, ebx 4, esp 5, ebp 6, esi 7, edi 8 (9.. = 16/8-bit aliases).
- Globals: `HASH 0x108807b8` (1024 buckets by `id & 0x3ff`, chain +0x30), `LIST_HEAD 0x108807b4`, `COSTS 0x10880680`
  (per register number), order table `0x108691a0` (.rdata) and its .data twin `0x108a9738` (temporaries), rotating pointer
  `0x108a7d0c` (class 0; `0x108a7d08`, `0x108a7d04` the other classes), `0x108a9208` = optimising flag, `0x108a87d4` = pass
  mode (2 = profile weights).

## 4. The formulas (**high**: read in the code and reproduced on every probe and row)

**Priority** (`0x10782473`, candidate +0x0c). For each block `b` of the function, in order:
- `w(b) = 1 << depth(b)` (block +0xa6); with PGO data (`0x108a87d4 == 2`) a count-based weight instead;
- `K(b)` = number of candidates referenced in `b` (the referenced-not-live list plus the live-list entries with flag 0x10;
  constants count, at cost 0 or 2);
- `cost_b` = the candidate's cost accumulated in `b` (+0x18): 2 per variable reference, 2 or 0 per constant reference
  (0 where an immediate would do);
- a candidate referenced in `b` and not yet coloured gains `w(b) * K(b) * cost_b`; a candidate live through `b` without a
  reference loses `w(b) * K(b)`.

Checked: prio.cpp p1 `a` = 1*4*8 = 32, `b` = 24, `x` = 16; p7 `i` = 1*5*2 + 2*4*6 = 58, `a` = 10 - 8 + 12 = 14 (`--blocks`
prints each term; the sums equal the table on all 68 candidates of 0050a650 too). So "one loop reference outranks ~7
straight-line ones" (research861) is `w = 2` times a larger `K` in the loop block.

**Spill benefit** (+0x40) = sum over blocks of `w(b) * cost_b` (no `K`), minus a small amount for a parameter's home store;
`<= 0` = not worth a register (`skip` in the trace). **Drop rule** (`0x10782978`): fewer than 2 references and spill <= 0 ->
dropped before the sort; exactly 2 references -> checked for copy forwarding.

**Order** (`0x1078482f` / `0x10782b26`): priority descending, then **key** (+0x44) descending, a new candidate inserted
before equal ones. The key is the number of the last tuple that writes the candidate, tuples numbered block by block and
each block from its last tuple to its first, so **among equal priorities the value defined earlier in a block has the
larger key and is coloured first** (prio.cpp p3: `a` key 19 vs `b` 14 -> `a` esi; p4 reversed -> `b` esi). This is what
research861 observed as "ties go to the value defined first".

**Choice** (`0x1078368e` + `0x107833f2`): every allowed register gets a cost (COSTS[reg]); a copy relation to a
coloured neighbour or a fixed register (return value in eax, `this`/first `__fastcall` argument in ecx) subtracts (`-1`,
`-2`, `-3` seen), interference penalties add, a neighbour left with a single register adds `100 * weight` (17800, 10000,
5600, 3800, 1200, 400 seen); the cheapest allowed register wins, ties to the **earlier table entry** (eax, ecx, edx, esi,
edi, ebx, ebp). Values live across a call have eax/ecx/edx removed from the allowed set at creation (`...sibp`), so they
take esi, edi, ebx, ebp in priority order; ebp only when the other three are taken (p6: `a` esi, `d` edi, `b` ebx, `c`
ebp). A chosen register is removed from every interfering candidate's allowed set; a candidate left with none is split
(`split pass over ids [...]`) or stays in memory.

**Constants**: candidates of kind 0xd; a scratch-register constant needs prio from 3+ stores (consts.cpp z3: `const 0`
prio 4 -> ecx = research861's `xor ecx,ecx`); a callee-saved one is given back after code generation when its benefit
(+0x40 sum, descriptor +0x38) is `<= 2` (zc4: `const 0` spill 2 -> edi, then "given back, immediates"; zc5: spill 3 -> edi
stays = `xor edi,edi`). `0x10785300` is the pass; the probe reports it as `-> immediates after codegen`.

**Expression temporaries** (`0x10775acc`, after the allocator): a temporary takes the first free register of eax, ecx,
edx **from the class's rotating pointer**, and the pointer moves past the one taken; registers holding live values are
skipped; when no scratch register is free the walk continues into esi, edi, ebx, ebp (the table continues) or a hint /
first-free rule applies. The pointer is reset to eax once per function. rotation.cpp r1: eax, ecx, edx, eax; r2 (base in
eax): ecx, edx, ecx, edx; r6: ecx, edx -- as research861 measured. Each `G[i] = G[j] + k` shows two assignments (the load
by rotation, the add result by hint).

## 5. Validation

| TU | expectation (research861 / byte-tactics) | probe |
|---|---|---|
| prio.cpp p1/p2 | esi/edi by weighted count | `a` 32 esi, `b` 24 edi (p2 reversed) |
| prio.cpp p3/p4 | tie -> value defined first | equal prio 24, keys 19 vs 14 (p4: 18 vs 14): first-defined esi |
| prio.cpp p5/p6 | c > a > x = b; esi, edi, ebx, ebp | p5 48/32/24/24 -> esi, edi, esi (x dies), ebx; p6 60/50/40/40/40 -> esi, edi, edi, ebx, ebp |
| prio.cpp p7 | one loop ref beats seven straight ones | `b` 26 (loop block w 2, K 4) vs `a` 14 (7 refs, -8 live-through): `b` ebx, `a` ebp |
| consts.cpp z3 / zc4 / zc5 | `xor ecx` / immediates / `xor edi` | const 0 prio 4 ecx; edi then given back (benefit 2); edi kept (3) |
| rotation.cpp r1 / r2 / r6 | eax,ecx,edx,eax / ecx,edx,ecx,edx / ecx,edx | identical (`--rotation`) |
| object identity | plain compile | `.text` byte-identical (0085c7d0.cpp, 2 sections); only timestamp and `.debug$S` compiler-name string differ |

## 6. The parked rows (chunks 856/859/860), read with the probe

Outputs: `rows/<addr>.out` (and `.json`) in the chunk dir; variant sources in `probe/var/`. None matched in this session
(the time went into the tool); each diagnosis names the deciding quantity.

**0x0085C7D0 CMoveOnCylinder::OnInit** (8 lines: `mov edx,[ecx]; lea eax,[esp+8]` vs retail `mov eax,[ecx]; lea edx,[esp+8]`
at the GetFacingVector call). Not a global-allocator question: both are expression temporaries. `--rotation` shows the
pointer at **edx** when the vptr temporary is assigned (sequence at the call line: edx, eax, edx(ecx busy), edi...); retail's
pair needs the pointer at **eax** (vptr eax; ecx is busy with the call's `this`, so the lea gets edx). The one scratch
temporary before it is the `CameraManager` load of the `SetPhysics` line (`ptr eax -> ecx` because eax held the `new`
result; pointer -> edx). **Proof of the lever**: variant C (`CGameCameraManager* mgr = CameraManager;` before
`SetPhysics`) removes that temporary (mgr becomes a candidate in edi), the call site becomes byte-identical to retail
(`var/C.out`, diff `0085c7d0.vC.diff`), but mgr is hoisted to entry (`mov edi,[esi+0x40]`) and the cross product's x87
operand order flips (md 24 vs 8). Remedy to find: a spelling that shifts the rotation by one (±1 mod 3) before the vptr
load without creating a callee-saved candidate -- e.g. make the `CameraManager` load a non-rotation assignment (a hint) or
add one folded temporary between `SetPhysics` and the call; variants A/D/F/G (statement swap, named `facing`, named
`phys`, named `observed`) do not move it (md 112/50/12/8). Budget 10-20 grades with `--rotation` as the metric: the pair is
right exactly when the first temporary of the call line gets eax.

**0x0050A650 CWorldMap::GetMapNavigationAreaInit** (6 lines: the `region_exits` free reloads `_M_start` into ebx, retail
eax). Candidate #37 `local` (lines 235, 239; prio 30, refs 4, allowed `acdsibp` at choose time -- eax, ecx, edx all free)
gets **ebx only because of a `-1` preference** (`costs: {ebx: -1, ebp: 17800}`): it is copied into `exit_it` (#38, ebx,
prio 40). So in ours the value loaded at `begin()` for the loop and the value reloaded for the free are **one web**; in
retail they are two (the free's reload has no copy relation and takes eax, the first table register). Remedy: a spelling
in which the post-loop `_M_start` read is a separate web -- the loop must not be initialised from the same load as the
free's (an `exit_end`/iterator-pair form, an index loop, or a body that c2 treats as clobbering the vector's fields, see
research861 2.4 the other way round). **Proof of the lever**: variant V1 (an index loop, `region_exits[exit_k]`) removes
the copy relation and #37 takes **eax** (`costs: {ebp: 17200}`, no ebx preference; `rows/0050a650.varV1.out`), but the
index loop changes the loop body (md 32, 2249 v 2233 bytes); variant V2 (`exit_end` hoisted into the for-init) keeps the
preference (#37 ebx, md 365). Remedy to find: the iterator loop with its `begin()` taken from a load the free does not
share (the `-1` must disappear from #37's costs; `--trace` shows it in one run).

**0x005253F0 CThingManager::LoadGameState** (21 lines: retail keeps the text UID's low dword in ebx across the loop and
reloads `this` from `[esp+0x24]`; ours keeps `this` in ebx and `uid` in memory). `this` (#9, prio **-406**, allowed `.....bp`
only: esi/edi are excluded by the string-copy hard registers) is split into #45 (prio 52 -> edi) and #35 (prio **-313**,
allowed `.....b.` -> ebx: ebx is its only register left, chosen despite a 1200 penalty). The loop's callee-saved registers
are: `found` esi (368), `text` edi (124-132), the ReadLine temporaries ebp (112) and ebx = `this` piece. There is **no
candidate for `uid`** at all: its address is taken (`NPCNameMap[uid]`, `operator[](const key&)` out of line), so it lives
in memory. For retail's shape `uid`'s low dword must be a register candidate with positive priority (it beats `this`'s
-313 trivially) -- i.e. in retail's source the key passed by reference was a different object (a copy; retail's
`mov ebx,eax; mov [esp+0x2c],ebx` is exactly uid-low kept in ebx and stored once into a key temporary). Remedy: build the
map key from a copy (`const unsigned __int64 key = uid;` or a by-value accessor) so `uid` itself is never address-exposed;
chunk 856's w2/w4 ("uid low/high copy as key", "uid from value copy") went the right way but changed more (md 232/219) --
the probe will show immediately whether a `uid` candidate appears and what register it gets.

**0x00A8B990 C3DMesh2::InitialiseBones** (md 317, 1608 v 1624): the table (`rows/00a8b990.out`, 60 listed candidates, many
splits) shows the bone-loop values: `#38 local (63-65,67) prio 10 -> esi`, `#15 local (61,69-70) split, memory`, `#48 local
69 -> esi`, `#49 local 69 -> ebx`, `#44 newFinish 67 -> esi`, `#43 newStart 67 -> ebx`; retail keeps `_M_finish` in edi and
`eos` in ebp with `bone.Info` spilled. Too many differences for a tie diagnosis; the probe's use here is to compare the
candidates' priorities between spellings (the hand860 note's "a by-value CCountedPointer argument / visible
`_M_insert_overflow`" ideas) rather than guessing. Not pursued.

## 7. Limits and follow-ups

- Lines come from the tuple's line word relative to the function's first line; the definition line is found by a regex on
  the plain name (`Class::name(` or `name(`), else lines print as `+N`. Inlined code carries the call's line.
- Globals print as `global` (their symbol record's name is not at storage +4); x87 candidates are listed when class 1
  has a sorted list, otherwise under "not in the list ... x87". The frame layout (byte-tactics `--frame`) and the
  inliner (`--inline`) are not traced yet -- both are feasible the same way (the per-function codegen init `0x10775782`
  and the frame packer are the natural hooks).
- The `hint / other` rule label in `--rotation` is coarse: only the rotation path (`0x10775b43`) and the first-free /
  score path (`0x10714cd4`) are told apart.
- `wait_mapped` polls `/proc/*/maps` until the c2 copy is mapped (the front end of a big TU runs for up to a minute first);
  if a run ever hangs, `kill_matching(<exe name>)` is what the tool itself does on exit.
- Alternatives tried / not needed: no relay between gdb and winedbg was required on Wine 9.0 (byte-tactics needed one for
  the first packet); hardware watchpoints were not needed (the +0x44 writer was found statically).

## 8. Later findings (2026-11-09)

Regime 1 (> 10000 IL tuples: block weight 1, globals as candidates), the frame packer (reversed: node order, slot join,
density qsort), the regime-0/1 inline formulas, the cross-jump edge order and the scheduler's parenthesis tuple are in
[MATCHING_PLAYBOOK.md](MATCHING_PLAYBOOK.md) section 2, with the tools that trace them.
