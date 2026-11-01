# h17_stl.py: naming wave 17, family stl -- STLport 4.6 _Rb_tree instantiations whose identity the c667/c675/c676 chunks proved.
#  (1) placeholder families: the landed STLport text of every row of the instantiation is kept verbatim; the per-instantiation
#      placeholders CUnprovenKey/Value/KeyOfValue_<K> become the proven types, and the CUnprovenContainer_<K> holder becomes the
#      real STLport std::map (stl/_map.h text as the landed map ctor 0x00AD3960 carries it):
#        006c4f80: map<long, COpinionDeed*> (~CTCHeroOpinionDeedLog 0x006C5E5B on OpinionDeedMap +0xC, Ego_r member type; c676)
#        009aa780: map<CCharString, NBankFileManager::CContainedBank> (~CRetailBank 0x009AC7C0 on ContainedBanks +0x18; c667, wave 16)
#  (2) the CTreeOwner stand-ins of trivially destroyed trees, re-authored on the same STLport text (the 0x009AA780 TU):
#        004cf810: set<unsigned long> (CIndexList temporary's destructor in 0x00AC8840; c675)
#        00527910: map<EGameAction, pair<C3DVector,long> > (UpdateWalkingControlForces 0x006AD260 map clear on +0x1A8; c676)
# The Leaf line is the symbol the TU defines for the graded member (read back from the object). Name-only: the recheck proves the bytes.
import os, sys, re, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import he
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
import verify_and_land as V, coff
R = he.R
def defsyms(a, p):
    d = os.path.join(he.W, 'pw', 'h17stl'); os.makedirs(d, exist_ok=True); o = os.path.join(d, a + '.obj')
    if os.path.exists(o): os.unlink(o)
    fl = R[a].get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
    r = subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, p]), capture_output=True, text=True, stdin=subprocess.DEVNULL, env=V.env())
    if not os.path.exists(o): raise SystemExit('compile %s: %s' % (a, [l for l in r.stdout.splitlines() if 'error' in l][:3]))
    return coff.syms(o)[0]
