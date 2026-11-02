# fam_isv.py <ov> <rows.txt>: 0x00CD23B9 IsScripting -> CScriptGameResourceObjectScriptedThingBase::IsValid() const
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F
FIN = '?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ'
def tr(t, a):
    if a == '00cd23b9': return None, 'the defining row (its comment records the old invented name)'
    if not re.search(r'\bIsScripting\b', t): return None, 'no IsScripting'
    # declarations: bool IsScripting(); / bool IsScripting() const; / bool IsScripting(void);
    t = re.sub(r'\bbool(\s+)IsScripting\s*\(\s*(?:void)?\s*\)\s*(?:const\s*)?;', r'bool\1IsValid() const;', t)
    t = re.sub(r'\bbool(\s+)IsScripting\s*\(\s*(?:void)?\s*\)\s*(?:const\s*)?\{', r'bool\1IsValid() const {', t)
    t = re.sub(r'\bIsScripting\b', 'IsValid', t)
    return t, 'IsScripting->IsValid'
def ck(S0, S1, a):
    had = any('IsScripting' in s for s in S0)
    return F.diffcheck(S0, S1, lambda s: 'IsScripting' in s, lambda s: s == FIN, [FIN] if had else None)
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'isv')
