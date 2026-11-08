"""Diagnostic frame-slot alignment; never a byte-match or symbol-identity gate."""
import argparse, collections, difflib, json, re, subprocess
import grade

p=argparse.ArgumentParser()
p.add_argument('address',choices=grade.LENGTHS)
p.add_argument('tag')
a=p.parse_args()
d=grade.S/'history'/a.address/a.tag
r=json.loads((d/'RESULT.json').read_text())
V=grade.V
listing=d/'frame_listing.asm'
if not listing.exists():
    e=V.env()
    e['INCLUDE']=';'.join(V.wpath(x)for x in[d/'hdr',grade.S/'support/rebuild/include',V.VC/'include'])
    cmd=[V.WINE,str(V.CL_EXE)]+V.tool_args(['/c','/O2','/Oy','/W3']+r['flags']+['/FAs','/Fa'+str(listing),'/Fo'+str(d/'listing.obj'),str(d/'candidate.cpp')])
    cp=subprocess.run(cmd,env=e,capture_output=True,text=True,cwd=d,timeout=240)
    (d/'listing_compiler.log').write_text(cp.stdout+cp.stderr)
    assert cp.returncode==0,cp.stderr
    secs,syms=grade.coff(d/'listing.obj')
    leaf=next(x for x in syms.values()if x['name']==r['leaf']and x['section']>0)
    assert secs[leaf['section']-1]['data']==(d/'built.bin').read_bytes()
    (d/'LISTING_VALIDATION.json').write_text(json.dumps({'ordinary_leaf_byte_identical':True},indent=2))
t=listing.read_text(errors='replace').split(r['leaf']+' PROC',1)[0]
t=t[t.rfind('_TEXT\tSEGMENT'):]
slots=[dict(name=m[0],offset=int(m[1]),size=int(m[2]))for m in re.findall(r'^([^\s=]+)\s*=\s*(-?\d+)\s*; size = (\d+)',t,re.M)]
(d/'frame_symbols.json').write_text(json.dumps(slots,indent=2))
def read(which):
    lines=(d/(which+'.disasm')).read_text().splitlines()
    out=[]
    for line in lines:
        addr,ins=line.split(' ',1)
        norm=re.sub(r'\b(?:eax|ebx|ecx|edx|esi|edi)\b','REG',ins)
        norm=re.sub(r'\b(?:al|bl|cl|dl)\b','BYTE',norm)
        norm=re.sub(r'\[(e[bs]p)(?: [-+] (?:0x[0-9a-f]+|[0-9]+))?\]',r'[\1 SLOT]',norm)
        if ins.split()[0].startswith('j') or ins.startswith('call '):norm=re.sub(r'0x[0-9a-f]+','TARGET',norm)
        else:norm=re.sub(r'0x[0-9a-f]+',lambda m:'ADDR'if int(m[0],16)>=0x400000 else m[0],norm)
        out.append((addr,ins,norm))
    return out
x,y=read('retail'),read('built')
sm=difflib.SequenceMatcher(None,[z[2]for z in x],[z[2]for z in y],autojunk=False)
maps=collections.defaultdict(collections.Counter)
sites=collections.defaultdict(list)
def disp(ins):
    m=re.search(r'\[ebp(?: ([+-]) (0x[0-9a-f]+|[0-9]+))?\]',ins)
    if not m:return None
    return int(m[2],0)*(-1 if m[1]=='-'else 1)if m[2]else 0
for i,j,n in sm.get_matching_blocks():
    for q,z in zip(x[i:i+n],y[j:j+n]):
        ro,bo=disp(q[1]),disp(z[1])
        if ro is not None and bo is not None:
            maps[bo][ro]+=1
            sites[bo].append((q[0],z[0]))
rows=[]
for bo,counts in sorted(maps.items()):
    ro,n=counts.most_common(1)[0]
    rows.append({'built':bo,'retail':ro,'count':n,'confidence':n/sum(counts.values()),'names':[s['name']for s in slots if s['offset']==bo],'sites':sites[bo]})
(d/'frame_map.json').write_text(json.dumps(rows,indent=2))
byret=collections.defaultdict(list)
for r in rows:
    if r['confidence']>=0.8:byret[r['retail']].append({k:v for k,v in r.items()if k!='sites'})
collisions={hex(k):v for k,v in byret.items()if len(v)>1}
(d/'frame_collisions.json').write_text(json.dumps(collisions,indent=2))
print(json.dumps({'slots':len(slots),'mapped':len(rows),'collisions':collisions},indent=2))