MAPCLS = '''template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef _Tp data_type;
    typedef _Tp mapped_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::allocator_type allocator_type;
    _Rep_type _M_t;
    map() : _M_t(_Compare(), allocator_type()) { }
    explicit map(const _Compare& __comp, const allocator_type& __a = allocator_type()) : _M_t(__comp, __a) { }
};
'''
FAM = {
 '00ad2790': dict(key='unsigned long', set=True, chunk='c677', prelude='',
   ident='// Identity (proven, chunk c677): std::set<unsigned long> find (QuadricOptimiserInternals::CIndexList) -- called from\n'
         '//   CIndexList::HasIndex 0x00AC7F90 (Ego_r ?HasIndex@CIndexList@QuadricOptimiserInternals@@QBE_NK@Z, const: so the const overload,\n'
         '//   returning const_iterator) and IsIllegal 0x00AC8360; unsigned key (jb/jae).\n',
   extra=[('>::iterator _Rb_tree<unsigned long, unsigned long, std::_Identity<unsigned long >, less<unsigned long >, allocator<unsigned long > >::find(const unsigned long&);',
           '>::const_iterator _Rb_tree<unsigned long, unsigned long, std::_Identity<unsigned long >, less<unsigned long >, allocator<unsigned long > >::find(const unsigned long&) const;')],
   rows=['00ad2790']),
 '006c4f80': dict(key='long', mapped='COpinionDeed*', chunk='c676',
   prelude='class COpinionDeed;\n',
   ident='// Identity (proven, chunk c676): ~CTCHeroOpinionDeedLog 0x006C5E5B calls _M_erase on OpinionDeedMap (+0xC), Ego_r.pdb\n'
         '//   std::map<long, COpinionDeed*> (the class member type); node 0x18 = 0x10 header + pair<const long, COpinionDeed*> (signed key: jl/jge).\n',
   rows=['006c4f80', '006c75a0', '006c7d30', '006c7d70', '006c7de0', '006c8230', '006c8270']),
 '009aa780': dict(key='CCharString', mapped='NBankFileManager::CContainedBank', chunk='c667',
   prelude='''namespace NBankFileManager {
class CContainedBank {
public:
    unsigned long DataVersion;                          // +0x00
    unsigned long EntryCount;                           // +0x04
    unsigned long InfoOffset;                           // +0x08
    unsigned long InfoSize;                             // +0x0C
    unsigned long Alignment;                            // +0x10
};
} // namespace NBankFileManager
''',
   ident='// Identity (proven, chunk c667): ~CRetailBank 0x009AC7C0 calls _M_erase 0x009AA780 on ContainedBanks +0x18, Ego_r.pdb\n'
         '//   std::map<CCharString, NBankFileManager::CContainedBank> (the class\'s only tree); node 0x28 = 0x10 header + pair<const CCharString,\n'
         '//   CContainedBank> (4 + 0x14); CContainedBank as the landed ~CRetailBank 0x009AC7C0 row models it.\n',
   rows=['009aaca0', '009ab6b0', '009ac0f0', '009ac130']),
}
def subst(a, K, f):
    p, rel, t = he._rw(a, 'src')
    if 'CUnproven' not in t: print('already', a); return
    val = f['key'] if f.get('set') else 'std::pair<const %s, %s >' % (f['key'], f['mapped'])
    fn0 = re.search(r'^// Leaf: (\S+)', t, re.M).group(1); t = t.replace('// Leaf: ' + fn0, '// Leaf: LEAF_PENDING')
    kov = ('std::_Identity<%s >' if f.get('set') else 'std::_Select1st<%s >') % val
    # header: drop the "Types: unproven" block, add identity + wave note
    lines = t.split('\n'); out = []; i = 0; skip = False
    for l in lines:
        if l.startswith('// Types: unproven'): skip = True; out.append(f['ident'].rstrip('\n')); out.append('// Naming wave 17: the per-instantiation placeholders CUnprovenKey/Value/KeyOfValue_%s replaced by the proven types (name-only).' % K); continue
        if skip and l.startswith('//   '): continue
        skip = False; out.append(l)
    t = '\n'.join(out)
    t = re.sub(r'(?m)^(struct CUnproven\w+_%s\b.*|inline bool operator<\(const CUnprovenKey_%s.*)\n' % (K, K), '', t, count=0)
    t = t.replace('namespace std {', f['prelude'] + 'namespace std {', 1) if f['prelude'] not in t else t
    cont = re.search(r'class CUnprovenContainer_%s \{.*?\n\};\n(CUnprovenContainer_%s::(~?)CUnprovenContainer_%s\(\).*\n?)' % (K, K, K), t, re.S)
    if cont:
        dtor = cont.group(2) == '~'
        inst = '' if dtor else 'template map<%s, %s >::map();\n' % (f['key'], f['mapped'])
        emit = ('\n// The implicit ~map (STLport declares none) inlines ~_Rb_tree: clear(), then free the header node. Retail emitted it out of\n'
                '//   line; these emitters (not part of the graded body) make the TU emit that COMDAT: Use_ first generates the implicit\n'
                '//   destructor at full inlining (its body), Kill_ then calls it with inlining off (the out-of-line reference).\n'
                'void Use_%s(std::map<%s, %s >* m) { m->~map(); }\n'
                '#pragma inline_depth(0)\nvoid Kill_%s(std::map<%s, %s >* m) { m->~map(); }\n#pragma inline_depth()\n' % (a, f['key'], f['mapped'], a, f['key'], f['mapped'])) if dtor else ''
        t = t[:cont.start()] + t[cont.end():]
        k = t.rstrip().rfind('}')
        t = t[:k] + MAPCLS + inst + t[k:].rstrip('\n') + '\n' + emit
        t = re.sub(r'CUnprovenContainer_%s::(~?)CUnprovenContainer_%s\(\)' % (K, K), lambda m: 'std::map<%s, %s >::%smap()' % (f['key'], f['mapped'], m.group(1)), t)
    for ph, rep in (('CUnprovenKeyOfValue_' + K, kov), ('CUnprovenValue_' + K, val), ('CUnprovenKey_' + K, f['key'])):
        t = t.replace(ph, rep)
    for o, n in f.get('extra', ()):
        if o in t: t = t.replace(o, n)
    t = t.replace('> >', '> >').replace('>>', '> >')
    he._w(p, t)
    fn = fn0
    tok = re.match(r'(\?\?[0-9_A-Z]|\?\w+@)', fn).group(1)
    D = defsyms(a, p)
    if cont: cand = [s for s in D if s.startswith(('??0?$map@' if not dtor else '??1?$map@'))]
    else: cand = [s for s in D if s.startswith(tok + ('?$_Rb_tree@' if tok.startswith('??') else '?$_Rb_tree@')) and 'CUnproven' not in s]
    if len(cand) > 1: cand = [s for s in cand if s[-3:] == fn[-3:]]
    if len(cand) != 1: raise SystemExit('leaf %s: %s' % (a, cand))
    t = t.replace('// Leaf: LEAF_PENDING', '// Leaf: ' + cand[0]); he._w(p, t); print('ok', a, cand[0][:120])
