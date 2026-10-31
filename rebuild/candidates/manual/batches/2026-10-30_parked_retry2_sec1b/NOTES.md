# retry/sec1b -- parked-row retry sweep, wave 2 section 1b (2026-10-30 .. 2026-10-31)

244 WORKLIST rows, every one attempted by hand (STATUS.tsv: one line per row, 244 + header).

| status | rows |
|---|---|
| matched (src/, RELOCATION_MATCH or MATCH, test/ PASS under g++ and VC7.1) | 20 |
| improved (differ/, fewer difflines than the parked base) | 57 |
| held (>= 1 graded edit, up to 3-5; residue in STATUS.tsv) | 167 |

Dry `verify_and_land.py payload.json oracle_final.tsv` (LANDVERIFY_WORK=lv, no --land): **WINS 20**, no FAIL, no
RELOC_IDENTITY, no RELOC_CONFLICT (after the 0x005D99D0 respell below). All 20 behaviour tests were rewritten to C++98
so the verifier's VC7.1 `/Od` test compile accepts them (no nullptr / unique_ptr / brace-init / nearbyintf; explicit
member init instead of `T()` value-init, which VC7.1 does not zero).

## Per-lever tally (the 20 matches)
- same-TU `__declspec(noinline)` stand-in body for a template callee (inside `#pragma auto_inline(off/on)`): 5 --
  0x0054B4B0 / 0x0054DF50 (`_M_insert_overflow`), 0x005DD090 (`_M_insert_overflow`), 0x005A45F6 / 0x005A731F (map
  `operator[]`). Stand-in names: GrowVectorStorage_54b4b0 / _54df50 / _5dd090, FindOrInsertSellList_5b3167.
- local formulation (named / reference locals, shared variables, nested if): 5 -- 0x004978A0, 0x005CDD86, 0x0040E710,
  0x004FAB40, 0x004AFBB0 (reference locals for the thing's name and id taken at entry fix the ebx/ebp colouring).
- TU optimisation pragmas: 3 -- 0x0041CB10 (`optimize("s",on)`), 0x00581B33 and 0x005786B5 (`optimize("s",on)` without
  `("y",off)` gives the biased `lea ebp,[esp-N]` frame).
- over-span bounds fix (catalog length ran into the next function; BOUNDS_FIX.tsv + oracle_extra.tsv): 2 -- 0x005E9840,
  0x0055A5D0 (plus 0x0054EAC0 395->243 and 0x0057F70F 54->36 on held/improved rows).
- model rebuilt from retail bytes / PDB signatures: 2 -- 0x00645099, 0x00646010 (retail_asm FloorToLong).
- shared engine headers (CCountedPointer.h): 2 -- 0x00558770, 0x00641020.
- STLport 4.6 verbatim: 1 -- 0x00412210.

Improved-row levers (57): the same-TU stand-in again (0x005CBC36 198->181, 0x004085F0, 0x00533D30, 0x005C4671),
retail_asm ROUND / FloorToLong / ceiling helpers (0x0041AFA0, 0x00646762, 0x0057C4F1, 0x005C7452, 0x005430F0),
branch order (0x0061AA80 84->15 lines, 0x0054FBC0), decoded call chains (0x0055D180 137->48, length exact; 0x005590C0
108->79), map-pointer / last-node locals in an inline find (0x004C9F60), PDB class models with inline dtors
(CDiskFileWin32 over CBaseClassNonCopyable in 0x0055D180), `__forceinline` user copy ctors (CRGBColour, C2DVector),
TC-map reference locals (0x004C0560/08C0/0C80), and first bodies for the stub rows (0x005C7452, 0x005430F0, 0x00428494,
0x004184BD -- partial).

Inert this session (recorded so nobody retries them): `optimize("a")` / `("w")`; declaration-order swaps for frame-slot
packing (0x00632830, 0x004B4260); `_Destroy` / GetTestQuestsToDisplay / map-insert stand-ins on 0x0061A9D0,
0x0061AA80, 0x00632830; dtor-form CDefPointer in 0x005C7452 (adds EH state, worse).

## Names
NAME_FIXES.tsv rows this session: 0x0048DA62 GetStringWidth (QBE const), 0x009E21D0 GFACoordToVCoord (references),
0x005D99D0 `?GetInventoryCategory@CTCInventoryItem@@SIJJ@Z` (the parked rows spelled it as the free function that
landed 0x005BE8D3 binds to 0x005D97B0 -> RELOC_CONFLICT; respelled in src/005a45f6.cpp and src/005a731f.cpp), plus
the earlier rows (deque helpers, GFAXToVX / GFAYToVY, ScaleAlpha, PeekGuiDef, sprite ctor, CLightInfo Reset, Transfer
templates, Draw2DText). Descriptive names that need review at landing are listed in the hand-back.
0x005DD090 was respelled to the Ego_r.pdb signature `?TransferBinaryIn@CAnimationEntry@@QAEXAAVCMemoryDataInputStream@@@Z`
(the stream by reference) with CDefString::TransferBinaryIn taking `CDataInputStream&` (PDB); still RELOCATION_MATCH.
0x005A45F6 / 0x005A731F keep `unsigned long count`: Ego_r.pdb mangles the parameter `long` (`JJ_N`), but `long` turns
retail's unsigned `ja` compares into `jg` (31 / 23 difflines), so the PDB build and the shipped build may differ there.
