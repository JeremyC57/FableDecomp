import os,sys
S=os.environ['S']
cat={l.split('\t')[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv')}
parked=set(open(S+'/w23/parked.txt').read().split())
rows=[l.split('\t') for l in open(S+'/batch/eh_open_oracle.tsv').read().splitlines()[1:]]
rows=[r for r in rows if r[0].lower() not in cat and r[0].lower() not in parked and '8bd1' not in r[3][:60]]
rows.sort(key=lambda r:int(r[2]))
n=int(sys.argv[1]) if len(sys.argv)>1 else 8
if len(sys.argv)>2: print(' '.join(r[0] for r in rows[:n]))
else:
    print(len(rows),'open'); [print(r[0],r[2],r[1][:60]) for r in rows[:n]]
