# test fix for the fam_auto rows whose test links the row and defines the callee stand-in (HE_OV=<overlay>)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('007a0400', 'tst', 'struct CDefStringTable { CCharString GetString(long id); };\nCCharString CDefStringTable::GetString(long id) {',
     'struct CDefStringTable { CCharString GetString(long id) const; };\nCCharString CDefStringTable::GetString(long id) const {')
