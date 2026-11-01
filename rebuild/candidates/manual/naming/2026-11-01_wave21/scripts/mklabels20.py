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
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W20.tsv'))][1:]
out = [('address', 'name', 'evidence')]
seen = set()
def bare(n):
    # the catalog's bare spelling: no class/struct/enum/union keywords, `const T`, `T*`, no space after a comma, `> >`,
    # and a ctor / dtor named without its repeated template arguments
    n = re.sub(r'\b(class|struct|enum|union) ', '', n)
    n = re.sub(r'\b((?:unsigned |signed )?(?:long long|long|int|short|char)) const\b', r'const \1', n)
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
FIX = {  # queue spellings the wave proved wrong / display-string finals: the settled mangled spelling
 '007808b0': '?InitFirePoints@CTCFlammable@@AAEXXZ',
 '00b21850': '?TransformFromPosition@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@M@Z',
 '00781670': '?find@?$_Rb_tree@VCDefString@@U?$pair@$$CBVCDefString@@J@std@@U?$_Select1st@U?$pair@$$CBVCDefString@@J@std@@@3@U?$less@VCDefString@@@3@V?$allocator@U?$pair@$$CBVCDefString@@J@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCDefString@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBVCDefString@@J@std@@@2@@2@ABVCDefString@@@Z',
 '0042b467': '?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@ABEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@std@@@2@ABVCCharString@@@Z',
 '00c61c60': '?GetCount@CPKArray@@QBEKXZ', '00c61bc0': '?GetCapacity@CPKArray@@QBEKXZ', '00c44a90': '?GetVolume@CAmbientEmitter@@QBEMXZ',
 '00c08800': '??0CSoundSystemConfig@@QAE@XZ', '00c087c0': '?SetChannel@CSoundSystemConfig@@QAEXK_N@Z', '00aac530': '??0CSoundEmitterLH@@QAE@PAVCSoundManagerLH@@@Z',
 '00ad4e20': '?_M_insert_overflow@?$vector@VCGroup@COptimisedVertex@QuadricOptimiserInternals@@V?$allocator@VCGroup@COptimisedVertex@QuadricOptimiserInternals@@@std@@@std@@IAEXPAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@ABV345@ABU__false_type@2@I_N@Z',
 '00ad7550': '??$sort@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@P6I_NABV123@0@Z@std@@YIXPAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@0P6I_NABV123@1@Z@Z',
 '00c36c70': '?Lookup@CAmbientScene@@QAEPAUCSequenceItem@@K@Z', '00c46da0': '?Add@SPosition2D@@QAEXABUCIndexedValue@@@Z', '00c73a00': '?ClassifyAudioExtension@CString_00c73a00@@QBEJXZ',
 '00c08cf0': '?SetSoundAllocator_00c08cf0@@YGXPAVCAllocator@LHA@@@Z', '00c08d00': '?SetSoundTempAllocator_00c08d00@@YGXPAVCAllocator@LHA@@@Z', '00c08d10': '?SetSoundPhysicalAllocator_00c08d10@@YGXPAVCAllocator@LHA@@@Z',
 '00cb09a0': '?WaitDispatchingMessages@@YGKPAXKPAUHWND__@@I0@Z', '00a3ef00': '??0CSoundManagerLH@@QAE@ABVCSoundManagerInit@@_N@Z',
}
for q in Q:
    ch, a, fin, ev, ws, o = q[:6]
    if o not in ('respelled', 'respelled-partial'): continue
    t = a.lower().replace('0x', '').zfill(8)
    fin = FIX.get(t, fin)
    if not fin.startswith('?') or fin == '?g_framesPerSecond@@3JA': continue   # the c682 descriptive row of 0x01375550: the cg10 ConstantFPS spelling was applied
    if '<' in fin:  # anonymous-namespace data symbol with the TU hash left open: the name without the hash
        m = re.match(r'\?(\w+)@\?A0x', fin); name = "`anonymous namespace'::" + m.group(1) if m else None
    else: name = qn(fin)
    if not name or (t, name) in seen: continue
    seen.add((t, name))
    name = bare(name).lstrip('*& ')
    if SQ(name) in set(SQ(x) for x in L.get(t, ())): continue
    out.append((t, name, 'naming wave 20 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = [
 ('0043020a', 'std::_Rb_tree<CCharString,std::pair<const CCharString,long>,std::_Select1st<std::pair<const CCharString,long> >,std::less<CCharString>,std::allocator<std::pair<const CCharString,long> > >::_M_erase', 'naming wave 20 (erase): the sibling of the c686 _M_find row, re-authored on the STLport _tree text (compiler Leaf); callers / own row respelled and rechecked'),
 ('00cbdf70', 'std::sort<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*,CCompareString<CCountedPointer<CEntityScriptBindingBase> > >', 'naming wave 20 (sort): the STLport sort instantiation of CScriptManager (binds __introsort_loop 0x00CBDAD0); re-authored byte-identical'),
 ('00cbd420', 'std::__final_insertion_sort<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*,CCompareString<CCountedPointer<CEntityScriptBindingBase> > >', 'naming wave 20 (sort): STLport text, byte-identical'),
 ('00cbb2d0', 'std::__unguarded_insertion_sort_aux<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*,std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >,CCompareString<CCountedPointer<CEntityScriptBindingBase> > >', 'naming wave 20 (sort): STLport text, byte-identical'),
 ('00b218b0', 'NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformFromIndex', 'naming wave 20 (spline): ?TransformFromIndex@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@J@Z (PDB name set)'),
 ('00b21940', 'NParticleEngine::CPSCSplineBase::GetPointAtPos', 'naming wave 20 (spline): ?GetPointAtPos@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@MM@Z (PDB name set)'),
 ('00b219a0', 'NParticleEngine::CPSCSplineBase::GetPointAtIndex', 'naming wave 20 (spline): ?GetPointAtIndex@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@JM@Z (PDB name set)'),
 ('007e6ec0', 'CThingSearchTools::GetPFirstThing<CThingFilter_HasDefName,CThing>', 'naming wave 20 (misc, DECISIONS_W19 item 6): the filter is CThingFilter_HasDefName (callee 0x006A7BE0 = HasDefName::operator(); 0x006A80A0 is HasTC); rebuilt from the landed 0x0056D050 twin, byte-identical'),
 ('00a3a410', 'CWADBank::AddFile', 'naming wave 20 (c686a): ?AddFile@CWADBank@@QAEXABVCWideString@@@Z (PDB) in UpdateMetaData 0x00502170'),
 ('00b6d250', 'std::vector<CTVertexLandscapeBackground,std::allocator<CTVertexLandscapeBackground> >::_M_clear', 'naming wave 20 (c688): ?_M_clear@?$vector@VCTVertexLandscapeBackground@@..@@IAEXXZ -- the _M_clear of the chunk 688 vector (its _M_insert_overflow 0x00B6D0B0 calls it); re-authored byte-identical'),
 ('00be4380', 'std::vector<CTVertexLandscapeBackground,std::allocator<CTVertexLandscapeBackground> >::push_back', 'naming wave 20 (c688): STLport push_back (calls _M_insert_overflow 0x00B6D0B0 with (finish, x, tag, 1, true)); re-authored byte-identical; not in FableWin.pdb (never grown in the debug build)'),
 ('0051b620', 'std::vector<CLevelLoader::CObject::CLevelFiles,std::allocator<CLevelLoader::CObject::CLevelFiles> >::PushBackEmpty', 'naming wave 20 (c688): the vector member (STLport push_back() shape, name unproven -- DEFERRED) on the real vector<CLevelLoader::CObject::CLevelFiles> model'),
 ('00ac97d0', 'QuadricOptimiserInternals::COptimisedVertex::RemapGroups', 'naming wave 20 (erase): _M_erase private (AAE) on its map model; unchanged name'),
 ('00c16690', 'StoreSoundAllocator_00c16690', 'naming wave 20 (c687): descriptive (libpk): the __fastcall store of the sound allocator global the respelled SetSoundAllocator_00c08cf0 0x00C08CF0 calls; own row DEFERRED (carries CHoverable_ProcessEvent)'),
 ('00c166d0', 'StoreSoundTempAllocator_00c166d0', 'naming wave 20 (c687): descriptive (libpk): the store SetSoundTempAllocator_00c08d00 0x00C08D00 calls; own row DEFERRED'),
 ('00c166b0', 'StoreSoundPhysicalAllocator_00c166b0', 'naming wave 20 (c687): descriptive (libpk): the store SetSoundPhysicalAllocator_00c08d10 0x00C08D10 calls; own row DEFERRED'),
]
for t, n, e in EXTRA:
    if SQ(n) not in set(SQ(x) for x in L.get(t, ())) and (t, n) not in seen and not any(r[0] == t and SQ(r[1]) == SQ(n) for r in out[1:]): out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
