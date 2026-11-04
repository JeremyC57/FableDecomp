# objsyms.py <workroot> <addr...> : graded symbol per row (same rule as verify_and_land.obj_text) + all function symbols
import sys,re,subprocess,glob,os,json
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
DS=re.compile(r"^([0-9a-f]+) <(.+)>:$")
DB=re.compile(r"^\s+[0-9a-f]+:\s+((?:[0-9a-f]{2} )+)")
TEXTSEC_D=re.compile(r"^Disassembly of section \.text(?:\$\w+)?:")
def funcs(path):
    pr=subprocess.run(['objdump','-d','-z',path],capture_output=True,text=True)
    fns=[];cur=None
    for line in pr.stdout.splitlines():
        if TEXTSEC_D.match(line): cur=None; continue
        s=DS.match(line)
        if s:
            if int(s.group(1),16)==0 or cur is None:
                cur=[s.group(2),0]; fns.append(cur)
            continue
        e=DB.match(line)
        if e and cur is not None: cur[1]+=len(e.group(1).split())
    return fns
def graded(path,leaf):
    fns=funcs(path)
    named=[f for f in fns if leaf and leaf in f[0]]
    sel=max(named or fns,key=lambda f:f[1]) if fns else None
    return sel,fns,bool(named)
if __name__=='__main__':
    root=sys.argv[1]; pay=json.load(open(sys.argv[2]))['result']['authored']
    for e in pay:
        a=e['address']; ob=glob.glob(f'{root}/{a[:2]}/{a[2:4]}/{a}/{a}.obj')
        if not ob: print(a,'NOOBJ',sep='\t'); continue
        sel,fns,nm=graded(ob[0],e['leaf'])
        print(a,sel[0] if sel else '-',sel[1] if sel else 0,'named' if nm else 'largest',' '.join(f[0] for f in fns),sep='\t')
