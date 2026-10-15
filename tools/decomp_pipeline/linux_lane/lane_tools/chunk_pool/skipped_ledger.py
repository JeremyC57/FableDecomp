# The skipped-row ledger: rebuild/candidates/manual/SKIPPED_ROWS.tsv (committed) -- every chunk row an authoring agent
# could not author as ONE function, so it is never forgotten (the chunk cutter never re-offers a used row).
#   python3 skipped_ledger.py add <batch> <addr>:<rowlen> [label] [note]   classify (split_rows.py) and append (idempotent)
#   python3 skipped_ledger.py refresh                                     mark rows done when every piece is in the catalog
#   python3 skipped_ledger.py to-chunk <NN> [--max-len 600]               turn the open multi/prefix pieces that are not
#        landed into an ordinary chunk ($S/w22/chunkNN.tsv + $S/batch/chunkNN_oracle.tsv), mark those rows in-chunk:NN;
#        then `python3 mkprompt25.py NN` and hand $S/w22/agentNN/PROMPT.md to any authoring agent, stage + land as usual.
#   python3 skipped_ledger.py show
# kinds: multi (several functions in one row -> author each piece), prefix (the row is the start of a longer function ->
# author the whole function), cold (a block of another function -> covered when the parent lands), ltcg (register
# conventions from link-time code generation that one translation unit cannot reproduce -> Windows box / leave).
import os,sys,csv
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
L='rebuild/candidates/manual/SKIPPED_ROWS.tsv'
F=['address','row_len','batch','label','kind','status','pieces','note']
def load():
    if not os.path.exists(L): return []
    return list(csv.DictReader(open(L),delimiter='\t'))
def save(rows):
    with open(L,'w',newline='') as f:
        w=csv.DictWriter(f,F,delimiter='\t',lineterminator='\n'); w.writeheader(); w.writerows(rows)
def catalog(): return {l.split('\t')[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv')}
def plist(s): return [(p.split(':')[0],int(p.split(':')[1])) for p in s.split(',') if p]
def add(batch,spec,label='',note='',kind=None,pieces=None):
    import split_rows
    a,n=spec.split(':'); a=a.lower().replace('0x','')
    rows=load()
    if any(r['address']==a for r in rows): return False
    if pieces is None:
        k,p=split_rows.classify(int(a,16),int(n)); pieces=','.join('%08x:%d'%x for x in p); kind=kind or k
    rows.append(dict(address=a,row_len=n,batch=batch,label=label,kind=kind,status='open',pieces=pieces,note=note))
    rows.sort(key=lambda r:r['address']); save(rows); return True
def refresh():
    cat=catalog(); rows=load(); ch=0
    for r in rows:
        if r['kind'] in ('multi','prefix') and r['pieces'] and r['status']!='done' and all(p in cat for p,_ in plist(r['pieces'])):
            r['status']='done'; ch+=1
        elif r['kind']=='cold' and r['pieces'] and r['status']!='done' and plist(r['pieces'])[0][0] in cat:
            r['status']='done'; ch+=1          # the parent landed: the block is part of its bytes / funclets
    save(rows); return ch
def to_chunk(nn,maxlen=600):
    import split_rows
    S=os.environ['S']; cat=catalog(); rows=load(); sel=[]
    for r in rows:
        if r['kind'] not in ('multi','prefix') or r['status']!='open': continue
        took=False
        for a,n in plist(r['pieces']):
            if a in cat or n>maxlen or n<1: continue
            mod=(r['label'].split('::')[0] if r['label'] else 'Unknown')
            sel.append((a,mod,'Piece_'+a,n,split_rows.rd(int(a,16),n).hex())); took=True
        if took: r['status']=f'in-chunk:{nn}'
    N='%02d'%int(nn)
    with open(f'{S}/w22/chunk{N}.tsv','w') as f:
        f.write('address\tmodule\tname\tlength\tbytes\n')
        for a,m,nm,n,h in sel: f.write(f'{a}\t{m}\t{nm}\t{n}\t{h}\n')
    with open(f'{S}/batch/chunk{N}_oracle.tsv','w') as f:
        f.write('address\tname\tlength\tbytes\n')
        for a,m,nm,n,h in sel: f.write(f'{a}\t{m}::{nm}\t{n}\t{h}\n')
    save(rows); return len(sel)
if __name__=='__main__':
    c=sys.argv[1]
    if c=='add': print('added' if add(sys.argv[2],sys.argv[3],*(sys.argv[4:6])) else 'already logged')
    elif c=='refresh': print(refresh(),'rows now done')
    elif c=='to-chunk':
        ml=int(sys.argv[sys.argv.index('--max-len')+1]) if '--max-len' in sys.argv else 600
        print(to_chunk(sys.argv[2],ml),'pieces written to chunk',sys.argv[2])
    else:
        for r in load(): print('\t'.join(r[k] for k in F))
