# Extra rules for every chunk agent (on top of your PROMPT.md)

- No git at all, not even read-only commands (git log/show/status/diff/grep).
- Never write anything under /home/user/FableDecomp; work only inside your own agentNNN directory. Never delete or move your src/, test/ or STATUS.tsv; they are the hand-over. Do not `cd` into your directory; use absolute paths, and always source the env lines the brief gives before compiling.
- Never kill processes you did not start; if a cl.exe of yours hangs, kill it by its explicit PID only (never pkill -f).
- Respect the 6-grade budget per row (owner 2026-11-10: "increase try budget slightly", was 4; keep a per-row count in STATUS.tsv notes): after 6 grades, park the row with its best source and a one-line "next idea". Grade with gall.sh or p.sh: g.sh ignores the `// Leaf:` and `// Flags:` lines, so /Gr and /GS rows show a false DIFFER through it. Keep exactly one real `// Flags:` line per source (any comment line starting `// Flags:` is parsed as flags).
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
