# mkoff.py: work/offlimits.txt (brief's held rows + still-reserved Codex p28 rows + every T3_PORT_QUEUE address) and
# work/other_agents_entries.txt (NF305 / NF306 ENTRIES targets); bare lowercase 8-hex addresses
import re, glob, os
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF307'
S = os.path.dirname(os.path.dirname(A)); R = '/home/user/FableDecomp'
held = '00B4D370 00BD3070 00890250 0091C080 0052C7E0 00A254A0 00ADC190 00ADC250 00ADC4E0 00ADCB90 00ADCE60 00BE8E60 00BFBD80 00BE6090 00BFB590'.split()
off = set(h.lower() for h in held)
for f in glob.glob(R + '/rebuild/candidates/manual/pending/2026-11-22_codex_pass28/RESERVED.tsv'):
    for l in list(open(f))[1:]:
        p = l.rstrip('\n').split('\t')
        if 'no longer reserved' not in l and re.match(r'[0-9A-Fa-f]{8}$', p[0]): off.add(p[0].lower())
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv'):
    p = l.split('\t')
    if len(p) > 1:
        m = re.match(r'0x([0-9A-Fa-f]{8})', p[1])
        if m: off.add(m.group(1).lower())
open(A + '/work/offlimits.txt', 'w').write('\n'.join(sorted(off)) + '\n')
oth = set()
for ag in ('agentNF305', 'agentNF306'):
    for l in list(open(S + '/w22/%s/ENTRIES.tsv' % ag))[1:]:
        oth.add(l.split('\t')[0].lower())
open(A + '/work/other_agents_entries.txt', 'w').write('\n'.join(sorted(oth)) + '\n')
print(len(off), len(oth))
# witnesses of NF305 / NF306 entries (every (xxxxxxxx) address in their spellings column) -> work/other_agents_witnesses.txt
wit = set()
for ag in ('agentNF305', 'agentNF306'):
    for l in list(open(S + '/w22/%s/ENTRIES.tsv' % ag))[1:]:
        p = l.rstrip('\n').split('\t')
        if len(p) > 11: wit.update(x.lower() for x in re.findall(r'\(([0-9a-fA-F]{8})\)', p[11]))
open(A + '/work/other_agents_witnesses.txt', 'w').write('\n'.join(sorted(wit)) + '\n')
print('witnesses', len(wit))
