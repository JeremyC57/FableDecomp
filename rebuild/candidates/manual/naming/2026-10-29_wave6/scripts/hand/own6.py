# wave 6 own-identity rewrites (whole-file replacements from naming/w6/own/; HE_OV=<overlay>). The files keep their paths
# here; RENAMES.tsv proposes the new names.
#   0x0040BC50 CUserProfileManager::SetSaveSlotFileName(long, const CWideString&)  (was CEngine::GetEditorAnimation)
#   0x0040BC80 CUserProfileManager::Apply()                                       (was free UPM_Prep/UPM_Commit stand-ins)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import path
O = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'own')
FILES = [('0040bc50', 'src', 'src40bc50.cpp'), ('0040bc50', 'tst', 'test40bc50.cpp'),
         ('0040bc80', 'src', 'src40bc80.cpp'), ('0040bc80', 'tst', 'test40bc80.cpp')]
for a, k, f in FILES:
    p, rel = path(a, k)
    new = open(os.path.join(O, f), 'rb').read()
    if open(p, 'rb').read() == new: print('already', a, k); continue
    open(p, 'wb').write(new); print('ok', a, k)
