import sys,subprocess,re,os
# callchk.py <addr> <leaf> <retail.dis> : compare call targets in order (built obj relocs vs retail)
S=os.environ['S']; a=sys.argv[1]; leaf=sys.argv[2]
T={'??YIs32vec4@@QAEAAV0@ABVI32vec4@@@Z':'c7b870','??5Is32vec4@@QAE?AV0@H@Z':'c7b8b0','??_2Is32vec4@@QAEAAV0@H@Z':'c7b8f0','@__security_check_cookie@4':'bfe9f9',
   '??0I32vec4@@QAE@T__m128i@@@Z':'c7b810','??0Is32vec4@@QAE@T__m128i@@@Z':'c7b840','??0Is16vec8@@QAE@T__m128i@@@Z':'c7b970','??0I16vec8@@QAE@T__m128i@@@Z':'I16','??0M128@@QAE@T__m128i@@@Z':'M128'}
m=subprocess.run(['python3',os.path.dirname(os.path.abspath(__file__))+'/relmap.py',S+'/wine/q/%s/%s.obj'%(a,a),leaf,a],capture_output=True,text=True).stdout
d=subprocess.run(['objdump','-d','-M','intel',S+'/wine/q/%s/%s.obj'%(a,a)],capture_output=True,text=True).stdout
B=[]
rel={}
for l in m.splitlines():
    x,s=l.split(); rel[int(x,16)]=s
r=[l.split()[-1][2:] for l in open(sys.argv[3]) if re.search(r'\bcall 0x',l)]
B=[T.get(s,s) for x,s in sorted(rel.items()) if not s.startswith('$') and 'security_cookie' not in s or 'check' in s]
print(len(r),len(B)); bad=[(i,x,y) for i,(x,y) in enumerate(zip(r,B)) if x!=y]
print('mismatch',bad[:10])
