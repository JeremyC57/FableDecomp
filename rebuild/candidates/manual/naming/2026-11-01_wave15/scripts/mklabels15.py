# mklabels15.py: LABELS.tsv -- for every queue target this wave respelled (fully or partly) and the decision / wave-14 targets,
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
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W15.tsv'))][1:]
out = [('address', 'name', 'evidence')]
seen = set()
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
    if name in L.get(t, ()): continue
    out.append((t, name, 'naming wave 15 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = [
 ('00c5ee50', 'ReleaseBuffer', 'naming wave 15 DECISIONS_W14_RESOLVED audio slot 5: slot 5 of the silent audio output vtable 0x012B9174 (zeroes the block count; chunk 661 label of the 0x00C5EE20 tail-jump target); the v05 / Close / Release spellings of 0x00C5EE60 / 0x00C5EEB0 / 0x00C5EEF0 unified (descriptive, no PDB/RTTI in the audio library)'),
 ('006961f0', 'IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> >', 'naming wave 15 DECISIONS_W14_RESOLVED 0x0097A630: ICF fold of the IS_THING_ALIVE<T> instantiations; 0x0097A630 names ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingCreatureBase@@@@@@YI_NABV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z (PTargetThing +0x14 is CIntelligentPointer<CThingCreatureBase>, Ego_r CAIStateGroup_EnemyCreatureBase +20)'),
 ('004c7aa0', 'CThing::GetWorld', 'naming wave 15 DECISIONS_W14_RESOLVED 00692940 / 00692c10: ?GetWorld@CThing@@IAEAAVCWorld@@XZ (protected) called from CCreatureActionBase through a friend declaration (Access: friend -- retail-consistent, not attested)'),
 ('00972590', 'CAIStateGroup_JackOfBladesStage2::Cleanup', 'naming wave 15 (wave-14 DEFERRED gw): re-modelled as the Ego_r member ?Cleanup@CAIStateGroup_JackOfBladesStage2@@UAEXXZ calling the base Cleanup 0x00972110, the protected const CAIStateGroupBase::GetWorld 0x008FCFC0 and CEnvironment::TransitionToDefaultThemeAllChannels 0x006B3950; MATCH'),
]
for t, n, e in EXTRA:
    if n not in L.get(t, ()) and (t, n) not in seen: out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
