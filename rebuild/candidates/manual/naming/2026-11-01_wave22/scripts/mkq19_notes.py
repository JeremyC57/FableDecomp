# mkq19_notes.py (exec'd by mkq17.py): per-target outcome overrides / notes, and skipsum() -- the fam_q skip reasons of a target.
import glob as _g
ABASE = dict((r[1], r) for r in A)
def skipsum(t):
    ls = sorted(_g.glob(os.path.join(W, 'logs', 'q_*_%s.log' % t)) + _g.glob(os.path.join(W, 'logs', 'compose_ov_final_*_q%s.log' % t)), key=os.path.getmtime)
    if not ls:
        a = ABASE.get(t)
        if a and a[5] and a[6].startswith('N'):
            return 'not driven this wave: the own row defines a stand-in (%s) -- own-row re-author needed before the callers' % a[6][2:].split()[0][:90]
        return 'not driven this wave: the remaining rows name per-row stand-in spellings (vtable / data / free-function views) -- per-row'
    c = collections.Counter()
    for l in open(ls[-1]):
        p = l.split()
        if len(p) >= 4 and p[0] == 'skip':
            k = ' '.join(p[2:4]).rstrip(':')
            k = {'free stand-in': 'free-function stand-in (no class to move)', 'no calls': 'no call site found (address bound through a view / data)',
                 'call of': 'qualified / indirect call', 'non-public': 'non-public member reached from an unrelated class'}.get(' '.join(p[2:4]), k)
            if p[2] == 'moved' or p[2] == 'inplace': k = 'compile error after moving %s' % p[3]
            if p[2].isdigit(): k = 'stand-in %s (definition / signature mismatch)' % p[3]
            if p[3] == 'defined': k = 'row defines its own %s model' % p[2]
            c[k] += 1
    a = ABASE.get(t)
    own = ('; the own row defines a stand-in (%s) -- own-row re-author needed' % a[6][2:].split()[0][:90]) if a and a[5] and a[6].startswith('N') else ''
    if not c: return ('callers respelled' + own) if own else 'fam_q: every driven row edited; the rows left are outside its reach (re-authoring)'
    return 'fam_q skips: ' + '; '.join('%d %s' % (n, k) for k, n in c.most_common(3)) + ' -- per-row re-modelling' + own
