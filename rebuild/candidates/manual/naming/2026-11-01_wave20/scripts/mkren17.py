# mkren17.py: RENAMES.tsv -- rows whose file name / Module carries the old (wrong) function or class name after wave 17.
# File-name rule (build_candidates.ps1 convention): <Module stripped to [A-Za-z0-9], first 80 chars>_<function stripped>_<addr>.cpp
# (the test adds _test); ??_G rows use the function token `scalardeletingdestructor`; a free function's Module is _global (prefix
# global). Paths are repo-rooted (rebuild/src/compiled/..., rebuild/tests/...); new_name is the catalog's bare spelling.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows
R = rows(); W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
A = set(l.strip() for l in open(os.path.join(W, 'runs/final.rows.txt')) if l.strip())
strip = lambda s: re.sub(r'[^A-Za-z0-9]', '', s)
def fname(mod, fn, a):
    m = 'global' if mod == '_global' else ('staticinit' if mod == '_static_init' else strip(mod)[:80])
    return '%s_%s_%s' % (m, strip(fn), a)
OPD = 'std::pair<const long,COpinionDeed*>'
T_OPD = 'std::_Rb_tree<long,%s,std::_Select1st<%s >,std::less<long>,std::allocator<%s > >' % (OPD, OPD, OPD)
M_OPD = 'std::map<long,COpinionDeed*>'
CB = 'std::pair<const CCharString,NBankFileManager::CContainedBank>'
T_CB = 'std::_Rb_tree<CCharString,%s,std::_Select1st<%s >,std::less<CCharString>,std::allocator<%s > >' % (CB, CB, CB)
M_CB = 'std::map<CCharString,NBankFileManager::CContainedBank>'
T_UL = 'std::_Rb_tree<unsigned long,unsigned long,std::_Identity<unsigned long>,std::less<unsigned long>,std::allocator<unsigned long> >'
GA = 'std::pair<const EGameAction,std::pair<C3DVector,long> >'
T_GA = 'std::_Rb_tree<EGameAction,%s,std::_Select1st<%s >,std::less<EGameAction>,std::allocator<%s > >' % (GA, GA, GA)
SE = 'NScreenEffect::CScreenEffectPriorityStack::CStackEntry*'
T_SE = 'std::_Rb_tree<%s,%s,std::_Identity<%s>,std::less<%s>,std::allocator<%s> >' % (SE, SE, SE, SE, SE)
EVQ = 'naming wave 17 (%s): %s'
MAN = {   # addr -> (new Module, function token, new_name (catalog bare spelling), evidence)
 '00dbefa0': ('NScript::CQ_NewOakValeIntroScript', 'scalardeletingdestructor', "NScript::CQ_NewOakValeIntroScript::`scalar deleting destructor'", EVQ % ('gdt', 'the ??_G stand-in re-authored as ??_GCQ_NewOakValeIntroScript@NScript@@UAEPAXI@Z')),
 '00d782c0': ('NScript::CQ_HangingTreeGoodScript', 'scalardeletingdestructor', "NScript::CQ_HangingTreeGoodScript::`scalar deleting destructor'", EVQ % ('gdt', 'the ??_G stand-in re-authored as ??_GCQ_HangingTreeGoodScript@NScript@@UAEPAXI@Z (c677)')),
 '00d25410': ('NScript::CQ_DragonBossFightScript', 'CQ_DragonBossFightScript', 'NScript::CQ_DragonBossFightScript::CQ_DragonBossFightScript', EVQ % ('qctor', 'c676: the ctor of CQ_DragonBossFightScript (Alloc 0x00D27240, S_QDBF), not CQ_NewOakValeIntroScript (0x00DAAC00)')),
 '006c4f80': (T_OPD, '_M_erase', T_OPD + '::_M_erase', EVQ % ('stl', 'placeholders replaced by map<long,COpinionDeed*> (c676)')),
 '006c75a0': (T_OPD, 'clear', T_OPD + '::clear', EVQ % ('stl', 'placeholders replaced by map<long,COpinionDeed*> (c676)')),
 '006c7d30': (T_OPD, '_Rb_tree', T_OPD + '::_Rb_tree', EVQ % ('stl', 'placeholders replaced by map<long,COpinionDeed*> (c676)')),
 '006c7d70': (T_OPD, '_Rb_tree', T_OPD + '::~_Rb_tree', EVQ % ('stl', 'placeholders replaced by map<long,COpinionDeed*> (c676)')),
 '006c7de0': (T_OPD, 'erase', T_OPD + '::erase', EVQ % ('stl', 'placeholders replaced by map<long,COpinionDeed*> (c676)')),
 '006c8230': (M_OPD, 'map', M_OPD + '::map', EVQ % ('stl', 'the CUnprovenContainer_006c4f80 holder is the STLport std::map<long,COpinionDeed*> (c676)')),
 '006c8270': (M_OPD, 'map', M_OPD + '::~map', EVQ % ('stl', 'the CUnprovenContainer_006c4f80 holder is the STLport std::map<long,COpinionDeed*> (c676; implicit ~map emitted by the Use_/Kill_ emitters)')),
 '009aaca0': (T_CB, 'clear', T_CB + '::clear', EVQ % ('stl', 'wave 16 DEFERRED 009aa780 sibling: placeholders replaced by map<CCharString,NBankFileManager::CContainedBank> (c667)')),
 '009ab6b0': (T_CB, '_Rb_tree', T_CB + '::~_Rb_tree', EVQ % ('stl', 'wave 16 DEFERRED 009aa780 sibling (c667)')),
 '009ac0f0': (M_CB, 'map', M_CB + '::map', EVQ % ('stl', 'wave 16 DEFERRED 009aa780 sibling: CUnprovenContainer_009aa780 is std::map<CCharString,NBankFileManager::CContainedBank> (c667)')),
 '009ac130': (M_CB, 'map', M_CB + '::~map', EVQ % ('stl', 'wave 16 DEFERRED 009aa780 sibling (c667; implicit ~map emitted by the Use_/Kill_ emitters)')),
 '004cf810': (T_UL, '_M_erase', T_UL + '::_M_erase', EVQ % ('stl', 'the CTreeOwner stand-in re-authored on STLport set<unsigned long>::_M_erase (c675)')),
 '00ad2790': (T_UL, 'find', T_UL + '::find', EVQ % ('stl', 'placeholders replaced by set<unsigned long>; the const find (CIndexList::HasIndex is const; c677)')),
 '00527910': (T_GA, '_M_erase', T_GA + '::_M_erase', EVQ % ('stl', 'the CTreeOwner stand-in re-authored on STLport map<EGameAction,pair<C3DVector,long> >::_M_erase (c676)')),
 '00882bc0': (T_SE, '_M_erase', T_SE + '::_M_erase', EVQ % ('stl', 'the CTreeOwner stand-in re-authored on STLport set<CStackEntry*>::_M_erase (c677)')),
 '004427d0': ('std::deque<CSubtitleRenderer::CSetTextInfo>', 'deque', 'std::deque<CSubtitleRenderer::CSetTextInfo>::~deque', EVQ % ('stl', 'placeholder element replaced by CSubtitleRenderer::CSetTextInfo (c675)')),
 '00772110': ('CTCShop', 'GetNumShoppers', 'CTCShop::GetNumShoppers', EVQ % ('own', 'c675: CTCShop::GetNumShoppers const, not CShaderPreParser::GetConstantCount')),
 '0049cf80': ('CRightHandedSet', 'CRightHandedSet', 'CRightHandedSet::CRightHandedSet', EVQ % ('own', 'c675: the CRightHandedSet copy constructor, not CNearestBuildingWithProperty\'s')),
 '0121ac30': ('_global', 'E_FORTY_FIVE_DEGREES_ORIENTATION', "`anonymous namespace'::`dynamic initializer for 'FORTY_FIVE_DEGREES_ORIENTATION''", EVQ % ('data', 'c675: the anonymous-namespace CTCInventoryItem constant, was g_InventoryOrientationRHSet1')),
 '0121acd0': ('_global', 'E_UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION', "`anonymous namespace'::`dynamic initializer for 'UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION''", EVQ % ('data', 'c675: was g_InventoryOrientationRHSet5')),
 '0121ad20': ('_global', 'E_UPSIDE_DOWN_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION', "`anonymous namespace'::`dynamic initializer for 'UPSIDE_DOWN_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION''", EVQ % ('data', 'c675: was g_InventoryOrientationRHSet6')),
 '0121ad50': ('_global', 'E_UPSIDE_DOWN_LEFT_FORTY_FIVE_DEGREES_ORIENTATION', "`anonymous namespace'::`dynamic initializer for 'UPSIDE_DOWN_LEFT_FORTY_FIVE_DEGREES_ORIENTATION''", EVQ % ('data', 'c675: was g_InventoryOrientationRHSet7')),
 '01224bc0': ('_static_init', 'g_AudioContext', "`dynamic initializer for 'g_AudioContext''", EVQ % ('data', 'c675: the audio context object @ 0x0143BDB0 unified on g_AudioContext (was g_CStatic_00c55cd0_0143bdb0)')),
 '00881370': ('NScreenEffect::CScreenEffectPriorityStack', 'CScreenEffectPriorityStack', 'NScreenEffect::CScreenEffectPriorityStack::CScreenEffectPriorityStack', EVQ % ('c677', 'the ctor moved into NScreenEffect (file name carried the global spelling)')),
 '00cfa2f0': ('NScript::CQ_AmbushTradersScript', 'Alloc', 'NScript::CQ_AmbushTradersScript::Alloc', EVQ % ('c677', 'the Create_CClass stand-in respelled as ?Alloc@CQ_AmbushTradersScript@NScript@@SI...')),
}
G681 = {'00e7f1f0': 'NScript::CV_FishingCompetitionScript::CFishCompOrganiser', '00e24c60': 'NScript::CQR_EscortTraderScript',
        '00e7b2c0': 'NScript::CV_ExposeMayorScript', '00da2f70': 'NScript::CQ_MinionCampScript', '00cf91f0': 'NScript::CLadyGreyWifeManagerScript',
        '00debc90': 'NScript::CQ_RansomVictimScript', '00e46a30': 'NScript::CV_BordelloScript'}
