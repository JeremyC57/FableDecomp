# fam_pkc.py <ov> <rows.txt>: chunk 586 lock family. The scoped guard 0x00C0FFC0 / 0x00C0FFE0 is
# PKCProtectionInstance(PKCCriticalSection*) / ~PKCProtectionInstance (".\PKCProtectionInstance.cpp" trace; bound from 00c10b00);
# the module lock 0x0143BAFC is `PKCCriticalSection g_soundLock` (?g_soundLock@@3VPKCCriticalSection@@A, chunk 586, bound from
# 00c27890); the other module lock 0x0143BB14 takes the landed PKC rows' spelling ?g_lock_0143bb14@@3VPKCCriticalSection@@A
# (bound from 00c10b00). Code unchanged: the guard is the same 8-byte object; casts between object pointers are no-ops.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
CT = '??0PKCProtectionInstance@@QAE@PAVPKCCriticalSection@@@Z'; DT = '??1PKCProtectionInstance@@QAE@XZ'
GS = '?g_soundLock@@3VPKCCriticalSection@@A'; GB = '?g_lock_0143bb14@@3VPKCCriticalSection@@A'
BT, BS = inputs.bindings()
OLDT = set()
for t in ('00c0ffc0', '00c0ffe0', '0143bafc', '0143bb14'): OLDT |= BT.get(t, set())
CTOR = re.compile(r'^([ \t]*)(?:explicit\s+)?(\w+)\s*\(([^)]*)\)\s*;[^\n]*0x00C0FFC0', re.M | re.I)
STATE = {}
def code(t): return re.sub(r'//[^\n]*', '', t)
def tr(t, a):
    ds = CTOR.findall(t)
    names = set(d[1] for d in ds)
    if not ds: return None, 'no 0x00C0FFC0 ctor declaration'
    if names == {'PKCProtectionInstance'}: return None, 'already PKCProtectionInstance'
    if len(names) != 1: return None, 'several guard classes %s' % sorted(names)
    L = ds[0][1]; prm = ds[0][2].strip()
    c = code(t)
    if re.search(r'\bPKC(CriticalSection|ProtectionInstance)\b', c): return None, 'file already names PKC types'
    if re.search(r'\bg_soundLock\b', c) and not re.search(r'extern\s+unsigned\s+char\s+g_soundLock\s*\[\s*\]', c): return None, 'g_soundLock already used'
    pm = re.match(r'^(const\s+)?(\w+(?:\s+\w+)?)\s*\*\s*(\w*)$', prm)
    if not pm: return None, 'ctor parameter %r' % prm
    B = pm.group(2); pname = pm.group(3) or 'section'
    old = set([L]); glob = []
    # 1. the guard class
    t = re.sub(r'\b' + L + r'\b', 'PKCProtectionInstance', t)
    t = re.sub(r'\bstruct(\s+PKCProtectionInstance\s*)\{', r'class\1{ public:', t)
    t = re.sub(r'\bstruct(\s+PKCProtectionInstance\s*);', r'class\1;', t)
    # 2. the section type
    tgt = re.match(r'^C\w*(LockTarget|GuardTarget|ProfileSection)\w*$', B)
    if tgt and not pm.group(1):
        body = re.search(r'\b(?:struct|class)\s+' + B + r'\s*(?::[^{;]*)?\{([^{}]*)\}', t)
        if body and body.group(1).strip(): tgt = None   # a section type with members: keep it, cast at the site
    if tgt and not pm.group(1):
        old.add(B)
        t = re.sub(r'\b' + B + r'\b', 'PKCCriticalSection', t)
        t = re.sub(r'\bstruct(\s+PKCCriticalSection\s*)([;{])', r'class\1\2', t)
        cast = False
    else:
        cast = True
        t = re.sub(r'(PKCProtectionInstance\s*\()\s*' + re.escape(prm) + r'\s*\)', r'\1PKCCriticalSection* ' + pname + ')', t, count=1)
    # 3. untyped section globals (`extern unsigned char g_x[];`) become PKCCriticalSection objects
    for m in re.finditer(r'extern\s+(?:unsigned\s+)?char\s+(\w+)\s*\[\s*\]\s*;([^\n]*)', t):
        if re.search(r'0x0143BAFC|0x0143BB14', m.group(2), re.I): glob.append(m.group(1))
    for g in glob:
        old.add(g)
        t = re.sub(r'extern\s+(?:unsigned\s+)?char\s+' + g + r'\s*\[\s*\]\s*;', 'extern PKCCriticalSection ' + g + ';', t)
    if cast:
        # declare the section type before its first use
        lines = t.split('\n')
        k = next(i for i, l in enumerate(lines) if l.strip() and not re.match(r'\s*(//|#)', l))
        lines.insert(k, 'class PKCCriticalSection;'); t = '\n'.join(lines)
    # 4. construction sites
    def site(m):
        arg = m.group(3).strip()
        if arg in glob: arg2 = '&' + arg
        elif re.match(r'^&\w+$', arg) and not cast: arg2 = arg
        elif not cast: arg2 = arg
        else: arg2 = '(PKCCriticalSection*)' + (arg if re.match(r'^[\w.>&-]+$', arg) else '(' + arg + ')')
        return '%s %s(%s);' % (m.group(1), m.group(2), arg2)
    t = re.sub(r'(PKCProtectionInstance)\s+(\w+)\s*\(([^;()]*(?:\([^;()]*\))?[^;()]*)\)\s*;', site, t)
    # 5. the two module locks' names
    for m in list(re.finditer(r'extern\s+PKCCriticalSection\s+(\w+)\s*;([^\n]*)', t)):
        g, cm = m.group(1), m.group(2)
        new = 'g_soundLock' if re.search(r'0x0143BAFC', cm, re.I) else ('g_lock_0143bb14' if re.search(r'0x0143BB14', cm, re.I) else None)
        if new and g != new:
            if re.search(r'\b' + new + r'\b', t): return None, 'both %s and %s' % (g, new)
            old.add(g); t = re.sub(r'\b' + g + r'\b', new, t)
    STATE[a] = old
    return t, 'guard %s(%s) -> PKCProtectionInstance%s' % (L, prm, ' (cast)' if cast else '')
def ck(S0, S1, a):
    old = STATE.get(a, set())
    rem_ok = lambda s: s in OLDT or any(o in s for o in old)
    add_ok = lambda s: s in (CT, DT, GS, GB)
    need = [CT, DT] if any(('??0' + o + '@@') in s for s in S0 for o in old) else None
    return F.diffcheck(S0, S1, rem_ok, add_ok, need)
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'pkc')
