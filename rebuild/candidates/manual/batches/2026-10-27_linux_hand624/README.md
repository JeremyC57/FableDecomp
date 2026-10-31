# Chunk 624 -- 6 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 624` (vland --land 6/6, multi-check 0): four STLport
std::__uninitialized_fill_n instantiations 0x00722BD0 (CTCSmokeGenerator::CGenerationPos), 0x00792550 (CTCDivineWrath::PerEffectInfo),
0x007979B0 (CTCUnholyPower::PerEffectInfo), 0x00806C80 (NPlayerTargeting::CTargetPoint) -- the ebx/edi tie that earlier waves
called a lane gap closes when the intelligent pointer's __forceinline copy ctor names its item (`CBaseObject* item = other.GetPItem();
SetPItem(item);`); CTCHeroPodium::GetCrowdPos 0x00771D50 (MATCH; the 16-byte frame is the inlined PDB helper
GetBestVacantPositionForThisPos returning std::pair<C3DVector,long> by value, C3DVector with its user copy ctor); and
CGraphicDataBank::FrameEnd 0x009FD660 (by-value `Max(ResourceList.GetFreeMemory(), 0L)`, const GetUnloadableMemory; shared
headers CResourceList.h / CTextureManager.h / CCountedPointer.h). At landing the stale second binding of 0x009FD660 from caller
0x009F8200 (struct CGraphicDataBank_009f8200) was respelled and the caller rechecked RELOCATION_MATCH + PASS.
Four rows stay DIFFER in `pending/2026-10-27_linux_hand624/differ/`: 006817f0 (switch bias `sub eax,2` vs `add eax,-2`; the only
two sub-biased switches in the exe are in this TU; flag sweep + QFE tried), 00bba3e0 (x87 issue order), 004310a7 / 00431143
(string slots swapped; nameless Transfer overloads have no PDB symbol).
Two extra matches from older rows in `pending/2026-10-27_linux_hand624/extra/`: 007af940 (chunk 622's fill_n row, matched with
the named-item change; needs a test) and 009fd3a0 (retry1 UpdateAvailableMemory twin, matched with the Max clamp; needs a test and
the PDB private access) -- handed to chunk 626's agent to finish. 3 NAME_FIXES (GetUnloadableMemory const 0x009FC070 and two
label notes) + 3 review rows queued (prefix c624). Labels: ghidra_out/labels_chunk624_2026-10-27.tsv.
