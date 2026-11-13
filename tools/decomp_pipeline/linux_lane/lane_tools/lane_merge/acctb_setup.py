"""Build $S/w22/acctb/<addr>/{payload.json,hdr/} for every MATCHED row of the acctB final index; write rows.tsv for the oracle. Never raises."""
import csv,json,os,re,shutil,sys
S=os.environ['S']; F='rebuild/candidates/manual/pending/2026-11-13_acctB_FINAL'; B=S+'/w22/acctb'
try:
    idx=[r for r in csv.DictReader(open(F+'/INDEX.tsv'),delimiter='\t') if r['state']=='MATCHED']
    pay={r['address'].lower():r for r in json.load(open(F+'/payload.json'))['result']['authored']}
    rows=['address\tmodule\tname\tlength\tsource']; order=[]; diffs=0
    for r in idx:
        a=r['address'].lower(); p=pay[a]; W=f'{B}/{a}'; os.makedirs(W,exist_ok=True)
        src=open(r['best_source'],encoding='utf-8',errors='replace').read(); tst=open(r['test'],encoding='utf-8',errors='replace').read()
        if src!=p['source_cpp'] or tst!=p['test_cpp']: diffs+=1
        name=p['name']; mod=p['module']
        if '::' in name and not name.startswith('?'): name=name.rsplit('::',1)[1]
        row={'address':a,'module':mod,'name':name,'source_cpp':src,'test_cpp':tst,'pass_pattern':a+'_TEST PASS'}
        fl=p.get('flags') or []
        if isinstance(fl,str): fl=fl.split()
        m=re.search(r'^// Flags:\s*(.+)$',src,re.M)
        if fl: row['flags']=fl
        json.dump({'result':{'authored':[row]}},open(W+'/payload.json','w'),indent=1)
        if os.path.isdir(W+'/hdr'): shutil.rmtree(W+'/hdr')
        if r['headers'] and os.path.isdir(r['headers']): shutil.copytree(r['headers'],W+'/hdr')
        else: os.makedirs(W+'/hdr')
        json.dump({'folder':r['best_source'].split('/')[4],'open':r['open_items'],'bytes':int(r['bytes']),'flags':fl,'srcflags':m.group(1) if m else '','leaf':p.get('leaf','')},open(W+'/meta.json','w'))
        rows.append(f"{a}\t{mod}\t{name}\t{r['bytes']}\t{r['best_source']}"); order.append(a)
    open(B+'/rows.tsv','w').write('\n'.join(rows)+'\n'); json.dump(order,open(B+'/order.json','w'))
    print('rows',len(order),'| index file differs from combined payload for',diffs)
except Exception as e: print('setup stopped:',type(e).__name__,str(e)[:300])
