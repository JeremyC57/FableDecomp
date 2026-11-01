# family vcs (hand, after fam_vcs.py): 0x00767200 transfers its std::vector<CCharString> through 0x0049B8D0, which is
# Ego_r/FableWin ??$TransferVector@VCCharString@@@CPersistContext@@QAEXPBDAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
# (already bound by 0x008ED2B0, whose row calls context.TransferVector): the row's one-argument Transfer<T> member template
# becomes TransferVector<T>(const char*, std::vector<T>&) and its two calls follow. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
A = '00767200'
edit(A, 'src', '    template <class T> void Transfer(const char* name, T& value);   // CPersistContext::Transfer<T> (member template, PDB)',
     '    template <class T> void TransferVector(const char* name, std::vector<T>& value);   // <CCharString> 0x0049B8D0 (Ego_r/FableWin CPersistContext::TransferVector)')
edit_all(A, 'src', 'context.Transfer("AugmentationDefNames", names);', 'context.TransferVector("AugmentationDefNames", names);', 2)
