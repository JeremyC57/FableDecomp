# h18_thingmgr_tests.py: naming wave 18, family thingmgr -- the 7 CThing accessor behaviour tests that defined the row's old global
# stand-in g_pThingManager now define the private static ?PCurrentThingManager@CThing@@0PAVCThingManager@@A in their CThing model
# and set it through a test-only static setter (the real class sets it in CThingManager's ctor / dtor).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
ROWS = ['004c7990', '004c79d0', '004c79e0', '004c79f0', '004c7a00', '004c7a10', '004c7a30']
for a in ROWS:
    p, rel, t = he._rw(a, 'tst')
    if 'PCurrentThingManager' in t: print('already', a); continue
    t, n = re.subn(r'(?m)^CThingManager\* g_pThingManager;\n', '', t); assert n == 1, (a, n)
    t, n = re.subn(r'(?m)^class CThing \{\n', 'class CThing {\n    static CThingManager* PCurrentThingManager;      // ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static; the source reads it)\npublic:\n    static void SetPCurrentThingManager(CThingManager* p) { PCurrentThingManager = p; }   // test-only setter\n', t); assert n == 1, (a, n)
    m = re.search(r'class CThing \{.*?\n\};\n', t, re.S); assert m, a
    t = t[:m.end()] + 'CThingManager* CThing::PCurrentThingManager;\n' + t[m.end():]
    t, n = re.subn(r'g_pThingManager = &tm;', 'CThing::SetPCurrentThingManager(&tm);', t); assert n == 1, (a, n)
    assert 'g_pThingManager' not in t, a
    he._w(p, t); print('ok', a)
