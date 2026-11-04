import sys,os,re,csv,collections,json
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, pdb, fname, ROOT
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
W=S+'/naming/vendor/w1'
def scope(a):
    v=int(a,16)
    if 0xC08AE0<=v<0xC57C90: return 'libpk'
    if v<0xC5E180 and v>=0xC57C90: return 'gamecoda'
    if 0xC5E180<=v<0xC89000: return 'libpk2'
    if 0xCA4000<=v<0xCB2000: return 'strmbase'
    return 'out'
def inventory():
    return {r['address']:r for r in csv.DictReader(open(W+'/INVENTORY.tsv'),delimiter='\t')}
def bindings():
    return list(csv.DictReader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'))
def src_path(a,ent=None):
    ent=ent or ps1_entries(); e=ent.get(a)
    return ROOT+'/rebuild/src/compiled/'+e['Source'] if e else None
def leaf_of(text):
    m=re.search(r'^//\s*Leaf:\s*(\S+)',text,re.M); return m.group(1) if m else None
