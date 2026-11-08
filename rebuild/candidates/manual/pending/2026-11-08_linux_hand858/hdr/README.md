# Chunk 858 header overlay

Every change is an additive opt-in branch (`#ifdef FABLE_RETAIL_*`); the default path of every header is byte-identical to the repo copy,
so no landed includer can change and no includer recheck was needed (SIBLINGS.tsv is empty for that reason). Each copy was diffed against
the repo header at hand-back time.

| header | opt-in macro | what | evidence | used by |
|---|---|---|---|---|
| engine/CDisplayManager.h | FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND | inline GetBackBuffer() (+0x2C), GetBestCompressedOpaqueTextureFormat 0x009BE830 | FableWin ?GetBackBuffer (inline in release), Ego_r 0x00B23700 run order vs retail 0x009BE830/870/8B0 | 0x00494900 |
| engine/CRenderManager2D.h | FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX | Draw2DTexture(box, colour, texture, flags) 0x009DC6E0 | Ego_r 0x00B302F0 box overload before the UV one (retail 0x009DC870); call shape in Output | 0x00494900 |
| engine/CSurface.h | FABLE_RETAIL_CSURFACE_BITMAP | GetDimensions 0x009F2F90, LoadFromBitmapFileToPreallocatedSurface 0x009F2DD0, inline GetWidth/GetHeight | Ego_r 0x00B46C70 / 0x00B46AA0 = landed retail addresses; FableWin inline getters | 0x00494900 |
| engine/CTexture.h | FABLE_RETAIL_CTEXTURE_NEXTVALIDDIMENSIONS | static __fastcall GetNextValidDimensions 0x009FA620 | Ego_r 0x00B6A3C0 ?GetNextValidDimensions@CTexture@@SI...; retail ecx = result, edx = extents | 0x00494900 |
| engine/CIntelligentPointer.h | FABLE_RETAIL_CINTELLIGENTPOINTER_OUTOFLINE | ctor(T*) 0x004A96F0 and operator= 0x004A9760 declared (out of line) | FableWin ??0 / ??4 of CIntelligentPointer<CThing>; InformOfDeed calls both | 0x00682810 (parked) |
| engine/CTCCreatureOpinionOfHero.h | FABLE_RETAIL_COPINIONTRANSIENTOFFSET_TRAITS, FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREAT | COpinionTransientOffset(offset, traits) 0x00681680; PUnknownThreatOpinionSource typed CIntelligentPointer<CThing> at +0x344 | Ego_r ctor name + retail call; Ego_r member type, InformOfDeed passes +0x344 to IS_THING_ALIVE 0x004CC340 / GetPItem / operator= | 0x00682810 (parked) |
| engine/CQ_GuildTrainingScript.h | FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_INIT | GossipSetup() 0x00D3B410 declared | both PDBs; Init's tail jump | 0x00D3B3D0 |
| engine/CQ_HobbeCaveScript.h | FABLE_RETAIL_CQ_HOBBECAVESCRIPT_ONPERSIST | virtual OnPersist (slot 4) declared | vtable 0x012D5540 slot 4 = 0x00D8DCC0; both PDBs | 0x00D8DCC0 |
| engine/CSystemManager.h | FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER (nested in FABLE_RETAIL_CSYSTEMMANAGER) | PFontManager at +0x84, the unnamed 8-byte composite at +0x7C (swapped; same size 0x180) | Output `mov ebp,[eax+84h]`; landed ~CText 0x0054F6E0 reads +0x84; ctor 0x009A7440 zeroes counted pointers at +0x6C/+0x74/+0x7C/+0x84; Graphic +0x6C / Mesh +0x74 from landed readers. Overlay confirmed in use: without the macro the row reads +0x7C and goes DIFFER (attempt o10) | 0x00494900 |
