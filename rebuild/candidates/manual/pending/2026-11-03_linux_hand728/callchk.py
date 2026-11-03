# callchk.py <addr> <len-hex> <obj> <leaf-substring>: pair retail direct call/jmp targets with our object's call relocations, in order
import sys,subprocess,re,os
addr,ln,obj,leaf=sys.argv[1:5]
S=os.environ['S']
ret=[t for t in [l.split()[-1] for l in subprocess.run(['python3',S+'/w17/xdis.py',addr,ln],capture_output=True,text=True).stdout.splitlines() if re.search(r' (call|jmp) 0x[0-9a-f]+$',l)] if not (int(addr,16)<=int(t,16)<int(addr,16)+int(ln,16))]
dis=subprocess.run(['objdump','-dr','--no-show-raw-insn',obj],capture_output=True,text=True).stdout
# pick the section of the leaf symbol
blocks=re.split(r'\n(?=[0-9a-f]{8} <)',dis)
blk=max([b for b in blocks if leaf in b.split('\n')[0]],key=len)
ours=[]; lines=blk.split('\n')
for i,l in enumerate(lines):
    if re.search(r'\t(call|jmp)\s',l) and i+1<len(lines) and 'DISP32' in lines[i+1] or ('call' in l and i+1<len(lines) and 'R_386_PC32' in lines[i+1]):
        ours.append(lines[i+1].split()[-1])
cat={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=3: cat.setdefault(p[1],set()).add(p[0])
for i in range(max(len(ret),len(ours))):
    r=ret[i] if i<len(ret) else '-'; o=ours[i] if i<len(ours) else '-'
    b=cat.get(r[2:].rjust(8,'0'),set())
    print(r, o, 'OK' if o in b else ('?' if not b else 'BOUND:'+'|'.join(sorted(b))[:120]))
