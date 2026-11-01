# family cds (hand): CDefString 0x00415D70 = ??BCDefString@@QBE?AVCCharString@@XZ (operator CCharString() const) and 0x00415DD0 =
# ?GetString@CDefString@@QBE?AVCCharString@@XZ (owner decision 2026-11-01: two identical bodies, two PDB members -- FableWin
# lists ??BCDefString 0x018C20E0 immediately before ?GetString@CDefString 0x018C2120, the order of retail 0x00415D70 /
# 0x00415DD0; Ego_r has only the conversion operator). Callers that reach 0x00415DD0 (graded relocation pairs) call
# GetString(); those that reach 0x00415D70 use the const conversion. The N_00415dd0 namespaced stand-in struct becomes
# the global class. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
GS = '    CCharString GetString() const;'
# 0x00415DD0: the definer
A = '00415dd0'
edit(A, 'src', '// CDefString::operator CCharString() const -- the def string\'s text: the global def-string table\'s string at TablePos\n// Leaf: ??BCDefString@@QBE?AVCCharString@@XZ',
     '// CDefString::GetString() const -- the def string\'s text: the global def-string table\'s string at TablePos\n// Leaf: ?GetString@CDefString@@QBE?AVCCharString@@XZ')
edit(A, 'src', '// Identity: Ego_r.pdb `??BCDefString@@QBE?AVCCharString@@XZ` (name set; the catalog name is already this operator) and',
     '// Identity (naming wave 13, owner decision 2026-11-01): FableWin.pdb `?GetString@CDefString@@QBE?AVCCharString@@XZ` 0x018C2120,\n'
     '//   listed right after `??BCDefString@@QBE?AVCCharString@@XZ` 0x018C20E0 as retail 0x00415DD0 follows the identical\n'
     '//   operator CCharString() const 0x00415D70 (was catalogued as the operator: one retail address, one name), and')
edit(A, 'src', '    long TablePos;                                 // +0x00\n    operator CCharString() const;\n};\n\nCDefString::operator CCharString() const\n{',
     '    long TablePos;                                 // +0x00\n    CCharString GetString() const;\n};\n\nCCharString CDefString::GetString() const\n{')
# callers of 0x00415DD0 through a member / pointed-to object (graded byte-neutral); the callers that convert a by-value
# CDefString temporary (004b4aa0 004b4c50 00591a66 005c0e81 005c3a95) and 008e7040 are DEFERRED: an explicit GetString()
# call there changes the code (4-23 bytes) and so does an inline operator forwarding to GetString.
for A in ('0071c7c0', '0071c9f0'):
    edit(A, 'src', '    operator CCharString() const;                       // 0x00415DD0',
         GS + '                      // 0x00415DD0 (FableWin ?GetString@CDefString@@QBE?AVCCharString@@XZ)')
    edit_all(A, 'src', 'const CCharString& helper = entry->HelperName;', 'const CCharString& helper = entry->HelperName.GetString();', 2)
# 0x00698A30 dereferences a const CDefString*: the conversion it was written with stays, as an inline operator forwarding to
# the out-of-line GetString (an explicit criteria->GetString() call changes 4 bytes; this form grades byte-exact)
edit('00698a30', 'src', '    operator CCharString() const;                           // 0x00415DD0',
     GS + '                          // 0x00415DD0 (FableWin ?GetString@CDefString@@QBE?AVCCharString@@XZ)\n'
     '    operator CCharString() const { return GetString(); }    // inline here (its out-of-line copy is 0x00415D70)')
# 0x00EC8230: reaches 0x00415D70 only
edit('00ec8230', 'src', '    operator CCharString();                             // 0x00415D70 (non-const twin; the landed 0x008E7040 binds it so)\n    operator CCharString() const;                       // 0x00415DD0 (const; the landed 0x0071C7C0 binds it so)',
     '    operator CCharString() const;                       // 0x00415D70 (Ego_r / FableWin ??BCDefString@@QBE?AVCCharString@@XZ; naming wave 13)\n'
     '    CCharString GetString() const;                      // 0x00415DD0 (FableWin ?GetString@CDefString@@QBE?AVCCharString@@XZ)')
# the definer's test calls the member by its new name
edit('00415dd0', 'tst', '// Test for 0x00415DD0 CDefString::operator CCharString() const:', '// Test for 0x00415DD0 CDefString::GetString() const (was catalogued as operator CCharString):')
edit('00415dd0', 'tst', '    long TablePos;\n    operator CCharString() const;\n};', '    long TablePos;\n    CCharString GetString() const;\n};')
edit('00415dd0', 'tst', '    CCharString s = d;', '    CCharString s = d.GetString();')
edit('00415dd0', 'tst', '    CCharString t = (CCharString)d;', '    CCharString t = d.GetString();')
