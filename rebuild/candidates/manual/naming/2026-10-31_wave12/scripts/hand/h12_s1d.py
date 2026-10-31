# sub-wave 1d (hand): leftovers of the scripted families. HE_OV=<overlay>
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
# ---- 0x008FD010 ?IsStopping@CAIStateGroupBase@@IBE_NXZ: declarations followed by their own `protected:` label
for a in ('0094a840', '00956460', '009621f0', '00969470'):
    resub(a, 'src', r'\n(\s*)bool IsStopping\(\);(\s*)protected: // 0x008FD010', r'\nprotected:\n\1bool IsStopping() const;\2// 0x008FD010 (Ego_r.pdb protected const)', 1)
