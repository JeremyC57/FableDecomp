# srccensus.py (NF326): text census of the CScriptThing unit members. For each member, every landed source (catalog Source, so the
# address comes from build_candidates.ps1, not from the file name) whose non-comment text DEFINES CScriptThing::<member> (a line
# `... CScriptThing::M(` that is not a declaration ending in `;`), with its qualifier (inline / FABLE_FORCEINLINE / strong) and
# whether the row's own // Leaf: names the member. -> work/srccensus.tsv (member, addr, kind, file)
# This is the text side only; the object side (COMDAT / strong / UND) comes from compiling (objcensus.py).
import re, sys
from pathlib import Path
R = Path('/home/user/FableDecomp')
A = Path(__file__).resolve().parent.parent
PS = (R / 'rebuild/build_candidates.ps1').read_text(encoding='utf-8')
src2addr = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", PS, re.S):
    b = m.group(1)
    k = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", b); s = re.search(r"\bSource = '([^']*)'", b)
    if k and s: src2addr.setdefault(s.group(1).replace('\\', '/'), k.group(1).lower())
MEM = sys.argv[1:] or ['GetDefName', 'GetDataString', 'SetDataString', 'GetPos', 'GetAngleXY', 'MsgIsHitBySpecialAbilityFrom',
                       'MsgIsHitByAnySpecialAbilityFrom', 'MsgIsHitByAnyAggressiveSpecialAbilityFrom', 'IsBeingCarriedBy', 'IsNull']
def strip_comments(t):
    t = re.sub(r'/\*.*?\*/', lambda m: '\n' * m.group(0).count('\n'), t, flags=re.S)
    return '\n'.join(l.split('//')[0] for l in t.split('\n'))
out = open(A / 'work/srccensus.tsv', 'w'); out.write('member\taddr\tkind\tleaf_is_member\tfile\n')
n = 0
for src, a in sorted(src2addr.items(), key=lambda x: x[1]):
    p = R / 'rebuild/src/compiled' / src
    try: raw = p.read_text(encoding='utf-8', errors='replace')
    except OSError: continue
    if 'CScriptThing::' not in raw: continue
    t = strip_comments(raw)
    leaf = re.search(r'^// Leaf:\s*(\S+)', raw, re.M); leaf = leaf.group(1) if leaf else ''
    for M in MEM:
        for mm in re.finditer(r'^([^\n;{}]*?)\bCScriptThing::' + M + r'\s*\(([^\n]*)$', t, re.M):
            head, tail = mm.group(1), mm.group(2)
            # a definition: the rest of the line (or the next lines) opens a body before any ';'
            rest = t[mm.end():mm.end() + 400]
            body = tail + rest
            if body.find('{') < 0 or (0 <= body.find(';') < body.find('{')): continue
            if re.search(r'(->|\.)\s*$', head): continue
            kind = 'FORCEINLINE' if 'FABLE_FORCEINLINE' in head or '__forceinline' in head else ('inline' if re.search(r'\binline\b', head) else 'strong')
            out.write('%s\t%s\t%s\t%s\t%s\n' % (M, a, kind, 'Y' if ('@CScriptThing' in leaf and leaf.startswith('?' + M + '@')) or leaf == M else '', src)); n += 1
out.close(); print('lines', n)
