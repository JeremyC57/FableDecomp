# tests that link the edited rows and define their stand-in callee: respelled with the row (HE_OV=<overlay>)
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import _rw, _w
# ---- 0x00F35B30: the Sunnyvale entity rows whose free `__fastcall helper(script*)` became CEntityScriptBase::IsActiveThreadTerminating
ISAT = {'00ce0350': 'helper_f35b30', '00d91540': 'SunnyvaleMasterHelper', '00dc44e0': 'helper_f35b30', '00dc86c0': 'SunnyvaleMasterHelper',
        '00e191a0': 'SunnyvaleMasterHelper', '00e1ba20': 'CQ_SunnyvaleMasterScript_Advance', '00da5870': 'helper_f35b30',
        '00dc84a0': 'Q_Sunnyvale_helper_f35b30', '00dec570': 'CQ_SunnyvaleMasterScript_helper', '00e192a0': 'SunnyvaleHelper_f35b30'}
CLS = 'class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (the row calls it so)\n'
for a, n in ISAT.items():
    p, rel, t = _rw(a, 'tst')
    if 'CEntityScriptBase::IsActiveThreadTerminating' in t: print('already', a, 'tst'); continue
    dec = r'(?:extern "C"\s+)?(?:bool|char)\s+__fastcall\s+' + n + r'\s*\(\s*CQ_SunnyvaleMasterScript\s*\*\s*(?:/\*\s*self\s*\*/|self)?\s*\)'
    t2, nd = re.subn(dec + r'\s*;[^\n]*\n', '', t)
    m = re.search(dec + r'\s*\{', t2)
    if not m: raise SystemExit('no definition in ' + rel)
    uses_self = 'self' in m.group(0)
    body_start = m.end()
    hdr = 'bool CEntityScriptBase::IsActiveThreadTerminating() const {' + ('\n    CQ_SunnyvaleMasterScript* self = (CQ_SunnyvaleMasterScript*)this;' if uses_self else '')
    t2 = t2[:m.start()] + hdr + t2[body_start:]
    # the class declaration goes before the first place the name was declared or defined
    first = min(x for x in (re.search(dec, t).start(),) if x is not None)
    ls = t2.rfind('\n', 0, min(first, len(t2))) + 1
    # place it just after the CQ_SunnyvaleMasterScript type (or at the top after includes)
    tm = re.search(r'\n(?:struct|class)\s+CQ_SunnyvaleMasterScript\b[^;]*?\n\};\n', t2, re.S)
    at = tm.end() if tm and tm.end() < t2.index(hdr) else ls
    t2 = t2[:at] + CLS + t2[at:]
    _w(p, t2); print('ok', a, 'tst', nd)
