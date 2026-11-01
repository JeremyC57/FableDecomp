# mkq23_notes.py (exec'd by mkq23.py): per-target outcomes / notes for wave 23.
EDX = {'006a80a0': ['004a9da0', '004a9e50', '005c0d9d', '00615750', '006186f0', '0065f744', '00782670', '00799c40', '0093f8a0', '00965e80'],
       '00c44eb0': _rows('emit'), '00c0ad10': _rows('cstrown'), '00c0dec0': _rows('cstrown'), '0099a300': _rows('bcnc')}
CLEAR = {}
LEFT = {'006a80a0': ['004a9ef0'], '00c44eb0': [], '00c0ad10': [],
        '00c0dec0': ['00c17260', '00c309e0', '00c3bb50', '00c3bcb0'],
        '009cbd30': ['009cbd30'], '009cbd50': ['009cbd50'],
        '0099a300': open(os.path.join(RUNS, 'left_0099a300.txt')).read().split()}
_FILT = ('family filt (fam_filt23.py, %d rows; DECISIONS_W22_RESOLVED item 2): the CThingSearchTools templates call `filter(*p)`, CFilter_Bind2/3 hold '
         'their filters by pointer (Ego_r.pdb F1/F2/F3) with the member template operator()(const T&) (FableWin.pdb ??$?RV...@?$CFilter_Bind2@...), and '
         'every bound filter takes its PDB `const T&` -- the 10 HasTC rows together plus 19 rows of the same templates binding In2DRange / InMap / '
         'InRegion / ... on pointers (namespace stand-ins N_<addr> hoisted to the global class). 0x004A9DA0 re-identified Bind2<HasTC, InRegion> (its callee '
         '0x006A7960 is InRegion; FableWin.pdb instantiates GetPBestThing on Bind2<HasTC,InRegion>; was In2DRange). Left: 0x004A9EF0 (Thing / '
         'GetThingList free-function model: per-row re-author)') % len(_rows('filt'))
