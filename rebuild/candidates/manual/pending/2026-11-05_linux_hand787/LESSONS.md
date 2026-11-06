# Chunk 787 lessons (GOTCHAS one-liners)
- WAIT_UNTIL(!IsRegionLoaded(x)) must be spelled `while (!(!Game->IsRegionLoaded(x)))`: it gives retail's `test al,al; setne bl`. The plain `while (Game->IsRegionLoaded(x))` gives `mov bl,al` (0x00DFA450).
- Quest Mains: the PrisonEscapeRescueMother binding model (AddEntityScriptBinding / PostAdd x2 / AddSpawnedFunction with CCharString("")) plus a new per-quest header at PDB offset -0x14 (and a further -4 after each STLport vector or map) matched 5 Mains/Inits on the first grade (chunk 787).
- Thread and Alloc labels are often wrong (cross-quest bodies such as EndMission and OnLoad). vland RELOC_NAME warnings plus the thread-name literal next to each CSpawnedFunc give the right name in one step.
- reloc_identity skips ??_C string relocations: check the string-literal order against retail by hand (or with a strchk script) before handing back.
- A `CTimer = long` assignment in quest Init is `inline CTimer::operator=(long t){ const long& index = TimerIndex; pDSTGame->SetTimer(index, t); return *this; }` (0x00CF9A50).
