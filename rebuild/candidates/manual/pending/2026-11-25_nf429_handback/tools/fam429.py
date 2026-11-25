import csv,collections,re,sys
def mcls(s):
    s=s.strip()
    m=re.match(r'\?\?_[GE7](\w+)@@',s)
    if m: return m.group(1)
    m=re.match(r'\?\?[01](\w+)@@',s)
    if m: return m.group(1)
    m=re.match(r'\?[^?@]+@(\w+)@@',s)
    if m: return m.group(1)
    return ''
def tcls(r):
    for k in ('rtti','pdb_spelling'):
        c=mcls(r[k])
        if c: return c
    cat=r['catalog']
    if '::' in cat: return cat.split('::')[0]
    return ''
def member(r):
    for k in ('rtti','pdb_spelling'):
        s=r[k]
        if s.startswith('??_G') or s.startswith('??_E'): return '??_G'
        if s.startswith('??0'): return '??0'
        if s.startswith('??1'): return '??1'
        m=re.match(r'\?([^?@]+)@',s)
        if m: return m.group(1)
    cat=r['catalog']
    return cat.split('::')[-1]
if __name__=='__main__':
    rows=list(csv.DictReader(open(sys.argv[1]),delimiter='\t',lineterminator='\n'))
    out=[]
    for r in rows:
        c=tcls(r)
        if c.startswith('CAIStateGroup') : 
            r['tclass']=c; r['member']=member(r); out.append(r)
    print(len(out), collections.Counter(r['status'] for r in out))
    print(collections.Counter(r['tclass'] for r in out if not r['tclass'].startswith('CAIStateGroup_')))
    print(collections.Counter(r['member'] for r in out).most_common(40))
    w=csv.DictWriter(open(sys.argv[2],'w'),fieldnames=list(out[0].keys()),delimiter='\t',lineterminator='\n'); w.writeheader(); w.writerows(out)