N = {
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): one mangled symbol cannot link twice; owner decision (carried from waves 15-22; brief: not in this wave)'),
 '00ad3a10': ('hold', 'HOLD (DECISIONS_W21_RESOLVED; brief: not in this wave): two retail copies of set<unsigned long>\'s copy ctor, per-module bindings needed'),
 '013ca828': ('deferred', 'GDefStringTable leftovers 0x005DCF70 / 0x008AF850 / 0x00890F40 (carried from wave 22: per-row re-author with the shared header; not reached)'),
 '007acc50': ('deferred', 'the six callers need per-row STLport map<CCharString,bool> re-models; wave 23 finding: 0x007ADB50 / 0x007ADD50 are two byte-identical copies of the '
              'map destructor (FableWin.pdb ??1?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAE@XZ; '
              'Ego_r has none) -- one symbol twice, the 0x00AD3A10 HOLD pattern (coordinator decision); 0x007AD180 / 0x007ADBF0 / 0x008A78E0 / 0x008A7930 '
              're-authoring not reached'),
 '00440990': ('deferred', 'mega family (23 rows): each caller defines its own same-TU _M_insert_overflow body on a hand vector (CLongVector / InsertGrow / CArray<long> / '
              'CRetailVector_*); the re-model is the STLport vector<_Tp, allocator<_Tp> > template of 0x00440990 / 0x00440F00 with the protected member per row '
              '(no explicit specialization: a device retail never had) -- not reached'),
 '00613ae0': ('deferred', 'callers 0x00616970 / 0x00617180 are own rows of the ~tree stand-in family (own-row re-author as the STLport map destructors) -- not reached'),
 '006196d0': ('deferred', 'callers 0x00619D00 / 0x00619E70 are own rows of the ~tree stand-in family (own-row re-author) -- not reached'),
 '00c0dec0': ('respelled-partial', 'family cstrown: the four own rows 0x00C0AD10 / 0x00C0DF50 / 0x00C0E0F0 / 0x00C0E360 re-authored on CString (item 3). Left: the views '
              '0x00C17260 (bound by 0x00C172F0 as ?LoadBank@@YGXPAVCLoadLog@@PAXPAUCRequest_00c09110@@@Z: identity conflict with its own LoadRecordAndReport_00c17260 '
              'model -- the log is a CString; per-row), 0x00C309E0 (8-byte `add ecx,4 ; jmp ~CString` whose only callers are scalar deleting dtors 0x00C30A50 / '
              '0x00C30A90 of vtable 0x012B4C28: a destructor of an unnamed libpk class with a CString at +4 -- class name needed), 0x00C3BB50 / 0x00C3BCB0 (String / '
              'Log / CriteriaText views) -- per-row'),
 '0076c000': ('deferred', 'leftovers 0x00AF59A0 (NParticleEngine-namespaced pair / sort model) and 0x0076C360 (?Callee_Do junk view) -- per-row re-author, not reached'),
 '0076c1a0': ('deferred', 'leftovers 0x00AF59A0 and 0x0076C6B0 (?Inner_Do junk view) -- per-row re-author, not reached'),
 '009cec60': ('deferred', 'CBankFile members wait for the engine-header migration (DECISIONS_W22_RESOLVED item 4; brief: not in this wave)'),
 '009cbd30': ('deferred', 'CBankFile header (DECISIONS_W22_RESOLVED item 4; brief: not in this wave)'),
 '009cbd50': ('deferred', 'CBankFile header (DECISIONS_W22_RESOLVED item 4; brief: not in this wave)'),
 '0099b510': ('deferred', 'the ~CWideString leftovers of wave 22 (namespaced models with qualified uses / namespace-level string-data types and the free-function views) -- per-row, not reached'),
 '0099a300': ('respelled-partial', 'family bcnc: the queue spelling is corrected to the PDB ??1CBaseClassNonCopyable@@MAE@XZ (Ego_r.pdb + FableWin.pdb, protected virtual; '
              'engine/CBaseClassNonCopyable.h; the compiler-emitted Leaf) -- the queue\'s UAE is a hand-mangled claim. The own row is re-authored as ~CBaseClassNonCopyable '
              '(with ~CBaseClass in the same TU, as retail\'s BaseClass.cpp, so the dead own-vtable store drops) and the c690 caller 0x00B3B8E0 takes the engine header. '
              'Left: the %d callers under BaseDtor_* / *_Destruct@@YIXPAX / CTCParamsPhysicsBase stand-ins (the temporaries\' implicit destructors chain to it: '
              'per-row re-models; the CTCParamsPhysicsBase rows bind ~CTCParamsPhysicsBase to 0x0099A300, which needs the inlined implicit dtor chain); the 319 rows naming ??1CBaseClassNonCopyable@@MAE@XZ already are right' % len(open(os.path.join(RUNS, 'left_0099a300.txt')).read().split())),
 '006a80a0': ('respelled', _FILT),
 '00c44eb0': ('respelled', 'family emit (DECISIONS_W22_RESOLVED item 1): ctor 0x00C44EB0 on ??0CAmbientEmitter@@QAE@PAUCResourceWorld@@PAX@Z (Init2 = the landed AcquireSounds), '
              '0x00C61C70 = ??0CAmbientEvent@@QAE@PAX_N@Z (was ??0CThingFilter_SolidToThing; flag loaded as a byte), 0x00C44C50 moved to CAmbientEmitter (it passes this '
              'to CAmbientEvent::Reschedule(CAmbientEmitter*)) keeping its stand-in member name InitializeThingFilters (DECISIONS_W23 item 2)'),
 '00c0ad10': ('respelled', 'family cstrown (DECISIONS_W22_RESOLVED item 3): ?Mid@CString@@QBE?AV1@HH@Z, ?Right@CString@@QBE?AV1@H@Z, '
              '?GetFileNameWithoutExtension@CString@@QBE?AV1@XZ, ?DescribeBitset_00c0ad10@@YI?AVCString@@PBV1@KG@Z (stand-in function name kept); callees '
              'AllocCopy(int) / Left(int) / ReverseFind / TrimLeft / TrimRight (0x00C0D7C0 is the landed TrimRight; 0x00C0E360 called it Remove) / SafeStrlen / << '),
}
