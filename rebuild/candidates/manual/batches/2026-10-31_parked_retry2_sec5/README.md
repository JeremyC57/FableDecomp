# Parked-row retry sweep, wave 2 section 5 (2026-10-31): 18 of 354 parked rows landed, 50 improved

Section 5 = retail 0x00989230-0x00B16410 (late engine: file installer, resource/texture managers, CBankFile, GameCODA head).
354 rows have a STATUS line; 315 got graded hand edits, 39 did not (10 SSE rows whose `movaps`/`mulps` are intrinsic-emittable,
so not retail_asm; 10 bare-fistp drafts with no body; 11 triage drafts with no body; 8 skeletons) → section 5b
(`$S/retry/sec5b`, 39 rows). Landed 18, all PASS (6 MATCH + 12 RELOCATION_MATCH), no retail_asm row: 0x009ACE90, 0x00A0DB60,
0x00996610, 0x00A8A9D0, 0x00B094B0, 0x009CE050, 0x00994B10, 0x0099C7E0, 0x009BAE70, 0x009C05C0, 0x00A0DCD0, 0x00A6BFC0,
0x00A98760, 0x00A987B0, 0x00AC09A0, 0x00AF8680, 0x00AF86C0, 0x00AF8B00. 50 improved in `../../pending/2026-10-31_parked_retry2_sec5/differ/`
(57 files, 9 retail_asm-classified: they use the landed FloatToLong/FloorToLong helpers), 7 header-moved in `hdr/`; 286 held.
Levers (matches): destroy a local object of the class elsewhere in the TU so VC7.1 inlines the implicit dtor into `??_G` 3; heap
pop as `if(!entry){if(grow){create;reload}else return 0;}` 3; extent fix — the function ends at the int3 VC7.1 emits after a
noreturn `_CxxThrowException` call 2; stepwise sums in retail term order 2; named local whose temporary dies with its block 2;
explicit temporary instead of a default argument 1; guarded scope + do/while 1; shared CCountedPointer.h 1; union in/out slots 1;
STLport `_Vector_base` ctor order 1; inverted if + shared CDefPointer headers 1. Improvements: landed FloatToLong/FloorToLong
helpers (`__forceinline`) 9; stepwise dot product 7; copies by value / named locals 6; in-place `__forceinline SetInverse` into a
named matrix 4 (0x00A11F20 now 428v428); catalog extent fixes 3 (0x00A69C20 381→186 at 4 diff lines, 0x009C5890 710→389, 0x0099E690
344→180); a guarded do/while down-counter stops the 4x unroll 1 (0x00ADDBE0 332→80).
BOUNDS_FIX.tsv: 7 rows (two landed over 46 / 306; five pending). NAME_FIXES.tsv: none proven wrong in landed code; parked models
respelled (CFileInstaller::DirtyDiscError, CTBaseSingleton<CFileInstaller>::Get, IgnoreTimeStampWhenCopying, ExitThread,
CriticalSection, FindNextEngineMaterial const) — 4 review rows in the queue (two RELOC_NAME labels; the parked respellings; the
SmoothFadeValue twins 0x00B05E10 / 0x00B1AD90 noted not landed; 0x009ACE90 defines CDefPointer::Reset in the TU).
Review notes: wrong class keys in parked sources (0x009CFBC0 / 0x009CCEF0 / 0x009D5BF0 / 0x00994A80 `*_Retail`, 0x009E3640 /
0x009E3DF0 `CPixelFormat_Methods` → CBankFile / CPixelFormat headers); volatile-only residues (0x009C3590, 0x009C4440, 0x009C3FB0,
0x00991C10, 0x009919C0 NeedsSort re-read; unfolded add-ref in 0x00A69B70 / 0x00A69C20); SHARED_HEADER_AVAILABLE on eight landed rows
(CDataOutputStream, CPixelFormat, CBankFile, CBaseClass, CManagedTextureData, CResourceList); lead: 0x0099FB60 inlines UnassignString
partially when its body follows the function.
Coordinator: vland --land 18/18, bindings_multi_check 0.
