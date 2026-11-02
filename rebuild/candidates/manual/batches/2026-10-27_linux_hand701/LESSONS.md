# Chunk 701 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 701) A virtual found in several vtables at the same slot belongs to the class whose own vtable is the base of the
  others: 0x009D58A0 sits at slot 5 of CBankFileAsync 0x0129B8D4, CWADBank and CStaticMapBankFile, while CBankFile's slot 5
  is the tail 0x009CE360 -> CBankFileAsync::Close (UAE). Read the RTTI COL type descriptor (vtable[-1] -> +0xC -> +8) for the
  full template name; rtti-vtables.tsv drops template arguments.
- (chunk 701) boost::scoped_ptr<T>::reset() over a COM interface compiles to `mov eax,[m]; push eax; mov [m],0; call
  operator delete` (the swap with a null temporary, then checked_delete's bare `delete` -- no destructor, no null test):
  CManagedTextureData::ReleaseTexture 0x00A69A80 (FableWin names scoped_ptr<IDirect3DVolumeTexture9> reset/swap).
- (chunk 701) A user destructor whose body is one member store (`WorldSeed = 0;` in ~CHeightMap 0x008F6170) or one member call
  (`CloseShaders();` in ~CShaderDataBank 0x00A620D0) is recognisable by the vtable store before it; an implicit member's
  own vtable store vanishes while its inline base dtor's store stays (StateBlock: only CBankStateBlock 0x0129C82C).
- (chunk 701) Byte-identical action dtors name differently by caller: 0x00718D50 (??_G 0x0084B600 + PerformExpression ctor
  temporaries) and 0x0084ADC0 (PerformExpressionExtended ctor temporaries) are two sibling classes sharing their first two
  members; the third "twin" 0x009822E0 differs in one offset and is ~CCreatureAction_SendMessage (uncalled; its ??_G inlines it).
- (chunk 701) An uncalled setter is located by the inline copy in its caller: CTCSummonSpell::CreateMinion 0x0079ACC0 expands
  CTCSummonableCreature::SetSummoner (0x0079B095..0x0079B0CA) right after the HasBeenSummoned store.
- (chunk 701) vland's SHARED_HEADER_AVAILABLE check is a plain substring test for `engine/<Class>.h`; a flattened engine
  header (vptr as a data member: CCreatureActionBase.h, CBankFile.h, CObserver.h) cannot be a polymorphic base, so the local
  model stays and the source states why (header debt).

Review at landing:
- No row needed more than 3 probes; no flags, no pragmas, no retail_asm, no volatile, no #pragma pack added (0x009D58A0's copy
  of the CBankFile member list dropped the header's pack(1): the layout is naturally aligned).
- Four rows (0x00F25E40, 0x00718D50, 0x0084ADC0, 0x009822E0) carry the ungraded DestroyOne emission helper for
  out-of-line implicit `??1` (accepted chunk 656/657/696-700 pattern).
- Local base models next to existing engine headers (header debt, NOTES.md): CBankFile (hold), CCreatureActionBase (flattened),
  CResource (no SetAsLRU), CShaderDataBank / CManagedTextureData / CCreatureAction_PerformExpression(Extended) (flattened
  generated layouts), NUISystem CObserver (flattened, global namespace).
- Already-landed sources with wrong names (NAME_FIXES.tsv): 0x00A62860 ??_GCShaderDataBank binds 0x00A62050 as
  DeleteShaderEntries (QAE) -- it is the private CloseShaders (AAE) and uses a `__forceinline virtual ~CShaderDataBank` model;
  0x00A62110 CShaderDataBank::Close is landed as a free `__fastcall Close(CShaderDataBank*)` calling stand-ins h1/h2 (should be
  `CloseShaders(); CBankFile::Close();`); 0x00502170 binds 0x009D58A0 as QAE (virtual: UAE); 0x00A26C50 binds it as `h2`;
  0x00A69AD0 binds 0x00A69A80 as CFrameData::Close; 0x00F25E10 / 0x00F25E20 / 0x00F25F00 are landed as gapscan / 
  CLandscapeBackgroundPatch stand-ins (they are CArenaCreatureDef's GetSizeofClass / ??_G / Copy); 0x00F263D0 is landed under
  a CFireheartMinigameDef00f263d0 file name (it is ??0CArenaCreatureDef@@QAE@XZ); ghidra_out/coverage.tsv labels 0x009FC450
  SetAsMRU (it is SetAsLRU).
