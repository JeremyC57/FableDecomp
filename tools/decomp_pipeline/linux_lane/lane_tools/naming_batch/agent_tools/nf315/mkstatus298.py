# mkstatus298.py: writes STATUS.tsv (entry, status, binder_rows, grade, note) for the ENTRIES lines reached, from the verdict table below
# (V: entry number -> (status, binders, grade, note)). Re-run after every change of verdict.
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
L=open(A+'/ENTRIES.tsv').read().splitlines()
NC='source read, not compiled'
V={}
def v(ns,st,b,g,note):
    for n in (ns if isinstance(ns,(list,tuple)) else [ns]): V[n]=(st,b,g,note)
exec(open(A+'/tools/verdicts298.py').read())
out=['entry\tstatus\tbinder_rows\tgrade\tnote']
for n in sorted(V):
    st,b,g,note=V[n]; out.append('0x%s\t%s\t%s\t%s\tentry %d (ENTRIES line %d): %s'%(L[n].split('\t')[0],st,b,g,n,n+1,note))
open(A+'/STATUS.tsv','w').write('\n'.join(out)+'\n'); print(len(V),'entries')
