# fam_filt24.py (from fam_filt23.py) <rows.txt> [rename.tsv]: naming wave 24, family filt -- DECISIONS_W22_RESOLVED item 2. Retail's CThingSearchTools templates
# call the filter on a reference (`filter(*p)`), CFilter_Bind2/3 hold their filters by pointer (Ego_r.pdb F1/F2/F3) and their member
# template operator()(const T&) forwards the reference (FableWin.pdb ??$?RV...@?$CFilter_Bind2@...@@QBE_NAB...@Z), and every
# CThingFilter_X::operator() takes the PDB `const T&` (??RCThingFilter_X@@QBE_NABV<T>@@@Z). Re-models the row text accordingly:
#   namespace stand-ins (namespace N_xxx { struct CThingFilter_X {...}; } using ...) -> the global class;
#   filter operator()(const CThing* p) -> operator()(const <PDB T>& p);
#   Bind operator()(T* p) -> operator()(const T& p) (standard First/Second blocks -> the PDB pointer members F1/F2/F3);
#   call sites filter(p) / filter((TThing*)p) -> filter(*p) / filter(*(TThing*)p).
# rename.tsv: "<row>\t<old class>\t<new class>" re-identifications (callee address label + PDB instantiation, see DECISIONS_W23).
# Overlay via he.py ($HE_OV). Prints ok/skip per row.
import sys, os, re
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hand24'))
import he
PDB = {}
for f in ('egor_pdb_names.tsv', 'fablewin_pdb_names.tsv'):
    for l in open('/home/user/FableDecomp/ghidra_out/' + f, errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2:
            m = re.match(r'^\?\?R(CThingFilter_\w+)@@QBE_NABV(\w+)@@@Z$', q[1])
            if m: PDB.setdefault(m.group(1), set()).add(m.group(2))
N = '// Naming (naming wave 24): the search templates call the filter on a reference (`filter(*p)`); CFilter_Bind2/3 hold their filters by pointer (Ego_r.pdb F1/F2/F3) and forward the reference; each CThingFilter_X::operator() takes its PDB `const T&` (DECISIONS_W22_RESOLVED item 2; wave 24: the six rows DECISIONS_W23_RESOLVED item 5 held out, re-identified from the filter addresses they call).'
rows = [l.split()[0] for l in open(sys.argv[1]) if l.strip()]
REN = {}
if len(sys.argv) > 2:
    for l in open(sys.argv[2]):
        if l.strip() and not l.startswith('#'):
            a, o, n = l.rstrip('\n').split('\t')[:3]; REN.setdefault(a, []).append((o, n))
BIND2 = re.compile(r'template <class A, class B> class CFilter_Bind2 \{ public:[^\n]*\n    const A& First;\n    const B& Second;\n    CFilter_Bind2\(const A& a, const B& b\) : First\(a\), Second\(b\) \{\}\n    template <class T> bool operator\(\)\(T\* thing\) const \{ return First\(thing\) && Second\(thing\); \}\n\};')
BIND3 = re.compile(r'template <class A, class B, class C> class CFilter_Bind3 \{ public:[^\n]*\n    const A& First;\n    const B& Second;\n    const C& Third;\n    CFilter_Bind3\(const A& a, const B& b, const C& c\) : First\(a\), Second\(b\), Third\(c\) \{\}\n    template <class T> bool operator\(\)\(T\* thing\) const \{ return First\(thing\) && Second\(thing\) && Third\(thing\); \}\n\};')
B2 = ('template <class A, class B> class CFilter_Bind2 { public:     // Ego_r.pdb: both filters held by pointer\n'
      '    const A* F1;\n    const B* F2;\n    CFilter_Bind2(const A& f1, const B& f2) : F1(&f1), F2(&f2) {}\n'
      '    template <class T> bool operator()(const T& thing) const { return (*F1)(thing) && (*F2)(thing); }\n};')
B3 = ('template <class A, class B, class C> class CFilter_Bind3 { public:     // Ego_r.pdb: the filters held by pointer\n'
      '    const A* F1;\n    const B* F2;\n    const C* F3;\n    CFilter_Bind3(const A& f1, const B& f2, const C& f3) : F1(&f1), F2(&f2), F3(&f3) {}\n'
      '    template <class T> bool operator()(const T& thing) const { return (*F1)(thing) && (*F2)(thing) && (*F3)(thing); }\n};')
for a in rows:
    p, rel, t = he._rw(a, 'src'); t0 = t
    if N in t: print('already', a); continue
    why = []
    for o, n in REN.get(a, []):
        t = re.sub(r'\b' + o + r'\b', n, t)
        t = t.replace(' (manifest: %s)' % n, ''); t = t.replace(n + '::operator() 0x', n + '::operator() (Identity, naming wave 24: was ' + o + '; FableWin.pdb instantiates this template on ' + n + ' and the callee label names it) 0x', 1)
    # namespace stand-ins -> global class
    t = re.sub(r'namespace (N_?\w+) \{ struct (CThingFilter_\w+) \{(.*?)\n\}; \} using \1::\2;', r'class \2 { public:\3\n};', t, flags=re.S)
    # filter operators
    def cls(m):
        name, body = m.group(1), m.group(2)
        T = PDB.get(name)
        if not T or len(T) != 1:
            why.append('nopdb:' + name); return m.group(0)
        T = list(T)[0]
        b2, c = re.subn(r'bool operator\(\)\((?:const )?\w+\s*\*\s*(\w+)\) const;', lambda q: 'bool operator()(const %s& %s) const;' % (T, q.group(1)), body)
        if c != 1 and 'operator()(const %s&' % T not in body: why.append('op%d:%s' % (c, name))
        return 'class %s { public:%s\n};' % (name, b2)
    t = re.sub(r'class (CThingFilter_\w+) \{ public:(.*?)\n\};', cls, t, flags=re.S)
    t = BIND2.sub(lambda m: B2, t); t = BIND3.sub(lambda m: B3, t)
    t = re.sub(r'template <class T> bool operator\(\)\(T\* thing\) const \{ return ([^\n]*?) \}', r'template <class T> bool operator()(const T& thing) const { return \1 }', t)
    # non-template adaptor operators taking the thing pointer -> the PDB member template on const T&
    def bindop(m):
        return re.sub(r'(__forceinline )?bool operator\(\)\((?:const )?CThing\s*\* thing\) const \{ return ([^\n]*?) \}',
                      lambda q: 'template <class T> %sbool operator()(const T& thing) const { return %s }' % (q.group(1) or '', q.group(2)), m.group(0))
    t = re.sub(r'class CFilter_Bind\d[^\n]*\{ public:.*?\n\};', bindop, t, flags=re.S)
    sc = 'static_cast<T*>(thing)' in t
    t, c = re.subn(r'\bfilter\(((?:\(\w+\*\))?\w+)\)', lambda q: ('filter(*static_cast<T*>(thing))' if sc and q.group(1) == 'thing' else 'filter(*%s)' % q.group(1)), t)
    if c == 0: why.append('nocall')
    if why:
        print('skip', a, ' '.join(why)); continue
    m = re.search(r'^// Leaf: [^\n]*\n', t, re.M); i = m.end() if m else t.index('\n') + 1
    t = t[:i] + N + '\n' + t[i:]
    he._w(p, t); print('edited', a)
