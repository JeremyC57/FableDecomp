# ocalls.py <addr> <tag>: call relocation targets (in order) inside our graded function
import sys,os,subprocess,re
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
A=os.environ['S']+'/w22/agent860'; a,tag=sys.argv[1:3]
src=open(f'{A}/w/{a}_{tag}/{a}.cpp').read(); leaf=re.search(r"^// Leaf: *(\S+)",src,re.M).group(1)
b,sec,sym=V.obj_text(f'{A}/w/{a}_{tag}/{a}.obj',leaf)
out=subprocess.run([V.OBJDUMP,'-dr','-z',f'{A}/w/{a}_{tag}/{a}.obj'],capture_output=True,text=True).stdout
on=False; prev=''
for l in out.splitlines():
    m=re.match(r'^[0-9a-f]+ <(.*)>:',l)
    if m: on=(m.group(1)==sym); continue
    if not on: continue
    if 'DISP32' in l or 'REL32' in l:
        if 'call' in prev: print(prev.split(':')[0].strip(), l.split()[-1])
    prev=l
