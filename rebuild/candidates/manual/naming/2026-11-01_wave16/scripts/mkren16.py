# mkren16.py: RENAMES.tsv -- rows whose file name / Module carries the old (wrong) function or class name after wave 16.
# File-name rule (build_candidates.ps1 convention): <Module stripped to [A-Za-z0-9], first 80 chars>_<function stripped>_<addr>.cpp
# (the test adds _test); ??_G rows use the function token `scalardeletingdestructor`; a free function's Module is _global (prefix global).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import und
from rows import rows
R = rows(); W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
A = [l.strip() for l in open(os.path.join(W, 'runs/final.rows.txt')) if l.strip()]
strip = lambda s: re.sub(r'[^A-Za-z0-9]', '', s)
def fname(mod, fn, a):
    m = 'global' if mod == '_global' else strip(mod)[:80]
    return '%s_%s_%s' % (m, strip(fn), a)
NEW = {}   # addr -> (new Module, function token, old function, new function, evidence)
for a in A:
    p = os.path.join(W, 'ov_final/rebuild/src/compiled', R[a]['Source'])
    t = open(p, errors='replace').read()
    m = re.search(r'^// Leaf: (\?\?_G\S+)', t, re.M)
    if not m: continue
    d = und.get([m.group(1)])[m.group(1)]
    c = re.search(r"([\w:]+)::`scalar deleting dtor'", d)
    if c: NEW[a] = (c.group(1), 'scalardeletingdestructor', '', '%s::`scalar deleting destructor\'' % c.group(1), 'naming wave 16 gdt: the ??_G stand-in re-authored as %s' % m.group(1))
