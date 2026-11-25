#!/usr/bin/env python3
"""fam438.py: classify every gen0 residue line (target, wrong, row) by the token diff of demangled wrong v PDB spelling into
edit families: QUAL (const / virtual / access / static only), PTRREF (+ pointer->reference), KEYW (+ struct->class), SCOPE (+ the
method's own class / namespace path), else COMPLEX. Writes work/fam.tsv."""
import csv,collections,re,json,difflib,sys
op={r['target']:r for r in csv.DictReader(open('tracker/OPEN.tsv'),delimiter='\t')}
res=[l.rstrip('\n').split('\t') for l in open('work/gen0_RESIDUE.tsv')][1:]
d=json.load(open('work/dem.json'))
def toks(x): return re.findall(r'\w+|::|[^\w\s]',x)
QUALW={'const','virtual','public','protected','private','static',':'}
def fam(s,P):
    a=d.get(s); b=d.get(P)
    if not a or not b: return 'NODEM',''
    ta,tb=toks(a),toks(b); sm=difflib.SequenceMatcher(None,ta,tb,autojunk=False)
    kinds=set(); ch=[]
    for o,i1,i2,j1,j2 in sm.get_opcodes():
        if o=='equal': continue
        x,y=ta[i1:i2],tb[j1:j2]; ch.append(' '.join(x)+'->'+' '.join(y))
        if set(x)|set(y)<=QUALW: kinds.add('QUAL')
        elif x==['*'] and y==['&'] or x==['&'] and y==['*']: kinds.add('PTRREF')
        elif set(x)|set(y)<= {'struct','class'}: kinds.add('KEYW')
        else: kinds.add('OTHER')
    # scope: same method name and params text after removing class path?
    if 'OTHER' in kinds:
        ma=re.search(r'(\w+)\(',a); mb=re.search(r'(\w+)\(',b)
        pa=a[a.find('('):] if '(' in a else ''; pb=b[b.find('('):] if '(' in b else ''
        if ma and mb and ma.group(1)==mb.group(1) and not s.startswith('??') and not P.startswith('??'):
            return ('SCOPE' if pa==pb else 'SCOPE+SIG'), ' ; '.join(ch)
        return 'COMPLEX',' ; '.join(ch)
    return '+'.join(sorted(kinds)) or 'SAME',' ; '.join(ch)
with open('work/fam.tsv','w') as f:
    c=collections.Counter()
    for t,s,w,why in res:
        k,ch=fam(s,op[t]['pdb_spelling']); c[k]+=1
        f.write('\t'.join([t,w,k,s,op[t]['pdb_spelling'],ch,why[:60]])+'\n')
print(c.most_common())
