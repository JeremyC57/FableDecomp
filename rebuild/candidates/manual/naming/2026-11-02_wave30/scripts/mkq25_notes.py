# mkq25_notes.py (exec'd by mkq25.py): per-target outcomes / notes for wave 25.
I8 = '0042db40 00523540 0055d070 00770e50 00778ae0 0077b250 0077b9b0 0077be40'
EDX = {
 '00ee8c10': ['00ee8c10'], '00ee8100': ['00ee8100'], '00c09920': ['00c09920'], '00aa7540': ['00aa7540'], '00a681f0': ['00a681f0'],
 '00a5c600': _rows('smp'), '00ac1ee0': _rows('ibuf'), '00a63200': ['00bc5420', '00b6db80'],
 '009e3830': _rows('pixfmt'), '00686a80': ['006afd40', '0076c810', '0077db90', '006af630', '007e00d0', '00681980'],
 I8: _rows('bcnc8'),
}
LEFT = {
 '00686a80': ['00744580', '007345d0'],
 '0139c8fc': ['00c0c0c0', '00c0dab0', '00c0b910', '00c0b9f0', '00c0ba40'],
 '00c2d9c0': ['00c16e10', '00c30e90', '00c3bb50', '00c3bcb0'],
 I8: [],
}
N = {
 '00c28690': ('deferred', 'brief: 0x00C28690 not in this wave (libpk CHandleBasedMemoryPool::Free second copy; owner decision, carried)'),
 '00ad3a10': ('hold', 'HOLD (brief: not in this wave): two retail copies of set<unsigned long>\'s copy ctor'),
 '013ca828': ('deferred', 'GDefStringTable leftovers 0x005DCF70 / 0x008AF850 / 0x00890F40 (per-row re-author with the shared header; not reached)'),
 '007acc50': ('deferred', 'scope 2 not reached: the per-row STLport map<CCharString,bool> re-models of 0x007AD180 / 0x007ADBF0 / 0x008A78E0 / 0x008A7930; '
              '0x007ADB50 / 0x007ADD50 stay HOLD (brief)'),
 '00440990': ('deferred', 'scope 2 not reached: the 23 vector<long>::_M_insert_overflow callers (each row models its own vector class -- CLongVector, CArray<long>, '
              'CRetailVector, InsertGrow views -- whose push_back inlines call 0x00440990; per-row re-model onto STLport vector<long, allocator<long> > '
              'with the protected IAE member, size_type I)'),
 '00613ae0': ('hold', 'HOLD (brief): two retail copies of one ~map destructor call it (0x00616970 / 0x00617180)'),
 '006196d0': ('hold', 'HOLD (brief): two retail copies of one ~map destructor call it (0x00619D00 / 0x00619E70)'),
 '0076c000': ('deferred', 'scope 2 not reached: the c690 sort-row leftovers (own row on the descriptive SElement16 / SCompare sort, callers 0x0076C360 / 0x00AF59A0)'),
 '0076c1a0': ('deferred', 'scope 2 not reached: as 0x0076C000 (callers 0x0076C6B0 / 0x00AF59A0)'),
 '009cec60': ('deferred', 'brief: the CBankFile rows wait for the engine-header migration'),
 '009cbd30': ('deferred', 'brief: the CBankFile rows wait for the engine-header migration'),
 '009cbd50': ('deferred', 'brief: the CBankFile rows wait for the engine-header migration'),
 '0099b510': ('deferred', 'carried: the ~CCharString-labelled ~CWideString binders (per-row CWideString models; destructor families of scope 3 not reached)'),
 '0099a300': ('respelled-partial', 'family bcnc8 (DECISIONS_W24_RESOLVED item 8): the eight implicit-dtor rows declare the PDB protected virtual destructor on '
              'their local CBaseClassNonCopyable (MAE, out of line at 0x0099A300) -- each derived vtable / inline dtor calls 0x0099A300 itself; family smp: '
              '0x00987F10 is ??1CSimpleMemoryPool@@UAE@XZ and 0x0122AB10 / AB30 / AB50 the pools\' atexit destructors (the junk Second_0099a300 / base_dtor_99A300 '
              'spellings retired). Left: the destructor-family stand-ins of scope 3 (not reached) and 0x00BC9C30 / 0x009CDF50 (fix lane)'),
 '00412330': ('deferred', 'scope 2 not reached: the CWideStringVector / VectorPushBackOverlay / template-tag stand-ins 0x004091C0 0x004128A0 0x005FDAD0 '
              '0x0071CE80 0x009AEF60 0x009AF480 (per-row re-model of the vector class; the own Leafs take the PDB vector<CWideString> parameter types)'),
 '0099b800': ('deferred', 'scope 2 not reached: 0x009994A0 / 0x00999620 define the CWideString bodies in-TU on AFileSupport / ExternalRetry namespace models '
              '(per-row re-model onto the global CWideString with ??4CWideString@@QAEABV0@PBG@Z)'),
 '00ee8c30': ('done', 'own row already defines it (chunk 692); its only caller is the ??_G 0x00EE8C10, respelled this wave'),
 '00ee8c10': ('respelled', 'family oak: re-authored on the compiler-generated ??_GCCS_OakValeRevisitedScript@NScript@@UAEPAXI@Z (the wave 16 ??_G model, '
              '0x00E68E90): was the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor'),
 '00ee8100': ('respelled', 'family oak: the ctor in namespace NScript with the PDB parameter pair (CScriptDataBase*, CGameScriptInterfaceBase&) (was a global '
              'struct taking (void*, CGameScriptInterface*); one pushed pointer each, bytes unchanged)'),
 '00ea4980': ('done', 'own row defines it; no landed caller: the only call site 0x00EA3A08 lies in an unlanded function (CLostTraderTrader::Main)'),
 '00ea4bb0': ('done', 'own row defines it; no landed caller: the only call site 0x00EA3C55 lies in an unlanded function (CLostTraderTrader::Main)'),
 '00a5c600': ('respelled', 'family smp: 0x00B3B8E0 declares it private (AAE); 0x00987F10 re-authored as ??1CSimpleMemoryPool@@UAE@XZ (FableWin.pdb; vtable '
              '0x0129A064, the ??_G 0x00987F30 follows); 0x0122AB10 / AB30 / AB50 re-authored as the `dynamic atexit destructor\' ($E2) of MatrixBufferPool / '
              'VectorBufferPool / GroupVectorBufferPool (Ego_r / FableWin), the initialiser rows\' own TU source'),
 '00ac1ee0': ('respelled', 'family ibuf: 0x00AC2070 (Release -> Uninitialise), 0x00BC5420 (CIndexBuffer_Retail / CVertexBufferWin32_Retail / CMovableResource_Retail '
              '-> the PDB classes; the row\'s own class CEnginePrimitiveManagerParticleLineGroup_Retail -> the PDB class), 0x00B6DB80 (CIndexBufferNative / '
              'CVertexBufferNative stand-ins; the row\'s CWaterStaticMap -> CEngineWaterRenderer; Leaf line added), 0x00B6D480 (CIndexBuffer_00b6d480)'),
 '00a63200': ('respelled', 'family ibuf: 0x00BC5420 (CVertexBufferWin32_Retail) and 0x00B6DB80 (CVertexBufferNative) now bind it'),
 '009e3830': ('respelled-partial', 'family pixfmt: 39 rows declare void Initialise(_D3DFORMAT) on CPixelFormat (was long / unsigned long / int / a D3DFORMAT '
              'enum / an out-of-line ??0CPixelFormat@@QAE@J@Z / @K@Z constructor) with the inline format constructor; the stand-ins CPixelFormatFromD3D / '
              'CPixelFormatNative / CPixelFormat_Methods / PixelFormat / PixelFormatMethods / CGlowPixelFormat become CPixelFormat (0x009FCEF0\'s '
              'Initialise(long,long,long,long,bool) 0x009E3790 with it; 0x009F3FF0 / 0x009F4120 / 0x009F4190 bind ConvertRGBToPFDData 0x009E3F60); Leaf lines '
              'added to 19 rows that had none. Left: 0x009F31C0 (its CPixelFormatFromD3D also carries a WriteColour taking CRGBColour by value, '
              'no PDB spelling -- the row stays on HEAD) and 0x009F9EE0 (its CPixelFormatByteLengthView lives in rebuild/include/fable_texture_lifecycle.h, a shared '
              'header)'),
 '00a681f0': ('respelled', 'family pixfmt: own row ?GetLevelByteLength@CVolumeTexture@@QBEJJ@Z (was GetByteLength(unsigned long)); no landed caller'),
 '005136b0': ('hold', 'HOLD (two-retail-copies rule): the own row (chunk 692) defines ??0?$vector@EV?$allocator@E@std@@@std@@QAE@I@Z, which reloc_bindings already '
              'binds to 0x00514E20 -- a byte-identical second retail copy (77 bytes; 14 landed callers) -- DECISIONS_W25'),
 '009fb110': ('done', 'no landed row: label only (labels_chunk692)'),
 '009fb3f0': ('done', 'no landed row: label only (labels_chunk692)'),
 '00a68ae0': ('done', 'no landed row: label only (labels_chunk692)'),
 '00c011a0': ('done', 'own row defines _deflate_stored (chunk 692); no other binder'),
 '00c08d40': ('done', 'no landed row: label only (labels_chunk692); its callee 0x00C09920 respelled this wave'),
 '00c09920': ('respelled', 'family pkc: own row ?GetChecked@?$PKCAutoDelete@VCUnprovenPKObject_00c099f0@@@@QAEPAVCUnprovenPKObject_00c099f0@@XZ (the class '
              'template the Release row 0x00C099F0 instantiates; was CAutoDeletePointer::GetChecked)'),
 '00aa7540': ('respelled', 'family cfl: own row ??1CChunkedFileLoader@@UAE@XZ (Ego_r CChunkedFileLoader; was ??1T@@QAE@XZ on a generic struct T)'),
 '0139c8fc': ('hold', 'HOLD: the bytes make 0x0139C8FC a pointer (to 0x0139C8EC = {-1,0,0,0}; 0x0139C900 is the char pointer to 0x0139C8F8), so the MFC '
              'spelling of DECISIONS_W24_RESOLVED item 3 is ?_afxDataNil@@3PAUCStringData@@A -- but retail\'s own assert text 0x00EB1FB3 reads '
              '"stringSrc.GetData() != _NullString" (the libpk source renamed it); decision needed (DECISIONS_W25 D1) before respelling either side'),
 '00686a80': ('respelled-partial', 'family peek (DECISIONS_W24_RESOLVED item 7): 0x006AFD40 CTCBase::PeekThingSearchTools, 0x0076C810 CTCEnemy::SetFaction, '
              '0x0077DB90 CTCHeroCentre::OnHeroWalkedThroughDoor, 0x006AF630 (owner unknown), 0x007E00D0 CTCTeleporter::FrameUpdate re-authored over the '
              'empty-base CTCBase::PeekWorld; family coh: 0x00681980 is CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining (with 0x00681940 '
              'GetToleranceToBeingHit and 0x006819D0 OnHit). Left: 0x00744580 (identity of the function unproven: it returns the def\'s Day / '
              'NightTimeVisibilityMultiplier, not GetExtendedSightRadius) and 0x007345D0 (its GetPRandomSeed is the GetWorldSeed owner item)'),
 '00c2d9c0': ('hold', 'left (brief: two sources or leave it): libpk has no PDB / RTTI / assert text naming the owner; the own row and 0x00C16C10 / 0x00C30E90 / '
              '0x00C16E10 already say CSegmentFile::OpenSegment (descriptive), 0x00C3BB50 / 0x00C3BCB0 say Reader::Find / CriteriaReader::FindSegment '
              '(descriptive) -- DECISIONS_W25 D2'),
 I8: ('respelled', 'family bcnc8 (DECISIONS_W24_RESOLVED item 8): per row the local CBaseClassNonCopyable declares the PDB protected virtual destructor (MAE, '
      'out of line at 0x0099A300), retiring the TU-generated public ??1CBaseClassNonCopyable@@UAE@XZ; the derived classes keep their own dtor as declared '
      '(CXMVPlayerBuffers, CThingManager, CDiskFileWin32 explicit; CTCBase implicit); all eight RELOCATION_MATCH + PASS'),
}
CLEAR = {}
