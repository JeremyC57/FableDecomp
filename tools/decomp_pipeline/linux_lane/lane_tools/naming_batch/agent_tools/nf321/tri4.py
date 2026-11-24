# tri4.py [idx..]: NF321 body-port triage per ENTRIES line (screen output + work/tri4.tsv)
#   pdb   : the PDB spelling (ENTRIES col 4, else the const / access spelling from the two PDB name sets for col-6's leading symbol)
#   hdr   : how engine/<X>.h declares the member (L<n> INLINE/DECL + #if stack), or NOHEADER / ABSENT
#   local : local class / struct models the HEAD row defines (names) and stand-in markers (_Methods, _Retail, extern "C", free __fastcall)
#   calls : every direct call / jmp target of the retail body (catalog bytes) with: landed row file stem, its Leaf, RTTI label, table
#           spellings bound at that target (unique, cut); and every `call [reg+N]` virtual slot (N/4)
import re, sys, collections
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf, capstone
from pathlib import Path
A = nf.A; R = nf.R
E = nf.entries()
sel = set(int(x) for x in sys.argv[1:]) if len(sys.argv) > 1 else None
PDB = collections.defaultdict(set)
for f in ('egor_pdb_names.tsv', 'fablewin_pdb_names.tsv'):
    for l in open(R / 'ghidra_out' / f, encoding='utf-8', errors='replace'):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1 and p[1].startswith('?'):
            m = re.match(r"\?([^@?]+)@([^@]+)@@", p[1])
            if m: PDB[(m.group(1), m.group(2))].add(p[1])
RTTI = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    pth = R / 'ghidra_out' / f
    if pth.exists():
        for l in open(pth, encoding='utf-8', errors='replace'):
            p = l.rstrip('\n').split('\t')
            if len(p) > 1: RTTI.setdefault(p[0].lower().replace('0x', ''), p[1])
TAB = collections.defaultdict(list)
for l in open(R / 'rebuild/oracles/reloc_bindings.tsv', encoding='utf-8', errors='replace'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: TAB[p[1].lower().replace('0x', '')].append((p[0], p[2]))
GUARD = re.compile(r"#\s*ifndef\s+(FABLE_RETAIL_HEADER_\w+|\w+_H|\w+_HPP|\w+_INCLUDED)\s*$")
def decls(h, meth):
    t = h.read_text(encoding='utf-8', errors='replace'); L = t.split('\n'); stack = []; out = []
    for n, ln in enumerate(L, 1):
        s = ln.strip()
        if re.match(r"#\s*if", s): stack.append(s); continue
        if re.match(r"#\s*(else|elif)", s) and stack: stack[-1] = stack[-1] + ' |ELSE:' + s; continue
        if re.match(r"#\s*endif", s) and stack: stack.pop(); continue
        c = ln.split('//')[0]
        if re.search(r"(^|[^\w:.>])" + re.escape(meth) + r"\s*\(", c) and not re.search(r"(->|\.)\s*" + re.escape(meth) + r"\s*\(", c):
            rest = ' '.join(L[n - 1:n + 3]).split('//')[0]
            k = rest.find(meth); depth = 0; j = rest.find('(', k); body = '?'
            for i in range(j, len(rest)):
                if rest[i] == '(': depth += 1
                elif rest[i] == ')':
                    depth -= 1
                    if depth == 0:
                        tail = rest[i + 1:].lstrip()
                        tail = re.sub(r"^(const\s*)?", "", tail).lstrip()
                        body = 'INLINE' if tail.startswith('{') else ('DECL' if tail.startswith(';') or tail.startswith('=') else '?' + tail[:10])
                        break
            inner = [x for x in stack if not GUARD.match(x)]
            out.append((n, body, ' & '.join(inner) or '-', c.strip()[:110]))
    return out
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
cat = nf.catalog()
def leaf(a):
    s = nf.src_of(a)
    if not s or not s.exists(): return None, None
    t = s.read_text(encoding='utf-8', errors='replace')
    m = re.search(r"^//\s*Leaf:\s*(\S+)", t, re.M)
    return s.stem, (m.group(1) if m else '-')
out = open(A / 'work/tri4.tsv', 'w')
for i, e in enumerate(E, 1):
    if sel and i not in sel: continue
    a = e[0].lower(); pdb = e[3]
    if not pdb:
        m = re.match(r"\?([^@?]+)@([^@]+)@@", e[5])
        cands = sorted(PDB.get((m.group(1), m.group(2)), [])) if m else []
        pdb = cands[0] if len(cands) == 1 else ('AMBIG:' + '|'.join(cands) if cands else 'NONE')
    m = re.match(r"\?([^@?]+)@([^@]+)@@", pdb.replace('AMBIG:', ''))
    meth, X = (m.group(1), m.group(2)) if m else ('?', '?')
    h = R / 'rebuild/include/engine' / (X + '.h')
    if not h.exists(): hd = 'NOHEADER'
    else:
        D = decls(h, meth)
        hd = ' ; '.join('L%d %s [%s]' % (n, b, st) for n, b, st, c in D) or 'ABSENT'
    s = nf.src_of(a); t = s.read_text(encoding='utf-8', errors='replace') if s and s.exists() else ''
    code = '\n'.join(x.split('//')[0] for x in t.split('\n'))
    locs = sorted(set(re.findall(r"^\s*(?:class|struct)\s+(\w+)\s*(?::[^{;]*)?\{", code, re.M)))
    mark = []
    if '_Methods' in code: mark.append('Methods')
    if '_Retail' in code: mark.append('Retail')
    if 'extern "C"' in code: mark.append('externC')
    if re.search(r"__fastcall\s+\w+_\w+\s*\(", code): mark.append('freefastcall')
    if 'reinterpret_cast' in code or re.search(r"\(\s*\w+\s*\*\s*\)\s*\(?\s*\(?\s*(char|unsigned char)\s*\*", code): mark.append('cast')
    b = bytes.fromhex(cat[a]['bytes']) if a in cat else b''
    calls = []; vs = []
    for ins in md.disasm(b, int(a, 16)):
        if ins.mnemonic in ('call', 'jmp') and ins.op_str.startswith('0x'):
            tg = int(ins.op_str, 16)
            if int(a, 16) <= tg < int(a, 16) + len(b): continue
            calls.append('%08x' % tg)
        elif ins.mnemonic == 'call' and re.match(r"dword ptr \[e\w\w(?: \+ (0x[0-9a-f]+|\d+))?\]$", ins.op_str):
            mm = re.search(r"\+ (0x[0-9a-f]+|\d+)", ins.op_str); off = int(mm.group(1), 0) if mm else 0
            vs.append(off // 4)
    print('== %d %s %s  pdb=%s' % (i, a, X + '::' + meth, pdb))
    print('   hdr: %s' % hd[:300])
    print('   local: %s %s' % (','.join(locs)[:300], ' '.join(mark)))
    if vs: print('   vslots: %s' % ' '.join(map(str, vs)))
    cs = []
    for tg in dict.fromkeys(calls):
        st, lf = leaf(tg)
        sp = sorted(set(x[0] for x in TAB.get(tg, [])))
        print('   call %s row=%s leaf=%s rtti=%s table=%s' % (tg, st, lf, RTTI.get(tg, '-'), ' | '.join(sp)[:400]))
        cs.append(tg)
    print(i, a, X, meth, pdb, hd[:200], ','.join(locs)[:200], ' '.join(mark), ' '.join(map(str, vs)), ' '.join(cs), sep='\t', file=out)
out.close()
