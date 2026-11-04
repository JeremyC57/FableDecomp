import sys
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
B='rebuild/oracles/reloc_bindings.tsv'
L=open(B).read().splitlines()[1:]
have={tuple(l.split('\t')[:2]) for l in L}; syms={l.split('\t')[0]:l.split('\t')[1] for l in L}
add=[];conf=[]
for f in ('BINDINGS.tsv','BINDINGS_inherited.tsv'):
  for l in open(S+'/wave35b/'+f).read().splitlines()[1:]:
    a,s,t,r=l.split('\t')[:4]
    if a!='ADD' or (s,t) in have or any(x[0]==s for x in add): continue
    if s in syms and syms[s]!=t: conf.append(s); continue
    add.append((s,t,r))
print(len(add),'conflicts',conf)
if len(sys.argv)>1:
  with open(B,'a') as fh:
    for s,t,r in add: fh.write('%s\t%s\t%s\n'%(s,t,r))
