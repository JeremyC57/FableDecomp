#!/usr/bin/env python3
"""fw.py <regex>: FableWin.exe /Od body of every public whose mangled name matches <regex>: VA, then fwdis output with callees named."""
import re, struct, sys, subprocess
E = '/tmp/claude-0/egod_pdb/FableWin.exe'; P = '/tmp/claude-0/egod_pdb/FableWin.publics.txt'
T = '/home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-25_codex_pass29/tools'
d = open(E, 'rb').read(); pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]
op = struct.unpack_from('<H', d, pe + 20)[0]; ib = struct.unpack_from('<I', d, pe + 24 + 28)[0]
secs = [struct.unpack_from('<IIII', d, pe + 24 + op + 40 * i + 8) for i in range(ns)]
lines = open(P, encoding='utf-8', errors='replace').read().splitlines()
pubs = {}; name = None
for l in lines:
    m = re.search(r'S_PUB32 \[size = \d+\] `(.*)`', l)
    if m: name = m.group(1); continue
    m = re.search(r'flags = function, addr = (\d+):(\d+)', l)
    if m and name:
        sec, off = int(m.group(1)), int(m.group(2)); pubs[name] = ib + secs[sec - 1][1] + off; name = None
rx = re.compile(sys.argv[1]); mx = sys.argv[2] if len(sys.argv) > 2 else '900'
byva = {v: k for k, v in pubs.items()}
for k, v in pubs.items():
    if not rx.search(k): continue
    print('##', k, hex(v))
    out = subprocess.run(['python3', T + '/fwdis.py', E, hex(v), mx], capture_output=True, text=True).stdout
    for l in out.splitlines():
        m = re.search(r'call\s+0x([0-9a-f]+)', l)
        if m:
            t = int(m.group(1), 16)
            i = d[t - ib - secs[0][1] + secs[0][3]:][:5] if False else None
            # follow incremental-link jmp thunk
            for vs, sva, rs, ro in secs:
                if sva <= t - ib < sva + max(vs, rs):
                    b = d[ro + t - ib - sva: ro + t - ib - sva + 5]
                    if b[0] == 0xE9: t = t + 5 + struct.unpack('<i', b[1:5])[0]
            l += '   ; ' + byva.get(t, '?')
        print(l)
