# NF217 lessons (one line each)
- Census every landed binder by compiling it once (cl only) and listing the bound symbols with objdump: the q12372 queue line named one odd binder, the census found three (0x00621720 / 0x00621C80 bound a third stand-in, SetScreen).
- An in-place respell that needs an enum which lives only beside a shared class (EScreenType in NHeroInformationScreens_CBase.h) is not in place: port the row's local models of that header's classes onto it; a local `enum` would be a new local model.
- Including a shared header can pull fable_string.h into a row that models CCharString locally: drop the local model and define FABLE_RETAIL_CCHARSTRING, or a literal argument fails C2664 (the default CCharString(const char*, long) has no default length).
- A strong explicit specialization (COMDAT selection 1) beside header pick-any copies is fixed by `template<> inline` + a guarded __declspec(noinline) left in the row's own optimize region; the header opt-in is unusable when it resets optimize to a command line with /Oy- (frame pointer).
- comdiff (every function body, relocations zeroed) proves a codegen-neutral re-port in one line; use it for text-unification entries (q12380: 0x00C576E0 / 0x00C57C90 byte-identical to HEAD).
- Run callers.py and absrefs.py in separate calls and tag the tool in REFS.tsv: their output formats are identical.
- Build RENAMES leaves from objdump of the graded objects, never from a format string (a stray `@` slipped into three leaves once).
- The GameCODA 2.10 SDK is fetchable as BUILDING.md says (44 MB, md5 matches) when `$S/sdk/gamecoda` is missing; libpk classes (CAudioObject, CircularQueue) are not in it.
