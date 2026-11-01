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
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W21.tsv'))][1:]
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
FIX = {  # queue spellings proven wrong / display-string finals: the settled mangled spelling
 '013ca828': '?GDefStringTable@@3VCDefStringTable@@A',
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
    out.append((t, name, 'naming wave 21 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = []
_rp = os.path.join(W, 'RENAMES.tsv')
if os.path.exists(_rp):
    for l in list(open(_rp))[1:]:
        r = l.rstrip('\n').split('\t')
        if len(r) >= 10: EXTRA.append((r[0], r[2], r[9] + '; rechecked MATCH / RELOCATION_MATCH + PASS'))
EXTRA += [
 ('0139c960', 'g_pSoundAllocator_0139c960', 'naming wave 21 (alloc): the libpk sound allocator global the __fastcall store 0x00C16690 writes (descriptive, libpk)'),
 ('0139c968', 'g_pSoundTempAllocator_0139c968', 'naming wave 21 (alloc): the temporary-sound allocator global (store 0x00C166D0; descriptive, libpk)'),
 ('0139c964', 'g_pSoundPhysicalAllocator_0139c964', 'naming wave 21 (alloc): the physical-sound allocator global (store 0x00C166B0; descriptive, libpk)'),
 ('0143bb34', 'g_DefaultAllocator_0143bb34', 'naming wave 21 (alloc): the libpk default malloc/free allocator object (vtable 0x012B2AB8, CDefaultAllocator_00c166f0: ctor 0x00C166F0, Allocate 0x00C16700, Free 0x00C16720, dynamic initializer 0x01224B40); the stores fall back to it (descriptive, libpk)'),
 ('00c166f0', 'CDefaultAllocator_00c166f0::CDefaultAllocator_00c166f0', 'naming wave 21 (alloc): was CVtable_012b2ab8 (descriptive, libpk)'),
 ('00c16700', 'CDefaultAllocator_00c166f0::Allocate', 'naming wave 21 (alloc): was CVtable_012b2ab8::Allocate'),
 ('00c16720', 'CDefaultAllocator_00c166f0::Free', 'naming wave 21 (alloc): was CVtable_012b2ab8::Free'),
 ('00c094a0', 'CSoundSystem::CSoundSystem', 'naming wave 21 (csound, DECISIONS_W20_RESOLVED): ??0CSoundSystem@@QAE@PAUCSoundSystemSetup@@PAUCSteeringWorldEntry@@@Z -- the one class name for vtable 0x012B1E24 (was CAudioOwner)'),
 ('00c09380', 'CSoundSystem::~CSoundSystem', 'naming wave 21 (csound): ??1CSoundSystem@@UAE@XZ (was CAudioOwner)'),
 ('00c0a6b0', "CSoundSystem::`scalar deleting destructor'", 'naming wave 21 (csound): ??_GCSoundSystem@@UAEPAXI@Z (was CAudioOwner)'),
 ('00c091f0', 'CSoundSystem::Update', 'naming wave 21 (csound): ?Update@CSoundSystem@@QAEXXZ (was CAudioOwner)'),
 ('00c08ae0', 'CSoundSystem::WaitForSprite', 'naming wave 21 (csound): ?WaitForSprite@CSoundSystem@@QAEXXZ (was CAudioOwner)'),
 ('00c096e0', 'CreateSoundSystem_00c096e0', 'naming wave 21 (csound): ?CreateSoundSystem_00c096e0@@YGPAUCSoundSystem@@PAUCSoundSystemSetup@@@Z, the __stdcall libpk factory the landed 0x00A3EF00 binds (was CWorldManager::InitializeSteeringAfterSave)'),
 ('00c13140', 'CSoundEventManager::Play', 'naming wave 21 (voice): ?Play@CSoundEventManager@@QAEJJJPBUCPKVoiceParams@@@Z -- CSoundSystem+0x38, the landed CSoundEventManager (was the propagated CTCHeroExperience::GetBlockParryReward; chunk 687 said CPKVoiceTable::Play: same object)'),
 ('00c10a30', 'CPKVoiceParams::CPKVoiceParams', 'naming wave 21 (voice): ??0CPKVoiceParams@@QAE@XZ (was SNavigationRequest)'),
 ('00c20480', 'CPKVoiceParams::SetMinDistance', 'naming wave 21 (voice): ?SetMinDistance@CPKVoiceParams@@QAEXM@Z (descriptive: 0x00C62530 passes the range minimum minus the listener distance; was CBrainOutput::SetAsPumpCombatActions)'),
 ('00c204a0', 'CPKVoiceParams::SetMaxDistance', 'naming wave 21 (voice): ?SetMaxDistance@CPKVoiceParams@@QAEXM@Z (descriptive; slot callee 0x00C1BBA0)'),
 ('00c20420', 'CPKVoiceParams::SetScale', 'naming wave 21 (voice): ?SetScale@CPKVoiceParams@@QAEXM@Z (descriptive; slot callee 0x00C1BC70)'),
 ('00c61cf0', 'CPKVoiceHandle::GetEventManager', 'naming wave 21 (voice): ?GetEventManager@CPKVoiceHandle@@QAEPAUCSoundEventManager@@PAVCAmbientEmitter@@@Z (was GetTable returning CPKVoiceTable: the same object as the landed CSoundEventManager)'),
 ('00c377e0', 'CSoundLoop_00c377f0::GetEventManager', 'naming wave 21 (voice): ?GetEventManager@CSoundLoop_00c377f0@@QAEPAUCSoundEventManager@@XZ (was GetSoundSystem returning CSoundSystem_00c377f0)'),
 ('00c37840', 'CSoundLoop_00c377f0::Update', 'naming wave 21 (voice): one class name for the loop record (was CSoundLoop_00c37840)'),
 ('00c37930', 'CSoundLoop_00c377f0::~CSoundLoop_00c377f0', 'naming wave 21 (voice): one class name for the loop record (was CSoundLoop_00c37930)'),
 ('00c27c10', 'CPKVoiceParams::Load', 'naming wave 21 (voice): the receiver is the CPKVoiceParams record (was SerializedState_00c27c10::Load)'),
 ('00c16690', 'StoreSoundAllocator_00c16690', 'naming wave 21 (alloc): own row re-authored ?StoreSoundAllocator_00c16690@@YIXPAVCAllocator@LHA@@@Z (was CHoverable_ProcessEvent)'),
 ('00c166d0', 'StoreSoundTempAllocator_00c166d0', 'naming wave 21 (alloc): own row re-authored (was CHoverable_ProcessEvent)'),
 ('00c166b0', 'StoreSoundPhysicalAllocator_00c166b0', 'naming wave 21 (alloc): own row re-authored (was CHoverable_ProcessEvent)'),
 ('009d5060', 'CDefString::SetString', 'naming wave 21 (gdst): ?SetString@CDefString@@QAEXABVCCharString@@@Z (PDB; TablePos = GDefStringTable.AddString(s); was a Singleton::method stand-in)'),
 ('00415d70', 'CDefString::operator CCharString', 'naming wave 21 (gdst): ??BCDefString@@QBE?AVCCharString@@XZ (PDB; was a CCharString::AssignTo stand-in)'),
 ('00c26c10', 'CPredicateChain_00c26c10::CPredicateChain_00c26c10', 'naming wave 21 (dbg): the Leaf ??0CPredicateChain_00c26c10@@QAE@PAX@Z set (was a junk leaf); member PKCSemaphore'),
 ('00c27070', 'CPredicateChain_00c27070::CPredicateChain_00c27070', 'naming wave 21 (dbg): the Leaf set; member PKCSemaphore'),
 ('00c355e0', 'CWorkerThread_00c355e0::CWorkerThread_00c355e0', 'naming wave 21 (dbg): the Leaf ??0CWorkerThread_00c355e0@@QAE@PAXPBUSThreadParams_00c355e0@@@Z set; member PKCSemaphore, the error-macro stream CString / CDebugBreak'),
 ('00c44d30', 'CAmbientEmitter::ClearEvents', 'naming wave 21 (voice): ?ClearEvents@CAmbientEmitter@@QAEXXZ -- the wave-20 emitter class (was CPKEmitter); its Release call follows the own row 0x00C61FB0'),
 ('00c20440', 'CPKVoiceParams::SetFilter_00c20440', 'naming wave 21 (voice): ?SetFilter_00c20440@CPKVoiceParams@@QAEXJ@Z, one name (the state row\'s Set5_00c20440 respelled; descriptive, libpk)'),
]
for t, n, e in EXTRA:
    if SQ(n) not in set(SQ(x) for x in L.get(t, ())) and (t, n) not in seen and not any(r[0] == t and SQ(r[1]) == SQ(n) for r in out[1:]): out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
