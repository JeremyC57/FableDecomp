# mklabels16.py: LABELS.tsv -- for every queue target this wave respelled (fully or partly) and the decision / wave-14 targets,
# the qualified name of the settled spelling, when no ghidra_out/labels_*.tsv row already carries that exact name.
import os, sys, re, glob, collections
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(W, 'scripts'))
import und
L = collections.defaultdict(set)
for p in glob.glob('/home/user/FableDecomp/ghidra_out/labels_*.tsv'):
    for l in open(p, errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2: L[q[0].lower().replace('0x', '').zfill(8)].add(q[1])
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W18.tsv'))][1:]
out = [('address', 'name', 'evidence')]
seen = set()
def bare(n):
    # the catalog's bare spelling: no class/struct/enum/union keywords, `const T`, `T*`, no space after a comma, `> >`,
    # and a ctor / dtor named without its repeated template arguments
    n = re.sub(r'\b(class|struct|enum|union) ', '', n)
    n = re.sub(r'([\w:]+(?:<[^<>]*>)?) const\b', r'const \1', n)
    n = re.sub(r'\s*\*', '*', n); n = re.sub(r',\s+', ',', n)
    while '>>' in n: n = n.replace('>>', '> >')
    d = 0; cut = -1
    for i, ch in enumerate(n):
        if ch == '<': d += 1
        elif ch == '>': d -= 1
        elif ch == ':' and d == 0 and n[i:i + 2] == '::': cut = i
    if cut > 0:
        head, tail = n[:cut], n[cut + 2:]
        cls = re.split(r'::', re.sub(r'<.*', '', head))[-1]
        mm = re.match(r'^(~?)(\w+)<.*>$', tail)
        if mm and mm.group(2) == cls: n = head + '::' + mm.group(1) + mm.group(2)
    return n
def SQ(n):
    n = re.sub(r'\s+', '', re.sub(r'\b(class|struct|enum|union) ', '', n))
    prev = None
    while prev != n: prev = n; n = re.sub(r',std::allocator<[^<>]*(?:<[^<>]*>[^<>]*)*>', '', n)
    return n
def qn(sym):
    d = und.get([sym])[sym]
    m = re.search(r"([\w:<>,*&]+)::`scalar deleting dtor'", re.sub(r',\s+', ',', re.sub(r'\b(class|struct|enum|union) ', '', d)))
    if m: return m.group(1).split(' ')[-1] + "::`scalar deleting destructor'"
    return und.qname(d) if d != sym else None
for q in Q:
    ch, a, fin, ev, ws, o = q[:6]
    if o not in ('respelled', 'respelled-partial'): continue
    t = a.lower().replace('0x', '').zfill(8)
    if not fin.startswith('?') or fin == '?g_framesPerSecond@@3JA': continue   # the c682 descriptive row of 0x01375550: the cg10 ConstantFPS spelling was applied
    if '<' in fin:  # anonymous-namespace data symbol with the TU hash left open: the name without the hash
        m = re.match(r'\?(\w+)@\?A0x', fin); name = "`anonymous namespace'::" + m.group(1) if m else None
    else: name = qn(fin)
    if not name or (t, name) in seen: continue
    seen.add((t, name))
    name = bare(name).lstrip('*& ')
    if SQ(name) in set(SQ(x) for x in L.get(t, ())): continue
    out.append((t, name, 'naming wave 18 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = [
 ('00e7ef70', 'NScript::CV_FishingCompetitionScript::CFishCompOrganiser::CFishCompOrganiser', 'naming wave 18 (qctor, c681): the nested-class ctor ??0CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@QAE@AAVCV_FishingCompetitionScript@2@PAVCScriptDataBase@@ABVCScriptThing@@@Z; re-authored byte-identical and rechecked'),
 ('00994b40', 'CMemoryDataOutputStream::GetLength', 'naming wave 18 (own, c681 / DECISIONS_W17): ?GetLength@CMemoryDataOutputStream@@QBEKXZ const (was CCreatureActionBase::GetFramesRemaining); re-authored byte-identical (MATCH) and rechecked'),
 ('004e34b2', 'std::vector<std::pair<long,CCharString> >::~vector', 'naming wave 18 (stl, c678): ??1?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@QAE@XZ -- the label from the chunk queue; the source re-author is DEFERRED (recheck-fail)'),
 ('004dd782', 'std::__destroy_aux<std::pair<long,CCharString>*>', 'naming wave 18 (stl, c678): ??$__destroy_aux@PAU?$pair@JVCCharString@@@std@@@std@@YIXPAU?$pair@JVCCharString@@@0@0ABU__false_type@0@@Z (the ~vector destroy chain; source re-author DEFERRED)'),
 ('004da68a', "std::pair<long,CCharString>::`scalar deleting destructor'", 'naming wave 18 (stl, c678): ??_G?$pair@JVCCharString@@@std@@QAEPAXI@Z, emitted by the __destroy_aux chain under optimize("s"); re-authored byte-identical and rechecked'),
]
for t, n, e in EXTRA:
    if SQ(n) not in set(SQ(x) for x in L.get(t, ())) and (t, n) not in seen: out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
