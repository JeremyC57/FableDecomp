# gspec.py <in.tsv> <out.json>: build hdredit.py specs. in.tsv lines: idx <TAB> access <TAB> declaration (no trailing comment) [<TAB> extra note].
# Groups by the entry's class header; class_line = the CLASS branch line from work/tri2.tsv; per declaration a trailing
# `// 0xADDR slot N` (slot from vtslot.py on the class's own vftable) and one comment line per entry with the mangled PDB spelling and the
# name sets that carry it (egor / fablewin), plus the RTTI placement. Base = the class's direct base(s) from the CLASS line.
import json, re, sys, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
T = {}
for l in open(A / 'work/tri2.tsv'):
    p = l.rstrip('\n').split('\t')
    if p[8] == 'CLASS': T.setdefault(int(p[0]), (int(p[7][1:]), p[4], p[5], p[9]))
out = {}; order = []
for l in open(sys.argv[1]):
    if not l.strip() or l.startswith('#'): continue
    p = l.rstrip('\n').split('\t'); i = int(p[0]); acc, decl = p[1], p[2]; note = p[3] if len(p) > 3 else ''
    e = E[i - 1]; a = e[0]; pdb = e[3]; X = re.match(r'\?[^@]+@([^@]+)@@', pdb).group(1)
    cl, eg, fw, base = T[i]
    v = subprocess.run(['python3', str(A / 'tools/vtslot.py'), a], capture_output=True, text=True).stdout
    m = re.search(r'([0-9A-F]{8}):\.\?AV' + re.escape(X) + r'@@\[(\d+)\]', v)
    slot = ('vtable 0x%s slot %s' % (m.group(1), m.group(2))) if m else 'no own vftable slot'
    sets = ' + '.join(x for x in (('Ego_r.pdb' if eg == 'EG' else ''), ('FableWin.pdb' if fw == 'FW' else '')) if x)
    h = 'engine/%s.h' % X
    if h not in out:
        out[h] = {'header': h, 'class_line': cl, 'macro': 'FABLE_RETAIL_%s_RTTI_DECLS' % X.upper(), 'comment': [
            'Opt-in (NF319, 2026-11-23): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement',
            '(ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual',
            'its base %s chain declares at the same slot (no new slot).' % base.replace('public', '').replace(':', '').strip()], 'decls': []}
        order.append(h)
    out[h]['comment'].append('0x%s %s: %s (%s)%s' % (a.upper(), slot, pdb, sets, ('; ' + note) if note else ''))
    out[h]['decls'].append([acc, '%s// 0x%s' % (decl.ljust(66), a.upper())])
json.dump([out[h] for h in order], open(sys.argv[2], 'w'), indent=1)
for h in order: print(h, len(out[h]['decls']))
