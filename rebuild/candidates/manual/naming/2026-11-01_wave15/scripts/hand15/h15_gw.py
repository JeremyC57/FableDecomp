# wave-14 DEFERRED gw 0x00972590: the free __fastcall Cleanup stand-in re-modelled as the Ego_r member
# ?Cleanup@CAIStateGroup_JackOfBladesStage2@@UAEXXZ so the protected const ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ can be
# called (text/00972590.cpp; the self-contained test is unchanged).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import path
p, rel = path('00972590', 'src')
new = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'text', '00972590.cpp')).read()
if open(p).read() == new: print('already 00972590 src')
else: open(p, 'w').write(new); print('ok 00972590 src')
