import re,os,sys,json,glob
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, ROOT, pdb, fname
W='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b'
AN=ROOT+'/rebuild/candidates/manual/naming/2026-11-04_anniversary'
def evidence():
    ev={}
    for f in [AN+'/wave3/labels_alloc.tsv',AN+'/labels_high.tsv',AN+'/labels_waves23.tsv']+glob.glob(AN+'/wave2/labels_*.tsv'):
        for l in open(f):
            a=l.rstrip('\n').split('\t')
            if len(a)>=3 and a[0]!='address' and a[0] not in ev: ev[a[0]]=(os.path.relpath(f,ROOT),a[2])
    return ev
def cats():
    return {l.split('\t')[0]:l.rstrip('\n').split('\t') for l in open(W+'/an/cat.tsv')}
def rowsrc(rel): return open(ROOT+'/rebuild/src/compiled/'+rel,encoding='utf-8').read()
def pdbwhere(sym):
    E,F=pdb(); return ' / '.join(n for n,s in (('Ego_r.pdb',E),('FableWin.pdb',F)) if sym in s)
def split_target(name):
    p=name.split('::'); assert p[0]=='NScript'
    return p[1:-1],p[-1]   # classes (outer..inner), method
def short_ev(e):
    e=re.sub(r'; Anniversary RTTI says [^;]*','',e)
    e=e.replace('; LANDED row (respell source)','')
    return e
