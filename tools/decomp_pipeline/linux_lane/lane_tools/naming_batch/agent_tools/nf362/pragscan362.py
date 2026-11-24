# pragscan362.py <src> [label=regex ...]: preprocess a row TU with clang -E (MSVC mode, _MSC_VER 1310, repo include tree; NF_TREE overrides)
# and print, in TU order, every `#pragma optimize(...)` with its origin file:line and the optimize state it leaves (VC7.1 semantics:
# ("", on) restores the command line; there is no push / pop), plus the first line matching each label's regex with the state in force
# there. The last line is the state at the END of the TU (where VC7.1 takes a template instance's mode, GOTCHAS 2026-11-24).
# State letters: y = frame-pointer omission (command line /Oy: on), s = favour size (command line: off), others listed when changed.
import sys, re, subprocess, os
R = '/home/user/FableDecomp'; S = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
src = sys.argv[1]; labels = [x.split('=', 1) for x in sys.argv[2:]]
inc = os.environ.get('NF_TREE', R + '/rebuild/include').split(':')
cmd = ['clang++', '-target', 'i686-pc-windows-msvc', '-fms-extensions', '-fms-compatibility', '-fms-compatibility-version=13.10', '-E', '-w',
       '-Xclang', '-fno-wchar'] + ['-I' + x for x in inc] + ['-isystem', S + '/wine/vc71/include', src]
out = subprocess.run(cmd, capture_output=True, text=True).stdout.splitlines()
CMD = {'y': 'on', 's': 'off', 't': 'off', 'g': 'on', 'a': 'off', 'p': 'off', 'w': 'off'}
st = dict(CMD); cur = ('?', 0); seen = set()
def fmt(): return ' '.join('%s=%s' % (k, v) for k, v in sorted(st.items()) if k in 'ys' or v != CMD[k])
def short(f): return f.replace(R + '/rebuild/include/', 'inc:').replace(R + '/rebuild/src/compiled/', 'src:')
for l in out:
    m = re.match(r'#\s*(\d+)\s+"([^"]*)"', l)
    if m: cur = (m.group(2), int(m.group(1)) - 1); continue
    cur = (cur[0], cur[1] + 1)
    m = re.match(r'\s*#pragma\s+optimize\s*\(\s*"([^"]*)"\s*,\s*(on|off)\s*\)', l)
    if m:
        letters, onoff = m.group(1), m.group(2)
        if letters == '': st = dict(CMD) if onoff == 'on' else {k: 'off' for k in CMD}
        else:
            for c in letters: st[c] = onoff
            if 's' in letters and onoff == 'on': st['t'] = 'off'
            if 't' in letters and onoff == 'on': st['s'] = 'off'
        print('PRAGMA %-28s %s:%d -> %s' % ('optimize("%s",%s)' % (letters, onoff), short(cur[0]), cur[1], fmt())); continue
    for lab, rx in labels:
        if lab not in seen and re.search(rx, l):
            seen.add(lab); print('MARK   %-28s %s:%d   state %s' % (lab, short(cur[0]), cur[1], fmt()))
for lab, rx in labels:
    if lab not in seen: print('MARK   %-28s (not found)' % lab)
print('END-OF-TU state %s' % fmt())
