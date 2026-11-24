# inclass.py: for each changed row (rows.txt), landed sources (not the self) that define a LOCAL model of class X (`class|struct X ... {`)
# and spell M( ... ) { on one line inside it: an in-class inline body that could emit the PDB symbol as a COMDAT. -> stdout
import re, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338')
R = '/home/user/FableDecomp/rebuild/src/compiled'
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')}
import os
for a in (open(os.environ['INCLASS_LIST']).read().split() if os.environ.get('INCLASS_LIST') else (A / 'rows.txt').read_text().split()):
    m = re.match(r"\?([^@]+)@([^@]+)@@", E[a][3]); M, X = m.group(1), m.group(2)
    fs = subprocess.run(['grep', '-rlE', r'^\s*(class|struct)\s+' + X + r'\b[^;]*$', R, '--include=*.cpp'], capture_output=True, text=True).stdout.split()
    for f in fs:
        if E[a][7] in f: continue
        t = open(f, encoding='utf-8', errors='replace').read()
        for cm in re.finditer(r'^\s*(class|struct)\s+' + X + r'\b[^;{]*\{', t, re.M):
            i = cm.end(); d = 1
            while d and i < len(t): d += {'{': 1, '}': -1}.get(t[i], 0); i += 1
            body = t[cm.end():i]
            for l in body.splitlines():
                c = l.split('//')[0]
                if re.search(r'\b' + M + r'\s*\([^;]*\)\s*(const)?\s*\{', c): print(a, X + '::' + M, f.replace(R + '/', ''), c.strip()[:100])
