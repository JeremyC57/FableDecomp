# Chunk 627 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 `/Od` (`#pragma optimize("", off)`) lays out function-scope locals in symbol-table (NAME hash) order, not declaration
  order: renaming a pointer local `record` -> `pRecord` moved it from ebp-4 (+4 pad, `sub esp,3Ch`) to retail's ebp-0x34
  below the 48-byte struct (`sub esp,38h`) (0x009F7050). An inner `{ }` block around one local also changes the layout.
  At /O2 local and parameter names had no effect (0x00994A80, 0x00475DFA, 0x00492ED1 sweeps).
- `return a && b && c;` in a bool function gives `mov eax,1 / xor eax,eax` tails (an `if (..) return true; return false;`
  gives `mov al,1`), and it changes the callee-saved map: the long-lived local takes edi and the block-only pointer gets a
  late `push ebp` (the if/return form gives ebp at entry + shrink-wrapped edi) (0x007723F0). A `mov eax,1` bool row is not BOOL.
- XY squared distance: a C2DVector temporary with an inline `X * X + Y * Y` member squares dx first (`fld st(1) ; fmul st(2)`
  twice); named dx/dy locals square dy first (`fld st(0) ; fmul st(1)`); `d2 = dx*dx; d2 += dy*dy;` also works (0x007723F0).
- Fable's IS_THING_ALIVE is ONE `template<class T> bool __fastcall IS_THING_ALIVE(const T& thing)` (`thing != 0 &&
  !thing->Dead`); over CIntelligentPointer<T> its `operator T*` and `operator->` give the two GetPItem calls, and the
  instantiations mangle exactly as FableWin.pdb (`??$IS_THING_ALIVE@PBV...@@@@YI_N..`, `..V?$CIntelligentPointer@..`).
- Init() (any shared virtual call) written in both arms of an if/else tail-merges with the init address in ecx in both arms;
  one call after the if/else leaves arm 2 with `lea edx` and rotates later vtable registers (0x0049DCC0, as CGame::Play).
- GetInterface into a local out pointer + `member = local;` under the found test loads the component into eax; the member's
  address as the out pointer loads ecx. Needed together with the inline GetBaseDef() getter (0x004BFC50).
- Inlined STLport `operator[]`: `*(begin() + n)` keeps base and index in separate registers (index esi / base edi here),
  `*(_M_start + n)` / `_M_start[n]` materialise `lea esi,[base+idx*8]` across a call (0x00475DFA).
- Retail's by-value 16-byte struct argument copy (`sub esp,10h ; mov eax,esp` with a two-deep load pipeline, first two words
  loaded before `sub esp`) is not reproduced by POD, user-copy-ctor or member-wise forms (0x00978B90, 0x00BEEDA0): POD gives
  the pipeline with `mov ecx,esp`, a user copy ctor gives `mov eax,esp` but one load ahead.

Review at landing:
- No row needed more than 4 grades except 0x00994A80 (5 grades: one exploratory batch of 4 variants after the first probe);
  it is parked. 0x007723F0 used 4 matching grades (d, e, f, g) plus one hygiene re-grade (h: CPlayer/CPlayerManager/CTCBase
  shared headers through the define pattern, still RELOCATION_MATCH).
- (Superseded by the follow-up section below.) vland (first read-only run, payload.json + oracle.tsv): 4 RELOCATION_MATCH + PASS. Warnings left: 004bfc50 SHARED_HEADER_AVAILABLE
  CDefPointeeBase/CDefPointer/CDefinitionManager (the local def-pointer model of the landed CatchMessage 0x004BFD10 of the same
  TU; engine/CDefPointer.h's Release()/Delete() shape was not tried); RELOC_NAME on 0x00991620 / 0x013B8394 (descriptive) and
  0x00487DD0 (catalog label CPlayer::GetControlledCreature vs the PDB PeekPControlledCreature -- NAME_FIXES.tsv).
- Local base models next to existing headers: CTCBase (004bfc50, 007723f0) and CDataInputStream/CCompressedMemoryDataInputStream
  (00994a80, 00475dfa) because the generated headers declare `void* __vftable`.
- Landed-source naming errors found: see NAME_FIXES.tsv (CDataInputStream::SetPosition QAE->UAE, DecompressStream bool*->bool&,
  front-end ctors CGame*->CGame&, ReadC2DVector->Read2DVector, GetPItem returns CBaseObject*, PeekPControlledCreature,
  GetMainPlayer const, PeekPlayerManager, GetSubDef template). Landed 0x00991620 (`CFlaggedObject_Retail::func_00991620`) and
  0x009915E0 (`CWorld::SetMiniMapEnabled`, a byte-twin-looking label next to it) should be re-checked against the sound manager.
- The chunk brief's hint "std::_Cons_val for pair<CCharString,C3DVector>" for 0x00BEEDA0 is wrong (it is __pop_heap).

## Follow-up (2026-10-31)
- The shared retail def model (engine/CDefPointer.h Release() with its trailing `Object = 0`, AddRef/Release/Delete of
  CIVCountedPointeeBase) reproduces the local model's bytes in a caller: the dead store after the destructor is dropped. Prefer
  the headers over the local CatchMessage-style def model; the same-TU GetSubDef stand-in can be the landed body verbatim.
- Placeholder spellings already landed for an address (DAT_013B8394, CFlaggedObject_Retail::func_00991620) beat a new
  descriptive name: a reinterpret_cast between the two landed `this` models costs no bytes.
