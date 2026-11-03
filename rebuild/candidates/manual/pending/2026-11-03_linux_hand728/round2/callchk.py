# callchk.py <addr> <len-hex> <obj> <leaf-substring>: pair retail direct call targets with our object's call relocations, in order;
# flag each pair whose our-symbol is not among the bindings of the retail target
import sys,subprocess,re,os
addr,ln,obj,leaf=sys.argv[1:5]
S=os.environ['S']
ret=[]
for l in subprocess.run(['python3',S+'/w17/xdis.py',addr,ln],capture_output=True,text=True).stdout.splitlines():
    m=re.search(r' (call|jmp) 0x([0-9a-f]+)$',l)
    if m and not (int(addr,16)<=int(m.group(2),16)<int(addr,16)+int(ln,16)): ret.append(m.group(2).rjust(8,'0'))
dis=subprocess.run(['objdump','-dr','--no-show-raw-insn',obj],capture_output=True,text=True).stdout
secs=dis.split('Disassembly of section')
blk=max([b for b in secs if re.search(r'\n[0-9a-f]{8} <[^>]*'+re.escape(leaf),b)],key=len)
ours=[]; lines=blk.split('\n')
st=[i for i,l in enumerate(lines) if re.match(r'[0-9a-f]{8} <[^>+]*'+re.escape(leaf),l)][0]; lines=lines[st:]
for l in lines[1:]:
    if re.match(r'[0-9a-f]{8} <[^$]',l): break
    m=re.search(r'\sDISP32\s+(\S+)',l)
    if m: ours.append(m.group(1))
cat={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=3: cat.setdefault(p[1].lower(),set()).add(p[0])
bad=0
for i in range(max(len(ret),len(ours))):
    r=ret[i] if i<len(ret) else '-'; o=ours[i] if i<len(ours) else '-'
    b=cat.get(r,set())
    st='OK' if o in b else ('UNBOUND' if not b else 'DIFF')
    if st!='OK': bad+=1
    if st!='OK' or '-v' in sys.argv: print(r, o, st, ('BOUND:'+'|'.join(sorted(b))[:200]) if st=='DIFF' else '')
print('calls retail',len(ret),'ours',len(ours),'non-OK',bad)