for _a, _c in G681.items():
    MAN[_a] = (_c, 'scalardeletingdestructor', _c + "::`scalar deleting destructor'", EVQ % ('gdt', 'c681: the CLandscapeBackgroundPatch vector-deleting stand-in re-authored as the compiler-emitted ??_G of ' + _c))
for _a, _c in (('00b9c6d0', 'CEnginePrimitiveManagerRepeatedStaticMeshes'), ('00b2f6e0', 'CEngineSurfaceCompositionManager')):
    MAN[_a] = (_c, 'scalardeletingdestructor', _c + "::`scalar deleting destructor'", EVQ % ('gdt', 'c681 ??_G re-authored'))
out = [('address',
 'old_name', 'new_name', 'old_module', 'new_module', 'old_source', 'new_source', 'old_test', 'new_test', 'evidence')]
for a in sorted(MAN):
    if a not in A: continue
    mod, tok, newf, ev = MAN[a]
    src, tst = R[a]['Source'], R[a].get('TestSource', '')
    ns = os.path.dirname(src) + '/' + fname(mod, tok, a) + '.cpp'
    nt = (os.path.dirname(tst) + '/' + fname(mod, tok, a) + '_test.cpp') if tst else ''
    if ns == src and mod == R[a]['Module']: continue
    out.append((a, os.path.basename(src)[:-4], newf, R[a]['Module'], mod, 'rebuild/src/compiled/' + src, 'rebuild/src/compiled/' + ns,
                ('rebuild/tests/' + tst) if tst else '', ('rebuild/tests/' + nt) if tst else '', ev))
open(os.path.join(W, 'RENAMES.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'renames')
