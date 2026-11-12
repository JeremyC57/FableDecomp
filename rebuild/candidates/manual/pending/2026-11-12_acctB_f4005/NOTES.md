# Hand chunk 4005 (CScriptDef::operator=): 1 of 1 byte-exact

Kept current for a hand-over. Everything is inside /opt/fable_lane/w22/agent4005; tools in tools/, probes in probe/, per-compile log
in grades.log, probe log in probes.log.

## What matched (1)
- 0x00F2A250 CScriptDef::operator= (11,711 bytes, `push ebx` 0x00F2A250 .. `ret 4` 0x00F2D00C; the row length is right, next
  COMDAT 0x00F2D010 is vector<CFeatDef>::operator=): RELOCATION_MATCH, no flags, test PASS under g++ and VC7.1.
  src/00f2a250.cpp, test/00f2a250.cpp.
- The function is the IMPLICIT operator=: the source holds no statement of it. What the row needed was (a) the header without the
  declared operator= and (b) the piece of retail's translation unit that decides two inline decisions in the base chain.

## Map (tools/map2.py -> map/events.txt, map/members.txt)
- 943 copy events after the base chain, one per member of engine/CScriptDef.h, in ascending offset = declaration order; 0 members
  without an event, 0 events without a member, every copy kind equals what the header's member type gives:
  548 long + 285 float + 45 CDefString as dword moves (eax / ecx / edx in rotation); 15 CCharString by 0x0099EFB0;
  50 vectors by vector<T>::operator=: CCharString 0x00432EE9 x14, float 0x0045EF2D x14, CDefString 0x0046078C x12, long
  0x00454886 x5, CConversation 0x00E54AA0, CFireHeartPatternDef 0x00D34D40, CFeatDef 0x00F2D010, CArenaRoundDef 0x00F25980,
  CHeroSoulsRoundDef 0x00F26C30 (one each). No header finding: the header is right for this row as it stands.
- Head 0x00F2A250..0x00F2A2B1: `call 0x00431F60` (CResource::operator=), +0x1c, +0x20, three xor / and / xor merges of +0x24
  (bits 1, 2, 4), +0x25, `lea ebx,[edi+29h] ; lea ebp,[esi+29h] ; call 0x00431F90`, Dirty byte `[ebx+0Dh]` -> `[ebp+0Dh]`.

## What decides the base chain (measured; commands below)
The only residue of the row was the head: retail expands CParentDefClassBase::operator= and CDefClassBase::operator= (and the
CVectorMap / CArray assignments) and calls only CResource::operator=; a plain build calls 0x00431ED0 (11625 bytes).
1. c2 decides an inline call by one of two rules (chunk 4004): a callee it has already compiled gets the COST rule, a callee not yet
   compiled gets the BUDGET rule (c2.dll 0x107284A3).
2. NEW (this chunk): a callee accepted by the budget rule must also pass a statement cap before it is expanded. c2.dll 0x1078DA87
   -> 0x107285BB -> 0x107284D5 walks the callee's IL statements, counts opcode 0x0D as 1 and 0x0F / 0x13 as 2, and refuses when
   the count exceeds (N + 2) * K + K, N = arguments of the call, K = [0x108A912C] = 3 (the extra K when [0x108A9208] is set, as it
   is here). For a thiscall operator= (2 arguments) the cap is 15. Measured counts (tools/inl/inl5.py; inl6.py with C2_LIMIT lifts
   the cap to read the full count): ??4CDefPointeeBase 4, ??4CArray 4, ??4CVectorMap 7, ??4CDefClassBase 38, ??4CParentDefClassBase
   51. On refusal the budget and G2 are restored and the call stays.
   So an implicit operator= that is compiled BEFORE the parent-def assignment can never expand it.
3. Retail's translation unit shows exactly this: CRegionScriptDef::operator= 0x00F29F70, the first user in the unit, CALLS
   0x00431ED0; CCutsceneDef::operator= 0x00F2A000 and this row, later in the unit, EXPAND it. After the first user, ??4PDCB and
   ??4CDefClassBase are compiled (first-use order: 0x00431ED0 itself calls 0x00431F10 for the same reason and expands the
   CVectorMap assignment), so later users get the cost rule: ??4CParentDefClassBase sz=49 c1=44 -> r=1, nested ??4CDefClassBase
   sz=77 c1=72 -> r=1 (trace of b2 / b4). ??4CVectorMap is never compiled on its own in the unit, so it stays a budget candidate:
   w=54, 7 statements, accepted under this row's budget (19358 = 2 x 9679 at entry, 19193 at the site: the two cost-rule
   expansions also pay their size words 57 and 108).
