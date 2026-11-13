# Hand chunk 2052 (CWorld .. CTCHeroReceiveItems): 2 of 4 byte-exact

## What matched (2)
- CWorld (both RELOCATION_MATCH at pass 1, behaviour tests PASS under g++ and VC7.1):
  - 0x0049F4C0 SaveGameStateInternal (2825 B, 15 grades + 7 header regrades). Persists a HEADER section through one
    CPersistContext over a header memory stream (two-argument `Transfer(name, value)` = `Transfer(name, value, GetDefaultVal<T>())`,
    GetDefaultVal returning a CPersistTraits<T> object's DefaultVal: the CCharString / CWideString defaults are construct / copy /
    destroy, scalars a re-stored 0), then game time + ENTITIES / PLAYER / QUESTS / REGIONS / FACTIONS into a body stream, then builds
    the "FableSav" file (version 0x2165, size slot, both streams zlib-compressed, zero padding to 0x4B000, CRC at the data end).
    Shape levers: `double start_time = StartTime;` before GFGetTime and `total = GFGetTime() - start_time + TimePlayed; TimePlayed =
    total;` (retail spills StartTime before the call and stores the local first); the file-assembly tail in its own block (the CRC
    word lands on the shared temporary slot [esp+18h]; at function scope it gets its own word, in narrower blocks other words).
    The in-game menu pointer of PeekTC is left uninitialised (retail reads a dead frame word on the miss path).
  - 0x004A21F0 LoadGameStateInternal (2931 B, 13 grades + 4 header regrades). Signature gate, StartProgress / PrepareForLoad, binary
    "FableSav" path (CFileDataInputStream with the shared inline ReadULONG x4, 64-bit total check, CArray<unsigned char> resize,
    two ReadZlibCompressed, CMemoryDataInputStream::OpenMemorySource, context.Init) or text path (LoadFromFile, CStringParser::Init
    (CCharString overload, inline), context.Init(parser ...)), then sections through one CPersistContextReadSection. The first draft
    had every instruction right but a 0x16C frame (retail 0x170): the packer trace (agent2009 pk.py, copied) plus sim2 what-ifs
    showed retail's file and parser carry more frame-packer weight; the source shape is an early
    `if (SerialisingHeroState) return true;` before the clock / menu / autosave tail (the extra return copy cross-jumps away but its
    destructor references count).
- Names: all callees in Ego_r.pdb spellings (LABELS.tsv). Descriptive only: the file-static flags g_SavingQuestScreenState 0x013B89D0 /
  g_SavingHeroState 0x013B89D1 (landed spelling of 0x004A05C0, reused: one address, one name) and the row locals.

## Not attempted (2)
- 0x0048E78E CSubtitleRenderer::DrawButtonGraphics (2662 B): not started; the two CWorld rows used the context budget. Real prologue
  (`push ebp ; lea ebp,[esp-68h] ; sub esp,16Ch`, ebp-biased frame), reads CMainGameComponent +0x24 / +0xA4, `ret 10h` (4 args),
  heavy x87 (button rectangles, CRGBColour pushes, vtable +0x5C draws on [this+0x34]).
- 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate (2974 B): not started (one row at a time). Real prologue (`sub esp,74h ; push esi ;
  mov esi,ecx`), `ret 4`.

## Header debt / evidence debts
- ESaveType is nested in CWorld in both PDBs (??$Transfer@W4ESaveType@CWorld@@@CPersistContext@@... = 0x004AB640); the repo CWorld.h
  declares a global `enum ESaveType;`, so both rows emit ??$Transfer@W4ESaveType@@@... (vland RELOC_NAME warning). Nesting it in an
  opt-in hits a VC7.1 bug: CPersistTraits<T>() `: DefaultVal()` cannot value-initialise a nested enum (C2440 / C2065) -- retail's
  CPersistTraits constructor text is probably not `: DefaultVal()` for enums; left for the landing session (NAME_FIXES.tsv).
- vland RELOC_NAME warnings: 0x004046B0 is bound as the STLport `_List_base` dtor by the shared list model (catalog label
  `list::~list`), 0x006B9610 catalog label `CFaction_LoadFromPersist` is wrong (Ego_r ?LoadGameState@CFactionManager@@...).
- Row 1 defines the two-argument Transfer and GetDefaultVal in the row (as the landed TransferBoasts 0x004B05C0); row 2 uses the header's
  FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS for Transfer(name, value) and defines GetDefaultVal in the row (+ a C3DVector(0,0,0)
  specialisation, FITTED: the PDB has ??0?$CPersistTraits@VC3DVector@@@@ but C3DVector's default ctor does not zero).
- FITTED v PROVEN: GetDefaultVal / Transfer forms PROVEN by the construct-copy-destroy temporaries; the named start_time copy, the
  file-assembly block, and the early `return true` are FITTED (frame / scheduling evidence only, each the first natural spelling
  that reproduced retail; alternatives listed in grades.log); FILE_ACCESS_CREATE (4) / FILE_ACCESS_READ (1) PROVEN by the pushes.
