# cascade.py <callers.tsv> <outdir> <retail-ctor-addr=Class ...>: re-spell the landed rows that call a constructor whose signature changed
# to (CThing&). callers.tsv = callers.py output (target, site, row, name). Generated Alloc factories are regenerated from the
# template; every other caller gets the callee's local constructor declaration respelled (CThing* -> CThing&) and, when the row is
# itself a constructor taking CThing* whose CThing& form is in a PDB, its own signature and leaf too. Writes <outdir>/<row>.cpp and
# <outdir>/plan.tsv (row, status, leaf, flags, detail, new_ctor_class_or_-).
import re, sys, glob, os
callers, outdir = sys.argv[1:3]
cls_of = dict(x.split('=') for x in sys.argv[3:])
pdb = set()
for p in ('ghidra_out/egor_pdb_names.tsv', 'ghidra_out/_fablewin_api.tsv'):
    for l in open(p, encoding='utf-8', errors='replace'): pdb.add(l.rstrip('\n').split('\t')[-1])
cat = {l.split('\t')[0]: l.split('\t')[1] for l in open('rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8') if '\t' in l}
def code_sub(pat, rep, text):
    out = []; n = 0
    for line in text.split('\n'):
        k = line.find('//')
        code, com = (line[:k], line[k:]) if k >= 0 else (line, '')
        code2, m = re.subn(pat, rep, code); n += m; out.append(code2 + com)
    return '\n'.join(out), n
