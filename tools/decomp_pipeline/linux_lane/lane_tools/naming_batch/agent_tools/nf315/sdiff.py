# sdiff.py <entry>...: for each stand-in spelling of an ENTRIES line, the first differing character position against the PDB spelling and
# 40 characters of context on each side (a quick read for long template names the demangler cannot parse).
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
L=open(A+'/ENTRIES.tsv').read().splitlines()
for n in map(int,sys.argv[1:]):
    c=L[n].split('\t'); pdb=c[6]
    for part in c[11].split(' || '):
        m=re.match(r'(\S+) \(([^)]*)\)',part)
        if not m or m.group(1)==pdb: continue
        s=m.group(1); i=next((k for k in range(min(len(s),len(pdb))) if s[k]!=pdb[k]),min(len(s),len(pdb)))
        print('#%d %s @%d PDB ..%s|%s  SI ..%s|%s'%(n,m.group(2),i,pdb[max(0,i-30):i],pdb[i:i+30],s[max(0,i-30):i],s[i:i+30]))
