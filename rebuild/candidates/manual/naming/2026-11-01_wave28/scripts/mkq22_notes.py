# mkq22_notes.py (exec'd by mkq22.py): per-target outcomes / notes for wave 22.
EDX = {'00c0dec0': _rows('cstr'), '00b67ec0': _rows('vec'), '00441020': ['00b67ec0'], '00c44f80': _rows('emit'), '00d34470': _rows('c689'),
       '0076c000': ['0076c750'], '0076c1a0': ['0076c750'], '0099b510': _rows('ws') + _rows('wsns')}
CLEAR = {}
LEFT = {'00c0dec0': ['00c0ad10', '00c0df50', '00c0e0f0', '00c0e360', '00c17260', '00c309e0', '00c3bb50', '00c3bcb0'],
        '00b67ec0': [], '00c44f80': [], '00d34470': [], '00441020': [], '009cbd30': ['009cbd30'], '009cbd50': ['009cbd50']}
_CS = ('family cstr (fam_cstr22.py, %d rows): every row reaching the libpk CString own rows 0x00C0DEC0 / 0x00C0DF00 / 0x00C0DF10 / 0x00C0BA40 or the '
       'break temporary 0x00C16750 under a stand-in (CDebugStream x59, CString_<addr> x41, CTrace[_<addr>] x39, CLogLine_ x5, CMember_ x3, '
       'CCharString_00c0aa40 / 00c0ac30, CStringMember_00c0ba40, CAssertString / CAssertTrace, CDebugStream_00c3ab50 / 00c3b0e0) respelled to '
       'class CString { operator<<(const char*) / (long) / (unsigned long); ~CString } and CDebugBreak; Append chains -> `<<` chains; the '
       'callees that take / return the string follow (Describe 0x00C21040, GetStatus 0x00C27760, OpenSegment 0x00C2D9C0, FindByName '
       '0x00C101D0, NewMessage 0x00C307E0, TraceOutput). Left: the own rows 0x00C0AD10 / 0x00C0DF50 / 0x00C0E0F0 / 0x00C0E360 (they define '
       'CString_<addr> members: own-row re-author), 0x00C17260 (free AppendText view), 0x00C309E0 (CObject_00c0ba40::Method view), 0x00C3BB50 '
       '(String / Log views; Number must stay unsigned long for 0x00C0DF10), 0x00C3BCB0 (CriteriaText / AppendErrorText views) -- per-row') % len(_rows('cstr'))
