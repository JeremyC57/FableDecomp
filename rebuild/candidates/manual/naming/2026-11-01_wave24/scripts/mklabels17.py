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
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W17.tsv'))][1:]
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
    m = re.search(r"([\w:<>, ]+)::`scalar deleting dtor'", d)
    if m: return m.group(1).split(' ')[-1] + "::`scalar deleting destructor'"
    return und.qname(d) if d != sym else None
for q in Q:
    ch, a, fin, ev, ws, o = q[:6]
    if o not in ('respelled', 'respelled-partial'): continue
    t = a.lower().replace('0x', '').zfill(8)
    if not fin.startswith('?'): continue
    if '<' in fin:  # anonymous-namespace data symbol with the TU hash left open: the name without the hash
        m = re.match(r'\?(\w+)@\?A0x', fin); name = "`anonymous namespace'::" + m.group(1) if m else None
    else: name = qn(fin)
    if not name or (t, name) in seen: continue
    seen.add((t, name))
    name = bare(name)
    if SQ(name) in set(SQ(x) for x in L.get(t, ())): continue
    out.append((t, name, 'naming wave 17 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = [
 ('006c75a0', 'std::_Rb_tree<long,std::pair<const long,COpinionDeed*>,std::_Select1st<std::pair<const long,COpinionDeed*> >,std::less<long>,std::allocator<std::pair<const long,COpinionDeed*> > >::clear', 'naming wave 17 (c676 map<long,COpinionDeed*>: the CUnprovenKey/Value placeholders replaced, ?clear@ re-emitted from the real STLport text, RELOCATION_MATCH + PASS (catalog _global::RbTree_Clear_006c75a0))'),
 ('006c7d30', 'std::_Rb_tree<long,std::pair<const long,COpinionDeed*>,std::_Select1st<std::pair<const long,COpinionDeed*> >,std::less<long>,std::allocator<std::pair<const long,COpinionDeed*> > >::_Rb_tree', 'naming wave 17 (c676 map<long,COpinionDeed*>: the tree ctor (catalog std::_Rb_tree<T_node0x18_006c7d30> / ComplexList_Init), RELOCATION_MATCH + PASS)'),
 ('006c7d70', 'std::_Rb_tree<long,std::pair<const long,COpinionDeed*>,std::_Select1st<std::pair<const long,COpinionDeed*> >,std::less<long>,std::allocator<std::pair<const long,COpinionDeed*> > >::~_Rb_tree', 'naming wave 17 (c676 map<long,COpinionDeed*>: the tree dtor (catalog LTextTreeWalkThrough::~LTextTreeWalkThrough is wrong), RELOCATION_MATCH + PASS)'),
 ('006c7de0', 'std::_Rb_tree<long,std::pair<const long,COpinionDeed*>,std::_Select1st<std::pair<const long,COpinionDeed*> >,std::less<long>,std::allocator<std::pair<const long,COpinionDeed*> > >::erase', 'naming wave 17 (c676 map<long,COpinionDeed*>: erase(iterator, iterator) (catalog LTextTreeWalkThrough_EraseRange), RELOCATION_MATCH + PASS)'),
 ('006c8230', 'std::map<long,COpinionDeed*>::map', 'naming wave 17 (c676: the CUnprovenContainer_006c4f80 holder is std::map<long,COpinionDeed*>; its ctor, RELOCATION_MATCH + PASS)'),
 ('006c8270', 'std::map<long,COpinionDeed*>::~map', 'naming wave 17 (c676: the implicit ~map of std::map<long,COpinionDeed*> (catalog LTextTreeWalkThrough::~LTextTreeWalkThrough is wrong), RELOCATION_MATCH + PASS)'),
 ('009aaca0', 'std::_Rb_tree<CCharString,std::pair<const CCharString,NBankFileManager::CContainedBank>,std::_Select1st<std::pair<const CCharString,NBankFileManager::CContainedBank> >,std::less<CCharString>,std::allocator<std::pair<const CCharString,NBankFileManager::CContainedBank> > >::clear', 'naming wave 17 (wave 16 DEFERRED 009aa780 sibling (c667 map<CCharString,NBankFileManager::CContainedBank>), RELOCATION_MATCH + PASS)'),
 ('009ab6b0', 'std::_Rb_tree<CCharString,std::pair<const CCharString,NBankFileManager::CContainedBank>,std::_Select1st<std::pair<const CCharString,NBankFileManager::CContainedBank> >,std::less<CCharString>,std::allocator<std::pair<const CCharString,NBankFileManager::CContainedBank> > >::~_Rb_tree', 'naming wave 17 (009aa780 sibling: the tree dtor (catalog LTextTreeWalkThrough_Dtor is wrong), RELOCATION_MATCH + PASS)'),
 ('009ac0f0', 'std::map<CCharString,NBankFileManager::CContainedBank>::map', 'naming wave 17 (009aa780 sibling: CUnprovenContainer_009aa780 is std::map<CCharString,NBankFileManager::CContainedBank>; its ctor, RELOCATION_MATCH + PASS)'),
 ('009ac130', 'std::map<CCharString,NBankFileManager::CContainedBank>::~map', 'naming wave 17 (009aa780 sibling: the implicit ~map (catalog LTextTreeWalkThrough_Dtor is wrong), RELOCATION_MATCH + PASS)'),
 ('0121ac30', "`anonymous namespace'::`dynamic initializer for 'FORTY_FIVE_DEGREES_ORIENTATION''", 'naming wave 17 (c675: the CTCInventoryItem TU constant at 0x013B8BCC (catalog g_InventoryOrientationRHSet1 / $E_InventoryOrientationRHSet1), RELOCATION_MATCH + PASS)'),
 ('0121acd0', "`anonymous namespace'::`dynamic initializer for 'UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION''", 'naming wave 17 (c675: constant at 0x013B8BE4 (was g_InventoryOrientationRHSet5))'),
 ('0121ad20', "`anonymous namespace'::`dynamic initializer for 'UPSIDE_DOWN_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION''", 'naming wave 17 (c675: constant at 0x0137CFD4 (was g_InventoryOrientationRHSet6))'),
 ('0121ad50', "`anonymous namespace'::`dynamic initializer for 'UPSIDE_DOWN_LEFT_FORTY_FIVE_DEGREES_ORIENTATION''", 'naming wave 17 (c675: constant at 0x0137CFEC (was g_InventoryOrientationRHSet7))'),
 ('013b8bcc', "`anonymous namespace'::FORTY_FIVE_DEGREES_ORIENTATION", 'naming wave 17 (c675: CTCInventoryItem TU constant (FableWin anonymous namespace; the landed GetRHSetFromOrientation 0x005D8EF0 reads it))'),
 ('013b8be4', "`anonymous namespace'::UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION", 'naming wave 17 (c675: CTCInventoryItem TU constant)'),
 ('0137cfd4', "`anonymous namespace'::UPSIDE_DOWN_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION", 'naming wave 17 (c675: CTCInventoryItem TU constant)'),
 ('0137cfec', "`anonymous namespace'::UPSIDE_DOWN_LEFT_FORTY_FIVE_DEGREES_ORIENTATION", 'naming wave 17 (c675: CTCInventoryItem TU constant)'),
 ('0137cf74', "`anonymous namespace'::DEFAULT_ORIENTATION", 'naming wave 17 (c675: CTCInventoryItem TU constant returned by 0x005D9730 (was g_DefaultRHSet))'),
 ('01224bc0', "`dynamic initializer for 'g_AudioContext''", 'naming wave 17 (c675: the static-init of the audio context object at 0x0143BDB0 (catalog g_CStatic_00c55cd0_0143bdb0), RELOCATION_MATCH + PASS)'),
]
for t, n, e in EXTRA:
    if SQ(n) not in set(SQ(x) for x in L.get(t, ())) and (t, n) not in seen: out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
