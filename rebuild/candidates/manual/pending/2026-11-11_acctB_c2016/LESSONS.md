# Chunk 2016 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x004CAA00) CThing::ConstructFromLevelScript is chunk 2008's ConstructFromBinarySave recipe plus the level-script parser shape of the
  landed CThingTrackNode / CThingAICreature overrides: visible CBasicString<char> Compare / operator==(const char*) bodies make VC7.1 hoist
  the key's null test out of an if / else-if keyword chain (one `repe cmpsb` ladder against "", one out-of-line operator== ladder).
- (0x004CAA00) A vector-map walk that keeps the next element in another register (`lea edi,[eax+8]` ... `cmp edi,[ebp+48h] ; mov eax,edi`)
  is `for (it = begin(); it != end(); it = next) { tc = it->second; next = it; ++next; ... }`; `++it` (or `tc = it->second; ++it;`)
  keeps one register.
- (0x004CAA00) Retail clears CThing's RenderListNext before RenderListPrev and UpdateListNext before UpdateListPrev (0x004CAA74..0x004CAA83),
  as the landed ConstructFromParams 0x004CA010 does.
- (0x0061AD60) Elements of a freshly filled std::vector<CRGBColour> written with byte stores straight into the buffer
  (0x0061ADC9..0x0061ADF0) are `colours[i].Initialise(r, g, b, a)`; `colours[i] = CRGBColour(...)` builds a stack temporary and copies a dword.
- (0x0061AD60) A string / vector temporary built BEFORE the stack pushes of a call (0x0061AFC2, 0x0061B195) is a named local; a temporary
  argument is built after the pushes to its right.
- (0x0061AD60) A bool flag stored before a call in the same arm (`mov byte ptr [esp+17h],1` ahead of the operator= call, 0x0061B065) was
  assigned before that call in the source: stores are not moved across calls; written after the call, VC7.1 keeps the flag in a register.
- (0x0061AD60) The same-TU callee lesson again, measured: a declaration-only GetTestQuestsToDisplay(std::vector<CTestQuest>&) left the
  local vector escaped (reload of _M_start for the final destroy / free, and the colour buffer's start in ecx instead of eax, 1633v1631);
  the landed callee's real body visible and not expanded (`#pragma auto_inline(off)`) gives RELOCATION_MATCH. Prove it first with a
  trivial noinline body (diagnostic probe), then write the real body.
- (0x0061AD60) STLport range destructors called by ~vector in a /Gr TU are `__fastcall` (`mov ecx,first ; mov edx,last ; push &tag`);
  hdr opt-in FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL. Overlaying a stlport header needs the row to include it first: repo headers
  include `"stlport/fable_containers.h"` relative to rebuild/include, which beats the -I overlay.
- (0x0061AD60) CInventoryDef: natural layout with byte-packed C2DVector / C3DVector / CEngineGraphic and 12-byte STLport vectors reproduces
  all 391 Ego_r.pdb member offsets at retail = PDB - 4 - 4 per earlier vector (sizeof 0xA34).
- (0x0061AD60) Header constants NInventory::WHITE / GREY / BLACK (FableWin dynamic initializers in 148 TUs) are statically initialised in
  retail and still read from memory (`mov edx,[0137D700h]`): namespace-scope `const CRGBColour X(r, g, b, a);` reproduces both.
- (0x007D6870) CTCTavernGameCardBase is 0x318 in retail: its members end at +0x314 and the class is 8-aligned (CTCTavernGame::LastRenderTime
  is a double) -- the "missing word" before a derived class's first member is tail padding, not a TLC member.
- (0x007D6870) x87 operand order inside an inlined CRightHandedSet::GetRight / C3DVector::Dot in a large function is not reproduced by a
  reduced fragment of the same block: probe in the full row, or trace c2.

Review at landing:
- 0x007D6870 is parked after 14 full-row compiles (pass 1 went 2 over its 10-grade budget through scripted variant compiles, counted
  honestly) and 2 pass-2 grades; pass 3 was not used (working budget), so it is not exhausted. differ/007d6870.cpp has no test yet.
- NAME_FIXES.tsv respells landed callers: 0x004CA230 (CreateTCsFromString -> CreateSerialiseCreatableTCs, protected), 0x005BCAFE /
  0x005BCE49 (free / stand-in spellings -> NInventory::CTCInventoryBase members), 0x009DDA50 (Draw2DBoxColours array parameter, QBV).
- 0x0061AD60's TU carries a full body of the landed GetTestQuestsToDisplay 0x0061A8A0 (under auto_inline(off)); that landed row is still
  a local stand-in model with a wrong mangling (`CTestQuestVector*`) -- re-author it from this body.
- The overlay CThing.h / CThingManager.h started from chunk 2008's overlay (not HEAD); merge with that hand-back's opt-ins.
