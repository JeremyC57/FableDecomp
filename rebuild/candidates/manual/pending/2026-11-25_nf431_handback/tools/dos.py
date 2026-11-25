#!/usr/bin/env python3
"""dos.py <W> <addr...>: dossier per row -> <W>/dos/<addr>.txt: RTTI PDB symbol, header declaration, retail disassembly, FableWin /Od body, HEAD row."""
import sys, os, re, subprocess
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
R = '/home/user/FableDecomp'; W = sys.argv[1]; os.makedirs(W + '/dos', exist_ok=True)
cat = {}
for l in open(R + '/rebuild/oracles/auto-re-candidates.tsv').read().splitlines()[1:]:
    q = l.split('\t'); cat[q[0].lower()] = q
rows = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(W + '/open_cgsi.tsv')}
hdr = open(R + '/rebuild/include/engine/CGameScriptInterface.h').read().splitlines()
md = Cs(CS_ARCH_X86, CS_MODE_32)
for a in sys.argv[2:]:
    r = rows[a]; sym = r[12].split(';')[0] if r[12] else r[6]
    o = [f'### {a} {r[1]} {sym}']
    meth = re.match(r'\?(\w+)@', sym).group(1) if sym.startswith('?') else ''
    o += [h for h in hdr if re.search(r'\b%s\(' % meth, h) and '0x' in h and a.upper()[2:] in h.upper()] or [h for h in hdr if re.search(r'\b%s\(' % meth, h)]
    b = bytes.fromhex(cat[a][3]); o.append(f'-- retail {len(b)} bytes')
    for i in md.disasm(b, int(a, 16)): o.append('%08x %-16s %s %s' % (i.address, i.bytes.hex(), i.mnemonic, i.op_str))
    fw = subprocess.run(['python3', W + '/tools/fw.py', '^' + re.escape(sym) + '$'], capture_output=True, text=True).stdout
    o.append('-- FableWin'); o.append(fw.strip() or '(none)')
    o.append('-- HEAD ' + r[10]); o.append(open(R + '/rebuild/src/compiled/' + r[10]).read())
    open(f'{W}/dos/{a}.txt', 'w').write('\n'.join(o) + '\n')
print('ok')