EDX = {'00c74510': ['00c74ce0', '00c74d30'], '00c674b0': ['00c689b0'], '009fe380': ['0041ac20', '0041c260', '009fe550', '009fe620', '009ff8d0', '00b9bcc0', '00ba9360', '00ba95d0', '00ba9a70'], '009ff6b0': ['0041ac20', '0041c260', '009ff8d0', '00ba9360', '00ba95d0', '00ba9a70', '009ff6b0'], '00caa4a0': ['00caaea0', '00caaf10'], '0143cdfc': ['00405650'], '00ab3640': ['00ab36b0', '00ab3640'], '00c674b0': ['00c689b0'], '00ab1480': ['00ab1480'], '00a89320': ['00a89320'], '00a89350': ['00a89350'], '0099f560': ['0099f560'], '008fd000': ['0095ab50', '008c2240']}
CLEAR_LEFT = {'00ab1480', '0143cdfc', '00c674b0', '00ca0ba0'}
N = {
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): one mangled symbol cannot link twice; owner decision (carried from waves 15-18)'),
 '00bda150': (None, 'family c684: the private ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ in the three Allocate* callers 0x00BDA360 / 0x00BDA3D0 / 0x00BDA470 (were ProcessDisposalLists / FlushCycle)'),
 '0077a890': (None, 'family c684: ?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z in OnDie 0x00663F50 (was the free CreateExperienceOrbs)'),
 '00a5b870': (None, 'family c684: both GFInterpolateVectors overloads (0x00A5B870 KKM, 0x00A5BBA0 K1KKM) in namespace NVectorMath in 0x00AE1260 / 0x00AE1600 (Ego_r donor order 00BD5D90 / 00BD60F0); 0x00AE2CB0 already spells the K1KPBMKK overload right (the bindings line is stale)'),
 '00c74510': (None, 'family c684: bool sixth parameter of ?ApplyRamps@CDspGain@@QAEXPBM0PAM01_NI@Z and of the callers\' own pass-through parameter in Apply 0x00C74CE0 / Crossfade 0x00C74D30 (re-graded: RELOCATION_MATCH, VC7.1 loads a bool parameter it passes on as a dword)'),
 '009fe380': (None, 'family gif: CFrameData nested in CGraphicInfoFrame with the const CGraphicInfoFrame* ctor (??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z) in 9 rows (the row\'s CFrameData* helper templated on the pointee so the nested forward declaration suffices); 0x00BEC000 / 0x00BEC470 / 0x00BEC950 (no CFrameData definition: the ctor is declared on a forward-declared class), 0x00A00D90 (CFrameData_00a00d90 : CIVCountedPointeeBase model) and 0x0041B550 (the whole resource model wrapped in namespace NUISystem beside the engine headers) -- per-row'),
 '009ff6b0': (None, 'family gif: the private const ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z beside the nested CFrameData in the 6 rows that declare it, and on the own row 0x009FF6B0 (was public QAE_NJ: private const, unsigned long flags; MATCH); 0x0041B550 (NUISystem-wrapped model) -- per-row'),
 '00ba9360': ('already-correct', 'landed by chunk 684 under the static-member spelling; no caller names a wrong spelling'),
 '00ba95d0': ('already-correct', 'landed by chunk 684 under the static-member spelling; no caller names a wrong spelling'),
 '00ba9a70': ('already-correct', 'landed by chunk 684 under the static-member spelling; no caller names a wrong spelling'),
 '00c677c0': ('already-correct', 'landed by chunk 684 under the descriptive CEaxContext::SetEnvironmentSize; no caller names a wrong spelling'),
 '007748e0': (None, 'family c685: ?AddSaleItemPlacesFromThing@CTCShop@@QAEXPBVCThing@@@Z in GetFreeSaleItemPlaces 0x00774B40 (was InitGoodsPlaces)'),
 '00ab1f30': ('already-correct', 'landed by chunk 685 under the private spelling; its caller GetIntersection 0x00AB2160 is not landed'),
 '00ab3640': ('respelled', 'family c685: ?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@SIXPAX@Z as the deleter the count block stores in Reset 0x00AB36B0 (was the free Destroy_00ab3640), and the own row 0x00AB3640 re-spelled from the generic `OnDie` delete stub (CTCMapwho_OnDie_00ab3640.cpp) to the static member template (file renamed; its callee 0x00AB34C0 is ??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ, FableWin.pdb: the catalog label CEntityBindingRecord_00ab34c0::~ is a propagated descriptive name -- LABELS.tsv)'),
 '00ab1480': ('respelled', 'family c685: the own row 0x00AB1480 (global_MoveCountedPointer00ab1480, Move_CountedPointer_00ab1480) re-spelled to the STLport 4.6 _algobase.h std::__copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, same, long>(first, last, result, const random_access_iterator_tag&, long*) template (RELOCATION_MATCH, file renamed). NOTE for the coordinator: the queue\'s final spelling (chunk 685, hand-mangled; in neither PDB) carries 4 parameters (PAV1@0ABU...): VC7.1 mangles the 5-parameter STLport text as ...PAV1@00ABUrandom_access_iterator_tag@0@PAJ@Z (probe: the 4-parameter shape would be a 2-template-argument __copy, and retail pops 0Ch = 3 stack arguments), so the landed leaf is the compiler\'s spelling, not the queue\'s'),
 '00ab2bd0': ('deferred', '0x00AB34C0 names the __destroy_aux instantiation through a DestroyRange(CCountedPointer<CBindingTarget>*, ..., SNonscalarPtrIteratorTag) stand-in: the STLport __destroy_aux<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*> model with the __false_type tag is a re-model of that row -- per-row'),
 '00bec680': ('deferred', 'the callers 0x00BEC8B0 / 0x00B98DE0 call free Inner_CollectStats / SubMesh_CollectStats stand-ins that take the mesh base / the mesh info pointer and a StatMap view where the static member takes const C3DMeshInfo& and std::map<unsigned long,unsigned long>&: a re-model of both rows (the std::map argument) -- per-row'),
 '00a36ec0': ('deferred', '0x00B9BCC0 / 0x00B9E950 (??1CCountedMeshPointer) and 0x00B82100 (CMeshPointer::Destroy) model the out-of-line ~CCountedPointer<const C3DMesh2> as a row-local class destructor / member: the template instantiation spelling needs the CCountedPointer<const C3DMesh2> template declared in the row (engine/CCountedPointer.h inlines the destructor) -- per-row'),
 '00a89320': ('respelled', 'family c685: own row 0x00A89320 made const (?GetVertexCount@C3DMesh2@@QBEKXZ, Ego_r; MATCH); no caller names a wrong spelling'),
 '00a89350': ('respelled', 'family c685: own row 0x00A89350 made const (?GetTriangleCount@C3DMesh2@@QBEKXZ, Ego_r; MATCH); 0x00B00DB0 already spells it'),
 '009cc240': (None, 'family c685 (fam_q18): ?GetEntryDataSize@CBankFile@@QBEKK@Z in 8 rows (const in place on CBankFile; moved from the CBankFileAsync / NGameText::CDataBank stand-ins); 0x009D5260 / 0x009D58D0 / 0x00A62120 (the moved model loses the row\'s access to CBankFileAsync::Align / CBankFile::ReadEntryData) and 0x00A42980 (BankNative view) -- per-row'),
 '00b179a0': ('already-correct', 'landed by chunk 685; no caller names a wrong spelling'),
 '00ca0ba0': ('already-correct', 'landed by chunk 685 (vendor text): the body is the TU-local static `_01forward` (its leaf __01forward@24 is internal; the object exports only the same-TU caller @_01forward_caller@24, which is what the index sees); nothing to respell'),
 '00ca3b80': ('already-correct', 'landed by chunk 685 (vendor text); no caller names a wrong spelling'),
 '00caa4a0': ('respelled', 'family c685: the C-linkage _AMovieSetupRegisterFilter@12 (extern "C" __stdcall) in CBaseFilter::Register 0x00CAAEA0 / Unregister 0x00CAAF10 (RELOCATION_MATCH; the first pass of the step missed the declaration -- a `^` without (?m) -- and was repaired before the final recheck)'),
 '00c674b0': ('respelled', 'family c685: CEaxContext::SetSlotLoadEffect(unsigned long, SGuid) in the slot-property dispatcher 0x00C689B0, whose own row is respelled to CEaxContext::SetSlotProperty (the catalog name chunk 685 gave it; descriptive, libpk) over the landed SetSlotLock / SetSlotVolume / SetSlotFlags (was CSkillTable_00c67470::SetSkillPropertiesEx; file renamed); the own row 0x00C674B0 already defines ?SetSlotLoadEffect@CEaxContext@@QAEJKUSGuid@@@Z (the queue\'s final is a display string)'),
 '00cb5180': (None, 'family c685: ?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z in CIME::Shutdown 0x004051C0 / Initialise 0x00405650 (were ImeUI_Initialize)'),
 '00cb2dd0': ('already-correct', 'not landed; no landed row names it'),
 '0143cdfc': ('respelled', 'family c685: the SDK callback pointer ImeUiCallback_DrawFans in CIME::Initialise 0x00405650 (was g_ImeDrawFans); the landed 0x00CB5180 already names it'),
 '0099f560': ('respelled', 'family c685: the own row 0x0099F560 (global_Destval, Dest_val) re-spelled to ?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ (Ego_r / FableWin) calling ?RemoveWhiteSpaceBegin@CCharString@@QAEXXZ 0x0099F450 / ?RemoveWhiteSpaceEnd@CCharString@@QAEXXZ 0x0099F4D0 (FableWin.pdb; RELOCATION_MATCH, file renamed; the two callee labels go to LABELS.tsv)'),
 '00cbf29f': ('already-correct', 'spelling confirmed by chunk 685; nothing to respell'),
}
