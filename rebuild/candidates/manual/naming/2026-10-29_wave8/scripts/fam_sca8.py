# fam_sca8.py <file>...: CThingCreatureBase::SetCurrentAction takes `const CCreatureActionBase&` (Ego_r.pdb + FableWin.pdb
#   ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z, 0x006644F0). Declarations `bool SetCurrentAction(
#   CCreatureActionBase* a);` become `const CCreatureActionBase& a`; one-argument calls pass `*e` (or drop a leading `&`;
#   `reinterpret_cast<CCreatureActionBase*>(&x)` becomes `reinterpret_cast<const CCreatureActionBase&>(x)`). Only files that
#   declare the pointer form are touched. Idempotent.
import sys, re
DECL = re.compile(r'\bbool(\s+)((?:\w+::)?)SetCurrentAction\((\s*)(class\s+)?CCreatureActionBase\s*\*\s*(\w*)\s*\)(\s*\{)?')
SKIP = {'006643c0'}   # CThingCreatureBase::UpdateAction: SRC_FAIL with the reference form (its own queue call), left as is
CALL = re.compile(r'(->|\.)SetCurrentAction\(')
def arg_end(t, i):
    d = 0; j = i
    while j < len(t):
        c = t[j]
        if c in '([': d += 1
        elif c in ')]':
            if d == 0: return j
            d -= 1
        elif c == ',' and d == 0: return None
        j += 1
    return None
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    if not DECL.search(t) or any(s in p for s in SKIP): print('skip', p, 'no pointer-form declaration'); continue
    def dsub(m):
        nm = m.group(5)
        if m.group(6) and nm:   # a stand-in definition: keep its pointer body
            return 'bool%s%sSetCurrentAction(%sconst %sCCreatureActionBase& %s_ref)%s CCreatureActionBase* %s = const_cast<CCreatureActionBase*>(&%s_ref);' % (
                m.group(1), m.group(2), m.group(3), m.group(4) or '', nm, m.group(6), nm, nm)
        return 'bool%s%sSetCurrentAction(%sconst %sCCreatureActionBase&%s)%s' % (m.group(1), m.group(2), m.group(3), m.group(4) or '', (' ' + nm) if nm else '', m.group(6) or '')
    t = DECL.sub(dsub, t)
    out = []; pos = 0
    for m in CALL.finditer(t):
        s = m.end(); e = arg_end(t, s)
        if e is None: continue
        a = t[s:e].strip()
        if not a or a.startswith('*'): continue
        rc = re.fullmatch(r'reinterpret_cast<\s*CCreatureActionBase\s*\*\s*>\(\s*&\s*(\w+)\s*\)', a)
        if rc: new = 'reinterpret_cast<const CCreatureActionBase&>(%s)' % rc.group(1)
        elif a.startswith('&'): new = a[1:].strip()
        elif re.fullmatch(r'[\w.\->:\[\]]+', a): new = '*' + a
        else: new = '*(' + a + ')'
        out.append(t[pos:s] + new); pos = e
    t = ''.join(out) + t[pos:]
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'nothing to respell')
