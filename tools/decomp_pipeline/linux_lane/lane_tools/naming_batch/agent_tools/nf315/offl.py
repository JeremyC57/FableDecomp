# offl.py: NF315 off-limits set -> work/offlimits.txt (bare lowercase addrs) + work/offwhy.tsv (addr, reason)
# NF312 / NF314 ENTRIES targets and witnesses, live locks of other agents, unexpired codex-p28 reservations, held rows, T3_PORT_QUEUE.
import re,os,glob
S='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad'; A=S+'/w22/agentNF315'; R='/home/user/FableDecomp/'
why={}
def add(a,r):
    a=a.lower().replace('0x','')
    if re.fullmatch(r'[0-9a-f]{8}',a): why.setdefault(a,set()).add(r)
for ag in ('NF312','NF314'):   # NF315: NF309 / NF313 landed; NF312 + NF314 run beside NF315
    L=open(S+'/w22/agent%s/ENTRIES.tsv'%ag).read().splitlines()[1:]
    for l in L:
        c=l.split('\t'); add(c[0],ag+'-target')
        for m in re.finditer(r'\(([^)]*)\)',c[11] if len(c)>11 else ''):
            for w in m.group(1).split(','): add(w,ag+'-witness')
# NF315 v2 (advisor): the ENTRIES witness lists are cut with '...', so also add every reloc_bindings witness of an NF312 / NF314 target
tg={a for a,r in why.items() if any(x.endswith('-target') for x in r)}
for l in open(R+'rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3 and r[1].lower() in tg:
        ag=[x.split('-')[0] for x in why[r[1].lower()] if x.endswith('-target')][0]
        for w in r[2].split(','): add(w,ag+'-witness')
for d in glob.glob(S+'/w22/nflocks/*'):
    try: o=open(d+'/owner').read().strip()
    except Exception: continue
    if o not in ('released','NF315'): add(os.path.basename(d),'lock-'+o)
for l in open(R+'rebuild/candidates/manual/pending/2026-11-22_codex_pass28/RESERVED.tsv').read().splitlines()[1:]:
    c=l.split('\t')
    if len(c)>3 and 'no longer reserved' not in c[3]: add(c[0],'reserved')
for a in '00B4D370 00BD3070 00890250 0091C080 0052C7E0 00A254A0 00ADC190 00ADC250 00ADC4E0 00ADCB90 00ADCE60 00BE8E60 00BFBD80 00BE6090 00BFB590'.split(): add(a,'held')
for l in open(R+'rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    c=l.split('\t')
    if len(c)>1: add(c[1],'t3q')
    # NF315 v3: every address named anywhere on a T3_PORT_QUEUE line (column 3 unit members, column 4 binder mentions)
    for x in re.findall(r'0x([0-9a-fA-F]{8})',l): add(x,'t3q-unit')
open(A+'/work/offlimits.txt','w').write('\n'.join(sorted(why))+'\n')
open(A+'/work/offwhy.tsv','w').write(''.join('%s\t%s\n'%(a,r) for a in sorted(why) for r in sorted(why[a])))
import collections; print(len(why), collections.Counter(r for v in why.values() for r in v))
