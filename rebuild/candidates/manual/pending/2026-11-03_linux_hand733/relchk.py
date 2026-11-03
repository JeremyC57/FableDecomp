# relchk.py <addr> : pair built-obj relocations (in the leaf function) with retail annotated operands, in order
import sys,subprocess,re
a=sys.argv[1]; S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
D=S+'/w22/chunk733'
obj=f'{S}/wine/q/{a}/{a}.obj'
leaf=sys.argv[2]
out=subprocess.run(['objdump','-dr','-M','intel',obj],capture_output=True,text=True).stdout
fn=None; rel=[]
for l in out.split('\n'):
    m=re.match(r'^[0-9a-f]+ <(.*)>:$',l)
    if m: fn=m.group(1); continue
    if fn and leaf in fn and ('DISP32' in l or 'DIR32' in l):
        rel.append(l.split()[-1])
ann=[l for l in open(f'{D}/dis/{a}.ann') if ';' in l or re.search(r'0x[0-9a-f]{6,}',l)]
ret=[]
for l in open(f'{D}/dis/{a}.ann'):
    ins=l.split(';')[0]
    if re.search(r'0x(1[0-9a-f]{6}|[4-9a-f][0-9a-f]{5}|c[0-9a-f]{5})\b',ins) and not re.search(r'^\S+ j',ins):
        ret.append((ins.strip(), l.split(';')[1].strip() if ';' in l else ''))
print(len(rel),'built relocs;',len(ret),'retail operands')
for i in range(max(len(rel),len(ret))):
    r=rel[i] if i<len(rel) else ''
    t=ret[i] if i<len(ret) else ('','')
    print(f'{r[:70]:72} | {t[0][:45]:45} | {t[1][:60]}')
