#!/usr/bin/env python3
"""fg.py -- fast grade: grade one or several variants of ONE row in a single call and show only what differs.

  python3 $S/tools/fg.py <addr> '<mangled leaf>' <v1.cpp> [v2.cpp ...] [--flags /d2inlS39 ...] [--note "idea tried"] [--lines N]

Environment: lane env sourced; W=<chunk dir> (ATTEMPTS.log is appended there; default: the directory of the first source);
FABLE_HDR_OVERLAY as you want it (default $W/hdr when that exists).
For each variant (graded one after another, never in parallel): one result line
    <addr> <file> MATCH | RELOCATION_MATCH | DIFFER(ours v retail) hunks=N | SRC_FAIL
then, for the BEST differing variant only, the differing hunks (R = retail, O = ours), at most --lines lines (default 60).
SRC_FAIL prints the first compiler errors. Never raises: always exits 0.
"""
import os, re, subprocess, sys, time, difflib

def _sweep_orphan_compilers(max_age=300):
    """Kill Wine cl.exe / link.exe older than max_age seconds: orphans of timed-out graders spin at 100% CPU for hours."""
    try:
        import subprocess as _sp, os as _os, signal as _sig
        out = _sp.run(['ps', '-eo', 'pid,etimes,comm'], capture_output=True, text=True, timeout=10).stdout
        for line in out.split('\n')[1:]:
            c = line.split()
            if len(c) >= 3 and c[2] in ('cl.exe', 'link.exe') and c[1].isdigit() and int(c[1]) > max_age:
                try: _os.kill(int(c[0]), _sig.SIGKILL)
                except Exception: pass
    except Exception:
        pass
_sweep_orphan_compilers()


def main():
    a = sys.argv[1:]
    flags, note, lines = [], '', 60
    if '--lines' in a:
        i = a.index('--lines'); lines = int(a[i + 1]); del a[i:i + 2]
    if '--note' in a:
        i = a.index('--note'); note = a[i + 1]; del a[i:i + 2]
    if '--flags' in a:
        i = a.index('--flags'); flags = a[i + 1:]; a = a[:i]
    addr, leaf, srcs = a[0].lower().replace('0x', ''), a[1], a[2:]
    S = os.environ['S']
    W = os.environ.get('W') or os.path.dirname(os.path.abspath(srcs[0]))
    env = dict(os.environ)
    if not env.get('FABLE_HDR_OVERLAY') and os.path.isdir(os.path.join(W, 'hdr')):
        env['FABLE_HDR_OVERLAY'] = os.path.join(W, 'hdr')
    res = []
    for s in srcs:
        cmd = ['python3', S + '/tools/q.py', addr, s, leaf] + (['--flags'] + flags if flags else [])
        try:
            p = subprocess.run(cmd, capture_output=True, text=True, errors='replace', timeout=240, env=env)
            out = (p.stdout or '') + (p.stderr or '')
        except subprocess.TimeoutExpired:
            out = addr + ' ? TIMEOUT (cl hung: usually an #error in a header -- run a gcc `cpp` pre-pass)'
            subprocess.run(['pkill', '-f', 'wine/q/' + addr], capture_output=True)
        first = next((l for l in out.split('\n') if l.startswith(addr + ' ')), out.strip().split('\n')[-1] if out.strip() else 'NO OUTPUT')
        m = re.search(r'\b(RELOCATION_MATCH|MATCH|DIFFER\(\d+v\d+\)|SRC_FAIL|TIMEOUT)', first)
        grade = m.group(1) if m else 'ERROR'
        hunks, text = diff(out) if grade.startswith('DIFFER') else (0, [])
        res.append((s, grade, hunks, text, out))
        print(f"{addr} {os.path.basename(s)} {grade}" + (f" hunks={hunks}" if grade.startswith('DIFFER') else ''))
        try:
            with open(os.path.join(W, 'ATTEMPTS.log'), 'a') as f:
                f.write(f"{time.strftime('%H:%M')} {addr} grade {os.path.basename(s)} {grade}" + (f" hunks={hunks}" if hunks else '') + (f" {' '.join(flags)}" if flags else '') + f" -- {note}\n")
        except OSError:
            pass
        if grade in ('MATCH', 'RELOCATION_MATCH'):
            break
    bad = [r for r in res if r[1].startswith('DIFFER')]
    if any(r[1] in ('MATCH', 'RELOCATION_MATCH') for r in res):
        return
    if bad:
        s, grade, hunks, text, _ = min(bad, key=lambda r: r[2])
        print(f"--- best differing variant: {os.path.basename(s)} ({grade}, {hunks} hunks)")
        print('\n'.join(text[:lines]))
        if len(text) > lines:
            print(f"... {len(text) - lines} more lines (--lines N)")
    for s, grade, _, _, out in res:
        if grade in ('SRC_FAIL', 'ERROR', 'TIMEOUT'):
            errs = [l[-220:] for l in out.split('\n') if ' error ' in l or 'fatal' in l or 'Error' in l][:8]
            print(f"--- {os.path.basename(s)} {grade}:"); print('\n'.join(errs) if errs else out[-600:])

def diff(out):
    L = out.split('\n')
    ii = [k for k, l in enumerate(L) if l.startswith('--- retail')]; jj = [k for k, l in enumerate(L) if l.startswith('--- built')]
    if not ii or not jj:
        return 0, []
    a = [l for l in L[ii[0] + 1:jj[0]] if l.strip()]; b = [l for l in L[jj[0] + 1:] if l.strip() and re.match(r'^[0-9a-f]{8} ', l)]
    def norm(l, ours):
        p = l.split(None, 2); by, t = (p[1] if len(p) > 1 else ''), (p[2] if len(p) > 2 else '')
        if t.startswith('call 0x') and by.startswith('e8'): return 'call REL'
        if t.startswith('j'): return 'J ' + t.split()[0]
        if ours and '00000000' in by: t = re.sub(r'(?<![0-9a-fx])0(?![0-9a-fx])', 'REL', t)
        t = re.sub(r'0x([0-9a-f]+)', lambda m: 'REL' if int(m.group(1), 16) >= 0x400000 else m.group(0), t)
        t = re.sub(r'\[(0x[0-9a-f]+|\d+|REL)\]', '[REL]', t); t = re.sub(r'\[(0x[0-9a-f]+|\d+|REL) \+ ', '[REL + ', t)
        return t
    na = [norm(x, False) for x in a]; nb = [norm(x, True) for x in b]
    text, hunks = [], 0
    for op, a1, a2, b1, b2 in difflib.SequenceMatcher(None, na, nb, autojunk=False).get_opcodes():
        if op == 'equal': continue
        hunks += 1; text.append(f'@@ {op}')
        text += ['  R ' + a[k] for k in range(a1, a2)] + ['  O ' + b[k] for k in range(b1, b2)]
    return hunks, text

if __name__ == '__main__':
    try:
        main()
    except Exception as e:   # never show a traceback
        print('fg.py could not grade:', type(e).__name__, str(e)[:200])
    sys.exit(0)
