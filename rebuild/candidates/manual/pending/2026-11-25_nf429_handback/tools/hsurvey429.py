#!/usr/bin/env python3
"""hsurvey429.py: per family class, header state -> evidence/hsurvey.tsv
(class, rtti direct base, header, has_flat (pack push + flat class), poly_branch (class C : public X under an #ifdef), vmodel, nopen)"""
import csv,re,os,collections
R='/home/user/FableDecomp'
B={}
for l in open(R+'/ghidra_out/rtti_bases.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>3 and '?A0x' not in p[1]: B[p[1]]=p[3].split('@')[0]
rows=list(csv.DictReader(open('evidence/classify.tsv'),delimiter='\t'))
n=collections.Counter(r['class'] for r in rows)
w=open('evidence/hsurvey.tsv','w'); w.write('class\tbase\thdr\tflat\tpoly\tvmodel\tpolysw\tnopen\n')
for C in sorted(n):
    h=f'{R}/rebuild/include/engine/{C}.h'
    if not os.path.exists(h): w.write(f'{C}\t{B.get(C,"?")}\t-\t-\t-\t-\t-\t{n[C]}\n'); continue
    t=open(h,errors='replace').read()
    flat='y' if re.search(r'^#pragma pack\(push',t,re.M) and re.search(r'^(struct|class)\s+%s\s*\{'%C,t,re.M) else 'n'
    pm=re.search(r'^class\s+%s\s*:\s*public\s+(\w+)'%C,t,re.M)
    vm='y' if 'FABLE_RETAIL_%s_VIRTUAL_MODEL'%C.upper() in t else 'n'
    sw='-'
    if pm:
        pre=t[:pm.start()]; ifs=re.findall(r'^#if(?:def)?\s+(.*)$',pre,re.M); sw=ifs[-1] if ifs else '?'
    w.write(f'{C}\t{B.get(C,"?")}\t{h[len(R)+1:]}\t{flat}\t{pm.group(1) if pm else "-"}\t{vm}\t{sw}\t{n[C]}\n')
