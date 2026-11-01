# Chunk 666 -- 10 of 10 landed (2026-11-01, NA011 of the never-attempted pool, authorable band)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA011). Landed via `coordinator/land_chunk.sh 666` (vland --land 10/10, 1 MATCH +
9 RELOCATION_MATCH, PASS, multi-check 0): 0x00E54D80 ~CConversation, 0x00534EB0 std::__find<CCountedPointer<CComponent>*>, 0x006A6880
CThingPhysical::WorldUpdate, 0x00B2B910 ~CCompositeSurfaceCache, 0x009FE6C0 CGraphicInfoFrame::CFrameData::~CFrameData, 0x009E3870
CPixelFormat::GetInit, 0x004615FB CArmourDef::Transfer, 0x004DF562 CWillResponseDef::Transfer, 0x0071C260 CTCHeroMorph::KillParticles,
0x007FC730 CTCKraken::SetMode. No row parked. Same-file bodies also byte-exact (land in a later pass with their own tests): 0x007FC6F0,
0x007FC710, 0x009FE440, 0x0071C1F0.
25 NAME_FIXES queued (c666, incl. four landed rows to re-author off stand-in classes: 0x009FE380, 0x009E3790, 0x009E3970, 0x009E3820;
~CCharString's 102 descriptive spellings are a respell family of their own). Labels: ghidra_out/labels_chunk666_2026-10-27.tsv (27).
Decisions at landing (owner delegation 2026-11-01): `__forceinline ~CGraphicFrame` accepted (header-helper rule); 0x0129BA40 keeps
`g_D3DFormatInfo` as descriptive until a data-symbol dump. Descriptive: the one-argument TransferMap<V,K> spelling for 0x00461660 /
0x00461894 (PDBs hold only the named-argument forms), enumerator names of EKrakenMode / EPixelFormatType. Lessons in GOTCHAS (1 line) and LESSONS.md.