N = {
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): one mangled symbol cannot link twice; owner decision (carried from waves 15-21; brief: not in this wave)'),
 '00ad3a10': ('hold', 'HOLD (DECISIONS_W21_RESOLVED; brief: not in this wave): two retail copies of set<unsigned long>\'s copy ctor, per-module bindings needed'),
 '013ca828': ('deferred', 'GDefStringTable leftovers: 0x005DCF70 / 0x008AF850 define same-TU noinline bodies of CDefStringTable::GetString on a namespaced CDefStringTable (respelling the class moves a defined body onto the PDB ?GetString@CDefStringTable@@ -- re-author with the shared header) and 0x00890F40 is a junk GObj model -- per-row re-author, not reached'),
 '007acc50': ('deferred', 'own row 0x007ACC50 defines the final; the six callers need per-row STLport map<CCharString,bool> re-models (0x007AD180 own Leaf ?erase@CStringBoolTree is junk -> the _Rb_tree::erase(iterator,iterator) instantiation; 0x008A78E0 / 0x008A7930 reach the established ?ActiveGossipCategories@CTCAIScratchpad@@2V?$map...@A as g_obj / GossipList; 0x007ADB50 / 0x007ADD50 in the ~tree stand-in family; 0x007ADBF0 map@X view) -- re-authoring, not reached'),
 '00440990': ('deferred', 'mega family (23 rows, 14+ hand spellings InsertGrow / CLongVector / CArray<long> / CRetailVector_*): each needs the STLport vector<long> model with the protected _M_insert_overflow -- per-row re-models, not reached'),
 '00441020': ('respelled', 'family vec: 0x00B67EC0 re-modelled on std::vector<float> / <unsigned char> / <unsigned long> in one edit (DECISIONS_W21_RESOLVED): binds ?_M_fill_insert@?$vector@MV?$allocator@M@std@@@std@@QAEXPAMIABM@Z (0x00441020, already bound by 0x00AC9D50), ?_M_fill_insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z (0x00485F3A, ADD) and the same-TU body ?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z (0x00464ACE, already bound by 0x00AA65A0)'),
 '00613ae0': ('deferred', 'callers 0x00616970 / 0x00617180 are own rows of the ~tree stand-in family (CRetailTree_erase00613ae0 destructors) -- own-row re-author as the STLport map destructors, not reached'),
 '006196d0': ('deferred', 'callers 0x00619D00 / 0x00619E70 are own rows of the ~tree stand-in family (CRetailTree_erase006196d0) -- own-row re-author, not reached'),
 '00d34470': ('respelled', 'family c689: the own row takes the PDB signature (CScriptDataBase*, CGameScriptInterfaceBase&) on the precedent of the landed CQ_HangingTreeGoodScript ctor 0x00D6EF20 (the novtable script-object base stores Game = &game); Leaf line names the full symbol; byte-identical'),
 '00c0dec0': ('respelled-partial', _CS),
 '00b67ec0': ('respelled', 'family vec: the three vectors re-modelled as STLport std::vector<unsigned long> / <float> / <unsigned char> (the hand CVector<T> retired); 0x00441020 / 0x00485F3A / 0x00464ACE move together'),
 '00c44f80': ('respelled', 'family emit: the destructor 0x00C44F80 is ~CAmbientEmitter (it runs the landed CAmbientEmitter::ReleaseSounds 0x00C44B80 and ClearEvents 0x00C44D30 on this; was CGameFilters_00c44f80 with CleanupA / CleanupB); the same object\'s `new` in CResourceWorld::AllocateFor 0x00C36180 and `delete` in FreeAllocation 0x00C36290 respelled from CResourceAllocation (ADD ??0CAmbientEmitter@@QAE@PAUCResourceWorld@@PAX@Z 0x00C44EB0, ??1CAmbientEmitter@@QAE@XZ 0x00C44F80; DECISIONS_W22 item 2: the own rows 0x00C44EB0 / 0x00C44C50 still carry CResourceAllocation_00c44eb0 / CGame_00c44c50 stand-ins)'),
 '00c059d0': ('already-correct', 'the chunk-690 own row compiles the zlib crc32.c TU with the static crc32_little as its Leaf (bound _crc32_little); no landed row names another spelling of 0x00C059D0'),
 '00c05cb0': ('already-correct', 'the chunk-690 own row (static crc32_big, same TU); no landed row names another spelling'),
 '00adefc0': ('already-correct', 'no landed row names a spelling of 0x00ADEFC0 (own row not landed)'),
 '00cb0700': ('already-correct', 'the chunk-690 own row defines the final; no other spelling named'),
 '00ca1360': ('already-correct', 'the chunk-690 own row defines the final; no other spelling named'),
 '00767da0': ('already-correct', 'the chunk-690 own row defines the final; no other spelling named'),
 '0076c000': ('respelled-partial', 'family c690: the landed CParticleUpdateParameters<unsigned long, unsigned long>::Update 0x0076C750 declares the nested comparators with class key V (FableWin.pdb) and now binds the final; left: 0x00AF59A0 (an NParticleEngine-namespaced pair / sort model) and 0x0076C360 (?Callee_Do junk view) -- per-row'),
 '0076c1a0': ('respelled-partial', 'family c690: 0x0076C750 binds the final (class-key V comparator); left: 0x00AF59A0 (NParticleEngine-namespaced model) and 0x0076C6B0 (?Inner_Do junk view) -- per-row'),
 '00b3b8e0': ('already-correct', 'the chunk-690 own row defines the final; no other spelling named'),
 '009cec60': ('deferred', 'the one other binder 0x00A427A0 (?DoUpdate@CLipSyncDataBank_Methods@@QAEXXZ) is CLipSyncDataBank::Update (Ego_r ?Update@CLipSyncDataBank@@UAEXXZ) calling the base CBankFile::Update non-virtually; engine/CLipSyncDataBank.h is flattened (no CBankFile base, no virtuals) -- engine-header item (migration wave 10), not name-only'),
 '009cbd30': ('deferred', 'own row 0x009CBD30 defines CScriptedThingOwner_Retail::func_009cbd30(long); the PDB ?SetBankUpdateStatus@CBankFile@@IAEXABVCCharString@@@Z is a protected CBankFile member and engine/CBankFile.h declares no member functions (the landed CBankFile rows use views) -- engine-header item'),
 '009cbd50': ('deferred', 'own row 0x009CBD50 defines a CScriptedThingOwner_Retail view; the PDB ?SetBankUpdateProgress@CBankFile@@AAEXKK@Z is a private CBankFile member -- engine-header item, as 0x009CBD30'),
 '00a620d0': ('already-correct', 'no landed row names another spelling of 0x00A620D0 (the own row binds the final)'),
 '0099b510': ('respelled-partial', 'families ws / wsns: the CWideString stand-ins of 0x00BC7AB0 (CListMember_0099b510), 0x0056C820 (StrB), 0x004B8110 (StrRefA) renamed, and the namespaced CWideString of 0x00569450 (FableVector_) / 0x006BE9D0 (N006be9d0) hoisted to the global PDB class; left: namespaced models with qualified uses or namespace-level string-data types (0x009994A0, 0x00999620, 0x006BEBF0, 0x009AC5D0, 0x009C7A60) and the free-function / view stand-ins (@FableBaseDtor_*, @WSdtor_*, Cleanup_*, ...) -- per-row'),
 '0099a300': ('deferred', 'destructor family (brief scope 3): the own row 0x0099A300 defines ??1CBaseClass@@UAE@XZ (a stand-in), and 54 rows reach it under MAE / CBaseClass / CTCParamsPhysicsBase spellings -- own-row re-author first; not reached'),
}
