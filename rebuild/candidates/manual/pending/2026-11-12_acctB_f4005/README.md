# acctB hand-back, longest-row lane 4005 (1 row matched: 11,711 bytes)

Authored by a Fable 5.2 agent (the owner's "longest reserved rows" lane) from the headers and notes of `f4004`,
re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00F2A250 `??4CScriptDef@@QAEAAV0@ABV0@@Z` (CScriptDef::operator=) | RELOCATION_MATCH 11711v11711, test PASS (g++ and VC7.1) | no flags, no pragma, no forced inline; 9 whole-function grades in one pass |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no warnings). Purity and padded-struct greps are clean. **The f4004 Transfer row 0x00F2D6E0
regrades RELOCATION_MATCH through q.py under this overlay** (the three persist headers here are byte-identical copies of
f4004's; this folder adds two more), so `hdr/` of this folder serves both rows.

## What the row is

The function is the compiler-written (implicit) operator=: the row source holds no statement of it. The agent's member
map lines 943 retail copy events up with the 943 members of `engine/CScriptDef.h` in declaration order, each with the
kind its header type gives (548 long, 285 float and 45 CDefString as dword moves, 15 CCharString through 0x0099EFB0, 50
vector operator= calls) -- no header findings. The only residue was the base-chain head: retail EXPANDS the
CParentDefClassBase and CDefClassBase assignments where a plain build calls 0x00431ED0.

## Review items for landing -- PROVEN v FITTED

1. **Proven:** implicit operator= (both PDB name sets, the Copy tail jump, the member map); Copy is an inline virtual
   (retail lays Copy out before operator= for all three def classes of the unit; a non-inline Copy gives 11,625 bytes);
   the "first user must call" mechanism (retail's CRegionScriptDef::operator= 0x00F29F70 does `call 0x00431ED0` at
   0x00F29F79; measured c2 gate: a not-yet-compiled callee is refused when its statement count exceeds
   (arguments + 2) * 3 + 3, c2.dll 0x107284D5).
2. **FITTED -- the row file is a four-function stand-in for retail's much larger unit:** CRegionScriptDef's inline Copy
   and Alloc, then CScriptDef's inline Copy and `CScriptDef::CScriptDef() {}`. As a free model check the agent graded the
   ten other functions its object emits against retail and all are equal (0x00F29F60, 0x00F29F70, 0x00F29FA0,
   0x00F29C10, 0x00431ED0, 0x00431F10, 0x00F2A240, 0x00F28F90, 0x00F29530, 0x00F29510) -- so here, unlike f4004, the
   constructor in the file IS retail's 0x00F28F90 body. Adding CCutsceneDef's Copy and constructor between the blocks
   leaves the row matching.
3. **FITTED -- inline base-constructor opt-ins** (`FABLE_RETAIL_DEF_CLASS_BASE_CTORS`, `FABLE_RETAIL_CVECTORMAP_CTOR`,
   both existing) are needed for the source order to work; without them the two vtable-driven groups come out reversed.
   c1's ordering rule for that was not derived.
4. **Headers** (`hdr/README.md`; additions only, default text unchanged): `engine/CScriptDef.h`
   `FABLE_RETAIL_CSCRIPTDEF_IMPLICIT_ASSIGN` and `engine/CRegionScriptDef.h`
   `FABLE_RETAIL_CREGIONSCRIPTDEF_IMPLICIT_ASSIGN` (each drops the declared operator= so the compiler writes it).
5. **Name fixes** (`NAME_FIXES.tsv`): four landed stand-in spellings for STLport `vector<T>::operator=` instances
   (0x0046078C, 0x00D34D40, 0x00E54AA0, 0x00F25980). `LABELS.tsv`: the row, its 12 callees and 0x00F29F70.
6. **For the catalog:** the landed `FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN` rows (0x004321F0, 0x00F2A000 and others) are
   candidates for redoing without the forced inline, by unit order. Left open by the agent: CCutsceneDef::operator=
   0x00F2A000 built in this unit expands the CVectorMap copy (215v203) where retail calls 0x006313A0 -- not explained;
   that landed row keeps its opt-ins.

## Behaviour test

`rows/00f2a250.test.cpp` is generated (agent-side `tools/mktest2.py`): the model's 943 members come from the header,
the expected table (offset and copy kind per event) from the retail disassembly. It checks member kinds and order, the
67 out-of-line assignments in retail order with the right destination and source, equality of all members and base
fields afterwards, deep copies of strings and vectors, the return value, self-assignment and Copy through the vtable.
18 of 18 single-edit mutants fail (one by aborting on a double free). A copy test cannot tell a long member from a
float one; the Transfer test in f4004 covers that.

## How to land

`rows/00f2a250.cpp` + `rows/00f2a250.test.cpp` with `hdr/engine/` (the five files; they also serve f4004's row), then
`payload.json` through your usual vland.
