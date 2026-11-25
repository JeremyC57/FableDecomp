# Extra rules for every chunk agent (on top of your PROMPT.md)

- No git at all, not even read-only commands (git log/show/status/diff/grep).
- Never write anything under /home/user/FableDecomp; work only inside your own agentNNN directory. Never delete or move your src/, test/ or STATUS.tsv; they are the hand-over. Do not `cd` into your directory; use absolute paths, and always source the env lines the brief gives before compiling.
- Never kill processes you did not start; if a cl.exe of yours hangs, kill it by its explicit PID only (never pkill -f).
- Respect the 10-grade budget per row (owner 2026-11-10: "make it 10 grades to make getting matches more likely", was 4; keep a per-row count in STATUS.tsv notes): the budget is 10 grades per row PER PASS (owner 2026-11-11: "it can do 10 per row per pass"; grades of earlier passes do not count); after 10 grades in a pass, park the row with its best source and a one-line "next idea". Grade with gall.sh or p.sh: g.sh ignores the `// Leaf:` and `// Flags:` lines, so /Gr and /GS rows show a false DIFFER through it. Keep exactly one real `// Flags:` line per source (any comment line starting `// Flags:` is parsed as flags).
- No `__asm`, `naked`, `_emit` or `volatile` anywhere, tests included, and no identifiers containing the substring `_emit` (use `gEmitter`, not `g_emitter`). A dead load that survives a constant-true condition is the signature of a volatile member: park such rows after 1 grade as "volatile candidate". A test that needs an SEH fault calls `RaiseException(0xE0000001, 0, 0, 0)` declared as `extern "C" __declspec(dllimport) void __stdcall RaiseException(unsigned long, unsigned long, unsigned long, const unsigned long*);` -- do not include <windows.h> in lane tests (cl hangs). Keep every source self-contained: an #include of a file missing from the work dir hangs cl with no error. VC7.1 `<cwchar>` has no `std::wcsrchr`; naming `wchar_t` needs `<stddef.h>`. VC7.1 tests cannot use local structs as template arguments; a brace-initialised iterator in a `for`-init is a syntax error.
- Jump-table switches: relocation masking cannot check which literal goes with which case, so verify every case-to-string pairing against the retail table by hand.
- Import thunks: 0x00BFEB22 is MSVCR71 `operator new[]` and 0x00BFEA1A scalar `operator new`; call the one retail calls (the lander rejects a mismatch as RELOC_IDENTITY).
- The oracle rows were bounds-checked before cutting (rebuild/candidates/manual/BOUNDS_FIX.tsv). One known exception: a function whose last instruction is a noreturn call (or `throw`) owns the 0xCC VC7.1 emits after it, which the cutter strips; if your object is exactly one 0xCC longer than the row for that reason, note it in STATUS.tsv as "int3-after-noreturn", keep the source and its passing test in differ/ (as <addr>.cpp and <addr>.test.cpp) with LINE 1 the usual `// <Module>::<Name>(<params>) -- ...` line and LINE 2 a `// Grade: DIFFER(N+1vN) -- int3-after-noreturn ...` line (it is landed against the N+1 span). Any other row that looks cut wrong: say so in NOTES.md with evidence.
- **[Superseded 2026-11-05 by `docs/pipeline/ASM_RULES.md`: no `__asm` in a row source any more; asm only in shared helpers.]** Retail inline asm (`rdtsc`, a bare `fistp dword`, `fnstcw`/`fldcw`, `cpuid`, esp stack switches, hand SIMD kernels) cannot come from C++. Owner 2026-10-27: such a row is reconstruction, grade `retail_asm` (crawl/purity.py), with no instruction-count cap: write C++ around the retail `__asm` block, add a `// Retail-asm:` line citing the retail address(es) and naming the instruction(s) no C++ route produces (and the routes you ruled out), no naked/_emit, no `ret` inside the asm. Keep such sources in a `retail_asm/` folder (the land scripts' grep refuses `__asm` in src/; the coordinator lands them with vland.py). The x87 operand order of a double global times a float/long (`fld qword [g]; fmul dword [x]`) is a known lane gap: park after 1 grade.
- STLport-over-CCharString algorithms: put the real `CBasicString<char>::operator<` body (landed 0x00429950) above the callers under `#pragma auto_inline(off)` (read `other` first). More generally, when the retail TU holds a callee's body, a same-TU stand-in body under `auto_inline(off)` + `__declspec(noinline)` lets VC7.1 keep registers and home temporaries in dead argument slots; if retail homes an STLport tag on a LIVE argument slot, give the tag-taking callee a same-TU noinline body that never touches the tag. `__false_type` and iterator tags need a user constructor or VC zero-stores them. In STLport sorts with a by-value comparator, compile operator() as a same-TU noinline stand-in (except in `__unguarded_insertion_sort_aux`, where it stays declaration-only).
- STLport 4.6: model containers and algorithms as templates in namespace `std` (owner 2026-10-25: retail's STLport has no own namespace, RTTI is `.?AU?$pair@...@std@@` and Fable.exe holds 0 `@_STL@@`; never write `_STL`, and not plain non-template functions) so the mangled names match retail; `__fastcall` goes on the template definition; explicit instantiation emits inline templates as their own COMDAT rows; model char_traits/allocator as `class`; vector ctors need the real `_Vector_base` + `_STLP_alloc_proxy`; `find`/`lower_bound` wrap out-of-line `_M_find`/`_M_lower_bound` returning node pointers; map walks use the static `_Rb_global::_M_increment(node)`; map::operator[] = `__default_constructed((_Tp*)0)` + map::insert wrapping insert_unique; iterators passed by value need a user copy ctor (list: split `_Nonconst_traits`/`_Const_traits`); a pair with a user ctor returns through a hidden pointer; `copy_backward`/`__pop_heap`/`__copy`/`__linear_insert`/`__uninitialized_copy`/push_back may need `__forceinline`; under /Gr STLport's free inline operators are `__fastcall`. Two-call float comparators where retail loads the second argument before the first result's `fstp` are a known lane gap: park after 2 grades. Generated engine headers for string-bearing classes may carry Ego_r layouts (0x1C strings); TLC STLport strings are 12 bytes.
- Return-block order: two identical early returns keep the merged block on the fall-through; one `||` moves it to the function end. Anonymous-namespace functions must be defined inside `namespace { }`; their leaves end in `?A0x` and match by prefix (`?Name@`).
- GetInterface: the one-ternary store `*ppOut = (T*)((it==last || t<it->type) ? last : it)->component` and the if/assign form compile to different loads; try both. GetInterface templates are `__forceinline`. CBaseClass is polymorphic.
- Interface type numbers come from GetInterfaceType (vtable slot 21): CTCHeroStats 4, CTCCarryable 0x47, CTCWeapon 0x48, CTCWeaponTrail 0x4C, CTCQuestCard 0x6C, CTCSpecialAbilities 0xA7, CTCCreatureHitNotification 0xCD.
- egor_pdb_names.tsv / fablewin_pdb_names.tsv are keyed by DONOR (debug-build) addresses, not retail: use them as name sets, never by address. FableWin.pdb `@Name@N` names are extern "C" `__fastcall`. PDB def types are `class` (mangled `V`), not `struct`; nested classes mangle with their outer class; STLport list/allocator are `class` but `pair` is a `struct` (PDB name sets mangle U?$pair@, never V). Retail STLport lists are 4 bytes and vectors 12, so PDB member offsets run high in STL-bearing classes. C3DVector is 1-aligned (pack(1)) in retail. engine/CTCBase.h models the vptr as a data member: never add a `virtual` derived override on top of it. Retail CThingPhysical has Scale at +0x9C and Health at +0xB4 (the generated header is off).
- Retail STL is STLport 4.x (not Dinkumware): name algorithm rows with the STLport spelling, not the propagated Ego_r spellings.
- Vtable RTTI names in rebuild/manifest/rtti-vtables.tsv drop template arguments; demangle the full `.?AV?$...` type descriptor with tools/decomp_pipeline/naming/msvc_demangle.py (read-only).
- Every source needs a `// Leaf:` line naming the exact object symbol (mangled prefix for operators/ctors/dtors/templates, e.g. `??$uninitialized_fill_n` so it doesn't also match `__uninitialized_fill_n`; function and member templates `??$name@`; member-template operators `??$?R...@`); check it against `objdump -t` of your object -- a leaf that names no symbol silently grades the largest function.
- finish.py's line-1 parser splits on `::` before the first `(`: keep template arguments that contain `::` out of line 1, but do not invent class names to do it (write the plain template name without arguments instead, e.g. `CVectorMap::Add(...)`). If no PDB name exists for a function, say "descriptive name" in NOTES.md.
- g++ test models that check `sizeof`: `unsigned long` is 8 bytes on x64, use `unsigned int`. VC7.1 has no NRVO: test helpers return temporaries, not named counted locals. Compare floats in tests with a tolerance.
- Run finish.py last, then reply with a short report: matched/parked counts, per-family summary, parked rows with what still differs, label corrections (incl. already-landed rows whose names look wrong), and GOTCHAS one-liners.
- (chunk 422) Same-TU `__declspec(noinline)` bodies of out-of-line callees that take a local's address make VC7.1 treat the local as uncaptured; try this before calling a register/dead-store diff an LTCG gap.
- (chunk 422) Catalog labels propagated across byte-identical bodies are often wrong for STLport helpers (sort/heap/list); name from the PDB name sets + callers, not the catalog.
- (chunk 431) The landing purity grep rejects `volatile` anywhere, even in a Win32 prototype: declare `InterlockedDecrement(long* addend)` (same `_InterlockedDecrement@4` import).
- (chunk 432) A `// Leaf:` whose class/struct letter (V/U) disagrees with the source matches no symbol and grading silently falls back to the largest function: always confirm the leaf with `objdump -t`.
- (chunk 433) 0xC08AE0..~0xC40000 is the libpk audio/middleware library: no PDB/RTTI, Dinkumware STL (also namespace `std`), LTCG static helpers with esi/edi/ebx register args (lane gap). Reuse $S/w22/agent431 and agent433 NOTES.md models.
- (retry pass 2, overrides earlier "park" advice) The x87 double-global-times-float gap is a spelling: write `(float)g_Double * x` (no code emitted without /Op; gives retail's `fld qword [g]; fmul dword [x]`). The map::operator[] late pair store is NOT a gap: give `insert_unique(hint, v)` (reading v) a same-TU noinline body. Rows over invented flat containers often match once rewritten as real STLport 4.6 templates (`_STLP_alloc_proxy`, `_Rb_tree_iterator` over `_Const_traits`/`_Nonconst_traits`, static `_Rb_global<bool>::_M_increment`).
- (chunk 437) The scratch grader ignores relocation identity: a call to scalar `operator delete` (??3) where retail calls the `operator delete[]` import 0xBFEB1C grades RELOCATION_MATCH but vland rejects it. Check that each call target matches retail's import.
- (chunk 439) Declare `void* __cdecl operator new[](unsigned int);` or `new T[n]` calls scalar new; declare ceil etc. `extern "C" __declspec(dllimport)` for the IAT call. Check every call target against retail.
- (chunk 441) vland also rejects one modelled global that binds several retail addresses: model each retail global separately.
- (chunk 446) 0xC89xxx..0xC9Exxx is Xiph Ogg Vorbis (libVorbis I 20030909, post-1.0.1 CVS) inside libpk: write rows from the Xiph sources under their real names; LTCG API = `extern "C" __fastcall`; memmove etc. need `__declspec(dllimport)` for the IAT call.
- Call-target check before finish.py: q.py masks call targets, the lander does not. Run
  `A=$S/w22/agentNNN python3 $S/w22/calls.py` over your rows, or
  `objdump -r` your object, and confirm every call goes to retail's target (e.g. STLport list nodes allocate
  with malloc 0xBFEA0E, not ::operator new).

## Owner rules and decisions, 2026-10-26 (override anything above that disagrees)
- CLASS KEY = RETAIL'S. MSVC mangles struct as `U`, class as `V`. Use the key retail's PDB name sets show for each type
  (grep `V<Type>@@` vs `U<Type>@@` in ghidra_out/egor_pdb_names.tsv + fablewin_pdb_names.tsv). For a `V` type write
  `class X { public: ... };` (leading `public:` keeps access). The generated engine headers still say `struct` for every
  type (a class-key pass is regenerating them): when a header's key is wrong, your own local declarations/forward decls
  and every `// Leaf:` must still use retail's key; note the header in NOTES.md "Header debt". C structs retail keyed `U`
  (and STLport `pair`) stay `struct`.
- RETAIL CONTAINER SIZES (STLport 4.6, verified against landed byte-exact code): list 4, vector 0xC, map/set/multimap
  (_Rb_tree: header +0, node_count +4, compare byte +8) 0xC, basic_string<char> 0xC, deque 0x28 (iterators 0x10 each),
  CVectorMap 0x10, vector<bool> 0x14. Ego_r-generated headers use Dinkumware sizes (list 0xC, vector 0x10, tree 0x10,
  string 0x1C, deque 0x18), so every member after a container is at the wrong offset (CStringParser real size 0x28,
  CAIBrain 0xA8 with Output +0x44, CGameCameraManager PObservedThing +0x100, CTCHeroStats off). Take offsets from
  retail code, never from those headers; record each header you had to work around in NOTES.md "Header debt".
  Exception: libpk (0xC08AE0..~0xC40000) really is Dinkumware.
- NO INVENTED STAND-IN TYPES for library code: no new CRetailTree*/CRetailList/CRetailVector-style classes; write the
  real STLport 4.6 template (`std::_Rb_tree<K,V,KoV,Cmp,Alloc>` etc.) instantiated with the PROVEN key/value types
  (owning class member type from the PDB, node size from the allocation, signed/unsigned compare from setl/setb).
  STLport _Rb_tree access (STLport 4.6 _tree.h): _M_insert/_M_copy/_M_erase private, _M_create_node/_M_clone_node
  protected. If the types are unprovable, use a descriptive `CUnproven<Kind>_<addr>` type and say so in NOTES.md.
- ONE RETAIL ADDRESS, ONE NAME. Before declaring any callee/global, look up its retail address in
  rebuild/oracles/reloc_bindings.tsv (symbol, target, address) and the catalog row's own name. If a PDB-confirmed
  spelling exists, use exactly that symbol (same class, key, access, const-ness, calling convention). Never invent a
  new spelling for an address that already has a correct one, and never declare an inherited base member on a derived
  stand-in struct (that mangles as Derived::f): reach it the way retail does (derive from the base, or the
  layout-rename include pattern: include the engine header under a layout name and derive from it). If every existing
  spelling is invented, prove the real one (PDB name set + vtable/RTTI + callee chain) or reuse the catalog row's
  current name; list every such callee in NOTES.md "Callee names".
- A destructor's identity is the vtable it stores first (e.g. 0x993B10 stores .?AVCDataStream@@ -> CDataStream::~CDataStream,
  not the derived class that called it). Derived destructors chain to the base destructor symbol.
- NScript script classes (CV_*, CQ_*, CQR_*) live in `namespace NScript` and are `class`-keyed, as both PDBs spell them.
- Names: from evidence only (PDB, vtable/RTTI, callers, strings). Propagated catalog labels are wrong more often than
  not. Where nothing proves a name, use one descriptive name and mark it "descriptive" in NOTES.md.

## Owner rule, 2026-10-28: names right the first time (overrides anything above that disagrees)
The owner wants every name an agent writes to be as close to retail as evidence allows, so that no later renaming pass is
needed. This covers the row's own function, every callee and global it declares, its class and template arguments, its
member names, and script names (NScript quest scripts: check work/aeon_lua_ports and docs/scripts/AEON_LUA_PORTS.md).
- **Prove before you write.** Settle each name from at least two sources: a PDB name-set entry plus donor order or size,
  an RTTI vtable slot, the sole caller's argument types, a literal string, or the real library source (STLport 4.6).
  Catalog and manifest labels are hints only; most small-function labels are propagated and wrong.
- **Exact spelling.** Class key (pdb_class_keys.tsv), namespace, const-ness, access (public QAE / protected IAE /
  private AAE, and the virtual / static forms), calling convention, return and parameter types: all as the PDB mangles
  them. Confirm the emitted symbol with `objdump -t`.
- **One retail address, one name, everywhere.** Before declaring a callee or global, grep `rebuild/oracles/reloc_bindings.tsv`
  for its retail address. If the bound spelling is right, use it exactly. If it is wrong, use the right one and record
  the fix (below); never copy a wrong spelling to keep the verifier quiet, and never bind a name that already belongs
  to another address. Byte-identical bodies at two addresses are two functions (retail has no identical-code folding).
- **No placeholders where evidence exists.** `vfunc_N`, `sub_<addr>`, `FUN_<addr>`, `_gapscan`, `Helper`, `Wrapper`,
  `View`, `Access`, `Methods`, `_Retail`, address-suffixed classes and free `@name@4` stand-ins for member calls are
  not acceptable when a real name can be found. A name that stays unproven after a real search is `CUnproven<Kind>_<addr>`
  or listed as Descriptive, with the search recorded in NOTES.md.
- **Fix what you find.** Hand back `NAME_FIXES.tsv` (`address<TAB>correct_symbol<TAB>evidence<TAB>wrong_spellings_seen`)
  for every retail address whose landed spelling (in reloc_bindings.tsv, a landed source or the catalog) you proved
  wrong. The coordinator respells the landed callers in the same landing and rechecks them, so no separate pass is needed.

## Owner decisions, 2026-11-01 (delegated to the coordinator under the rule "do what retail did"; override anything above that disagrees)
- **`volatile` where retail's bytes prove it.** A member that retail re-reads from memory with no intervening call or store
  (`cmp [ebp+N],reg ; je ; mov ecx,[ebp+N]`) and that another thread writes (completion records, thread flags) carries the
  qualifier in the retail class: write it with `volatile` and a `// Retail-volatile: <address>: <instruction pair>; <who writes it>`
  line (0x009C42F0 / 0x009C4F20 CFileInstaller::PCompletedRead). Still never as a spill trick: no `volatile` on a local, a
  parameter, or a member only one thread touches; the lander's grep is relaxed only for a source carrying the evidence line.
- **Portable spelling (owner, 2026-11-08):** write `FABLE_FORCEINLINE` (rebuild/include/fable_compiler.h), never a bare
  `__forceinline`: it is `__forceinline` under MSVC (same bytes) and `inline` elsewhere; the lander rejects a bare one (NOT_PORTABLE).
  Everywhere below, "`__forceinline`" means that spelling.
- **`__forceinline` on Lionhead header helpers is retail.** GetTC / PeekTC / GetInterface, C3DVector arithmetic and GetLength,
  CCountedPointer copy / Release: no out-of-line copy exists in the Ego_r / FableWin PDB name sets or in retail Fable.exe
  (thousands of use sites, all expanded), and plain `inline` leaves a call under VC7.1's budget. Keep `__forceinline` there;
  no re-author pass. Where an out-of-line copy DOES exist (HasTC: one in each PDB; GetInterface: one retail instance), prefer
  plain `inline` when the bytes hold and record which form the row needed. STLport members stay `inline` (their source is known).
- **(Superseded 2026-11-08 by the owner's library rule below.)** ~~libpk / GameCODA / Vorbis rows are ordinary C++ reconstruction.~~ The Rich header puts every compiled object on cl 3077, there
  is no vendor `.lib` to prove `toolchain_library`, and no PDB: author them as C++ under their own header names from the trace
  strings (GOTCHAS 0xC1xxxx..0xC9xxxx lines), Dinkumware STL for libpk.
- **Data-symbol spellings (VIRTUAL_SCREEN_WIDTH / HEIGHT, PMainGameComponent) stay as established** until a PDB data-symbol dump
  exists on the owner's machine (`cvdump -s` of Ego_r.pdb, S_GDATA32 / S_PUB32); keep them gated for respell waves, use the
  established spelling in new rows.
- **Parked pairs land together.** 0x00626E40 ConstructItemCategoryList is reconstructed first (same retail TU, before
  0x006276F0 GetItemsListUI); 0x00935E40 PutOutFire with its identical sibling 0x009363D0. Cut as one "pairs" chunk.

## Owner direction, 2026-11-08: third-party library code is isolated, identified and linked, not hand-written
Owner, verbatim: "if the audio code is a library, then we can typically skip it in a byte matching decompilation. The core strategy is
to isolate, identify, and link original or rebuilt library object files rather than rewriting them by hand."
- Third-party vendor code is not chunk material: the VC7.1 CRT, zlib 1.2.1, LZO 1.08, Xiph libogg 1.1.1/1.1.2 + libvorbis 1.0.1
  (except Lionhead's patched vorbisfile.c, which stays reconstruction), DirectShow BaseClasses (strmbase), DXUT ImeUi.cpp. Region map:
  docs/pipeline/LIBRARY_MAP.tsv; sources, versions, flags and routes: docs/pipeline/LIBRARY_LINKING.md.
- The route for those bytes is the one `toolchain_library` already uses (docs/pipeline/LIBRARY_SIGNATURES.md): build the vendor's
  original source unmodified with VC7.1 at the matching flags (or the vendor's shipped .asm), prove it against retail with the signature
  tools, and link it. Proven 2026-11-08: zlib 1.2.1 `/TC /MD /GS /DZ_PREFIX` grades RELOCATION_MATCH for z_inflate 0x00C02470.
- Correction (research, 2026-11-08): the audio block is NOT a vendor library. libpk + the "CODA" audio engine
  (0x00C086C0..0x00C89C10) and LHNetworkLib are Lionhead in-house code compiled in Lionhead's tree by the game's own cl 13.10.3077
  (Rich header: no foreign compiler; `d:\dev\audio\ver_fable_1.5_b\libpk\` paths and same-directory `.\PKC*.cpp` /
  `.\CCodaFrameStreamer.cpp` asserts). No legitimate source, SDK or object exists for them or for Sensaura's GameCODA (leaks are
  never used). The owner's condition ("if the audio code is a library") does not hold, so they stay reconstruction and are chunk
  material again.
- Already-landed hand-written library rows (owner answer, 2026-11-08: "Keep, label as library"): they stay landed and keep their
  grades; they are labelled as library code and reported apart from game code. When a vendor-built object is proven for the same
  range, the linked object becomes the row's provenance and the hand-written body stays as a readable reference.

## Owner decision, 2026-11-08: inline-budget residues may use an inline control (overrides the 2026-11-02 / 2026-11-03 bans)
Owner, verbatim: "as long as it does not affect portability, matches retail bytes, behaves the same as retail, and is clean c++.
then whatever you need to do, you can do it" (answer to: may a row whose only residue is VC7.1's inline-budget split -- retail
expands an inline callee at some sites and calls it at others -- use a compiler inline setting?).
- Scope: residues that are inline decisions (expanded vs called copies of an inline function, inline depth, caller-size cap).
  Not register choice, not block layout, not anything that needs asm; ASM_RULES and byte purity are unchanged.
- Order of preference: (1) plain C++ with retail-plausible control flow (the 2026-11-03 note below); (2) a per-row compiler flag
  with the source untouched: payload `"flags": ["/d2inlT<N>"]` / `"/d2inlS<N>"` (or a documented `/Ob<n>`), which becomes the
  catalog `CompilerFlags` and the row's `// Flags:` line -- the least invasive device, no effect on any other compiler;
  (3) a source-level MSVC inline control scoped to the affected code: `#pragma inline_depth(N)` ... `#pragma inline_depth()`,
  `#pragma auto_inline(off)` ... `(on)`, `__declspec(noinline)` / `__forceinline` on a row-local helper. A source device must
  leave plain, portable C++ for other compilers (pragmas other compilers ignore, or `#ifdef _MSC_VER` around the declspec).
- Conditions, all required: vland MATCH / RELOCATION_MATCH; the behaviour test PASSes (same behaviour as retail); no
  `__asm` / `naked` / `_emit` / `volatile`; shared headers as usual, no invented duplicate class model (two-view CScriptThing
  is NOT covered); pick the smallest N that matches and say why.
- Mark it: line 2 (or the line after the Leaf/Flags header) `// Inline-budget: <device> -- <which callee/sites; diagnostic evidence>`.
- The 65 landed `// Revisit: inline-depth` rows are compliant under this decision; their markers stay as provenance. The
  2026-11-07 ruling against a row-local `__forceinline` helper (0x0094AB00) stays for non-inline residues; for a pure
  inline-budget residue this decision allows it (preference 3).

## Coordinator note, 2026-11-03: per-call-site inline splits without pragmas
Retail mixes expanded and called copies of one plain-inline function inside one function; VC7.1 decides per call site from c2's
static block frequency (docs/pipeline/INLINE_DEPTH_REVISIT.md, 2026-11-03 section). Reproduce a split with plain `inline` and
retail-plausible control flow: a `do { ... } while (0)` statement macro used uniformly (e.g. at every terminate check), a loop
guard, a real loop. (2026-11-08: when this fails, the owner decision above allows an inline control.) Wrapping only some exits is accepted
only with a line-2 note that retail's construct there is unproven.

## Owner decision, 2026-11-02: inline_depth rows kept and marked
- The 129 landed rows with `#pragma inline_depth` regions and the 17 two-view CScriptThing rows stay. Line 2 of each carries `// Revisit: inline-depth` or
  `// Revisit: two-view-CScriptThing` (docs/pipeline/INLINE_DEPTH_REVISIT.md). (2026-11-08: `#pragma inline_depth` allowed again under the owner decision above; the two-view model is still banned.) A chunk that finds a pragma-free
  shape for one of them reports it; the coordinator tries it on the family and removes the marker from every row it redoes.

## Owner 2026-11-02: SDK packing boundaries retail used
"If that is how retail did it then you should too." Where the retail bytes prove a structure was compiled byte-packed by a Microsoft SDK
header (member offsets that natural alignment cannot give), include the SDK's own boundary exactly as the SDK header does:
`#include <pshpack1.h>` ... `#include <poppack.h>` (rebuild/include/pshpack1.h / poppack.h, directive text only). Cite the offsets in the
header comment. This is not a licence for authored `#pragma pack`: no packing without byte evidence (first use: CImaAdpcmCodec 0x00C5FA50,
mmreg.h's tWAVEFORMATEX / ima_adpcmwaveformat_tag).

## Owner decisions, 2026-11-03 ("faithfully recreate what retail did"; override anything above that disagrees)
- **D1 -- STLport `_Rb_tree::find` / `_M_find` are member templates.** Stock STLport 4.6.1 `stl/_tree.h` lines 457-467 declare
  `template <class _KT> iterator find(const _KT&)`, the const twin, and the private `template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT&) const`
  whenever `_STLP_MEMBER_TEMPLATES` is set (VC7.1: yes). Spell them so: `??$_M_find@<K>@?$_Rb_tree@...@std@@ABEPAU?$_Rb_tree_node@...@1@AB<K>@Z`
  (private `ABE`, node return, never `_Rb_tree_node_base*`, never a member of `map`/`set`) and `??$find@<K>@?$_Rb_tree@...` (`QAE` iterator /
  `QBE` const_iterator, hidden result). `map::find` / `set::find` stay non-template wrappers over `_M_t.find(k)`. The tree always has the five
  STLport arguments (`_Select1st` / `_Identity`, `less`, `class allocator`). A definer instantiates the member template explicitly
  (`template ... _Rb_tree<...>::_M_find<K>(const K&) const;` -- `template class _Rb_tree<...>;` does not instantiate member templates).
  `stlport/fable_map.h` already models it. Records: `rebuild/candidates/manual/naming/2026-11-03_decisions/`.
- **D2: STLport dispatch tags have an empty user constructor.** `stlport/fable_containers.h` declares `struct __true_type {
  __true_type() { } };` / `struct __false_type { __false_type() { } };` on the default path (the old opt-in
  `FABLE_RETAIL_STLP_TAG_CTORS` branch is gone). Retail never zero-fills a tag temporary: it passes the tag's address without a
  store (UpdateShadowedSpotLights 0x00B47F82..91, SwitchDoorsOnAndOff 0x00DEFB10). Include the shared header; do not declare the
  tags locally. Rows that still do are listed in `rebuild/candidates/manual/naming/2026-11-03_decisions/MIGRATION_TAGS.tsv` (2,464
  with a local user ctor, 630 without; untouched, migrate in a wave). All 2,263 landed includers rechecked byte-identical.
- **D3: GameCODA 2.10 EAX translation uses the SDK names.** Class `CEAXListener` (vtable 0x012B9340, was `CEaxContext`),
  `CEAXBuffer` (0x012B9344, was `CEaxSource`), `CEAXBufferList` (0x012B9338, was `CEaxSourceNode`), root `CCodaObjectBase`
  (0x012B5868, was `CPKObject`); the notification packet is `_CHANGES` (was `SEaxEvent`). One model each:
  `engine/CEAXListener.h`, `engine/CEAXBuffer.h`, `engine/CCodaObjectBase.h` (+ `EAXNativeTypes.h`, `CodaEnvironment.h`,
  `CodaMemory.h`, `DSound3D.h`); SDK member names with the offsets listed in the headers. Memory is the SDK's extern "C"
  `CodaMalloc` / `CodaFree` (0x00C3C360 / 0x00C3C370); `CCodaObjectBase`'s operator new / delete are inline forwarders to them,
  never a `??2`/`??3` symbol. SDK-proven method names: `AddToChangesList` 0x00C5FFB0, `Set3DBuffer` (private) 0x00C60FF0,
  `CommitEAX3` 0x00C65A00, `GetEAX4FXSlot` 0x00C66330, `GetEAX4Source` 0x00C66C90, `IsNaN(const _D3DVECTOR&)` 0x00C60B30;
  the rest keep their descriptive names on the new class. EAX 4.0 GUIDs are extern "C" (`DEFINE_GUID` = `EXTERN_C`;
  GameCODA EAX4Translation.obj relocates `_EAX_REVERB_EFFECT`): `_EAX_NULL_GUID` 0x012B9438, `_EAX_PrimaryFXSlotID`
  0x012B9458, `_EAXPROPERTYID_EAX40_FXSlot0..3` 0x012B9468.., `_EAX_REVERB_EFFECT` 0x012B94B8 (all in `engine/EAX40.h`).
  Exception by the same evidence rule: the SAL module's own copy at 0x012BEE90 is the C++ `?SAL_EAX30_ListenerProperties@@3U_GUID@@B`
  (the original GameCODA Update object relocates it; CSalOpenALReverb::Update 0x00C71870 is byte-identical). Local models spell
  the SDK struct tags (`_GUID`, `_EAXVECTOR`, `_EAXREVERBPROPERTIES`, ...), never `SGuid` / `EAXVECTOR` tags.
  Records: `rebuild/candidates/manual/naming/2026-11-03_decisions/`.

## Owner decision, 2026-11-03 (portable C++): asm only in shared helper headers
**[This section and its 2026-11-04 bullets are superseded by `docs/pipeline/ASM_RULES.md` (owner-confirmed 2026-11-05). Where they disagree, that page wins; notably the fixed "impossible list" is replaced by "the original code used asm", proven from retail bytes.]**
The end goal is a port with identical game logic, so no row source may contain `__asm`. Where retail itself ran an
inline-asm helper (bare `fistp`, `fnstcw`/`fldcw`, `rdtsc`, `cpuid`, `frndint`/`fscale`, the SSE vertex kernels, the libpk
MMX mixers), that helper lives in ONE shared header under its PDB name with two paths: the byte-matching `__forceinline`
`__asm` body (with the `// Retail-asm: <retail address, instruction>` line; grade `retail_asm` is unchanged) and, under
`#ifdef FABLE_PORTABLE`, plain C++ with identical semantics -- a compiler intrinsic only where plain C++ provably cannot
reproduce them. The rounding mode the asm depends on is stated in the header (a bare `fistp` rounds per the control word,
it is not floor). Every helper has a dual-path test (rebuild/tests/portable/) that runs both paths on the same inputs and
asserts equal results; a row that needs a helper calls it and is otherwise ordinary C++. Agents hand back new helpers as
headers, never as asm in the row.
- **Reaffirmed by the owner 2026-11-04:** rows are C++ recreations of retail's own code. Exhaust C++ spellings first (operand
  order, casts, temporaries, inline helpers, same-TU noinline callees); asm is the last resort and only for instructions VC7.1
  cannot emit from C++ (bare `fistp`, `fnstcw`/`fldcw`, hand SSE/MMX kernels). Isolate it: the shared helper header carries the
  `__asm` path and a pure C++ fallback under `#ifdef FABLE_PORTABLE` (the platform switch), so a modern compiler builds the same
  logic without asm. The lander rejects `__asm` in a row source or test.
- **Owner 2026-11-04, priority order:** (1) no asm: pure C++ is the goal; (2) only where that is proven impossible, the isolated
  helper above. "Impossible" needs proof, not assumption. Proven under VC7.1 (GOTCHAS rasm1 line, re-tested 2026-11-04: `(long)x`
  gives `__ftol2`, `/QIfist` gives `fistp qword`): bare `fistp dword`, `rdtsc`, `cpuid`, `pushfd`, MASM-aligned SIMD kernels. These look
  like asm but are C++: `exp`/`cos`/`sqrt` via /Oi, `#pragma intrinsic(log10)`, `_MM_GET/SET_ROUNDING_MODE`, and `(float)g_Double * x`.
  NOTES.md lists every helper call a row makes, with the reason it is unavoidable.
- Owner 2026-11-04: a row that needs an impossible-list instruction still LANDS (grade `retail_asm`): the row itself is C++ and calls the isolated shared helper (`__asm` path + `FABLE_PORTABLE` C++ fallback + dual-path test). It goes in src/ like any other match, never in differ/.
- Owner 2026-11-04 (C++ exhausted first, per site): before any row calls an isolated helper, every normal C++ method must be tried at THAT rounding/asm site and recorded. (1) Confirm in the retail disassembly that the site really is a bare `fistp dword` / `fnstcw`+`fldcw` / `rdtsc` / hand SIMD, not `__ftol2`, `fistp qword`, `cvttss2si` or a same-TU C++ callee. (2) Compile each C++ spelling: `(long)x`, `(int)x`, `(short)x`, a cast through a temporary, `(long)(x+0.5f)`, floor/ceil with /Oi and `#pragma intrinsic`, /QIfist, float vs double operands, a same-TU static/inline C++ callee, local vs member store; and grep landed rows that matched the same retail sequence without a helper. (3) Only if all fail, use the helper, and NOTES.md must carry a "C++ attempts before helper" table (site address, retail instructions, each spelling tried, what VC7.1 emitted). No table, no helper: the coordinator rejects the row. Never use a helper, asm or pragma for a non-rounding residue.
- Owner 2026-11-04 (intrinsic and macro wrapping): at every rounding/asm site the order is (1) plain C++; (2) compiler intrinsics -- pure C++, a match is genuine, not retail_asm. VC7.1 RTM ships `<mmintrin.h>`/`<xmmintrin.h>`/`<emmintrin.h>` (`_mm_cvtss_si32`, `_mm_cvttss_si32`, `_mm_cvtsd_si32`, `_mm_getcsr`/`_mm_setcsr`, packed `_mm_*`, `_m_*`) and `#pragma intrinsic` (`_rotl`, fabs, sqrt, log10 with /Oi); it has NO `__rdtsc`, `__cpuid` or x87-fistp intrinsic, so check what each emits; (3) macro wrapping -- the isolated asm may be a macro in a shared header (e.g. `GF_FLOAT_TO_LONG_NEAR(dst, src)`: `__asm` by default, plain C++ under `FABLE_PORTABLE`, `// Retail-asm:` line, dual-path test). An `__asm` macro names the caller variables directly (no parameter copy), a `__forceinline` function routes the operand through a frame slot, so pick the form whose bytes match retail and test both; (4) the existing `__forceinline` helper functions. The NOTES.md per-site attempts table records the intrinsic and macro-vs-function trials. The hand SSE/MMX parks (0x00988E80, 0x00989230, 0x00A9D480/580/750, 0x00ADF960, 0x00AE1320, 0x00AE29D0) and the landed asm_bake rows get the same order.

## Owner decisions, 2026-11-04: vendor-code names (libpk, GameCODA, strmbase)
- Vendor code with no PDB is named "as accurately named as you can based on either vendor code or based on what the code is doing".
  - A name from the vendor's public source (DirectShow BaseClasses, Xiph, MFC) is preferred.
  - Otherwise, a name describing what the body does is allowed in place of an address placeholder.
  - The class comes from the `__FILE__` / vtable / structure evidence. The method name must describe the body accurately; record that
    behaviour in the evidence column.
  - One retail address keeps one name, and the landed callers are respelled with it.
  - Fable code keeps the two-source PDB rule.
- Accepted: respells that replace a wrong propagated Fable label on a vendor callee (e.g. CTCOnHeroPush::IsAngry on a libpk
  function) with the vendor wave's decided name, even though vland then reports RELOC_NAME against the old label until the label is
  applied.

## Owner decision, 2026-11-05: stand-in names
- **The rule:** "stand in names are fine as long as they are consistent and named as accurately as possible based on what the code is
  doing and how lion head names things in retail."
- **A row may grade a stand-in symbol** instead of the catalog function's own symbol, when retail's exact form cannot be emitted
  from that TU. Examples:
  - a free `CEngineMap_PeekWaterHasType` for `CEngineMap::PeekWaterHasType`;
  - the real method on a stand-in class named after the real class.
- **Accurate** means the name says what the code does, with the real class and method names where they are known, in Lionhead
  style (`Class_Method` for a free function standing for a member). Never accurate:
  - generic placeholder classes or methods (`T`, `Slot`, `Sub`, `Owner`, `M` ...);
  - address-only names, when a behaviour name can be proven;
  - a name that belongs to a different function.
- **Consistent** means one retail target has one stand-in spelling across every row that binds it. The symbol index's MULTINAME
  checks this.
- **verify_and_land prints a non-blocking `WARN STANDIN_LEAF`** only for an inaccurate stand-in, with its reason
  (`tools/decomp_pipeline/standin_leaf.py`).

## Owner decision, 2026-11-05: asm rule set (confirmed with the owner; supersedes the 2026-10-24 in-row allowance)
**Canonical text: `docs/pipeline/ASM_RULES.md`.** This section is its summary. The 2026-11-01 `__forceinline` rule for Lionhead header helpers is about retail's inlining, not asm, and is unchanged.
1. **Plain C++ first, always.** Try every C++ spelling (statement order, temporaries, types, inline or out-of-line callees,
   flags the TU already uses) before anything else.
2. **Anything beyond plain C++ is allowed only where the original code used asm.** The owner put it as: "the asm rule set past
   plain c++ rule should only be needed if the original code used asm."
   - The evidence is retail bytes that no VC7.1 C++ spelling emits: an instruction or sequence the compiler never generates. Cite
     its retail address and instruction.
   - A register choice, scheduling, frame-slot or other compiler residue never qualifies. A row whose only gap is a residue stays
     parked until a C++ shape matches.
3. **Where step 2 applies, in this order:**
   1. a compiler intrinsic, if one emits the instruction (genuine);
   2. an isolated, macro-wrapped `__asm` in a shared header, with a `FABLE_PORTABLE` plain-C++ twin, a `// Retail-asm-helper:`
      and `// Retail-asm:` evidence block, and a dual-path test (`tools/decomp_pipeline/portable_twins_check.py`);
   3. a `__forceinline` helper, only if necessary: for example, VC7.1 never auto-inlines a function that contains asm, so a
      wrapper that retail expanded must be forced. It lives in the helper header's retail branch, so the FABLE_PORTABLE build
      never sees it.
4. **Each site keeps a table** of the C++ and intrinsic attempts made before the helper.
5. **No `__asm`, `naked`, `_emit` or `volatile` in any row source or test.** A row reaches asm only through a shared helper
   header, and grades `retail_asm`.
6. **Existing code:**
   - The 99 landed `retail_asm` rows that carry their `__asm` in the row source move it into shared helpers, with identical bytes.
   - The 14 `naked_asm` / `emit_bake` oracle bakes go through steps 1-3. Those that match land; the rest stay parked answer keys.
   - The 160 tests that contain asm are rewritten as plain C++ calls.
- **Agent briefs** (owner, 2026-11-05): every delegated agent is briefed as an expert in reverse engineering and decompiling retail x86 game code into clean, portable C++ (chunk_pool/BRIEF.md opening).
- **Never park on "register colouring", "slot order" or "scheduling"** (owner, 2026-11-09: "find any means possible to get them to
  match bytes, behavior, without compromising portability"). VC7.1 is deterministic: measure the first wrong decision in c2 and
  turn it into an inequality. Prove the lever with a throwaway diagnostic build, then search source shapes for the natural spelling
  with a cheap fitness. Read docs/pipeline/MATCHING_PLAYBOOK.md before parking any row. It lists the measured c2 rules (regime-1
  priorities, enregistered globals, the frame packer, inline frequency, cross-jump edge order, x87 / scheduler order) and the
  measured dead ends. A park needs the measured reason for each residue, not a guess.
- **Made-up names (owner, 2026-11-10):** "you can use a made up name only if you cant find original name anywhere, are consistent in the names usage, and the name is as accurate as you can make it and makes sense". Search every name source first (Ego_r / FableWin / Ego_d name sets, Fable.exe strings, refs/fse_api_manifest.json, EgoCore, the sibling repos); then one descriptive name per address used everywhere, saying what the body does (not `VirtualNN_<addr>`, not `*_Unproven`), marked "descriptive" in NOTES.md.
- **Best-guess names (owner, 2026-11-25):** "for anything where proving a name is impossible, as long as you try to make it as close to what you believe retail would have named the function based on what it does, it should be fine". Search first, as above. When two sources cannot be found, land the best-evidence name instead of holding the row: a one-source name (one PDB name set, a GameCODA SDK `.lib` name, the retail-layout T predictor, a caller binding), else a name in retail's own style for that class and module (its PDB neighbours' prefixes, verbs and Hungarian). Rules: never replace a name two sources prove; one address = one name everywhere; no placeholders (`VirtualNN_`, `Unproven`, `sub_`); add one line per address to `rebuild/oracles/descriptive_names.tsv` (address, symbol, basis = one-source:<which> / style, note) so later evidence can correct it. The tracker counts these as `agree_descriptive`, apart from proven. This relaxes R-NF428-1 and R-NF430-1: a held row whose only gap was the second source may now land on its best name.

## Library code and naming (owner, 2026-11-12)
- Bytes the build takes DIRECTLY from a library (grade `toolchain_library`: VS 2003 CRT objects, import thunks, a vendor's assembled `.asm`) get no naming work.
- Library code this project reconstructed by hand (vendor rows written as C++) gets a LIGHT renaming only: readable descriptive names so the code can be followed, marked as made-up in NOTES. Do not spend naming agents hunting exact names for it.
- Engine rows keep the full rule above: exact PDB spelling, two sources.

## Naming is paused (owner, 2026-11-12)
- Placeholder-family naming waits until byte matching is done. Do not launch naming agents for `sub_<addr>`, `_unattributed::`, `CUnproven*`, `vfunc_N` or address-suffixed names.
- The name-fix queue stays: every landing still records landed names it proves wrong (NAME_FIXES_QUEUE.tsv). Clear the queue in one short pass at about 30 entries, or at once for a binding conflict (one symbol bound to two addresses).
- Author agents still spell every name in a NEW row right the first time (the rule above); pausing naming does not relax that.
- Run two author agents instead of one author plus one naming agent.

## Inferred statements (owner, 2026-11-12)
- When the retail bytes prove a source construct existed but not its exact wording, an inferred statement may land if the row then behaves exactly as retail (the bytes match and the statement cannot change the result).
- Mark it in the source header (`// Inferred:` + the retail bytes that require it + what was ruled out) and in the batch README. Prefer a wording a programmer could plausibly have written (e.g. a test whose arm was compiled out).

## 2026-11-19 owner: include order is an accepted lever
"header includes can be order sensitive, so if you find a row with an issue that seems similar, then try header order variants."
c2 numbers candidate records partly by c1 symbol ordinal, so the order of a row's #includes can decide a register / reload /
scheduling tie (0x00680050, landed 2026-11-19). On a tie that survives every statement-level variant, permute the row's includes
(real shared headers only; adding an include the TU plausibly had is fine; no new declarations). A row matched this way carries an
`// Include-order:` note: the order, the measured mechanism, and that it is fragile to shared-header changes (recheck after them).

## 2026-11-19 owner: grouping brackets are an accepted lever
"it seems like a lot of stuff needs brackets so we should allow them."
Brackets that emit no code (c1 PAREN tuples, opcode 0x269) move VC7.1's 81-tuple scheduling-region cuts and list-scheduler order; today
they closed 0x00F361D0, 0x0047ED76, 0x0047D296, 0x0047DD9D, 0x00853670, 0x008539C0 and 0x007BBB3B, and they are reproduced across six
same-TU CTCInGameMenu rows. They change neither behaviour nor portability, so a bracket is now an accepted spelling, like include order:
- Allowed in the row source and in shared helper text (a helper bracket goes under a NEW nested opt-in; regrade every landed user of
  that helper). Prefer the placement a programmer would write -- grouping a sub-expression (`(w * 0.5f) - x`, `(a + b) + c`), a
  bracketed return or argument -- over pure redundant wraps (`((f()))`); when several placements match, take the most natural one or the
  one with a same-TU precedent. A redundant wrap is acceptable only when no natural placement matches.
- Mark it with one `// Bracket:` line: which bracket is byte-required, the retail address it fixes, and (if measured) which other
  placements match or fail. No `// Inferred:` note and no owner review is needed for a bracket alone.
- It does NOT cover other unsourced constructs (extra locals, references, by-value parameters, dead code): those stay `// Inferred:`.

## 2026-11-19 advisor decision (owner delegated): Inferred scope / lifetime constructs need no review flag
When retail's frame size, stack-slot sharing or code size fixes a construct's scope or lifetime (an inner `{ }` block, a named local in an
inner block where a temporary or a function-scope local gives the wrong frame) and only the wording is unsourced, the row carries its
`// Inferred:` note (with the measurements) and lands without an owner-review flag. Fitted tie-breakers that the bytes do not otherwise require
(an extra reference local, a by-value operand, a dead test) remain `// Inferred:` with the evidence and are reported in the batch README.

## 2026-11-19 owner: rows byte-identical to the GameCODA SDK library may take the library grade, if portability is kept
Owner, verbatim: "if it does not interfere with portability, it should be fine" (asked whether rows byte-identical to prebuilt GameCODA CODAApi.lib
functions may be graded as library code like the CRT rows). Shape: advisor decision (owner delegated).
- **Author first.** Write the row as plain C++ on the shared audio headers from the CODAApiDbg.lib CodeView, the recipe that closed 0x00C54C10. If it
  matches, it lands as ordinary C++. Library grade is only for a row that still differs after a bounded effort, with no 100-grade chase.
- **Library grade = proof + portable twin.** CRT stubs are portable because every target has a CRT; CODAApi.lib is binary-only Win32 x86, so a bare
  body-less stub would break the portable build. A library-graded GameCODA row therefore needs all three of:
  - a `// Toolchain-library:` proof that libsig `lib_verify.py` reproduces, with every resolvable relocation agreeing, not "relocations masked", and the
    member's `@comp.id` checked against the Rich header;
  - a plain-C++ twin under `FABLE_PORTABLE` with the same semantics;
  - a dual-path test, as in ASM_RULES.
  Until all three exist the row stays held.
- **Wording:** "byte-identical to GameCODA 2.10 <lib> member <obj> <symbol>", never "linked from". Retail linked a different SDK build (member offsets
  and channel counts differ in places).
- **Scope:** CAGE / Toolkit ship source, so hits there follow the vendor_libs.tsv rule (compile the unmodified vendor source), not this one. Landed C++
  hits stay C++.

## 2026-11-19 advisor decision (owner delegated): `__assume(0)` as an unreachable-end marker
The owner's acctC rule (WF_RULES.md line 117) rules out `__assume` as a spelling of a release ASSERT. That stands.

A separate, narrow use is accepted: `__assume(0)` as an `_MSC_VER`-guarded unreachable marker, the same form the shared CPersistContext.h switch
defaults use. All of these are required:
- retail lacks the guarding test at cited addresses;
- a second release build (Ego_r VC10) drops the same test;
- the plain fall-off-end spelling was graded and differs.

Rules for the source:
- Write `#ifdef _MSC_VER __assume(0); #elif defined(__GNUC__) __builtin_unreachable(); #endif` so the portable build has a terminator.
- Mark it with an `// Inferred:` line.
- Never use it for its side effects (GOTCHAS: it changes float parameter forwarding and scratch rotation).

First case: 0x005DC340 CAnimationSet::GetAnim (hand4f b2).

## 2026-11-20 advisor decision (owner delegated): explicit-instantiation scaffolding for private members

The nf86 / nf88 pick-any scaffolding (GOTCHAS) names the COMDAT member from an anonymous-namespace helper. When that member is private and
the derived-class helper cannot name it, a row may instead name it as the template argument of an explicit instantiation in an anonymous
namespace (`template class CIsEqualReference<&CNavigatorAStar::IsEqual>;`; access is not checked there, C++03 14.7.2/8):

- Only for a private member the derived-class helper cannot name; otherwise use the nf86 / nf88 form.
- Mark it `// Fidelity: scaffolding, not retail`. Check with objdump that the helper symbol is in the per-TU `?A0x...` namespace.
- Never widen the member's access in a header opt-in (it changes the leaf, `ABE` -> `QBE`), and never add a `friend` to a shared header.

First case: 0x00A1A170 CNavigatorAStar::IsEqual (landing nf150).

## 2026-11-20 advisor decision (owner delegated): a static's initializer / atexit-destructor pair may land with its known LNK2005
- A static's dynamic initializer and atexit destructor are emitted only by a TU that defines the static. When the two are separate
  rows, each row must define the static, so linked together they give LNK2005 on it. No row-level respelling removes it.
- Such a pair may land when all of these hold: it is the initializer and the atexit destructor of one static; both rows are in the
  same landing; no other landed object defines the symbol. Record it as DEBT in NAME_FIXES_QUEUE.
- The fix is the TU-emitter rule (queued at the nf159 landing): one TU emits the initializer, the destructor and the static.
- This is not the 0x00DCF6E0 case (nf163). There a row-level fix existed (make the owner's copy pick-any), so the row was held.

Cases: 0x0121F0E0 / 0x01229BE0 CTCAIScratchpad::ActiveGossipCategories (nf89; clashes at HEAD), 0x0121E030 / 0x01229280
CCreatureActionBase::RegisteredActionEvents (nf164).

## 2026-11-20 advisor decision (owner delegated): `engine/_quarantine/<Class>.h` is not a shared header for the local-model rule
- gen_class_headers.py writes a class to `_quarantine/` when donor and retail disagree on its layout (a GetSizeofClass size
  conflict, or more conflicting files than agreeing ones). Such headers are kept off the rewriter path.
- The verifier's shared-header test (verify_and_land.shared_header_redefinitions, the SHARED_HEADER_AVAILABLE warning) does not
  flag a local model of a quarantined class. Landed 0x0096CD70 models CAIStateGroup_EatAtTable locally with no warning.
  So for the rule a quarantined class is header-less: a row may model it locally, in the landed sibling's shape (q11715).
- A quarantine header may still take declaration-only opt-ins for callers (nf160, CEngineLandscapeMap). It never models
  `this`, a base or a by-value member of a landed row.
- Take any spelling that passes such a class as an argument from the PDB symbol, not from the header: the 2026-10-26
  class-key pass left most CAIStateGroup_* / CCombatSequence* keys unproven.
- The end state is each such header's promotion to `engine/` (queued). Until then, the generator's hand-edit guard covers only
  `engine/` (RETAIL_MARK). A regeneration would overwrite a hand opt-in in `_quarantine/` (queued as a tools item).

## 2026-11-20 advisor decision (owner delegated): pick-any copies of functions that have their own landed row
- A row's object may define a pick-any (COMDAT) copy of a function that has its own landed row only if that copy grades MATCH or
  RELOCATION_MATCH against the owner's address (or is byte-identical to the owner row's body, relocations aside: see the 2026-11-20
  relocation-target addition below). A NEW copy with a
  different body is held, or made declaration-only through a header opt-in (the `FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY` pattern).
  Copies already present at HEAD are debt, queued, not licence for more (NF176 0x0051BCD0, NF173 q6927 / 0x007502D0, NF178 `_M_erase`).
- Stand-in DATA COMDATs (a row-local stand-in vtable) follow the HEAD-counterpart rule: if the stand-in already differed from another
  landed row's copy at HEAD, a further change lands and the port onto the shared header is queued (nf176 0x00502170).
- Landings check it: every symbol a changed row newly defines that the binding table maps to another address is compared with its owner
  (`symdiff.txt` TABLE: lines; `$S/tools/cmpbody.py`).
- Ownership comes from the address, not from a table line (advisor, nf181). A binding-table line made by another row's stand-in
  binder is a split to queue, not proof that the address owns the symbol. Before holding on a DIFF, check two things. First, what does
  the changed row itself bind at that address (gate pairs)? Second, what does the address's body say (the vtable it stores, the row's
  Leaf)? If the row binds the owner's own Leaf there, and the address's body is another function, the new copy has no owner: it lands,
  and the wrong table lines and labels are queued (nf181 0x00485CF1 / 0x00485420 against 0x00993B10 = ~CDataStream and 0x009BBBF0 =
  CheckUnknownSymbols). A row that binds the copied symbol itself at a retail site, with different bytes, is held.
- Copies with no table line (a self leaf of an unreferenced owner) are compared by hand against the owner's object (nf181 0x009568E0
  against 0x009566C0).

## 2026-11-20 advisor decision (owner delegated): size-pragma scaffolding for pick-any copies; STLport spellings for STL instances
- A shared-header opt-in may wrap an inline definition in `#pragma optimize(...)` (marked `Fidelity: scaffolding, not retail`) only
  to give a pick-any COMDAT the owner's body. All three conditions must hold:
  - the row's own body grades the same with the macro removed, and only that one COMDAT changes;
  - the owner row sets the same pragmas itself (cite it in the opt-in comment);
  - the comment says the block ends with `optimize("", on)` (VC7.1 has no push / pop for optimize), so a row using it must not
    carry a row-head optimize pragma before its include of the header. A row-head pragma placed after that include is untouched by
    the reset and is allowed (advisor decision, owner delegated, landing nf189: 0x005D66BA includes fable_map.h, then sets
    `optimize("y", off)` / `("s", on)` as its owner TU did).
  The owner's strong out-of-line copy then gives one LNK2005 with identical bytes; queue the owner's port onto the header (nf184:
  FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY vs 0x0042AAB3, FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY vs 0x005B33F4).
- STL template instances: the PDB name sets (egor / fablewin / egod) hold no STL instance names, so the attested spelling is the one the
  repo's STLport headers produce (descriptive-by-library). A count-neutral move of a caller from a fake stand-in (e.g. `set<>::insert_unique`)
  onto that header spelling lands when the self row's body is that STLport function; queue the self row and the other binders to
  converge (nf184 0x009A7D40 -> 0x009AAEB0).

## 2026-11-20 advisor decision (owner delegated): per-TU anonymous-namespace hashes are one spelling
- VC7.1 names an anonymous namespace `?A0x<hash>` from the compile path, so each row (its own TU) emits its own hash for one retail
  anonymous-namespace name, and no row text can reproduce retail's. Count spellings with `\?A0x[0-9a-f]{8}` stripped: the name is the
  identifier plus its type code plus "anonymous namespace" (nf192: the streaming-font glyph-cache statics 0x013BDA24..0x013BDA40,
  Ego_d PUB32 `?A0xfe46a155`; same ruling for q11864 GFontTextureCache 0x013C5A80).
- After the strip, each address must still have exactly one spelling, type code included. A leftover stand-in or a different type
  code is a split as usual.
- Leaf lines, queue lines, NAME_FIXES and READMEs write the PDB spelling with the PDB's hash (as 0x009A3640's Leaf does);
  RENAMES may write `?A0x<tu>`. A hash found in no PDB came from a rebuild object: correct it to the PDB's.
- Until gate.py / tablecheck strip the hash (queued), judge GREW / CREATED lines at such addresses by the normalised count.

## 2026-11-20 advisor decision (owner delegated): "relocations aside" means relocation values, not relocation targets
- A pick-any copy matches its owner only if its bytes match AND each relocation binds the same symbol as the owner's. Only the
  relocation values (the bytes the linker fills in) may differ. If one relocation points at another symbol, that is a different copy:
  if the linker keeps it, the image differs. Example: an equal-bytes copy that binds a `""` literal where the owner binds
  `?NullChar@CCharString@@1DB`. Hold such a row, or make that function declaration-only through an opt-in (nf201 0x008E9850, held).
  `cmpbody.py` reports both, as `bytes equal` and `relocs equal`; both must be True.
- (nf205 amendment) "Same symbol" means the same retail address: a relocation that names the owner's stand-in spelling for the same
  retail address (both symbols external, both bound at that address in reloc_bindings.tsv) is not a different target, so the copy
  matches and the stand-in is queued for respelling (0x008E9850's GetCRC copy: ?NullChar@CCharString@@1DB against 0x00404310's
  ?g_EmptyString@@3QBDB, both 0x0129AAF4). A symbol defined inside the row's own object (a string literal COMDAT) is always a different target.
  (nf212 refinement, advisor decision, owner delegated) The exception is a pick-any COMDAT defined in the object under the exact
  spelling the owner's own copy names: the linker keeps one body for that name, so the call resolves to the same function. All three
  must hold: (1) the owner's copy names the same symbol (objdump -dr on the owner object); (2) the in-object symbol is a pick-any COMDAT
  (no LNK2005 on it in the landing link); (3) it is itself equal to its own owner (pickany_check OK). 0x00A2ED10 / 0x004B4A10:
  push_back reaches the in-object IAE `_M_insert_overflow`, which equals 0x00433530 (316 = 316), and owner 0x0044BFF0's push_back names
  the same symbol. A different spelling (the 0x006EAC50 hold at nf210) or a strong definition is still a different target.
- (nf203) A function copy that the HEAD object already defined, and that already differed from its owner, may change body in a port and
  stays debt (the function-copy counterpart of the nf176 data-COMDAT rule). A row is held only for a newly defined copy, or for a copy
  that matched at HEAD and stops matching (0x00993B20's ??_GCDataStream / ??_GCDataInputStream: 34 v 36 at HEAD, 28 v 36 after).

## 2026-11-21 advisor decision (owner delegated, landing p25): pick-any checks for new rows; size-pragma condition 2
- A NEW row has no "before" object, so its pick-any check compares every function COMDAT its object exports that the binding table
  maps to another address with Fable.exe at that address (relocation fields zeroed on both sides; full symbol names, never
  truncated). An owner-object comparison that reports MISSING (the owner names another spelling) does not count as checked.
- Every differing copy gets the two checks from the 2026-11-20 rule: does the row's own leaf relocate to the symbol, and did HEAD
  already bind that symbol there? Leaf-bound, newly bound and different: the row is held (p25: 0x0078FD60 / 0x00795300 on
  GetAllThingsInRoughArea, map::operator[], vector::push_back). Not leaf-bound and bound at HEAD: debt, queued. Not leaf-bound and
  the table line is a folded alias of another function: lands, with the split queued (p25: 0x00B0DC20's 5-byte
  ~CIntelligentPointer<CParticleEmitter> thunk against ~CBaseIntelligentPointer 0x00A01C10).
- Size-pragma scaffolding, condition 2 ("the owner row sets the same pragmas itself"): it is met when the pragma restores the owner
  row's command-line mode (e.g. `optimize("t", on)` in a `/Os` row, for an owner built with the default /O2 /Oy /W3). Cite the owner's
  catalog flags in the opt-in comment. Conditions 1 and 3 stay as written. (p25 0x0064FDB4 Clear: the device worked, but HasTC and
  LowerBound still differed, so the row was held after its one bounded attempt and the header change did not land.)

## Coordinator note, 2026-11-22: the authoring checklist from Codex pass 28
Before declaring a long row stuck or parking it, run the 20-item checklist in `docs/pipeline/CODEX_P28_LESSONS.md` ("Authoring checklist"):
regrade the draft on today's headers, finish every block, prove the mangled Leaf, copy the TU flags, no local types, every debug-record local in
its scope, callee signatures and the wrappers the source called, branch order as written, the NScript generator form, callee-body visibility
toggles, cleanup / termination targets, expression shape last, a behaviour test with distinct arguments. Park only after at least five genuinely
different logged source shapes that do not shrink the residue. Item 19 (donor debug-build code) is not available on the lane yet.

## 2026-11-24 owner decision: a build mode that compiles rows in retail's object grouping and order
Owner (2026-11-24), on Account B's finding that some residues come from compile order inside one retail object: "2 sounds promising" --
option 2 of three (1: accept a per-row workaround device; 2: add a build mode that compiles rows in retail's object grouping and order;
3: leave the rows open). Evidence (B24A5, 0x00E93EB0 CV_HauntedHouseScript::Main): a gdb hook on c2 shows retail compiled Main BEFORE
`TRetVoid::Return` and `vector<CScriptThing>::operator[]` and every other callee before Main, which reproduces retail's two odd calls exactly
(budget 34701 accepted / 35072 rejected at the 35000 limit); Ego_r puts every quest script in one object, register_scripts.obj. A one-row TU
always compiles callees first, so no row spelling can express that order. Affected so far: 0x00E93EB0, 0x00E20600; likely the late-function
inline residues of other quest scripts and the TesselateEdge twins (0x00BE4660 / 0x00BE4FE0, one object with 0x00BE3970).
Consequence: rows whose residue is proven to be compile order are NOT to be fixed with row devices (option 1 is not chosen); they wait for the
build mode. Design and ownership are agreed in docs/journal/2026-11/COORDINATION_LOG.md.
**Safeguards (owner, 2026-11-24: "ensure that this doesnt cause any issues with byte matching accuracy and code behavior matching retail code
behavior").** Object mode is allowed only behind these gates; each is a hard rule, not a guideline.
- **What is measured and what is not.** Measured: with a gdb hook forcing two callees down c2's budget path (plus an unexplained +250
  caller-growth bump), 0x00E93EB0's late calls come out as retail's (that object still graded DIFFER 5333v5321). NOT shown: that putting rows in
  one TU makes stock c2 compile them in that order -- B24A4 measured that c2 compiles callees first whatever the definition order, and a large
  TU changed nothing. So the approval is for a mode whose working form is still unproven.
- **Gate 0 (blocks everything else; scratch only, touches no landed row):** stock cl 13.10.3077, real landed sources, no hook, no patched
  c2, no bump: 0x00E93EB0 must show both odd calls and the `~vector` call. Tracing to observe is allowed; anything that forces a decision is
  an oracle, not a build. If Gate 0 fails, report that the approved option has no working form yet; never substitute a forcing device.
- **Same comparison:** same oracle, same relocation masking, same grader core as the per-row path. Object mode changes what is compiled,
  never how it is compared. Object-mode rows get their own grade label; the default per-row build and its grades are unchanged.
- **Order from evidence, fixed per object:** one order per retail object, taken from evidence (retail address order / Ego_r module order),
  checked on a small control object the lane links itself to confirm that address order reflects emission order; fixed before any grading and
  never tuned per row.
- **One flag set and one header configuration per object:** rows carry their own flags and `FABLE_RETAIL_*` defines, and with include guards
  the first row's defines would silently win: any conflict is an ERROR, not a merge. Pragma and macro state (`inline_depth`, `optimize`, the
  VC6 for-scope macro) must not leak across row boundaries. (Note: landed /d2inlS15 rows and the stock-flag 0x00D0A830 come from the same
  retail object, so one of those flag sets is a fitted device; object mode has to settle that, not paper over it.)
- **Mates:** only landed, verified sources; no stand-in or invented bodies. The mate set of every object-mode row is recorded; any later
  landing into that object rechecks all of its object-mode rows.
- **Regression control:** every landed row of the object is regraded in object mode; each one that changes is reported as a finding, never
  patched to fit. **Negative control:** a deliberately wrong body and a permuted order must grade DIFFER.
- **Bindings and pick-any** are checked in object mode too: with more declarations visible a relocation-masked match could hide a different
  callee; every COMDAT copy in the object is compared with retail.
- **Behaviour:** a row's source is byte-identical in both modes and still compiles alone, so the portable (`FABLE_PORTABLE`) build and the
  behaviour tests are unchanged. `vt.py` compiles test fixtures standalone, so the tests do not exercise the grouped build: behaviour equality
  in object mode rests on the byte match plus the binding check, and is stated that way.

## 2026-11-24 advisor decision (owner delegated): a new row whose leaf calls a NEQ copy identical to one already landed
Amends the 2026-11-20 entry ("Copies already present at HEAD are debt, queued, not licence for more") and the 2026-11-21 entry
("leaf-bound, newly bound and different: the row is held") for one case. A hold exists to make the author take the fix. When the only
copy-EQ texts move the row's own leaf and the cause sits in many landed sources, the hold never ends and buys nothing: the row's copy
is the one the linker can already pick. The row lands as debt only when ALL of these hold:
1. **Same copy already landed.** The NEQ copy equals a landed row's HEAD copy at object level: bytes, relocation symbols and COMDAT
   selection (compare the two objects; "same form" is not enough). This one blocks the landing.
2. **No fix on record.** Measured probes show every copy-EQ text changes the row's leaf, and that record is committed (an agent hand-back).
3. **Nothing else.** Every other leaf-called copy is EQ under both checkers (`pickany_vs_retail.py`, `pk_all.py`) on the lander's object.
4. **Booked.** The row carries a `// Pick-any:` note, and the copy's T3 line lists it as an emitter.
Where a fix exists the row still holds and takes it (0x00839940 waits for NF341's GetDef<CMaterialDef> repair). The section 6 reason
(NAMING_AND_HEADERS: the linker can pick your copy and break the other function) does not apply: the copy is identical to one already
in the link. First use: 0x00ED8680 CTB_Bard::Main (~CScriptGameResourceObjectMovieBase 0x006E7B80, identical to landed 0x00D62480's copy;
NF343 group 2: both copy-EQ texts move the leaf 3636 v 3652; batches/2026-11-24_landing_acctB_b24d).

## 2026-11-25 advisor decision (owner delegated), landing nf398: size-pragma scaffolding, a narrow extension
- Condition 1 ("only that one COMDAT changes"): a compiler-generated function that is instantiated inside the wrapped definition may
  change along with it, as long as its new copy is EQ at its bound retail address. Example: the `??_G?$pair` that the wrapped
  `_Destroy` calls became the 31-byte body of 0x004DBE39, where the plain /O2 copy is 33 bytes.
- Condition 2 ("the owner row sets the same pragmas itself"): when the wrapped function's own retail home has no row, the condition is
  met by two things together. First, a unique tier-2 home for the copy (here 0x004DD6F3, which has no E8/E9 references). Second, an
  owner row for its callee that sets the pragma in its source, not through CompilerFlags (here 0x004DBE39 and 0x004DFB8F, both
  `#pragma optimize("s",on)`). A later row at the home address makes the condition literal.
- Each opt-in in a block is checked on its own with the others in place. For example, 0x00813460 without
  FABLE_RETAIL_STLP_GROUP_MEMBER_TREE_CLEAR_SIZE_OWNER changes only `clear`, from 41 to 51 bytes, and its leaf stays RELOCATION_MATCH.
  Condition 3 is unchanged.

## 2026-11-25 advisor decision (owner delegated), NF409 R-NF409-2: row-end optimize reset as scaffolding
This narrowly extends the p25 clause, which allows a pragma that restores the owner's command-line mode. A row may end with `#pragma optimize("", on)`, marked `Fidelity: scaffolding, not retail`, when all of the following hold:
- It changes only end-of-TU template instances. VC7.1 takes their optimize mode at the end of the TU.
- It never changes the leaf, or anything spelled out in the row text.
- Every COMDAT it changes is EQ at its bound retail address, or tier 1.

The row comment cites the measurement and the flags:
- a comdat_cmp of the row with and without the pragma, showing that only the template chain changes;
- that the row has no CompilerFlags override;
- that the tier-1 definers carry no size pragma.

Example: 0x00656EAD's pick-any `~vector<CEnginePrimitiveHandle>` becomes EQ at 0x00440C40, while the row's own size-mode `??_G` is unchanged.

## Typed-in retail addresses (baked grades, 2026-11-25)

Never write a retail address as an integer in row code. This covers:
- casting a string VA to a pointer, or passing it as an `int`;
- `self->vtbl = (void*)0x01230BA0;`;
- a global, IAT slot or function address typed as a hex number;
- a catch funclet returning its resume address as a constant.

The bytes match, but the address has no relocation, so the code is not reconstructed. These rows grade apart from genuine:
- `baked_string` is detected by `crawl/purity.py baked_string_addresses` and `crawl/baked_symbols.py`.
- `baked_address` (advisor decision, owner delegated) is detected by `purity.baked_data_addresses`, keyed on `rebuild/oracles/data_vas.tsv`.

What to write instead:
- a string: the literal;
- a vftable store: the class, whose inlined ctor/dtor stores its `??_7`;
- a global or function: its bound symbol.

Audit: `rebuild/candidates/manual/audits/2026-11-25_baked_address/`.
