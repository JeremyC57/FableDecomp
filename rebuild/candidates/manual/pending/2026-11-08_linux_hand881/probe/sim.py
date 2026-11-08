import re,sys
# symbolic simulator for the SSE2 resampler rows; prints per-case statements
lines=[]
for l in open(sys.argv[1]):
    p=l.split(None,2)
    if len(p)<3 or len(p[0])!=8: continue
    try: a=int(p[0],16)
    except: continue
    lines.append((a,p[2].strip()))
lo=int(sys.argv[2],16); hi=int(sys.argv[3],16)
lines=[x for x in lines if lo<=x[0]<hi]
R={}; X={}; M={}; esp=0
def ival(s):
    s=s.strip()
    if re.match(r'^-?0x[0-9a-f]+$',s): return int(s,16)
    if re.match(r'^-?\d+$',s): return int(s)
    return None
def addr(s):
    m=re.match(r'(?:xmmword|dword) ptr \[(\w+)(?: ([+-]) (0x[0-9a-f]+|\d+))?\]',s)
    if not m: return None
    b=m.group(1); off=int(m.group(3),16) if m.group(3) and m.group(3).startswith('0x') else int(m.group(3) or 0)
    if m.group(2)=='-': off=-off
    return b,off
def mem(s):
    b,off=addr(s)
    if b=='esp': return ('stk',off+esp)
    if b=='ebp': return ('arg',off)
    v=R.get(b)
    if isinstance(v,tuple) and isinstance(v[1],int): return (v[0],v[1]+off)
    return ('?'+b,off)
def name(m):
    k,o=m
    if k=='in': return 'in[%g]'%(o/16)
    if k=='c': return 'c[%g]'%(o/16)
    if k=='out': return 'out[%g]'%(o/16)
    if k=='stk': return 'S%x'%o
    return '%s%+d'%(k,o)
out=[]
def emit(s): out.append(s)
cnt=0
for a,ins in lines:
    m=re.match(r'(\w+)\s*(.*)',ins); op=m.group(1); args=[x.strip() for x in re.split(r',(?![^\[]*\])',m.group(2))] if m.group(2) else []
    if op=='jmp' and 'eax*4' in ins: emit('--- switch'); continue
    if op in('movdqa','movdqu'):
        d,s=args
        if re.match(r'xmm\d$',d):
            if re.match(r'xmm\d$',s): X[d]=X.get(s,'?')
            else:
                mm=mem(s)
                X[d]=M.get(mm, name(mm)) if mm[0]=='stk' else name(mm)
        else:
            mm=mem(d)
            if mm[0]=='stk': M[mm]=X.get(s,'?')
            else: emit('%x: %s = %s'%(a,name(mm),X.get(s,'?')))
        continue
    if op in ('pmaddwd','paddd','punpckldq','punpckhdq','paddsw','pmulhw','packssdw','punpcklwd','punpckhwd','psubd','pmulhuw'):
        d,s=args
        sv=X.get(s,'?') if re.match(r'xmm\d$',s) else (M.get(mem(s),name(mem(s))))
        sym={'pmaddwd':'*','paddd':'+'}.get(op)
        if sym: X[d]='(%s %s %s)'%(X.get(d,'?'),sym,sv)
        else: X[d]='%s(%s, %s)'%(op,X.get(d,'?'),sv)
        continue
    if op in('psrad','psraw','pslld','psrld'):
        X[args[0]]='(%s >> %s)'%(X.get(args[0],'?'),args[1]); continue
    if op=='movd':
        d,s=args
        if re.match(r'xmm\d$',d): X[d]='movd(%s)'%s
        continue
    if op=='push':
        esp-=4
        v=ival(args[0]); 
        M[('stk',esp)]=('imm',v) if v is not None else R.get(args[0],args[0]); continue
    if op=='pop': esp+=4; continue
    if op=='call':
        tgt=args[0]
        th=R.get('ecx')
        if tgt=='0xc7b870':
            arg=M.get(('stk',esp)); esp+=4
            an=M.get(arg,name(arg)) if isinstance(arg,tuple) and arg[0]=='stk' else str(arg)
            emit('%x: CALL %s += %s'%(a,name(th) if isinstance(th,tuple) else th, an))
            if isinstance(th,tuple) and th[0]=='stk': M[th]='(%s += %s)'%(M.get(th,name(th)),an)
            R['eax']=th
        elif tgt=='0xc7b8b0':
            res=M.get(('stk',esp)); cntv=M.get(('stk',esp+4)); esp+=8
            v='(%s >>call %s)'%(M.get(th,name(th)) if isinstance(th,tuple) else th, cntv)
            M[res]=v; R['eax']=res
            emit('%x: CALL %s = %s'%(a,name(res),v))
        else:
            emit('%x: CALL %s'%(a,tgt))
        continue
    if op=='lea':
        d,s=args; mm=addr(s.replace('[','dword ptr ['))
        b,off=mm
        if b=='esp': R[d]=('stk',off+esp)
        else:
            v=R.get(b); R[d]=(v[0],v[1]+off) if isinstance(v,tuple) else None
        continue
    if op=='mov':
        d,s=args
        if d.startswith('dword'):
            mm=mem(d); 
            if mm[0]=='stk': M[mm]=R.get(s, ival(s))
            continue
        if s.startswith('dword'):
            mm=mem(s)
            if mm==('arg',8): R[d]=('in',0)
            elif mm==('arg',0xc): R[d]=('c',0)
            elif mm==('arg',0x18) or mm==('arg',0x1c) and False: R[d]=('out',0)
            elif mm[0]=='stk': R[d]=M.get(mm)
            else: R[d]=('ld',mm)
            continue
        R[d]=R.get(s, ival(s)); continue
    if op=='add' or op=='sub':
        d,s=args; v=ival(s)
        if d=='esp': esp+= v if op=='add' else -v; continue
        if isinstance(R.get(d),tuple) and v is not None and isinstance(R[d][1],int):
            R[d]=(R[d][0],R[d][1]+(v if op=='add' else -v))
        continue
    if op=='ret': emit('--- ret'); esp=0; continue
    if op in('dec','test','cmp','jne','jbe','ja','je','jmp','and','push','nop','emms','xor','inc'): 
        if op=='jne': emit('%x: loop'%a)
        continue
for l in out: print(l)
