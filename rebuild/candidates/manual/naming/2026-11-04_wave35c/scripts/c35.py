# common helpers for wave 35c
import sys, os, re, json, struct, collections
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, pdb, fname, ROOT
os.environ.setdefault('S','/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'); W=os.environ['S']+'/wave35c'
def rows():
    return [l.rstrip('\n').split('\t') for l in open(W+'/ROWS.tsv')][1:]
_refs=None
def dword_refs(target):
    """all file offsets -> VA of dwords equal to target in non-text sections"""
    global _refs
    from pe import PE
    if _refs is None:
        p=PE(); idx=collections.defaultdict(list)
        for s in p.secs:
            if s[0]=='.text': continue
            d=p.d[s[3]:s[3]+s[4]]
            for i in range(0,len(d)-3,4):
                v=struct.unpack_from('<I',d,i)[0]
                if 0x401000<=v<0x1400000: idx[v].append(s[1]+i)
        _refs=(p,idx)
    return _refs[1].get(target,[]), _refs[0]
def src_of(addr):
    e=ps1_entries().get(addr)
    return open(ROOT+'/rebuild/src/compiled/'+e['Source'],encoding='utf-8').read()
def write(path,text):
    os.makedirs(os.path.dirname(path),exist_ok=True); open(path,'w',encoding='utf-8').write(text)
