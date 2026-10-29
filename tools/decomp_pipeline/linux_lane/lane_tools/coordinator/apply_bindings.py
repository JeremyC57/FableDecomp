# usage (repo root): python3 apply_bindings.py <wave dir>/BINDINGS.tsv
# Apply a naming wave's BINDINGS.tsv (action, symbol, target, by_row, note) to rebuild/oracles/reloc_bindings.tsv:
# RETIRE removes (symbol, target); RETARGET-HASH removes the old hash spelling; RETARGET moves a symbol to a new target;
# ADD / ADD-HASH add (symbol, target, by_row). KEEP, NO-BIND and ADD-PROVISIONAL are ignored. Then run
# bindings_multi_check.py: it must print 0 (no symbol bound to two targets).
import sys
B=[l.rstrip('\n').split('\t') for l in open(sys.argv[1])][1:]
p='rebuild/oracles/reloc_bindings.tsv'; L=open(p).read().rstrip('\n').split('\n'); hdr=L[0]; rows=[l.split('\t') for l in L[1:]]
rm={(b[1],b[2]) for b in B if b[0]=='RETIRE'}
rmh={b[1] for b in B if b[0]=='RETARGET-HASH'}
n0=len(rows); rows=[r for r in rows if (r[0],r[1]) not in rm and r[0] not in rmh]; ret=n0-len(rows)
have={(r[0],r[1]) for r in rows}; add=0; rt=0
for b in B:
    if b[0]=='RETARGET':
        for r in rows:
            if r[0]==b[1]: r[1]=b[2]; r[2]=b[3]; rt+=1
for b in B:
    if b[0] in('ADD','ADD-HASH') and (b[1],b[2]) not in have: rows.append([b[1],b[2],b[3]]); have.add((b[1],b[2])); add+=1
open(p,'w').write(hdr+'\n'+'\n'.join('\t'.join(r) for r in rows)+'\n')
print('retired',ret,'retarget',rt,'added',add)
