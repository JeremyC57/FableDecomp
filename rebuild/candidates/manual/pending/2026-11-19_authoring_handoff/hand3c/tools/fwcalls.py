import re,sys,subprocess,os
W=os.environ.get('W','/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c')
names={}
for l in open('/home/user/FableDecomp/ghidra_out/fablewin_pdb_names.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=2: names[int(p[0],16)]=p[1]
out=subprocess.run(['python3',W+'/tools/pedis.py','/tmp/claude-0/egod_pdb/FableWin.exe',sys.argv[1],sys.argv[2]],capture_output=True,text=True).stdout
full='-v' in sys.argv
for l in out.splitlines():
    m=re.search(r'call 0x([0-9a-f]+)',l)
    if m:
        a=int(m.group(1),16); n=names.get(a)
        if not n:
            r=subprocess.run(['python3',W+'/tools/pedis.py','/tmp/claude-0/egod_pdb/FableWin.exe','%x'%a,'5'],capture_output=True,text=True).stdout
            m2=re.search(r'jmp 0x([0-9a-f]+)',r)
            if m2: n=names.get(int(m2.group(1),16),'?')
        print(l[:60],';',n)
    elif full: print(l)
    if re.search(r'\bret\b',l): break
