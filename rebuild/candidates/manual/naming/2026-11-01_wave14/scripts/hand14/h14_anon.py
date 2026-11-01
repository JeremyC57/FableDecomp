# family anon (hand), part 2: 0x008E8A10 is Ego_r ??RCIsImposterHit@?A0x4c72bf35@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z
# (an empty functor in an anonymous namespace; Ego_r layout CIsImposterHit 1 byte, unary_negate<CIsImposterHit> _Functor +0).
# The std::count_if rows 0x008E8A90 / 0x008E8B80 named it CThingFilter_IsNotInMode at global scope: the class takes the PDB
# name inside `namespace {` (same members, same empty layout; the count_if instantiation symbols follow). HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
OLD = ('class CThingFilter_IsNotInMode : public std::unary_function<CIntelligentPointer<CThing>, bool> {\npublic:\n'
       '    CThingFilter_IsNotInMode() {}\n    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x008E8A10\n};\n')
NEW = ('namespace {\nclass CIsImposterHit : public std::unary_function<CIntelligentPointer<CThing>, bool> {   // Ego_r.pdb anonymous namespace\npublic:\n'
       '    CIsImposterHit() {}\n    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x008E8A10\n};\n}\n')
for a in ('008e8a90', '008e8b80'):
    edit(a, 'src', OLD, NEW)
    edit_all(a, 'src', 'CThingFilter_IsNotInMode', 'CIsImposterHit')
# 0x0097ADF0: re-authored as the STLport instantiation std::__push_heap<CThing**, int, CThing*, `anonymous namespace'::
# CThingCompare_InDirectionOfAndNearestToHeuristicBitch> its caller 0x0097AF70 binds (reloc_bindings ??$__push_heap@...@?A0x...)
# -- the row was a global `_Push_heap` stand-in; same body, the comparator class in the anonymous namespace with the PDB
# operator()(const CThing*, const CThing*) const (fam_cmp makes the parameters const first).
A = '0097adf0'
edit(A, 'src', '// _global::_Push_heap(CThing**, long, long, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch) -- heap sift-up',
     "// std::__push_heap(CThing** first, int hole, int top, CThing* value, `anonymous namespace'::CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- heap sift-up")
edit(A, 'src', '// Leaf: _Push_heap\n', '// Leaf: ??$__push_heap@PAPAVCThing@@HPAV1@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@\n')
edit(A, 'src', '// Shape: SGI/STLport `__push_heap`; the drop is tail-duplicated into both exits (as 0x005140A0).\n',
     '// Shape: SGI/STLport `__push_heap`; the drop is tail-duplicated into both exits (as 0x005140A0).\n'
     '// Identity: the caller std::__adjust_heap 0x0097AF70 binds this address as ??$__push_heap@PAPAVCThing@@HPAV1@V'
     'CThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x...@@@std@@ (naming wave 14; the global `_Push_heap` was a stand-in).\n')
edit(A, 'src', 'struct CThingCompare_InDirectionOfAndNearestToHeuristicBitch {\n',
     'namespace {\nclass CThingCompare_InDirectionOfAndNearestToHeuristicBitch {   // Ego_r.pdb 16 bytes, anonymous namespace (?A0xeaff0e08)\npublic:\n')
edit(A, 'src', '    bool operator()(const CThing* a, const CThing* b) const;   // 0x0097ABF0\n};\n\nvoid __fastcall _Push_heap(CThing** first, long hole, long top, CThing* value, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp)\n{\n    long parent',
     '    bool operator()(const CThing* a, const CThing* b) const;   // 0x0097ABF0\n};\n}\n\nnamespace std {\n'
     'template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>\n'
     'void __fastcall __push_heap(_RandomAccessIterator first, _Distance hole, _Distance top, _Tp value, _Compare comp)\n{\n    _Distance parent')
edit(A, 'src', '    first[hole] = value;\n}', '    first[hole] = value;\n}\n'
     'template void __fastcall __push_heap<CThing**, int, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch>(CThing**, int, int, CThing*,\n'
     '    CThingCompare_InDirectionOfAndNearestToHeuristicBitch);\n}')
# 0x008E7E10 (re-authoring; byte-exact in the probe): the counted 8-byte entries are CIntelligentPointer<CThing> (CIsImposterHit's argument) and the
# 4-byte "matcher" local is the empty CIsImposterHit functor; Matches(entry) is its const operator().
A = '008e7e10'
edit(A, 'src', 'struct SIconEntry_008e7e10 { long m_a; long m_b; };   // 8 bytes\nstruct SIconMatcher_008e8a10 {                     // a 4-byte functor (never written here)\n    long m_state;\n    bool Matches(const SIconEntry_008e7e10& entry);   // 0x008E8A10\n};\n',
     'class CThing;\ntemplate <class T> class CIntelligentPointer { public:   // 8 bytes (vptr + item)\n    void* m_vtable;\n    T* m_pItem;\n};\n'
     'namespace {\nclass CIsImposterHit { public:                     // Ego_r.pdb empty functor in an anonymous namespace\n'
     '    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x008E8A10\n};\n}\n')
edit_all(A, 'src', 'SIconEntry_008e7e10*', 'CIntelligentPointer<CThing>*')
edit(A, 'src', '    SIconMatcher_008e8a10 matcher;\n', '    CIsImposterHit matcher;\n')
edit(A, 'src', 'matcher.Matches(*it)', 'matcher(*it)')
