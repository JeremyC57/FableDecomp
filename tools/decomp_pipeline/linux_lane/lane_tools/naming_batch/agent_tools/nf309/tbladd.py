# tbladd.py <action> <symbol> <target> <witness> <note>: append a TABLE_CHANGES.tsv line after checking it against reloc_bindings.tsv
# (delete / rewitness: the (symbol, target) line must exist; add: must not exist). NF309.
import sys,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309/'
act,sym,tgt,wit,note=sys.argv[1:6]
T=set()
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.add((r[0],r[1].lower()))
ok=((sym,tgt) in T) if act in ('delete','rewitness') else ((sym,tgt) not in T) if act=='add' else True
if not ok: sys.exit('REFUSED %s %s %s (table state)'%(act,sym,tgt))
p=A+'TABLE_CHANGES.tsv'; new=not os.path.exists(p)
with open(p,'a') as f:
    if new: f.write('action\tsymbol\ttarget\twitness\tnote\n')
    f.write('\t'.join([act,sym,tgt,wit,note])+'\n')
print('OK',act,sym[:70],tgt)
