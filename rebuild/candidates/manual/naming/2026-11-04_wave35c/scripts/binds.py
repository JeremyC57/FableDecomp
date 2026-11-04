# binds.py addr...: reloc_bindings symbols bound to each target, with first binder
import sys, csv
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ROOT
want=set(a.lower() for a in sys.argv[1:])
for r in csv.DictReader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'):
    if r['target'] in want: print(r['target'],r['symbol'],'by',r['address'])