def regen(a, inst_typedefs, inst, prelude, title, ident):
    base = open(os.path.join(he.rows.ROOT, 'rebuild/src/compiled', R['009aa780']['Source']), encoding='utf-8', newline='').read()
    cut = base.index('// Model: STLport 4.6')
    head = title + '\n// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 51 bytes.\n' % a.upper() + ident
    body = base[cut:]
    body = re.sub(r'Replaces the invented stand-in \(was [^)]*\)\.', 'Replaces the invented stand-in (was _unattributed::FreeTree_CTreeOwner_%s).' % a, body)
    s = body.index('class CCharStringData;'); e = body.index('namespace std {')
    body = body[:s] + prelude + body[e:]
    body = re.sub(r'typedef pair<const CCharString, NBankFileManager::CContainedBank> _ContainedBankPair;\ntemplate void .*\n', inst_typedefs + inst + '\n', body)
    p, rel, t0 = he._rw(a, 'src')
    he._w(p, head + body)
    D = defsyms(a, p)
    cand = [s for s in D if s.startswith('?_M_erase@?$_Rb_tree@')]
    if len(cand) != 1: raise SystemExit('leaf %s: %s' % (a, cand))
    t = head + body
    t = t.replace('// Model: STLport 4.6', '// Leaf: %s\n// Model: STLport 4.6' % cand[0], 1)
    # Leaf after Model block (as the 0x009AA780 text has it)
    lines = t.split('\n'); li = [i for i, l in enumerate(lines) if l.startswith('// Leaf: ')][0]; leaf = lines.pop(li)
    mi = max(i for i, l in enumerate(lines) if l.startswith('//') and i < 20 and not lines[i + 1].startswith('//   ') or False) if False else None
    k = next(i for i, l in enumerate(lines) if l.startswith('// Model: STLport'))
    while lines[k + 1].startswith('//   '): k += 1
    lines.insert(k + 1, leaf); t = '\n'.join(lines)
    he._w(p, t); print('ok', a, cand[0][:120])
only = set(sys.argv[1:])
for K, f in FAM.items():
    for a in f['rows'] + ([K] if K not in f['rows'] and K != '009aa780' else []):
        if only and a not in only: continue
        subst(a, K, f)