4. Order of emission. An inline virtual that only the vtable needs is emitted user first, then what it uses (Copy, ??4CScriptDef,
   ??4PDCB, ??4CDefClassBase); a non-inline Copy gets its implicit assignments first, bottom-up (??4CDefClassBase, ??4PDCB, ??4CScriptDef,
   Copy), and then ??4PDCB swallows ??4CDefClassBase (105 bytes v retail's 49 at 0x00431ED0) and is refused by the cost rule (c1=100).
   Retail's layout has Copy before operator= for all three classes (0x00F29F60 / 70, 0x00F29FF0 / 0x00F2A000, 0x00F2A240 / 50).
5. Source that reproduces it (b4 = src): CRegionScriptDef's inline Copy + Alloc (the user of its vtable through the implicit
   constructor), then CScriptDef's inline Copy + constructor, with retail's inline base constructors
   (FABLE_RETAIL_DEF_CLASS_BASE_CTORS, FABLE_RETAIL_CVECTORMAP_CTOR). Object order: ??0CRegionScriptDef, Copy, ??4CRegionScriptDef,
   ??4PDCB, ??4CDefClassBase, ??_G, ??0CScriptDef, Copy, ??4CScriptDef, ??_G, ??1, Alloc.
   EVERY function of that object equals its retail counterpart (tools/sib.py b4): ??4CRegionScriptDef 0x00F29F70, Copy 0x00F29F60,
   Alloc 0x00F29FA0, ??0CRegionScriptDef 0x00F29C10, ??4CParentDefClassBase 0x00431ED0, ??4CDefClassBase 0x00431F10, Copy 0x00F2A240,
   ??0CScriptDef 0x00F28F90, ??1CScriptDef 0x00F29530, ??_GCScriptDef 0x00F29510.
   Without the two CTORS opt-ins the base constructors are calls and the object order of the two blocks flips (measured: b1 / b2 /
   b3), which is why b2 matched with the blocks in the other order; b4 is the form kept because it follows retail's address order
   and all its functions equal retail.

## Whole-function compiles (grades.log has the same list)
| # | probe | change | result |
|---|-------|--------|--------|
| 1 | a1 | implicit op= (non-inline Copy), PDCB_IMPLICIT + CDEFCLASSBASE_IMPLICIT + CRESOURCE_ASSIGN + STLP_VECTOR_ASSIGN | 11625v11711, first diff 0x00F2A250 (call 0x00431ED0) |
| 2 | a2 | + inline Copy emitted through the vtable (ctor) | 11625, same |
| 3 | b1 | + CRegionScriptDef inline Copy + Alloc in front | 11625 (object order: CScriptDef group first) |
| 4 | b2 | CRegionScriptDef block after the CScriptDef block | 11711, all 2060 instructions equal |
| 5 | b3 | b2 + DEF_CLASS_BASE_CTORS + CVECTORMAP_CTOR | 11625 (order flipped back) |
| 6 | b4 | b1 order + the two CTORS opt-ins | 11711, all equal; all TU functions equal retail |
| 7 | b5 | b4 with non-inline Copy members | 11625 (??4PDCB 105 bytes) |
| 8 | src | b4 + header comment, q.py | RELOCATION_MATCH |
| 9 | src | comment-only edit (vector count 51 -> 50), gall.sh | RELOCATION_MATCH |
Probe compiles (not grades): p1 x4, p2 x8, p3 x6, p4 x4 (+1 listing), o1 x6, c1 x1 (the row + CCutsceneDef, see Open); c2 traces: c1, a1, a2 (x3: inl3 / inl4 / inl5 + limit
what-if), b1, b2, b3, p3 x2, p4 x2; Transfer row regrade x1.

## Commands
- compile a full source: `python3 tools/cc.py <src> <tag>` (object in tmp/b/<tag>/); `python3 tools/pd.py <tag> [--sum]` masked
  instruction diff; `python3 tools/rel.py <tag> --all` relocation symbol pairing; `python3 tools/bm.py <tag>` relocation-masked byte
  compare (ROW=00f2d6e0 for the Transfer row); `python3 tools/sib.py <tag> <symbol>=<retail va> ...` other functions of the object
  against retail; `bash tools/tr.sh <src> <tag> <filter> [flags]` c2 inline trace (INL_PY=inl5.py adds the statement-cap events,
  inl6.py + C2_LIMIT=n is the cap what-if); `bash tools/pp.sh` / `tools/ord.sh` small-probe listing / object order;
  `python3 tools/c2dis.py <va> <len>`, `tools/c2call.py <va>` c2.dll reading.
