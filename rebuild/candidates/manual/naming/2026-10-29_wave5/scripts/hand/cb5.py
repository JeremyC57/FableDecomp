# wave 5 hand edit (exact single replacements; HE_OV=<overlay>, default ov_hand):
# 0x00A62120: the root CBaseClassNonCopyable model is a struct with public ctor/dtor; the PDB has the protected pair
#   ??0CBaseClassNonCopyable@@IAE@XZ (0x0099A2F0) / ??1CBaseClassNonCopyable@@MAE@XZ (0x0099A300) (DECISIONS.tsv,
#   proven; class key V from pdb_class_keys). Source and test (the test defines the two stubs out of line).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
OLD = ('struct CBaseClassNonCopyable {\n    CBaseClassNonCopyable();                                 // 0x0099A2F0\n'
       '    virtual ~CBaseClassNonCopyable();\n};')
NEW = ('class CBaseClassNonCopyable {\nprotected:\n    CBaseClassNonCopyable();                                 // 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (protected)\n'
       '    virtual ~CBaseClassNonCopyable();                        // 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ (protected)\n};')
for k in ('src', 'tst'):
    edit('00a62120', k, OLD, NEW)
