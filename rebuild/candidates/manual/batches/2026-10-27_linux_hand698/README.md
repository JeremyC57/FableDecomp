# Chunk 698 (NA041) -- 10 of 10 byte-exact destructors; the nine "~CPatchTesselationEdgeStrip" rows were nine classes (2026-11-01)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; every row matched on its first probe): the implicit
~CTCSpecialAbilities 0x0072DA60 and ~CTCTavernGameCardBlackjack 0x007DCA90, the implicit ~CCRCSymbolMap 0x004D1D50 (its global's atexit dtor
0x01228DD0), ~C3DAnimationStats 0x00A4EB50, ~CEngineWorldMap 0x00B42E70 (`{ CloseStaticMapFile(); DeleteMap(); }`), DirectShow ~CDynamicOutputPin
0x00CA9690 (amfilter.cpp verbatim, /EHsc), ~CNavigationAreaInit 0x0051D590 and ~CCombatDebugInfo 0x006F2FD0 (no callers: named from Ego_r order and
FableWin's ctor / dtor pair), ~C3DMeshFileXUVTangentChunk 0x00AA7320 (no callers: its own ??_G 0x00AA7370 holds the same body inline), and
~CCompressorZlib 0x00A3ACC0 (the one right label). Six implicit-dtor rows carry the one-line ungraded DestroyOne helper (accepted, chunks
656 / 657 / 696 / 697). Decisions (coordinator): 0x0051D590's out-of-line `~CNavigationAreaUpdate()` declaration is accepted -- it mangles to the same
??1CNavigationAreaUpdate@@QAE@XZ as retail's compiler-generated dtor and gives the same call; vland's SHARED_HEADER_AVAILABLE warnings (CEngineComponent
on 0x00B42E70: the header flattens the vptr; CBaseObject on 0x00CA9690: the DirectShow false positive) and the two RELOC_NAME warnings on 0x00B42C70 /
0x00B43BB0 (labels fixed here) are accepted.

26 labels applied (`ghidra_out/labels_chunk698_2026-10-27.tsv`); NAME_FIXES.tsv 26 rows queued c698 for naming wave 29: vector<vector<CCard*>> dtor
0x007DC890 (landed as CRetailVector_SBufferEntry12), CEngineWorldMap::CloseStaticMapFile 0x00B40000 / private DeleteMap 0x00B42C70 / the STLport
_List_base<CEngineMap*> dtor 0x00B43BB0 (landed as a free SortTreeRecursively stand-in), ~CNavigationAreaUpdate 0x0051D530, CCombatDebugInfo's copy ctor
0x006F3000, C3DMeshFileXUVTangentChunk's ??_G 0x00AA7370 and the virtual ~CChunkedFileChunk 0x00AA75C0, CCompressorZlib::FinishDecompressStream 0x00A3ABB0,
the zlib binders of z_inflateEnd 0x00C038D0 / z_deflateEnd 0x00C00AE0 (both PDBs spell _z_*; Z_PREFIX build), the ??_G stand-ins 0x0072DA40 / 0x00A399E0,
~CTCTavernGameCardBase 0x007D5E80, ~CBankFileAsync 0x009D6490 (CBankFile family: on HOLD), ~CBasePin 0x00CA7C80. Header debt (NOTES.md): CTCBase.h,
CEngineComponent.h, CCompressorZlib.h, CEngineWorldMap.h flatten the vptr (CEngineWorldMap.h also carries Ego_r members past +0x1B4 this build lacks);
no Blackjack header (retail base 0x318, Ego_r's is the Dinkumware size).