if not only or '004cf810' in only:
    regen('004cf810', '', 'template void _Rb_tree<unsigned long, unsigned long, _Identity<unsigned long>, less<unsigned long>, allocator<unsigned long> >::_M_erase(_Rb_tree_node<unsigned long>*);', '',
          '// std::_Rb_tree<unsigned long, unsigned long, std::_Identity<unsigned long>, std::less<unsigned long>, std::allocator<unsigned long> >::_M_erase(_Rb_tree_node<unsigned long>*)\n'
          '//   -- STLport 4.6 _M_erase(_Link_type) of std::set<unsigned long>: erase without rebalancing -- recurse right, free the node, continue left',
          '// Identity (proven, chunk c675): the CIndexList (std::set<unsigned long>) temporary\'s destructor in\n'
          '//   GetRemainingVertices 0x00AC8840 calls it; node 0x14 = 0x10 header + unsigned long (trivially destroyed: no destructor call).\n'
          '// Naming wave 17: landed as the invented CTreeOwner_004cf810::FreeTree_CTreeOwner_004cf810(SNode*) (child +0xC = _M_right,\n'
          '//   sibling +0x8 = _M_left); re-authored on the real STLport text (name-only: bytes unchanged).\n')
if not only or '00527910' in only:
    regen('00527910', 'typedef pair<C3DVector, long> _AccelerationPair;\ntypedef pair<const EGameAction, _AccelerationPair> _GameActionAccelerationPair;\n',
          'template void _Rb_tree<EGameAction, _GameActionAccelerationPair, _Select1st<_GameActionAccelerationPair>, less<EGameAction>, allocator<_GameActionAccelerationPair> >::_M_erase(_Rb_tree_node<_GameActionAccelerationPair>*);',
          'enum EGameAction { };   // Ego_r.pdb enum (enumerators not needed by this body)\nclass C3DVector {\npublic:\n    float X;\n    float Y;\n    float Z;\n};\n',
          '// std::_Rb_tree<EGameAction, std::pair<const EGameAction, std::pair<C3DVector, long> >, std::_Select1st<...>, std::less<EGameAction>,\n'
          '//   std::allocator<...> >::_M_erase(_Rb_tree_node<...>*) -- STLport 4.6 _M_erase(_Link_type): erase without rebalancing -- recurse right,\n'
          '//   free the node, continue left',
          '// Identity (proven, chunk c676): UpdateWalkingControlForces 0x006AD260 clears the +0x1A8 std::map<EGameAction, std::pair<C3DVector,long> >\n'
          '//   (Ego_r CThingPlayerCreature member type) and vland binds this symbol of that RELOCATION_MATCH TU to 0x00527910; the value is\n'
          '//   trivially destroyed (no destructor call).\n'
          '// Naming wave 17: landed as the invented CTreeOwner_00527910::FreeTree_CTreeOwner_00527910(SNode*) (child +0xC = _M_right,\n'
          '//   sibling +0x8 = _M_left); re-authored on the real STLport text (name-only: bytes unchanged).\n')
if not only or '00882bc0' in only:
    regen('00882bc0', 'typedef NScreenEffect::CScreenEffectPriorityStack::CStackEntry* _StackEntryPtr;\n',
          'template void _Rb_tree<_StackEntryPtr, _StackEntryPtr, _Identity<_StackEntryPtr>, less<_StackEntryPtr>, allocator<_StackEntryPtr> >::_M_erase(_Rb_tree_node<_StackEntryPtr>*);',
          'namespace NScreenEffect {\nclass CScreenEffectPriorityStack {\npublic:\n    struct CStackEntry;\n};\n} // namespace NScreenEffect\n',
          '// std::_Rb_tree<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, ..., std::_Identity<...>, std::less<...>, std::allocator<...> >::_M_erase(_Rb_tree_node<...>*)\n'
          '//   -- STLport 4.6 _M_erase(_Link_type) of std::set<CStackEntry*>: erase without rebalancing -- recurse right, free the node, continue left',
          '// Identity (proven, chunk c677): the out-of-line erase of the CScreenEffectPriorityStack key sets: Clear 0x00881210 and\n'
          '//   ~CScreenEffectPriorityStack 0x008814C0 call it (vland RELOC_NAME); STLport _M_erase is private (AAE); pointer keys are trivially destroyed.\n'
          '// Naming wave 17: landed as the invented CTreeOwner_00882bc0::FreeTree_CTreeOwner_00882bc0(SNode*) (child +0xC = _M_right,\n'
          '//   sibling +0x8 = _M_left); re-authored on the real STLport text (name-only: bytes unchanged).\n')
