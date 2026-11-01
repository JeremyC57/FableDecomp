# h20_erase.py: naming wave 20, family erase -- the STLport _Rb_tree::_M_erase instantiations the chunk 686 rows prove, re-authored on the
# landed STLport 4.6 _tree.h text (the 0x00834FD0 model) with the proven types, byte-identical only:
#   00ad14e0 ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKM@std@@U?$_Select1st@U?$pair@$$CBKM@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKM@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBKM@std@@@2@@Z
#            -- the map<unsigned long, float> Groups tree of QuadricOptimiserInternals::COptimisedVertex (chunk 686: RemapGroups 0x00AC97D0 clears
#            it; ~COptimisedVertex 0x00AD7400 erases it at +0x9c); was the descriptive CTreeOwner_00ad14e0::FreeTree;
#   0043020a ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@std@@@2@@Z
#            -- the map<CCharString, long> of _M_find 0x0042B467 / insert_unique 0x00431606 (c686 queue: the helper-count map of InitFirePoints;
#            the body destroys the key through ~CCharString 0x0099EAE0 before free); was the propagated LTextTreeWalkThrough::BuildTreeArray;
#   00ac97d0 (chunk 686, name-only): _M_erase is private in STLport 4.6 _tree.h (AAE; 347 landed rows), not protected (IAE).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_erase.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
TPL = open('/home/user/FableDecomp/rebuild/src/compiled/00/83/stdRbtreeCUnprovenKey00834fd0CUnprovenValue00834fd0CUnprovenKeyOfValue00834fd0st_Merase_00834fd0.cpp', encoding='utf-8', errors='surrogateescape', newline='').read()
i = TPL.index('extern "C" void* __cdecl malloc'); j = TPL.index('struct CUnprovenKey_00834fd0'); k = TPL.index('namespace std {')
BODY = TPL[k:TPL.index('template void _Rb_tree<CUnprovenKey_00834fd0')]
PRE = TPL[i:j]
KM = '''// std::_Rb_tree<unsigned long, std::pair<const unsigned long, float>, std::_Select1st<std::pair<const unsigned long, float> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, float> > >::_M_erase(std::_Rb_tree_node<std::pair<const unsigned long, float> >* x) -- STLport erase without rebalancing: recurse right, free each node, continue left
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD14E0, 51 bytes.
// Types (naming wave 20, chunk 686): the map<unsigned long, float> Groups tree of QuadricOptimiserInternals::COptimisedVertex -- the landed
//   COptimisedVertex::RemapGroups 0x00AC97D0 (chunk 686, Ego_r.pdb) clears it through this instantiation (`Groups = newGroups`, the local's
//   destructor) and the landed ~COptimisedVertex 0x00AD7400 erases it at +0x9c (Ego_r.pdb Groups: map<unsigned long, float>); the value is
//   trivially destroyed, the node (0x18) freed. Was the descriptive CTreeOwner_00ad14e0::FreeTree (one of 23 bodies of this shape).
// Model: STLport 4.6 stl/_tree.h + stl/_tree.c text in namespace std (speed TU); explicit instantiation emits the member as
//   its own COMDAT, as retail kept every used inline member (no /OPT:REF).
// Leaf: ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKM@std@@U?$_Select1st@U?$pair@$$CBKM@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKM@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBKM@std@@@2@@Z
''' + PRE + BODY + '''typedef pair<const unsigned long, float> _Val;
template void _Rb_tree<unsigned long, _Val, _Select1st<_Val>, less<unsigned long>, allocator<_Val> >::_M_erase(_Rb_tree_node<_Val>*);
}
'''
CS = '''// std::_Rb_tree<CCharString, std::pair<const CCharString, long>, std::_Select1st<std::pair<const CCharString, long> >, std::less<CCharString>, std::allocator<std::pair<const CCharString, long> > >::_M_erase(std::_Rb_tree_node<std::pair<const CCharString, long> >* x) -- STLport erase without rebalancing: recurse right, destroy the key string, free each node, continue left
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043020A, 53 bytes.
// Types (naming wave 20, chunk 686): the map<CCharString, long> of this TU (its _M_find 0x0042B467 and insert_unique 0x00431606; chunk 686:
//   the helper-count map of CTCFlammable::InitFirePoints 0x007808B0): the key is destroyed through CCharString::~CCharString 0x0099EAE0
//   (`lea ecx,[esi+10h]`), the node (0x18) freed. Was the propagated LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray.
// Model: STLport 4.6 stl/_tree.h + stl/_tree.c text in namespace std (speed TU); explicit instantiation emits the member as
//   its own COMDAT, as retail kept every used inline member (no /OPT:REF).
// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@std@@@2@@Z
// (the Leaf is the compiler's spelling of the STLport text -- its back-references, not the hand-mangled @2@ form of the queue row)
''' + PRE + '''class CCharString { public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
''' + BODY + '''typedef pair<const CCharString, long> _Val;
template void _Rb_tree<CCharString, _Val, _Select1st<_Val>, less<CCharString>, allocator<_Val> >::_M_erase(_Rb_tree_node<_Val>*);
}
'''
for a, txt in (('00ad14e0', KM), ('0043020a', CS)):
    if a in _X: continue
    he.put(a, 'src', txt)
a = '00ac97d0'
if a not in _X:
    he.edit(a, 'src', '    void _M_erase(_Link_type __x);                                                   // 0x00AD14E0\nprivate:\n',
            'private:\n    void _M_erase(_Link_type __x);                                                   // 0x00AD14E0 (private in STLport 4.6 _tree.h: AAE, as the 347 landed _M_erase rows; naming wave 20)\n')
