# wrep.py <addr> <old> <new> <expected-count> [<old> <new> <count> ...]: copy orig/<addr>.cpp (or rows/<addr>.cpp when it exists and
# the first arg is +<addr>) to rows/<addr>.cpp with exact-substring replacements, each with an exact expected occurrence count. NF314.
import sys,os
A=os.path.dirname(os.path.abspath(__file__))+'/..'
a=sys.argv[1]; cont=a.startswith('+'); a=a.lstrip('+')
src=A+('/rows/%s.cpp' if cont else '/orig/%s.cpp')%a
t=open(src,encoding='utf-8').read(); r=sys.argv[2:]
for i in range(0,len(r),3):
    o,n,c=r[i],r[i+1],int(r[i+2])
    if t.count(o)!=c: sys.exit('FAIL %s: %d (want %d) of %r'%(a,t.count(o),c,o[:90]))
    t=t.replace(o,n)
open(A+'/rows/%s.cpp'%a,'w',encoding='utf-8').write(t); print('OK',a,len(r)//3)
