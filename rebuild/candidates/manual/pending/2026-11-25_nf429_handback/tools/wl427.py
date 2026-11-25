#!/usr/bin/env python3
"""NF427 worklist: open tracker rows of the creature-action family."""
import csv,re,collections,sys,json
FAM=r'(CCreatureAction\w+|CCombatAction_\w+|CCombatSequence_\w+|CCombatActionBase)'
rows=list(csv.DictReader(open('trk/OPEN.tsv'),delimiter='\t'))
def famcls(r):
    if r['klass'] and re.fullmatch(FAM,r['klass']): return r['klass']
    for f in ('rtti','pdb_spelling','self_leaf','spellings'):
        m=re.search(r'\?[?\w$]+@'+FAM+r'@@',r[f] or '')
        if m: return m.group(1)
    return None
out=[]
for r in rows:
    c=famcls(r)
    if c: r['fam']=c; out.append(r)
def member(r):
    s=r['rtti'] or r['pdb_spelling'] or r['self_leaf']
    m=re.match(r'\?(\?_[GE]|\?[0-9]|[A-Za-z_]\w*)',s or '')
    return m.group(1) if m else '?'
for r in out: r['member']=member(r)
w=csv.writer(open('evidence/worklist.tsv','w'),delimiter='\t')
keys=['target','status','tier','fam','member','rtti','pdb_spelling','self_leaf','landed','catalog','source','spellings']
w.writerow(keys)
for r in out: w.writerow([r[k] for k in keys])
print(len(out),len(set(r['fam'] for r in out)))
print(collections.Counter((r['status'],r['tier']) for r in out))
print(collections.Counter(r['member'] for r in out).most_common(60))
print('landed',collections.Counter(r['landed'] for r in out))
