# tc.py: TABLE_CHANGES.tsv + RENAMES.tsv section 2 from the proof maps (evidence/proof_*.txt `renames:` lines, per caller), the table
# (reloc_bindings.tsv) and a list of stand-in spellings other (unchanged) landed rows still bind (KEEP below, from the exe-caller reads).
import re,collections
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF292/'
T=collections.defaultdict(list)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T[r[0]].append((r[1],r[2]))
KEEP={'?WriteUShort@CDataOutputStream@@QAEXG@Z':'00bfc770 (NF290 binder row, not changed; CONDITIONAL: not compiled, its scope unverified -- rewitness only if its object binds this exact spelling, else delete)',
      '?RegisterEnable@CEnginePrimitiveManager@@QAEXABVCCharString@@@Z':'00babb50 / 00bad040 (NF290 binder rows, not changed)'}
COND={'?RemoveBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z':'009d6490 (T3-queued, not compiled) declares RemoveBank on its own model; delete only if its object does not bind this exact spelling'}
ren=[]; pairs=collections.OrderedDict()
for f in ('proof_o1.txt','proof_b1.txt','proof_b2.txt','proof_b2_const.txt'):
    for l in open(A+'evidence/'+f):
        m=re.match(r'##### (\w+) (?:explicit )?(?:renames|\(name)[^:]*: (.*)$',l.strip())
        if not m: continue
        a=m.group(1)
        for kv in m.group(2).split():
            if '=' not in kv or '?A0x' in kv: continue
            o,n=kv.split('=',1); pairs.setdefault((o,n),[]).append(a)
added=set(); out=['action\tsymbol\ttarget\twitness\tnote']; r2=[]
for (o,n),callers in pairs.items():
    tg=[t for t,w in T.get(o,[])]; tgt=tg[0] if tg else '?'
    for c in callers: r2.append('0x%s\t%s\t%s\t0x%s'%(c,o,n,tgt))
    wit=[w for t,w in T.get(o,[])]
    if o in KEEP:
        if wit and wit[0] in callers: out.append('rewitness\t%s\t%s\t%s\tstand-in still bound by %s; old witness %s respelled by NF292'%(o,tgt,KEEP[o].split()[0],KEEP[o],wit[0]))
    elif o in COND: out.append('delete\t%s\t%s\t%s\tCONDITIONAL: %s'%(o,tgt,wit[0] if wit else '-',COND[o]))
    else: out.append('delete\t%s\t%s\t%s\tsole binder(s) %s respelled by NF292 (exe callers read: no other landed row binds this stand-in)'%(o,tgt,wit[0] if wit else '-',','.join(callers)))
    if not T.get(n) and (n,tgt) not in added: added.add((n,tgt)); out.append('add\t%s\t%s\t%s\tPDB spelling (Ego_r/FableWin name sets, evidence/pdbq.txt) had no table line; witness = NF292 respelled binder'%(n,tgt,callers[0]))
    elif T.get(n) and tgt not in [t for t,w in T[n]]: out.append('add\t%s\t%s\t%s\tPDB line exists for another target?? CHECK'%(n,tgt,callers[0]))
out=[x if '009cb960' not in x else x+' | CONDITIONAL on decision 1 (0x009CB960 is a T3_PORT_QUEUE address)' for x in out]
r2=[x if not x.startswith('0x009cb960') else x+'\t# CONDITIONAL decision 1' for x in r2]
open(A+'TABLE_CHANGES.tsv','w').write('\n'.join(out)+'\n')
hdr='address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\n# Section 2 (caller\told_symbol\tnew_symbol\tcallee)\n'
open(A+'RENAMES.tsv','w').write(hdr+'\n'.join(r2)+'\n')
print(len(out)-1,'table lines;',len(r2),'renames'); print('\n'.join(x[:200] for x in out if 'CHECK' in x or x.startswith(('rewitness','add'))))
