# mkq21_notes.py (exec'd by mkq21.py): per-target outcome overrides / notes for wave 21, and skipsum() (no fam_q driver ran this wave).
import glob as _g
ABASE = dict((r[1], r) for r in A)
def skipsum(t):
    a = ABASE.get(t)
    if a and a[5] and a[6].startswith('N'):
        return 'not driven this wave: the own row defines a stand-in (%s) -- own-row re-author needed before the callers' % a[6][2:].split()[0][:90]
    return 'not driven this wave: the remaining rows name per-row stand-in spellings (vtable / data / free-function views) -- per-row'
def _rows(fam):
    p = os.path.join(RUNS, fam + '.rows.txt')
    return [l.strip() for l in open(p) if l.strip()] if os.path.exists(p) else []
EDX = {'012b1e24': _rows('csound'), '00c10a30': _rows('voice'), '00c29fc0': _rows('dbg'), '013ca828': _rows('gdst'), '00ad3a10': []}
CLEAR_LEFT = {'012b1e24', '00c10a30', '00c29fc0'}
N = {
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): one mangled symbol cannot link twice; owner decision (carried from waves 15-20)'),
 '012b1e24': ('respelled', 'family csound: CAudioOwner -> CSoundSystem in every landed row modelling vtable 0x012B1E24 (ctor 0x00C094A0 with the CSoundSystemSetup record, dtor 0x00C09380, ??_G 0x00C0A6B0, Update 0x00C091F0, WaitForSprite 0x00C08AE0, the owner parameters / members of CSoundEventManager 0x00C11D00 and CAudioParamBlock 0x00C0B200 / 0x00C0B380 / 0x00C0AF20, comments 0x00C12880 / 0x00C0A9A0) and the factory 0x00C096E0 re-authored as the __stdcall CreateSoundSystem_00c096e0(CSoundSystemSetup*) the landed 0x00A3EF00 binds (was CWorldManager::InitializeSteeringAfterSave over a CSteeringGroup stand-in)'),
 '00c10a30': ('respelled', 'family voice: CPKVoiceParams in the ctor 0x00C10A30 (was SNavigationRequest), the setters 0x00C20480 / 0x00C204A0 / 0x00C20420 (SetMinDistance / SetMaxDistance / SetScale(float), descriptive; were CBrainOutput::SetAsPumpCombatActions stand-ins), the callers 0x00C62530 / 0x00C621B0 / 0x00C37840 and the state rows 0x00C27C10 / 0x00C26680; Play 0x00C13140 is ?Play@CSoundEventManager@@QAEJJJPBUCPKVoiceParams@@@Z -- the table is CSoundSystem+0x38, the landed CSoundEventManager (vtable 0x012B2748), so the chunk-687 CPKVoiceTable rows 0x00C61CF0 / 0x00C61F60 / 0x00C61FB0 and the sound-loop rows 0x00C377E0 / 0x00C377F0 / 0x00C37930 are respelled to it (one class, one name; DECISIONS_W21)'),
 '00c29fc0': ('respelled', 'family dbg: the stand-ins of the callers (CMember_00c29fc0 x8, CTimer_ x2, CEventPart_) -> the landed PKCSemaphore (ctor 0x00C29FC0 / dtor 0x00C29FB0); the error-macro stream of 0x00C355E0 follows the landed own rows (CString 0x00C0BA40 / 0x00C0DEC0 / 0x00C0DF00, CDebugBreak 0x00C16750) as 0x00C1AC40 already does -- the decision\'s CDebugStream is the two deviant rows 0x00C29FC0 / 0x00C09920 (DECISIONS_W21 item)'),
 '00ad3a10': ('hold', 'HELD (DECISIONS_W21): the PDB set<unsigned long> copy-constructor spelling ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z is already bound to 0x00513700 by the landed 0x0051C470 (retail holds two copies of the instantiation, CLevelLoader TU / QuadricOptimiser TU); binding it to 0x00AD3A10 too (the six callers, scripts/hand21/h21_setcopy.py, rechecked MATCH except 0x00AD7E80) makes a multi-target symbol -- the coordinator decides which address keeps the PDB form'),
 '013ca828': ('respelled-partial', 'family gdst: the established ?GDefStringTable@@3VCDefStringTable@@A in every TU that named 0x013CA828 otherwise (fam_gdst21 driver + hand rows); left: 0x005DCF70 / 0x008AF850 (same-TU stand-in bodies of CDefStringTable::GetString, re-author) and 0x00890F40 (junk model) -- DEFERRED'),
}
# chunk 689 rows (appended by the coordinator; wave 21 family c689)
_C = 'c689 own row re-authored on proven text and rechecked'
N.update({
 '007acc50': ('deferred', 'own row 0x007ACC50 already defines the final; the callers bind per-row stand-ins: 0x007ADB50 / 0x007ADD50 (the ~tree stand-in family CRetailTree_erase*, 119 bodies: own rows define stand-in destructors), 0x007AD180 CStringBoolTree, 0x007ADBF0 map@X_007adbf0, 0x008A78E0 List::Erase, 0x008A7930 GossipList::Release -- per-row map<CCharString,bool> re-models, not driven this wave'),
 '00440990': ('deferred', 'own row 0x00440990 already defines the final (size_type I, coordinator-corrected); the vector<long> _M_insert_overflow callers bind 14+ hand spellings (InsertGrow / CLongVector / CArray<long> / CRetailVector_* families) -- a mega family, per-row re-models; not driven this wave'),
 '00441020': ('deferred', 'own row 0x00441020 already defines the final; the one other binder 0x00B67EC0 (CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed) models all three of its vectors on one CVector<T> template, so the respell also moves its vector<unsigned char>::_M_fill_insert 0x00485F3A and vector<unsigned long>::_M_insert_overflow 0x00464ACE spellings -- per-row re-model, not driven this wave'),
 '00613ae0': ('respelled-partial', _C + ': 0x00613AE0 is the STLport _Rb_tree<ECategory, pair<const ECategory, NUISystem::CComponent*>>::_M_erase (the 0x00834FD0 _tree text), the spelling ~CTCInventoryMap 0x00606F80 binds; was CTreeOwner_00613ae0::FreeTree. Left: 0x00616970 / 0x00617180, the ~tree stand-in family (CRetailTree_erase00613ae0 destructors, own rows)'),
 '006196d0': ('respelled-partial', _C + ': 0x006196D0 is the STLport _Rb_tree<long, pair<const long, CRegion>>::_M_erase (~CRegion 0x0051A110 on the value), the spelling ~CTCInventoryMap 0x00606F80 binds; was CTreeOwner_006196d0::FreeTree over a CMember_0051a110 stand-in. Left: 0x00619D00 / 0x00619E70 (~tree stand-in family)'),
 '00ad30c0': ('respelled', _C + ': the landed model\'s hand names (CSTLportVector / CVectorBase / CAllocProxy / CAllocator) moved to std::vector / _Vector_base / _STLP_alloc_proxy / allocator, size_type unsigned int; MeasureSegments 0x00AC9D50 binds this spelling'),
 '00ab0fc0': ('respelled', _C + ': std::__uninitialized_copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, ..> explicitly instantiated from the landed 0x00AB3300 STLport text (the __fastcall same-TU helper); was a free Uninit_copy stand-in'),
 '00619f50': ('respelled', _C + ': ??_GCTCInventoryMap@@UAEPAXI@Z emitted with the vtable by a construction helper (the 0x008C37C0 / 0x00BB6520 shape); was the __fastcall CTCInventoryMap_scalar_deleting_destructor stand-in'),
 '00d3aff0': ('respelled', _C + ': ??_GCQ_FireHeartScript@NScript@@UAEPAXI@Z (slot 0 of vtable 0x012CCB90) emitted with the vtable by a construction helper; was the CLandscapeBackgroundPatch `vector_deleting_destructor\' stand-in'),
 '00d34470': ('deferred', 'own row 0x00D34470 defines ??0CQ_FireHeartScript@NScript@@QAE@PAXPAVCGameScriptInterface@@@Z; the PDB signature (CScriptDataBase*, CGameScriptInterfaceBase&) needs the ctor and its CScriptObject base-ctor model re-authored (the base spelling moves too) -- per-row, not driven this wave; label applied'),
 '00606f80': ('already-correct', 'own row (chunk 689) defines ??1CTCInventoryMap@@UAE@XZ; label only'),
 '00d3b010': ('already-correct', 'own row (chunk 689) defines ??1CQ_FireHeartScript@NScript@@UAE@XZ; label only'),
 '00cfa340': ('already-correct', 'own row (chunk 689) defines ??1CQ_AmbushTradersScript@NScript@@UAE@XZ; label only'),
 '00c67c00': ('already-correct', 'own row (chunk 689) defines the final; label only'),
 '00cb0370': ('already-correct', 'own row (chunk 689) defines ?llMulDiv@@YG_J_J000@Z; label only'),
 '00ca02b0': ('already-correct', 'static in libvorbis res0.c (no external symbol): own row (chunk 689) defines it inside the _encodepart TU; label only'),
 '00c04850': ('already-correct', 'static in zlib trees.c: own row (chunk 689) defines it in the trees TU; label only'),
 '00ac9d50': ('already-correct', 'own row (chunk 689) defines the final; label only'),
 '00ab1990': ('already-correct', 'own row not landed; no landed row names a wrong spelling; label only'),
 '00a73e50': ('already-correct', 'own row not landed; no landed row names a wrong spelling; label only'),
})
CLEAR_LEFT |= {'00ad30c0', '00ab0fc0', '00619f50', '00d3aff0'}
for _a in ('00613ae0', '006196d0', '00ad30c0', '00ab0fc0', '00619f50', '00d3aff0'): EDX[_a] = [_a]
CLEAR_LEFT |= {'00ca02b0', '00c04850'}