- test: `python3 tools/mktest2.py` (regenerates test/00f2a250.cpp), `bash tall.sh`, `python3 tall.py`, `python3 tools/mut2.py`.

## Names (two sources each)
- ??4CScriptDef@@QAEAAV0@ABV0@@Z: Ego_r.pdb + FableWin.pdb name sets; the tail-jump target of ?Copy@CScriptDef@@UAEXPBVCDefClassBase@@@Z
  (vtable 0x012FB5C8 slot 19). Catalog label confirmed.
- Callees (PDB name set entry + relocation pairing of the matched object, tools/rel.py: 12 symbols, 12 targets, 0 inconsistencies):
  0x00431F60 ??4CResource, 0x00431F90 vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >::operator=, 0x0099EFB0
  ??4CCharString@@QAEABV0@ABV0@@Z, 0x00432EE9 / 0x0045EF2D / 0x0046078C / 0x00454886 vector<CCharString / float / CDefString /
  long>::operator=, 0x00E54AA0 / 0x00D34D40 / 0x00F2D010 / 0x00F25980 / 0x00F26C30 vector<CConversation / CFireHeartPatternDef /
  CFeatDef / CArenaRoundDef / CHeroSoulsRoundDef>::operator=. Each vector instance: both PDB name sets hold the
  `??4?$vector@V<T>@@V?$allocator@V<T>@@@std@@@std@@QAEAAV01@ABV01@@Z` symbol, and the member assigned at the call site is the
  PDB-typed std::vector<T> member of CScriptDef (map/events.txt).
- Landed spellings found wrong (NAME_FIXES.tsv): 0x0046078C, 0x00D34D40, 0x00E54AA0, 0x00F25980 are bound to row-local stand-ins.
- Descriptive (made-up) names: none.

## Header overlay (hdr/README.md)
- engine/CScriptDef.h: opt-in FABLE_RETAIL_CSCRIPTDEF_IMPLICIT_ASSIGN (no declared operator=).
- engine/CRegionScriptDef.h: opt-in FABLE_RETAIL_CREGIONSCRIPTDEF_IMPLICIT_ASSIGN (no declared operator=).
- engine/CPersistContext.h, CPersistTraits.h, CPersistContext_TransferableVector.h: chunk 4004's, unchanged.
- Transfer row 0x00F2D6E0 regraded under this overlay: 23755v23755 RELOCATION_MATCH (tools/bm.py), 6409 instructions equal, 965
  symbols pair one to one (compiled into tmp/b/t_transfer, not through q.py, so the shared q/ work directory of the coordinator's
  run on agent4004 is not touched).

## Test (test/00f2a250.cpp, generated by tools/mktest2.py)
See the file's header. Expected table from retail (map/events.txt: offset + kind of copy per event), model members from the header
(943, header order and types); the function under test is the model's implicit operator= reached through Copy. 67 out-of-line
assignments are checked in retail's order with destination and source member (CResource, the sub-def map's vector, 15 strings, 50
vectors); all 943 members + base fields equal afterwards; deep copies; return value; self-assignment (same 67 calls, nothing
changes); Copy through the vtable. tools/mut2.py: 18 single-edit mutants of the model, 18 fail the test (member swaps of four
kinds, a moved member, four type changes, shallow vector copy [aborts on the double free], string copy dropped, Copy from itself,
empty Copy, explicit base assignments that forget SubDefInfoMap / the Template bit / Dirty / the CResource call, a CDefString
assignment that copies nothing). A long member typed float cannot be seen by a copy test (both are dword moves): that part of the
header is covered by the Transfer row's test.

## Untried / open
- CCutsceneDef::operator= 0x00F2A000 (same unit, between the two blocks in retail): probe c1 adds its inline Copy + constructor
  between the blocks. The row still matches (RELOCATION_MATCH), ??0CCutsceneDef and its Copy equal retail, its parent-def chain is
  expanded by the cost rule as in retail, but its CVectorMap assignment is expanded too (215v203: retail calls 0x006313A0). Trace:
  `B w=54 budget=835` -> accepted. Retail's budget at that site must have been under 54 (or the instance was refused another way);
  not pursued, it is the landed row's concern (it uses FORCEINLINE / OUTOFLINE opt-ins).
- The exact rule by which c1 orders two vtable-driven groups when the base constructors are calls (b1 / b2) was not derived.
