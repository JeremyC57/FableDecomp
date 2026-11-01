# shared: landed row table (address -> dict with Source/TestSource/...) and helpers
import re
ROOT='/home/user/FableDecomp'
def rows():
    bc=open(ROOT+'/rebuild/build_candidates.ps1',encoding='utf-8').read()
    ent={}
    for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
        b=m.group(1); a=re.search(r"Address = '([0-9a-fA-F]{8})'",b)
        if not a: continue
        d=dict(re.findall(r"(\w+) = '([^']*)'",b)); ent[a.group(1).lower()]=d
    return ent
def na(a):
    a=a.strip().lower()
    if a.startswith('0x'): a=a[2:]
    return a.zfill(8)
