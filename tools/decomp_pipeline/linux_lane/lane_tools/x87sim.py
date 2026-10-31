#!/usr/bin/env python3
"""x87sim.py <q.py output file> [retail|built] : symbolic x87 simulation; prints each fstp target = expression"""
import sys, re
t=open(sys.argv[1]).read(); side=sys.argv[2] if len(sys.argv)>2 else 'retail'
if side=='retail': blk=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0]
else: blk=re.findall(r"--- built.*",t,re.S)[0]
lines=[l.split(None,2) for l in blk.splitlines()[1:]]
st=[]; out=[]
adj=[0]
def m(op):
    op=op.replace('dword ptr ','').strip()
    mm=re.match(r'\[esp(?: \+ (0x[0-9a-f]+|\d+))?\]$',op)
    if mm:
        v=int(mm.group(1),0) if mm.group(1) else 0
        op='[esp + %s]'%hex(v+adj[0])
    return op
for l in lines:
    if len(l)<3: continue
    ins=l[2]; mn=ins.split()[0]; ops=ins[len(mn):].strip()
    def sti(o):
        mm=re.match(r'st\((\d)\)',o); return int(mm.group(1)) if mm else (0 if o=='st(0)' else None)
    if mn=='push': adj[0]-=4
    elif mn=='pop': adj[0]+=4
    elif mn=='call': adj[0]=0
    try:
        if mn in('fld','fild'):
            if ops.startswith('st('): st.append(st[-1-sti(ops)])
            else: st.append(m(ops))
        elif mn in('fmul','fadd','fsub','fsubr','fdiv','fdivr'):
            o={'fmul':'*','fadd':'+','fsub':'-','fsubr':'-r','fdiv':'/','fdivr':'/r'}[mn]
            parts=[p.strip() for p in ops.split(',')]
            if len(parts)==1 and not parts[0].startswith('st'):
                a=st[-1]; b=m(parts[0])
                st[-1]=f"({b} {o[0]} {a})" if o.endswith('r') else f"({a} {o} {b})"
            else:
                i=sti(parts[0]); j=sti(parts[1]) if len(parts)>1 else 0
                a=st[-1-i]; b=st[-1-j]
                st[-1-i]=f"({b} {o[0]} {a})" if o.endswith('r') else f"({a} {o} {b})"
        elif mn in('faddp','fmulp','fsubp','fsubrp','fdivp','fdivrp'):
            o={'faddp':'+','fmulp':'*','fsubp':'-','fsubrp':'-r','fdivp':'/','fdivrp':'/r'}[mn]
            i=sti(ops) if ops else 1
            a=st[-1-i]; b=st[-1]
            st[-1-i]=f"({b} {o[0]} {a})" if o.endswith('r') else f"({a} {o} {b})"
            st.pop()
        elif mn=='fxch':
            i=sti(ops) if ops else 1; st[-1],st[-1-i]=st[-1-i],st[-1]
        elif mn in('fstp','fistp'):
            if ops.startswith('st('):
                i=sti(ops); st[-1-i]=st[-1]; st.pop()
            else: out.append((l[0],m(ops),st[-1])); st.pop()
        elif mn in('fst','fist'):
            out.append((l[0],m(ops),st[-1]))
        elif mn in('fchs','fabs','fsqrt'):
            st[-1]=f"{mn}({st[-1]})"
        elif mn in ('fcomp','fcom','fcompp'):
            out.append((l[0],'CMP '+ops,st[-1] if st else '?'))
            if mn=='fcomp': st.pop()
            if mn=='fcompp': st.pop(); st.pop()
        elif mn=='call' and st:
            out.append((l[0],'CALL-with-stack',str(len(st))))
        elif mn.startswith('f'):
            out.append((l[0],'UNHANDLED '+ins,''))
    except IndexError:
        out.append((l[0],'STACKERR '+ins,'')); st=[]
for a,d,e in out: print(a,d,'=',e)
