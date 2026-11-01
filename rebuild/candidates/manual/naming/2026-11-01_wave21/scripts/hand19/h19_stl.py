# h19_stl.py: naming wave 19, family rfail -- the wave 18 [recheck-fail] STLport rows re-run on the real STLport 4.6 _Destroy text:
#   004e34b2 ~vector<pair<long,CCharString> > (the _Destroy / __destroy chain __forceinline'd so the __false_type temporary is the [esp+7] byte,
#   as retail), 004dd782 __destroy_aux<pair<long,CCharString>*> (__fastcall template definition), with self-contained behaviour tests.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
T = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'texts')
def txt(n): return open(os.path.join(T, n), encoding='utf-8', newline='').read()
for a in ('004e34b2', '004dd782'):
    he.put(a, 'src', txt(a + '.cpp'))
    he.put(a, 'tst', txt(a + '_test.cpp'))
