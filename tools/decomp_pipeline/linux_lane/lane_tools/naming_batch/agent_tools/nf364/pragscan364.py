# pragscan364.py <old_tree> <new_tree> <addr>=<new_src> ...: NF362's optimize-state replay (VC7.1: ("", on) = command line, no push /
# pop; s on clears t and t on clears s), full letter set. For each row: HEAD text on old_tree and the new text on new_tree, preprocessed
# with clang -E (MSVC mode). Reports for the NEW TU: the state just before the NF364 block's first pragma (BEFORE), the state at the
# Reset<CComponent> specialization line (SPEC), the state after the block's last pragma (AFTER), the end-of-TU state (END), the HEAD
# END state, and the number of optimize pragmas outside the block (new) vs HEAD. Verdict OK when BEFORE == AFTER, SPEC has y=on s=on
# t=off and every other letter at BEFORE's value, END(new) == END(HEAD), and the non-block pragma sequence is HEAD's.
import sys, re, subprocess, os
S = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
R = '/home/user/FableDecomp'
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
CMD = {'y': 'on', 's': 'off', 't': 'off', 'g': 'on', 'a': 'off', 'p': 'off', 'w': 'off'}
def pp(tree, src):
    cmd = ['clang++', '-target', 'i686-pc-windows-msvc', '-fms-extensions', '-fms-compatibility', '-fms-compatibility-version=13.10', '-E', '-w',
           '-Xclang', '-fno-wchar', '-I' + tree, '-isystem', S + '/wine/vc71/include', src]
    return subprocess.run(cmd, capture_output=True, text=True).stdout.splitlines()
def replay(lines):
    st = dict(CMD); cur = ('?', 0); ev = []
    for l in lines:
        m = re.match(r'#\s*(\d+)\s+"([^"]*)"', l)
        if m: cur = (m.group(2), int(m.group(1)) - 1); continue
        cur = (cur[0], cur[1] + 1)
        m = re.match(r'\s*#pragma\s+optimize\s*\(\s*"([^"]*)"\s*,\s*(on|off)\s*\)', l)
        if m:
            letters, onoff = m.group(1), m.group(2); before = dict(st)
            if letters == '': st = dict(CMD) if onoff == 'on' else {k: 'off' for k in CMD}
            else:
                for c in letters: st[c] = onoff
                if 's' in letters and onoff == 'on': st['t'] = 'off'
                if 't' in letters and onoff == 'on': st['s'] = 'off'
            ev.append(('P', cur, '%s,%s' % (letters, onoff), before, dict(st))); continue
        if re.search(r'void\s+CCountedPointer\s*<\s*NUISystem::CComponent\s*>::Reset\s*\(', l): ev.append(('SPEC', cur, l.strip()[:60], dict(st), dict(st)))
    return ev, st
def fmt(d): return ' '.join('%s=%s' % (k, d[k]) for k in sorted(d))
old, new = sys.argv[1], sys.argv[2]; bad = 0
for spec in sys.argv[3:]:
    a, src = spec.split('=', 1)
    eh, endh = replay(pp(old, str(nf.src_of(a)))); en, endn = replay(pp(new, src))
    inblk = lambda e: e[1][0].endswith('engine/CCountedPointer.h') and e[1][1] >= 602
    bp = [e for e in en if e[0] == 'P' and inblk(e)]; sp = [e for e in en if e[0] == 'SPEC']
    outside_n = [(e[2]) for e in en if e[0] == 'P' and not inblk(e)]; outside_h = [e[2] for e in eh if e[0] == 'P']
    before = bp[0][3] if bp else None; after = bp[-1][4] if bp else None
    specst = sp[-1][3] if sp else None
    want_spec = dict(before or {}); want_spec.update(y='on', s='on', t='off')
    ok = bool(bp) and len(bp) == 4 and len(sp) >= 1 and before == after and specst == want_spec and endn == endh and outside_n == outside_h and before.get('y') == 'off' and before.get('s') == 'on'
    bad += not ok
    print('%s %s blockpragmas=%d spec_lines=%d | BEFORE %s | SPEC %s | AFTER %s | END(new) %s | END(head) %s | pragmas outside block new=%s head=%s' % (
        a, 'OK' if ok else 'FAIL', len(bp), len(sp), fmt(before) if before else '-', fmt(specst) if specst else '-', fmt(after) if after else '-',
        fmt(endn), fmt(endh), outside_n, outside_h))
print('PRAGSCAN364', 'ALL_OK' if not bad else 'FAIL=%d' % bad)
