# mkq24_notes.py (exec'd by mkq24.py): per-target outcomes / notes for wave 24.
F6 = '0x006156E0 0x006D3FB0 0x00777000 0x007EE660 0x009223A0 0x0093F930'
EDX = {
 '009d4ce0': ['009d4ce0'], '009b08c0': ['009b08c0', '004336bc'], '009acb10': ['009acb10', '00416005'], '00cbf647': ['00cbf647'],
 '009a76a0': ['009a76a0', '004336bc', '006f9be0'], '013ca7d4': ['009a76a0', '009a76c0', '01224030'],
 '00412330': ['00412330', '004126e0', '00495890', '00e81b40'], '00433df0': ['004336bc'],
 '009a76c0': ['009a76c0'], '00a38e50': ['00a38e50'], '00a38670': ['00a38670', '00a38e50'], '00a381d0': ['00a381d0', '00a38670'], '00a39900': ['00a38550'],
 '013d27e8': ['00a38670'], '0049d870': ['007066a0'], '0099b800': ['009e90a0', '00495fe0'], '004ba810': ['004bb600'],
 F6: ['006156e0', '006d3fb0', '00777000', '007ee660', '009223a0', '0093f930', '007ee5a0', '00774b40'],
 '00c17260': ['00c17260'], '00c0c0c0': ['00c0c0c0'],
 '00c0dec0': ['00c17260', '00c309e0', '00c3bb50', '00c3bcb0'],
 '0099a300': _rows('bcnc'),
}
LEFT = {
 '00412330': ['004091c0', '004128a0', '005fdad0', '0071ce80', '009aef60', '009af480'],
 '0099b800': ['009994a0', '00999620'],
 '00c0dec0': [],
 '0099a300': sorted(set('0042db40 00523540 0055d070 00770e50 00778ae0 0077b250 0077b9b0 0077be40 00bc9c30 009cdf50 '
                        '00446be0 00719120 007e7e40 00995df0 00a8e040 0083b7f0 0083bbf0 0083c5d0 0083dbe0 0069f2b0 007158f0 00778d60 00778e70 '
                        '00779000 00779d20 007e8870 007f18b0 0122ab10 0122ab30 0122ab50 007190f0 009a0b00 00aac7f0 00905440 00905470 006e11f0 '
                        '004120c0 00ab3550 006ac910 00a46420 00719150 00987f10 00719cb0 007190c0 00835c80 004016b0 00b98560 00b15b20 009050a0 '
                        '00bb1760 00412020 00905070 009e7bd0 00b564c0 009054a0 0122aad0 00820e20'.split())),
}
CLEAR = {}
_C691 = 'labels_chunk691; '
N = {
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): one mangled symbol cannot link twice; owner decision (carried from waves 15-23; brief: 0x00C28690 not in this wave)'),
 '00ad3a10': ('hold', 'HOLD (DECISIONS_W21_RESOLVED; brief: not in this wave): two retail copies of set<unsigned long>\'s copy ctor, per-module bindings needed'),
 '013ca828': ('deferred', 'GDefStringTable leftovers 0x005DCF70 / 0x008AF850 / 0x00890F40 (carried from wave 22: per-row re-author with the shared header; not reached)'),
 '007acc50': ('deferred', 'per-row STLport map<CCharString,bool> re-models not reached (scope 2). 0x007ADB50 / 0x007ADD50 stay HOLD (two retail copies of the FableWin '
              'map<CCharString,bool> destructor, DECISIONS_W23_RESOLVED item 7); 0x007AD180 / 0x007ADBF0 / 0x008A78E0 / 0x008A7930 left'),
 '00440990': ('deferred', 'the 23 vector<long>::_M_insert_overflow callers (size_type I, protected, STLport vector<_Tp, allocator<_Tp> > per row) not reached (scope 2)'),
 '00613ae0': ('hold', 'the two callers 0x00616970 / 0x00617180 are byte-identical 66-byte out-of-line ~map<ECategory, NUISystem::CComponent*> bodies (both erase through '
              '0x00613AE0): two retail copies of one destructor symbol -- the 0x00AD3A10 / 0x007ADB50 HOLD pattern (wave 23 GOTCHAS); not respelled'),
 '006196d0': ('hold', 'the two callers 0x00619D00 / 0x00619E70 are byte-identical out-of-line ~map<long, CRegion> bodies (both erase through 0x006196D0): two retail '
              'copies of one destructor symbol -- HOLD as 0x00613AE0'),
 '00c0dec0': ('respelled', 'family cstr: the four remaining views re-modelled on the libpk CString / CLoadLog -- 0x00C17260 (LoadBank, DECISIONS_W23 item 8), '
              '0x00C309E0 (the destructor of a record holding a CString at +4: `add ecx,4 ; jmp ~CString`, called only from the scalar deleting destructors), '
              '0x00C3BB50 / 0x00C3BCB0 (CLoadLog::AddError() << text through CString::operator<< 0x00C0DEC0 / (unsigned long) 0x00C0DF10)'),
 '0076c000': ('deferred', 'leftovers 0x00AF59A0 (NParticleEngine-namespaced pair / sort model) and 0x0076C360 (?Callee_Do junk view): per-row std::sort re-author, not reached (scope 2)'),
 '0076c1a0': ('deferred', 'leftovers 0x00AF59A0 and 0x0076C6B0 (?Inner_Do junk view): per-row re-author, not reached (scope 2)'),
 '009cec60': ('deferred', 'CBankFile members wait for the engine-header migration (DECISIONS_W22_RESOLVED item 4; brief: not in this wave)'),
 '009cbd30': ('deferred', 'CBankFile header (DECISIONS_W22_RESOLVED item 4; brief: not in this wave)'),
 '009cbd50': ('deferred', 'CBankFile header (DECISIONS_W22_RESOLVED item 4; brief: not in this wave)'),
 '0099b510': ('deferred', 'the ~CWideString leftovers (namespaced in-TU string models AFileSupport / ExternalRetry_00999620 / NGameText / N006bebf0 / N009ac5d0 and the '
              'free-function views): per-row, not reached'),
 '0099a300': ('respelled-partial', 'family bcnc (fam_bcnc24.py): %d rows whose local CBaseClassNonCopyable declared the virtual destructor public (UAE) now declare it '
              'protected -- ??1CBaseClassNonCopyable@@MAE@XZ, Ego_r.pdb / FableWin.pdb and engine/CBaseClassNonCopyable.h (and FableWin\'s ??_G...MAE where the '
              'row emits it). Left: 8 rows whose destructor is implicit (declaring it emits a CBaseClassNonCopyable vtable / ??_G: re-model per row), 2 rows '
              'that already recheck DIFFER at HEAD (0x00BC9C30 34v34, 0x009CDF50 246v246 -- lane gap, not a naming issue), the 5 non-virtual QAE models '
              '(a virtual destructor would add a vptr) and the BaseDtor_* / *_Destruct@@YIXPAX / CTCParamsPhysicsBase / AutoTiny stand-ins (per-row)' % len(_rows('bcnc'))),
 '00cd3f50': ('done', 'landed under the final by chunk 691 (own row defines it); nothing to respell'),
 '00e3a6b0': ('done', 'landed under the final by chunk 691; nothing to respell'),
 '00d89710': ('done', 'no own row landed; the only naming row 0x00D87CB0 already binds the final; nothing to respell'),
 '0084fdb0': ('done', 'landed under the final by chunk 691; nothing to respell'),
 '004ba810': ('respelled', 'family misc: ~CGameScriptThingManager 0x004BB600 -- its stand-in base CScriptThingListBase_004bb600::Clear is Ego_r\'s member '
              'list<CScriptThingInfo> ScriptThings whose inline destructor calls _List_base<...>::clear 0x004BA810 (then NullThing, as before)'),
 '00c0ce50': ('done', 'landed under the final by chunk 691; nothing to respell'),
 '009d4ce0': ('respelled', 'family defmgr: own row SaveBinary -> ?SaveTable@CDefStringTable@@QAEX_N@Z (' + _C691 + 'Ego_r/FableWin member order)'),
 '009b08c0': ('respelled', 'family defmgr: own row on the PDB CDefinitionManager (engine-header rename pattern; was NDefinitionManager::CDefinitionManager) with its '
              'callees at their PDB access (IAE CheckDependencies / CreateSymbolsFromPathList / DoCompilePathList / RemoveUnusedMemory on std::vector<CWideString>); '
              'caller 0x004336BC (was InitAndCompile)'),
 '009acb10': ('respelled', 'family defmgr: own row re-authored as ?ClearSymbols@CDefinitionManager@@QAEXXZ (PSymbolMap_Data->Clear(); was a junk T::Run forwarder); '
              'caller 0x00416005 (was FollowUp_9ACB10 on CGameDefinitionManager; CGameDefinitionManager now derives from CDefinitionManager)'),
 '00cbf647': ('respelled', 'family defmgr: ?SetGlobalScriptDef@@YIXXZ on the ESTABLISHED pGlobalScriptDef CDefPointer<const CScriptDef> through FableWin\'s '
              'GetDef<CScriptDef>(const CCharString&, CDefPointer<const CScriptDef>*) const (was GFLoadScriptDef_00cbf647 / g_scriptDef)'),
 '009a76a0': ('respelled', 'family defmgr: own row re-authored as NBankFileManager::GetHeaderDir (returns SymbolHeaderDir; was a junk C::Get on g_str_9a76a0); '
              'callers 0x004336BC (GetBankDirectory) and 0x006F9BE0 (GetDataDirectory)'),
 '009a76c0': ('respelled', 'own row already defined the final; its global respelled to NBankFileManager::SymbolHeaderDir (family defmgr)'),
 '013ca7d4': ('respelled', 'family defmgr: ?SymbolHeaderDir@NBankFileManager@@3VCWideString@@A -- the dynamic initializer 0x01224030 defines it (Ego_r '
              'NBankFileManager::`dynamic initializer for \'SymbolHeaderDir\'\'), GetHeaderDir / SetHeaderDir read / write it (was g_CCharString_013ca7d4 / g_str_9a76a0)'),
 '00412330': ('respelled-partial', 'family vws: the own row and the callers modelling a real std::vector (0x004126E0, 0x00495890, 0x00E81B40) take STLport\'s allocator '
              'argument and the protected member (IAE). Left: the CWideStringVector / VectorPushBackOverlay / template-tag stand-ins 0x004091C0, 0x004128A0, '
              '0x005FDAD0, 0x0071CE80, 0x009AEF60, 0x009AF480 (per-row re-models of the vector class)'),
 '00433df0': ('respelled', 'family defmgr: 0x004336BC re-modelled on std::vector<CWideString> (push_back / ~vector / operator= out of line; was CWideStringVector)'),
 '00a38e50': ('respelled', 'family crc: own row ?ParseBuffer@CCRCSymbolMap@@IAE_NPBDABVCCharString@@@Z (protected, const char*, the wanted name const CCharString&)'),
 '00a38670': ('respelled', 'family crc: own row ?ParseEnum@CCRCSymbolMap@@IAE_NAAVCStringParser@@ABVCCharString@@@Z (protected, parser by reference); caller 0x00A38E50'),
 '00a381d0': ('respelled', 'family crc: own row ?Error@CCRCSymbolMap@@IAAXABVCWideString@@JPBDZZ (protected; CWideString with the PDB ToCharString 0x0099B2C0; was a '
              'CRetailWideString view); caller 0x00A38670'),
 '00a39900': ('respelled', 'family crc: CCRCSymbolMap::Clear 0x00A38550 calls the vector map\'s DeallocateMemory (was a CVectorMap::Clear not in the PDB)'),
 '013d27e8': ('respelled', 'family crc: the static CCRCSymbolMap::CurrentFilename in ParseEnum 0x00A38670 (was CRetailWideString g_CRCSymbolFileName)'),
 '0049d870': ('respelled', 'family misc: 0x007066A0 calls ?GetFrame@CWorld@@SIJXZ (was on a CWorld_Calls stand-in)'),
 '0099b800': ('respelled-partial', 'family wsop: 0x009E90A0 re-authored as ?Open@CMemoryFile@@QAEXXZ (Pathname = L""; was a free Open with a junk Sub::init) and '
              '0x00495FE0 (WorldToLoad = L""; was a CFrontendModePointer_00495fe0 stand-in). Left: 0x009994A0 / 0x00999620 (in-TU AFileSupport / '
              'ExternalRetry string models that define the CWideString bodies: per-row re-model)'),
 F6: ('respelled', 'family filt6 (DECISIONS_W23_RESOLVED item 5): each row re-identified from the filter operator it calls and the Ego_r / FableWin instantiation, '
      'then respelled with the wave 23 filt transform: 0x006156E0 GetPFirstLeafThing<HasTC, CThingMarker> (list 0xB = CThingMarker), 0x006D3FB0 '
      'GetPFirstThing<HasTC, CThing>, 0x007EE660 GetPFirstThing<HasTC, const CThingVillage>, 0x0093F930 GetPBestThing<In2DRange, Nearest, CThing>, '
      '0x00777000 GetPFirstThing<Bind2<IsNot, In3DRange>, CThing>, 0x009223A0 GetPBestThing<Bind3<IsNot, IsNot, HasScriptName>, Nearest, CThing>; callers '
      '0x007EE5A0 (const CThingVillage*) and 0x00774B40 (the const member template over Bind2<IsNot, In3DRange>) bind the PDB instantiations. The own rows stay '
      'on the CThingSearchToolsMethods view (DECISIONS_W23_RESOLVED item 3: engine-header migration)'),
 '00c17260': ('respelled', 'family cstr (DECISIONS_W23_RESOLVED item 8): LoadBank kept -- the caller 0x00C172F0\'s same-TU static, corroborated by the landed bindings '
              'of its callees (CLoadLog::IsOk 0x00C307C0 by 0x00C2E480, CBankData by 0x00C17940, the CLoadLog* argument of 0x00C30F60 by 0x00C16D90); libpk has '
              'no PDB so the name stays descriptive. The own row (was LoadRecordAndReport_00c17260 on a propagated AI model) spells its body as the caller does'),
 '00c0c0c0': ('respelled', 'family cstr (DECISIONS_W23_RESOLVED item 9): own row re-authored as ??4CString@@QAEABV0@ABV0@@Z (MFC strcore.cpp operator=), the nil '
              'header _NullString (retail\'s own assert text names it; 0x00C0DAB0 binds it) and g_pfnAssertHook'),
}
