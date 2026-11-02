# mkq29_notes.py (exec'd by mkq29.py): per queue row (1-based index into QUEUE_W29_input.tsv, header excluded) outcome / note for
# wave 29; rows not listed are carried (not reached).
_d = lambda *f: sorted(set(x for x in sum([_rows(x) for x in f], [])))
EDX_I = {
 27: _d('sfbr'), 28: _d('sfbh'), 49: ['00b40000', '00b428e0'], 50: ['00b42c70'], 63: ['00a3abb0'], 64: ['00a3abb0'], 65: ['00a3a930'],
}
LEFT_I = {27: [], 28: [], 49: [], 50: [], 63: [], 64: [], 65: [], 41: ['0072da40'], 46: ['00a399e0']}
N_I = {
 27: ('respelled', 'family sfbr: the 14 sources that modelled the base as the invented CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime '
      'pair (virtual ~CSpawnedFuncBaseRuntime, in-TU RuntimeHookCDD410 / CDD420) derive their thread class from the retail CSpawnedFuncBase of '
      'engine/CSpawnedFuncBase.h (ctor ??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z 0x00CDD450, implicit dtor 0x00CDD4C0); 14/14 RELOCATION_MATCH + PASS'),
 28: ('respelled', 'family sfbh: the retail branch of engine/CSpawnedFuncBase.h has the implicit destructor and the Ego_r.pdb member types '
      '(CCharString Name / AssociatedRegionName, CCountedPointer<const NScript::CPredicateBase> PPredicate) under the CEntityScriptBinding.h '
      'contract (the includer defines CCharString and CCountedPointer<T> first); 346 includers meet it (engine/CCountedPointer.h 221, '
      'fable_string.h 121, a declaration-only CCountedPointer 3, a moved local template 1); all 512 landed includers rechecked'),
 49: ('respelled', 'family ewm: own row ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ (was the catalog CloseStaticMap); its binder 0x00B428E0 drops '
      'the invented CEngineWorldMapRetail (own ?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z, private OpenStaticMaps / '
      'LoadWaterData in their PDB spellings)'),
 50: ('respelled', 'family ewm: own row is the private overload ?DeleteMap@CEngineWorldMap@@AAEXXZ (was the catalog DeleteAllMaps)'),
 63: ('respelled', 'family zlib: own row ?FinishDecompressStream@CCompressorZlib@@QAEXXZ (Leaf added; was the invented CCompressorZlib_Retail)'),
 64: ('respelled', 'family zlib: 0x00A3ABB0 calls _z_inflateEnd (was _inflateEnd)'),
 65: ('respelled', 'family zlib: 0x00A3A930 calls _z_deflateEnd (was _EndDeflateStream; its test stub renamed with it); own Leaf '
      '?FinishCompressStream@CCompressorZlib@@QAEXXZ added'),
 41: ('deferred', 'wave 29 finding: the ??_G re-author (MakeOne idiom) reproduces the 30 bytes, but emitting the vtable needs a construction: '
      'retail\'s ctor is ??0CTCSpecialAbilities 0x0072C6E0 (out of line, CTCBase(CThing*) base), so an implicit default ctor cannot be used '
      'as at 0x00ED7DB0 (CTB_Bard\'s base has a default ctor). DECISIONS_W29 D3'),
 46: ('deferred', 'wave 29 finding: as row 41 -- CCRCSymbolMap\'s ctor 0x00A38500 is out of line; the emission site needs a decision (DECISIONS_W29 D3)'),
}