L = '_List_base'
MAN = {
 '004cd9b0': ('CConversationAttitude', 'GetAnimName', 'GetAnimGroupName', 'CConversationAttitude::GetAnimName', 'naming wave 16 data: reads ConversationAttitudeToAnimName 0x013B8A38 (catalog GetAnimGroupName is wrong)'),
 '00510d40': ('std::_List_base<CIntelligentPointer<CThingAICreature> >', 'clear', 'clear_CRetailList_00510d40', 'std::_List_base<CIntelligentPointer<CThingAICreature> >::clear', 'naming wave 16 stl'),
 '00a53300': ('std::_List_base<CIntelligentPointer<C3DAnimation2> >', 'clear', 'clear_CRetailList_00a53300', 'std::_List_base<CIntelligentPointer<C3DAnimation2> >::clear', 'naming wave 16 stl'),
 '00a3ff60': ('std::_List_base<CSoundObjectLH*>', '_List_base', 'SortTreeRecursively', 'std::_List_base<CSoundObjectLH*>::~_List_base', 'naming wave 16 stl'),
 '00a40bd0': ('std::vector<std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*> >', 'vector', 'CRetailVector_SPairCMember_0099eae012', 'std::vector<std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*> >::~vector', 'naming wave 16 stl'),
 '009c2f50': ('std::vector<CQuickDrawTriInfo>', 'vector', 'CRetailVector_SOcclusionEntry_009c2f50', 'std::vector<CQuickDrawTriInfo>::~vector', 'naming wave 16 stl'),
 '009aa780': ('std::_Rb_tree<CCharString,std::pair<const CCharString,NBankFileManager::CContainedBank>,std::_Select1st<std::pair<const CCharString,NBankFileManager::CContainedBank> >,std::less<CCharString>,std::allocator<std::pair<const CCharString,NBankFileManager::CContainedBank> > >', '_M_erase', '_M_erase', 'std::_Rb_tree<...map<CCharString,NBankFileManager::CContainedBank>...>::_M_erase', 'naming wave 16 stl: placeholders replaced by the proven types'),
 '006925b0': ('CCreatureActionBase', 'GetTotalTimeWorldFrames', 'GetTotalTime', 'CCreatureActionBase::GetTotalTimeWorldFrames', 'naming wave 16 own'),
 '006925c0': ('CCreatureActionBase', 'GetFramesRunning', 'DrawGetEnvironment', 'CCreatureActionBase::GetFramesRunning', 'naming wave 16 own'),
 '00692910': ('CCreatureActionBase', 'PeekDefinitionManager', 'Destval', 'CCreatureActionBase::PeekDefinitionManager', 'naming wave 16 own'),
 '008fcfc0': ('CAIStateGroupBase', 'GetWorld', 'GetPWorld', 'CAIStateGroupBase::GetWorld', 'naming wave 16 own'),
 '0099a330': ('CInitBaseClass', 'IsValid', 'GetCanItemBeQuickAssigned', 'CInitBaseClass::IsValid', 'naming wave 16 own'),
 '00ac1ee0': ('CIndexBuffer', 'Uninitialise', 'CVolume::ReleaseVolume', 'CIndexBuffer::Uninitialise', 'naming wave 16 own'),
 '00a0b6c0': ('CSoundBankLH', 'Unload_00a0b6c0', 'CEnginePrimitiveRenderer::ClearAllChildPrimitives', 'CSoundBankLH::Unload_00a0b6c0', 'naming wave 16 own (descriptive per c670)'),
 '00b59840': ('CEngineLocalDetailGenerator', 'DeleteMap', 'CEngineResourceManager::DestroyMapStrip_00b59840', 'CEngineLocalDetailGenerator::DeleteMap', 'naming wave 16 own'),
 '00cbfab8': ('_global', 'SetScriptActiveStatus', 'SetMiniMapRegionExitTextOffsetX', 'SetScriptActiveStatus', 'naming wave 16 own'),
 '00e8b700': ('NScript::CV_FisticuffsClubScript', 'Alloc', 'Create_CClass_00e81f60', 'NScript::CV_FisticuffsClubScript::Alloc', 'naming wave 16 qctor'),
 '00ee6c00': ('NScript::CV_TourGuideScript', 'Alloc', 'Create_CClass_00ee41f0', 'NScript::CV_TourGuideScript::Alloc', 'naming wave 16 qctor'),
 '00ed4aa0': ('NScript::CV_SwordInTheStoneScript', 'CV_SwordInTheStoneScript', 'CV_SwordInTheStoneScript', 'NScript::CV_SwordInTheStoneScript::CV_SwordInTheStoneScript', 'naming wave 16 qctor: class moved into namespace NScript'),
 '01228ed8': ('_global', 'E_CrimeText', 'CDebugManager::ThrowException', "`dynamic atexit destructor for 'CrimeText''", 'naming wave 16 data: the CrimeText TU _$E2 (sibling 0x0121A7CA global_ECrimeText)'),
 '00759a00': ('CTCPhysicsRigidBody', 'SetRHSet', 'vfunc_75', 'CTCPhysicsRigidBody::SetRHSet', 'wave 15 leftover: respelled in wave 15 (slot 75 = Ego_r SetRHSet); file name still carries vfunc75'),
 '007c75e0': ('CTCTurncoatSpell', 'OnRemove', 'FinalCleanup', 'CTCTurncoatSpell::OnRemove', 'wave 15 leftover: respelled in wave 15 (slot 2 = Ego_r OnRemove); file name still carries FinalCleanup'),
 '007f7700': ('CTCActivationReceptorBase', 'Deactivate', 'CXMVPlaybackInfo::CXMVPlaybackInfo', 'CTCActivationReceptorBase::Deactivate', 'wave 15 leftover: respelled in wave 15 (Leaf ?Deactivate@CTCActivationReceptorBase@@UAEXXZ); file name still carries CXMVPlaybackInfo'),
}
NEW.update(MAN)
out = [('address', 'old_name', 'new_name', 'old_module', 'new_module', 'old_source', 'new_source', 'old_test', 'new_test', 'evidence')]
for a in sorted(NEW):
    mod, tok, oldf, newf, ev = NEW[a]
    src, tst = R[a]['Source'], R[a].get('TestSource', '')
    d = os.path.dirname(src)
    ns = d + '/' + fname(mod, tok, a) + '.cpp'
    nt = (os.path.dirname(tst) + '/' + fname(mod, tok, a) + '_test.cpp') if tst else ''
    if ns == src and mod == R[a]['Module']: continue
    oldn = os.path.basename(src)[:-4]
    out.append((a, oldn, newf, R[a]['Module'], mod, src, ns, tst, nt if tst else '', ev))
open(os.path.join(W, 'RENAMES.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'renames')