ALLOC = '''#pragma optimize("s",on)
// {X}::Alloc(CThing& thing) -- the registry factory: a new {X} for `thing` (operator new(0x{size:x}) 0x00BFEA1A, null-checked, then
//   the constructor 0x{ctor}), returned as its CTCBase
// Leaf: {leaf}
// Retail: Fable.exe (TLC, VC7.1) function @ 0x{A}, oracle length 29 bytes.
{rb}// Identity: Ego_r.pdb / FableWin.pdb {leaf} (public static __fastcall); the constructor is
//   ??0{X}@@QAE@AAVCThing@@@Z (0x{ctor}, landed). (Landed earlier as a generated free function {X}_Alloc(CThing*) returning {X}*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class {X} : public CTCBase {{
public:
    {X}(CThing& thing);                        // 0x{ctor}
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x{size:x} - 0xC];              // the class's own members (not modelled); sizeof({X}) = 0x{size:x}
}};

CTCBase* __fastcall {X}::Alloc(CThing& thing)
{{
    return new {X}(thing);
}}
'''
plan = []
seen = set()
for l in open(callers):
    tgt, site, row, name = l.rstrip('\n').split('\t')
    if row == '-' or (row, tgt) in seen: continue
    seen.add((row, tgt))
    X = cls_of[tgt]
    f = glob.glob('rebuild/src/compiled/*/*/*_%s.cpp' % row)[0]
    t = open(f, encoding='utf-8', newline='').read()
    src = '%s/%s.cpp' % (outdir, row)
    if os.path.exists(src): t = open(src, encoding='utf-8').read()     # second callee in the same row: build on the first edit
    if '\r\n' in t: plan.append((row, 'MANUAL', '', '', 'CRLF', '-')); continue
    fl = re.search(r'^// Flags:\s*(.*)$', t, re.M); flags = ' '.join(x for x in (fl.group(1).split() if fl else []) if x != 'none')
    parts = name.split('::')
    if name.endswith('::Alloc'):
        Y = '::'.join(parts[:-1])
        leaf = '?Alloc@%s@@SIPAVCTCBase@@AAVCThing@@@Z' % Y
        m = re.search(r'_body\[0x([0-9a-fA-F]+)\]', t)
        if Y == X and m and re.search(r'\w+_Alloc\([\w ]+?\*? ?\w+\)', t) and leaf in pdb:
            rb = re.search(r'^// Retail bytes:.*\n', t, re.M)
            out = ALLOC.format(X=X, size=int(m.group(1), 16), ctor=tgt.upper(), leaf=leaf, A=row.upper(), rb=rb.group(0) if rb else '')
            open(src, 'w', encoding='utf-8').write(out); plan.append((row, 'ALLOC', leaf, flags, 'template', '-')); continue
        plan.append((row, 'MANUAL', leaf, flags, 'alloc not the generated shape' + ('' if leaf in pdb else ' / leaf not in PDB'), '-')); continue
    # generated namespace wrapper around a stand-in: namespace N_<addr> { struct Name { ... }; } using N_<addr>::Name;
    t = re.sub(r'namespace (N_[0-9a-f]{8}) \{ (struct|class) (\w+) \{(.*?)\n\}; \} using \1::\3;', r'\2 \3 {\4\n};', t, flags=re.S)
    # a local class standing in for the callee under a propagated label: its ctor is tagged with the callee address
    mw = re.search(r'^(?:class|struct) (\w+)\b[^\n]*\{[^}]*?\n\s*\1\((?:void|CThing)\s*\*\s*\w*\);\s*// 0x%s\b' % tgt.upper(), t, re.M | re.S)
    renamed = ''
    if mw and mw.group(1) != X and not re.search(r'\b%s\b' % re.escape(X), '\n'.join(l.split('//')[0] for l in t.split('\n'))):
        wrong = mw.group(1); t, _ = code_sub(r'\b%s\b' % re.escape(wrong), X, t)
        t = re.sub(r'^(// [^\n]*?)\bbase %s\(' % re.escape(wrong), r'\1base %s(' % X, t, count=1, flags=re.M)
        t = t.replace('call %s::%s ' % (wrong, wrong), 'call %s::%s ' % (X, X))
        renamed = ' (local stand-in %s renamed)' % wrong
    # respell the callee's local constructor declaration
    t, n = code_sub(r'\b%s\((const )?(?:CThing|void)\s*\*\s*(\w*)\)' % re.escape(X), lambda m: '%s(CThing&%s)' % (X, (' ' + m.group(2)) if m.group(2) else ''), t)
    if n == 0:
        if re.search(r'\b%s\(CThing&' % re.escape(X), t): plan.append((row, 'ALREADY_OK', '', flags, 'declares %s(CThing&) already' % X, '-'))
        else: plan.append((row, 'MANUAL', '', flags, 'callee %s(CThing*) declaration not found' % X, '-'))
        continue
    newc = '-'; leaf = ''
    if len(parts) >= 2 and parts[-1] == parts[-2]:
        Y = parts[-1]; qual = '@'.join(reversed(parts[:-1]))
        leaf = '??0%s@@QAE@AAVCThing@@@Z' % qual
        own = len(re.findall(r'\b%s\((const )?(?:CThing|void)\s*\*\s*\w*\)' % re.escape(Y), t))
        if own:
            if leaf not in pdb: plan.append((row, 'MANUAL', leaf, flags, 'own ctor leaf not in PDB', '-')); continue
            t, n2 = code_sub(r'\b%s\((const )?(?:CThing|void)\s*\*\s*(\w*)\)' % re.escape(Y), lambda m: '%s(CThing&%s)' % (Y, (' ' + m.group(2)) if m.group(2) else ''), t)
            t = re.sub(r'\b(%s::%s)\((?:CThing|void)\*\)' % (re.escape(Y), re.escape(Y)), r'\1(CThing&)', t)
            newc = Y
            if re.search(r'^// Leaf:.*$', t, re.M): t = re.sub(r'^// Leaf:.*$', '// Leaf: ' + leaf, t, count=1, flags=re.M)
            else:
                m2 = re.search(r'^// Retail:.*$', t, re.M) or re.search(r'^//.*$', t, re.M)
                t = t[:m2.start()] + '// Leaf: ' + leaf + '\n' + t[m2.start():]
        else:
            m3 = re.search(r'^// Leaf:\s*(\S+)', t, re.M); leaf = m3.group(1) if m3 else ''
    else:
        m3 = re.search(r'^// Leaf:\s*(\S+)', t, re.M); leaf = m3.group(1) if m3 else ''
    t = re.sub(r'\b(%s::%s)\(CThing\*\)' % (re.escape(X), re.escape(X)), r'\1(CThing&)', t)
    note = '// Respelled 2026-11-16: %s(CThing&) (0x%s) as both PDBs spell it%s.\n' % (X, tgt.upper(), '; this constructor too' if newc != '-' else '')
    m4 = re.search(r'^// Leaf:.*\n', t, re.M) or re.search(r'^//.*\n', t, re.M)
    t = t[:m4.end()] + note + t[m4.end():]
    open(src, 'w', encoding='utf-8').write(t)
    if 'class CThing;' not in t and 'engine/CThing.h' not in t and 'engine/CTCBase.h' not in t:
        m5 = re.search(r'^(class|struct) ', t, re.M); t = t[:m5.start()] + 'class CThing;\n' + t[m5.start():]
        open(src, 'w', encoding='utf-8').write(t)
    plan.append((row, 'RESPELLED', leaf, flags, 'callee ' + X + renamed, newc))
with open(outdir + '/plan.tsv', 'a') as o:
    for p in plan: o.write('\t'.join(p) + '\n')
import collections; print(collections.Counter(p[1] for p in plan)); print(collections.Counter(p[4] for p in plan if p[1] == 'MANUAL').most_common(8))
