"""egod_type.py <types.txt> <hex index>... | --locals <locals.txt>
Resolves Ego_d CodeView type indices to C++-ish names from an `llvm-pdbutil dump -types Ego_d.pdb` text dump (types.txt).
With --locals, rewrites egod_locals.py output, appending the resolved type after each `type 0x...`."""
import sys, re, pickle, os
T = sys.argv[1]; cache = T + '.idx.pickle'
if os.path.exists(cache): recs = pickle.load(open(cache, 'rb'))
else:
    recs = {}; cur = None
    for l in open(T, errors='replace'):
        m = re.match(r'\s*0x([0-9A-F]+) \| (LF_\w+) \[size = \d+\](?: `(.*)`)?', l)
        if m: cur = int(m.group(1), 16); recs[cur] = [m.group(2), m.group(3) or '', []]; continue
        if cur is not None and len(recs[cur][2]) < 4: recs[cur][2].append(l.strip())
    pickle.dump(recs, open(cache, 'wb'))
def name(ti, d=0):
    if d > 8: return '?'
    if ti < 0x1000:
        prim = {0x03:'void',0x10:'signed char',0x20:'unsigned char',0x70:'char',0x11:'short',0x21:'unsigned short',0x12:'long',
                0x22:'unsigned long',0x74:'int',0x75:'unsigned int',0x30:'bool',0x40:'float',0x41:'double',0x13:'__int64',
                0x23:'unsigned __int64',0x71:'wchar_t',0x68:'signed char',0x69:'unsigned char',0x72:'short',0x73:'unsigned short',0x76:'__int64',0x77:'unsigned __int64'}
        base = prim.get(ti & 0xff, 'prim%x' % ti)
        return base + ('*' if ti & 0xf00 else '')
    r = recs.get(ti)
    if not r: return '0x%x' % ti
    k, nm, body = r; b = ' '.join(body)
    if k in ('LF_CLASS', 'LF_STRUCTURE', 'LF_UNION', 'LF_ENUM'): return nm
    if k == 'LF_POINTER':
        m = re.search(r'referent = 0x([0-9A-F]+)', b); mode = '&&' if 'mode = rvalue ref' in b else ('&' if 'mode = ref' in b else ('::*' if 'member' in b else '*'))
        return name(int(m.group(1), 16), d + 1) + mode if m else '?*'
    if k == 'LF_MODIFIER':
        m = re.search(r'referent = 0x([0-9A-F]+)', b); mods = re.search(r'modifiers = (\w+)', b)
        return (mods.group(1) + ' ' if mods else '') + (name(int(m.group(1), 16), d + 1) if m else '?')
    if k == 'LF_ARRAY':
        m = re.search(r'size: (\d+).*element type: 0x([0-9A-F]+)', b)
        return name(int(m.group(2), 16), d + 1) + '[bytes=%s]' % m.group(1) if m else 'array'
    if k in ('LF_PROCEDURE', 'LF_MFUNCTION'): return 'function'
    return k
if sys.argv[2] == '--locals':
    for l in open(sys.argv[3]):
        l = l.rstrip('\n'); m = re.search(r'type 0x([0-9a-f]+)', l)
        print(l + ('   [' + name(int(m.group(1), 16)) + ']' if m else ''))
else:
    for a in sys.argv[2:]: print(a, name(int(a, 16)))
