# closure358.py: transitive leg. For every compiled census object that DEFs one of the three PDB symbols, list the object's OTHER
# function COMDATs whose relocations name that symbol (intermediates). Where pk_all gives an intermediate a retail address, scan retail
# call / jmp rel32 sites to it and map them to landed rows; print the landed rows NOT already in work/cand.tsv (they could emit the
# intermediate, and with it the copy, without calling the copy in retail). Output work/closure.tsv.
import sys, os, re, struct, bisect, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = nf.W
SYM = {'00429c6f': '?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z',
       '0042b312': '??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z',
       '0042b040': '??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z'}
cand = set(l.split('\t')[0] for l in open(W / 'cand.tsv'))
inter = collections.defaultdict(set)   # (copy, intermediate symbol) -> rows
addr = {}
for obj in sorted(W.glob('obj/H_*.obj')):
    a = obj.stem[2:]; secs, sl = nf.coff(obj)
    pk = W / 'pkc' / ('H_%s.pkall.tsv' % a)
    pka = {}
    if pk.exists():
        for l in open(pk):
            p = l.rstrip('\n').split('\t')
            if len(p) >= 10 and p[2] != '-': pka[p[9]] = p[2]
    # section index -> defining external/static COMDAT symbol name
    secsym = {}
    for i, n, v, sec, typ, scl in sl:
        if sec > 0 and scl in (2, 3) and (typ >> 4) == 2 and v == 0: secsym.setdefault(sec, n)
    for k, s in enumerate(secs, 1):
        if not s['name'].startswith('.text'): continue
        names = {x[1] for x in s['rel']}
        for c, sym in SYM.items():
            if sym in names and secsym.get(k) not in (sym, None):
                n = secsym[k]; inter[(c, n)].add(a)
                if n in pka: addr[n] = pka[n]
landed = set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", nf.PS)); cat = nf.catalog(); ln = {}
for x in landed:
    r = cat.get(x)
    if r and r.get('length', '').isdigit(): ln[int(x, 16)] = int(r['length'])
st = sorted(ln)
def row(site):
    k = bisect.bisect_right(st, site) - 1
    return '%08x' % st[k] if k >= 0 and st[k] <= site < st[k] + ln[st[k]] else None
T = {int(v, 16): n for n, v in addr.items()}; refs = collections.defaultdict(set)
for nm, va, d, ch in nf.exe():
    if not ch & 0x20: continue
    for m in re.finditer(rb'[\xe8\xe9]', d):
        p = m.start()
        if p + 5 > len(d): break
        tgt = (va + p + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff
        if tgt in T:
            r = row(va + p)
            if r: refs[tgt].add(r)
with open(W / 'closure.tsv', 'w') as o:
    for (c, n), rows in sorted(inter.items()):
        ad = addr.get(n, '-'); rr = refs.get(int(ad, 16), set()) if ad != '-' else set()
        o.write('%s\t%s\t%s\temitted_by=%s\tretail_callers=%d\tnew_rows=%s\n' % (c, ad, n[:110], ','.join(sorted(rows)), len(rr), ','.join(sorted(rr - cand - {ad})) or '-'))
print(open(W / 'closure.tsv').read())
