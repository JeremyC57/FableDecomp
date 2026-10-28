# Extra rules for every chunk agent (on top of your PROMPT.md)

- No git at all, not even read-only commands (git log/show/status/diff/grep).
- Never write anything under /home/user/FableDecomp; work only inside your own agentNNN directory. Never delete or move your src/, test/ or STATUS.tsv; they are the hand-over. Do not `cd` into your directory; use absolute paths, and always source the env lines the brief gives before compiling.
- Never kill processes you did not start; if a cl.exe of yours hangs, kill it by its explicit PID only (never pkill -f).
- Respect the 4-grade budget per row (keep a per-row count in STATUS.tsv notes): after 4 grades, park the row with its best source and a one-line "next idea". Grade with gall.sh or p.sh: g.sh ignores the `// Leaf:` and `// Flags:` lines, so /Gr and /GS rows show a false DIFFER through it. Keep exactly one real `// Flags:` line per source (any comment line starting `// Flags:` is parsed as flags).
- No `__asm`, `naked`, `_emit` or `volatile` anywhere, tests included, and no identifiers containing the substring `_emit` (use `gEmitter`, not `g_emitter`). A dead load that survives a constant-true condition is the signature of a volatile member: park such rows after 1 grade as "volatile candidate". A test that needs an SEH fault calls `RaiseException(0xE0000001, 0, 0, 0)` declared as `extern "C" __declspec(dllimport) void __stdcall RaiseException(unsigned long, unsigned long, unsigned long, const unsigned long*);` -- do not include <windows.h> in lane tests (cl hangs). Keep every source self-contained: an #include of a file missing from the work dir hangs cl with no error. VC7.1 `<cwchar>` has no `std::wcsrchr`; naming `wchar_t` needs `<stddef.h>`. VC7.1 tests cannot use local structs as template arguments; a brace-initialised iterator in a `for`-init is a syntax error.
- Jump-table switches: relocation masking cannot check which literal goes with which case, so verify every case-to-string pairing against the retail table by hand.
- Import thunks: 0x00BFEB22 is MSVCR71 `operator new[]` and 0x00BFEA1A scalar `operator new`; call the one retail calls (the lander rejects a mismatch as RELOC_IDENTITY).
- The oracle rows were bounds-checked before cutting (rebuild/candidates/manual/BOUNDS_FIX.tsv). One known exception: a function whose last instruction is a noreturn call (or `throw`) owns the 0xCC VC7.1 emits after it, which the cutter strips; if your object is exactly one 0xCC longer than the row for that reason, note it in STATUS.tsv as "int3-after-noreturn", keep the source and its passing test in differ/ (as <addr>.cpp and <addr>.test.cpp) with LINE 1 the usual `// <Module>::<Name>(<params>) -- ...` line and LINE 2 a `// Grade: DIFFER(N+1vN) -- int3-after-noreturn ...` line (it is landed against the N+1 span). Any other row that looks cut wrong: say so in NOTES.md with evidence.
- Retail inline asm (`rdtsc`, a bare `fistp dword`, `fnstcw`/`fldcw`, `cpuid`, esp stack switches, hand SIMD kernels) cannot come from C++. Owner 2026-10-27: such a row is reconstruction, grade `retail_asm` (crawl/purity.py), with no instruction-count cap: write C++ around the retail `__asm` block, add a `// Retail-asm:` line citing the retail address(es) and naming the instruction(s) no C++ route produces (and the routes you ruled out), no naked/_emit, no `ret` inside the asm. Keep such sources in a `retail_asm/` folder (the land scripts' grep refuses `__asm` in src/; the coordinator lands them with vland.py). The x87 operand order of a double global times a float/long (`fld qword [g]; fmul dword [x]`) is a known lane gap: park after 1 grade.
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
