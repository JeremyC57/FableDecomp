# fam_nest.py <overlay> <list.tsv>: nest an entity class landed at global scope into its PDB owner NScript::<Script>
#   list columns: addr meth ent_old ent_new script [old_script_standin] [evidence] [kind V|P] [meth_old]
# kind V (default): the leaf becomes ?<meth>@<ent_new>@<script>@NScript@@EAEXXZ (private virtual override);
# kind P: a private non-virtual member, ?<meth>@<ent_new>@<script>@NScript@@AAEXXZ (meth_old respelled to meth).
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hand'))
import famlib
from he import block_end, ns_chain
ov, lst = sys.argv[1:3]
P = {}
for l in open(lst):
    q = l.rstrip('\n').split('\t')
    if len(q) >= 5 and not l.startswith('#'): P[q[0]] = q
TAG = 'naming wave 11'
def code_sub(t, pat, rep):
    out = []
    for ln in t.split('\n'):
        out.append(ln if ln.lstrip().startswith('//') else re.sub(pat, rep, ln))
    return '\n'.join(out)
def transform(t, a):
    q = P[a]; meth, eo, en, scr = q[1:5]; oscr = q[5] if len(q) > 5 and q[5] else scr; ev = q[6] if len(q) > 6 else ''
    kind = q[7] if len(q) > 7 and q[7] else 'V'; mo = q[8] if len(q) > 8 and q[8] else meth
    sym = '?%s@%s@%s@NScript@@%s' % (meth, en, scr, 'EAEXXZ' if kind == 'V' else 'AAEXXZ')
    if ('class %s::%s' % (scr, en)) in t: return t, 'already nested'
    if mo != meth: t = code_sub(t, r'\b' + mo + r'\b', meth)
    if oscr != scr:
        t = re.sub(r'\b' + oscr + r'\b', scr, t)
    if eo != en:
        t = code_sub(t, r'\b' + eo + r'\b', en)
    # the entity block
    m = re.search(r'\n(class|struct)\s+' + en + r'\s*(:\s*(?:public\s+)?(\w+))?\s*\{', t)
    if not m: return None, 'no entity block'
    if ns_chain(t, m.start()): return None, 'entity block not at global scope'
    e = block_end(t, m.start() + 1)
    body = t[m.end():e - 2]          # between { and };
    # Main/Init: a private virtual override (slot 1 / slot 2)
    def priv(b, mm):
        r = re.compile(r'\n([ \t]*)(?:virtual\s+)?void\s+' + mm + r'\s*\(\s*\)\s*;[^\n]*')
        x = r.search(b)
        if not x: return b, False
        before = b[:x.start()]
        acc = re.findall(r'\b(public|private|protected)\s*:', before)
        if kind == 'V':
            if acc and acc[-1] == 'private' and 'virtual' in x.group(0): return b, True
            new = '\nprivate:\n    virtual void %s();   // PDB ?%s@<entity>@..@@EAEXXZ: every entity override is private\npublic:' % (mm, mm)
        else:
            if acc and acc[-1] == 'private': return b, True
            new = '\nprivate:\n    void %s();   // PDB %s (private)\npublic:' % (mm, sym)
        return b[:x.start()] + new + b[x.end():], True
    body, ok = priv(body, meth)
    if not ok: return None, 'no %s declaration in the entity block' % meth
    head = '\nnamespace NScript {\nclass %s::%s%s {' % (scr, en, (' : public ' + m.group(3)) if m.group(3) else '')
    if m.group(1) == 'struct': head += '\npublic:'
    eblock = head + body + '};\n}'
    # the owner script: global definition (wrap), forward declaration (replace), or a skeleton before the entity block
    sd = re.search(r'\n(class|struct)\s+' + scr + r'\s*\{', t)
    pre, post = t[:m.start()], t[e:]
    nested_decl = '    class %s;                        // the entity class below (%s)\n' % (en, TAG)
    if sd and sd.start() < m.start():
        if ns_chain(t, sd.start()): return None, 'script definition not at global scope'
        se = block_end(t, sd.start() + 1)
        sbody = t[sd.end():se - 2]
        if not sbody.endswith('\n'): sbody += '\n'
        sb = '\nnamespace NScript {\nclass %s {' % scr + ('\npublic:' if sd.group(1) == 'struct' else '') + sbody + 'public:\n' + nested_decl + '};\n}\nusing NScript::%s;' % scr
        pre = t[:sd.start()] + sb + t[se:m.start()]
    elif sd:
        return None, 'script defined after the entity block'
    else:
        skel = '\nnamespace NScript {\nclass %s {                // the PDB owner script (%s); only its nested entity class is modelled\npublic:\n%s};\n}\nusing NScript::%s;' % (scr, TAG, nested_decl, scr)
        fd = list(re.finditer(r'\n(class|struct)\s+' + scr + r'\s*;[^\n]*', pre))
        if fd:
            f0 = fd[0]
            pre = pre[:f0.start()] + skel + pre[f0.end():]
            for f in fd[1:]: pre = pre.replace(f.group(0), '', 1)
        else:
            pre = pre + skel
    t = pre + eblock + post
    # out-of-class member definitions and other qualified uses
    t = code_sub(t, r'(?<![\w:])' + en + r'::', 'NScript::%s::%s::' % (scr, en))
    # header comment, Leaf
    L = t.split('\n')
    L[0] = re.sub(r'^// (?:[\w:]*::)?' + eo + r'::', '// NScript::%s::%s::' % (scr, en), L[0])
    lf = [i for i, x in enumerate(L) if x.startswith('// Leaf: ')]
    idl = '// Identity (%s): %s (Ego_r.pdb / FableWin.pdb name lists)%s' % (TAG, sym, ('; ' + ev) if ev else '')
    if lf:
        i = lf[0]; cur = L[i][9:].strip()
        if cur not in sym: L[i] = '// Leaf: ' + sym
        L.insert(i + 1, idl)
    else:
        L.insert(1, idl)
    return '\n'.join(L), 'nested in NScript::%s as %s' % (scr, en)
def check(S0, S1, a):
    q = P[a]; meth, eo, en, scr = q[1:5]; oscr = q[5] if len(q) > 5 and q[5] else scr
    kind = q[7] if len(q) > 7 and q[7] else 'V'
    sym = '?%s@%s@%s@NScript@@%s' % (meth, en, scr, 'EAEXXZ' if kind == 'V' else 'AAEXXZ')
    def rem_ok(s): return ('@' + eo + '@' in s) or ('@' + oscr + '@' in s) or ('V' + oscr + '@' in s) or ('U' + oscr + '@' in s) or ('@' + en + '@' in s)
    def add_ok(s): return ('@' + scr + '@NScript@@' in s)
    return famlib.diffcheck(S0, S1, rem_ok, add_ok, need=[sym])
famlib.run(ov, sorted(P), transform, check, tag='nest')
