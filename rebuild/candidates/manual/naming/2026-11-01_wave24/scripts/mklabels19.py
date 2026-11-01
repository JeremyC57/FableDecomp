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
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W19.tsv'))][1:]
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
    out.append((t, name, 'naming wave 19 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = [
 ('00c25860', 'C3DVector::Cross', 'naming wave 19 (c680): ?Cross@C3DVector@@QBE?BV1@ABV1@@Z -- FableWin.pdb name set (the only C3DVector cross-product member of either PDB) + the body (this x other through the hidden result pointer, QBE, ret 8); the own row (was the descriptive CrossProduct) respelled and rechecked MATCH'),
 ('0063a390', 'CWorldMap::GetThemeSizeZAt', 'naming wave 19 (c680): ?GetThemeSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z -- Ego_r.pdb donor order GetGroundSizeZAt 006E7EC0 / GetThemeSizeZAt 006E7F50 / GetWaterSizeZAt 006E7FE0 = retail 0x0063A300 / 0x0063A390 / 0x0063A420 (0x90 apart in both); the twin rows differ only in the CMap callee; own row + 0x00912700 respelled and rechecked'),
 ('008200b0', 'CMap::GetThemeSizeZAt', 'naming wave 19 (c680): ?GetThemeSizeZAt@CMap@@IBEMABVC3DVector@@@Z -- the callee of CWorldMap::GetThemeSizeZAt 0x0063A390 (Ego_r 0067B2D0, after CMap::GetGroundSizeZAt 006796E0 = retail 0x0081EA50 of the twin row 0x0063A300); the catalog label CMap::GetThemeMinCameraHeightAtPosition is a propagation; not landed'),
 ('00c73770', 'EnsureWindowsVersionDetected_00c73770', 'naming wave 19 (c680): descriptive (libpk / GameCODA, no PDB): the one-time guard around the landed ?DetectWindowsVersion@@YAXXZ 0x00C73660 (ecx in / eax out); re-authored with the callee\'s real signature, byte-identical (RELOCATION_MATCH); was the propagated CDefString::SetDefaultString'),
 ('00c689b0', 'CEaxContext::SetSlotProperty', 'naming wave 19 (c685): descriptive (libpk, no PDB): the EAX 4.0 FX-slot property dispatcher over the landed CEaxContext slot setters SetSlotLock 0x00C67470 / SetSlotLoadEffect 0x00C674B0 / SetSlotVolume 0x00C67720 / SetSlotFlags 0x00C67430; was the propagated CSkillTable_00c67470::SetSkillPropertiesEx; respelled and rechecked'),
 ('004e34b2', 'std::vector<std::pair<long,CCharString> >::~vector', 'naming wave 19 (rfail, wave 18 recheck-fail): ??1?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@QAE@XZ re-authored on the STLport 4.6 _Destroy text, byte-identical and rechecked'),
 ('004dd782', 'std::__destroy_aux<std::pair<long,CCharString>*>', 'naming wave 19 (rfail, wave 18 recheck-fail): ??$__destroy_aux@PAU?$pair@JVCCharString@@@std@@@std@@YIXPAU?$pair@JVCCharString@@@0@0ABU__false_type@0@@Z re-authored on the STLport text, byte-identical and rechecked'),
 ('00ab34c0', 'C3DAnimationBuilder::CBoneInfo::~CBoneInfo', 'naming wave 19 (c685): ??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ -- FableWin.pdb name; the callee of CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData 0x00AB3640 (chunk 685, the deleter BuildBoneInfoTree 0x00AB1F30 stores) and the row that destroys the bone info\'s vector of counted pointers through __destroy_aux 0x00AB2BD0; the catalog\'s CEntityBindingRecord_00ab34c0::~CEntityBindingRecord_00ab34c0 is a propagated descriptive name'),
 ('0099f450', 'CCharString::RemoveWhiteSpaceBegin', 'naming wave 19 (c685): ?RemoveWhiteSpaceBegin@CCharString@@QAEXXZ -- FableWin.pdb name; first callee of CCharString::RemoveWhiteSpaceBeginEnd 0x0099F560 (Ego_r, chunk 685), the donor pair RemoveWhiteSpaceBegin / RemoveWhiteSpaceEnd adjacent in FableWin (02F62C90 / 02F62DA0 / 02F62EB0 = Begin / End / BeginEnd, the retail order 0099F450 / 0099F4D0 / 0099F560)'),
 ('0099f4d0', 'CCharString::RemoveWhiteSpaceEnd', 'naming wave 19 (c685): ?RemoveWhiteSpaceEnd@CCharString@@QAEXXZ -- FableWin.pdb name; second callee of CCharString::RemoveWhiteSpaceBeginEnd 0x0099F560 (Ego_r, chunk 685; the landed-names label of 0x0099F560 already named it), donor order as above'),
 ('00ab1480', 'std::__copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*,CCountedPointer<C3DAnimationBuilder::CBoneInfo>*,long>', 'naming wave 19 (c685): the STLport 4.6 _algobase.h random-access __copy instantiation (chunk 685; the row\'s leaf is the VC7.1 spelling ??$__copy@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@J@std@@YIPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAJ@Z); was the descriptive Move_CountedPointer_00ab1480'),
 ('0099f560', 'CCharString::RemoveWhiteSpaceBeginEnd', 'naming wave 19 (c685): ?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ (Ego_r / FableWin.pdb; chunk 685) -- own row re-spelled from the descriptive Dest_val'),
]
for t, n, e in EXTRA:
    if SQ(n) not in set(SQ(x) for x in L.get(t, ())) and (t, n) not in seen and not any(r[0] == t and SQ(r[1]) == SQ(n) for r in out[1:]): out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
