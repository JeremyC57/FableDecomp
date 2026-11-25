#!/usr/bin/env python3
"""NF429 worklist: open tracker rows of the AI state-group family (CAIStateGroup* classes). Class key: the RTTI spelling's class,
then the PDB spelling's, then the tracker's klass (stand-in names never win over RTTI). Rows dropped from the family with a reason:
non-function addresses (0x012Bxxxx data, no catalog bytes), `_00c...` Unwind stand-in classes, CAIStateGroupInfo (not a state group)."""
import csv,re,collections,sys
FAM=r'CAIStateGroup\w*'
rows=list(csv.DictReader(open('trk/OPEN.tsv'),delimiter='\t'))
def mcls(s):
    s=(s or '').strip()
    for pat in (r'\?\?_[GE7](\w+)@@', r'\?\?[01](\w+)@@', r'\?[^?@]+@(\w+)@@'):
        m=re.match(pat,s)
        if m: return m.group(1)
    return ''
def famcls(r):
    for k in ('rtti','pdb_spelling'):
        c=mcls(r[k])
        if c: return c
    return r['klass'] or mcls(r['self_leaf'])
def member(r):
    s=r['rtti'] or r['pdb_spelling'] or r['self_leaf']
    m=re.match(r'\?(\?_[GE]|\?[0-9]|[A-Za-z_]\w*)',s or '')
    return m.group(1) if m else '?'
out=[]; drop=[]
for r in rows:
    c=famcls(r)
    if not re.fullmatch(FAM,c or ''): continue
    r['fam']=c; r['member']=member(r)
    why=''
    if r['target'].startswith('012'): why='data address (vftable / RTTI), not a function'
    elif re.search(r'_00[0-9a-f]{6}$',c): why='Unwind / stand-in class %s (no RTTI class)'%c
    elif c=='CAIStateGroupInfo': why='CAIStateGroupInfo is not a state-group class (no CAIStateGroupBase in its RTTI chain)'
    (drop if why else out).append((r,why))
keys=['target','status','tier','fam','member','rtti','pdb_spelling','self_leaf','landed','catalog','source','spellings']
w=csv.writer(open('evidence/worklist.tsv','w'),delimiter='\t',lineterminator='\n'); w.writerow(keys)
for r,_ in out: w.writerow([r[k] for k in keys])
d=csv.writer(open('evidence/dropped.tsv','w'),delimiter='\t',lineterminator='\n'); d.writerow(['target','status','fam','member','reason'])
for r,why in drop: d.writerow([r['target'],r['status'],r['fam'],r['member'],why])
print('family',len(out),'classes',len(set(r['fam'] for r,_ in out)),'dropped',len(drop))
print(collections.Counter(r['status'] for r,_ in out))
print('no ??_G/??_E:',collections.Counter(r['status'] for r,_ in out if r['member'] not in ('?_G','?_E')))
