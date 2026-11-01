# probe (anon family): 0x008E7E10 onto CIsImposterHit; kept separate until it rechecks
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
# 0x008E7E10 (probe, re-authoring): the counted 8-byte entries are CIntelligentPointer<CThing> (CIsImposterHit's argument) and the
# 4-byte "matcher" local is the empty CIsImposterHit functor; Matches(entry) is its const operator().
A = '008e7e10'
edit(A, 'src', 'struct SIconEntry_008e7e10 { long m_a; long m_b; };   // 8 bytes\nstruct SIconMatcher_008e8a10 {                     // a 4-byte functor (never written here)\n    long m_state;\n    bool Matches(const SIconEntry_008e7e10& entry);   // 0x008E8A10\n};\n',
     'class CThing;\ntemplate <class T> class CIntelligentPointer { public:   // 8 bytes (vptr + item)\n    void* m_vtable;\n    T* m_pItem;\n};\n'
     'namespace {\nclass CIsImposterHit { public:                     // Ego_r.pdb empty functor in an anonymous namespace\n'
     '    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x008E8A10\n};\n}\n')
edit_all(A, 'src', 'SIconEntry_008e7e10*', 'CIntelligentPointer<CThing>*')
edit(A, 'src', '    SIconMatcher_008e8a10 matcher;\n', '    CIsImposterHit matcher;\n')
edit(A, 'src', 'matcher.Matches(*it)', 'matcher(*it)')
