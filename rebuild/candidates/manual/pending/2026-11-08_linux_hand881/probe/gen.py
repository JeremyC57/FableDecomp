import sys
# gen.py <out.cpp> flags... : StereoFIR generator
#  W1 += arg wrap, W2 k3/4 inner wrap, W3 sum unpack wrap, W4 sum2 unpack wrap, W5 a/b wrap, W7 e/f wrap
#  O<n> output-stage spelling variants
F=set(sys.argv[2:])
head=open(sys.argv[1].rsplit('/',1)[0]+'/head_s.txt' if '/' in sys.argv[1] else 'head_s.txt').read()
o=[head,'    switch (increment)\n    {\n']
def term(k,j,wrap):
    m='_mm_madd_epi16(in[%d], c[%d])'%(k,j)
    return 'Is32vec4(%s)'%m if wrap else m
def red(name,x,y,wrap):
    lo='_mm_unpacklo_epi32(%s, %s)'%(x,y); hi='_mm_unpackhi_epi32(%s, %s)'%(x,y)
    if wrap: lo='Is32vec4(%s)'%lo; hi='Is32vec4(%s)'%hi
    return '            Is32vec4 %s = _mm_add_epi32(%s, %s);\n'%(name,lo,hi)
def outp(v):
    s='            %s >>= 14;\n'%v
    if 'OA' in F: s+='            *out1 = _mm_add_epi32(*out1, %s);\n'%v
    else: s+='            *out1 = Is32vec4(_mm_add_epi32(*out1, %s));\n'%v
    s+='            ++out1;\n'
    s+='            %s >>= 1;\n'%v
    if 'OA' in F: s+='            *out2 = _mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(%s, panGain)) >> 14);\n'%v
    else: s+='            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(%s, panGain)) >> 14));\n'%v
    s+='            ++out2;\n'
    return s
for n in range(8):
    o.append('    case %d:\n        for (unsigned long i = 0; i < count; ++i)\n        {\n'%n)
    for blk,(rows,base) in enumerate([((4,5,6,7),1),((0,1,2,3),2)]):
        nm={r:('s%d' if blk==0 else 't%d')%rows.index(r) for r in rows}
        taps={}
        for r in rows:
            taps[r]=[(base-1+t,5*r+t) for t in range(4)] if r<n else [(base+t,5*r+1+t) for t in range(4)]
        started=set()
        for k in sorted(set(k for r in rows for k,_ in taps[r])):
            for r in rows:
                for kk,j in taps[r]:
                    if kk!=k: continue
                    v=nm[r]
                    if r not in started:
                        o.append('            Is32vec4 %s = %s;\n'%(v,term(k,j,False))); started.add(r)
                    elif blk==1 and k in (3,4):
                        o.append('            %s = Is32vec4(_mm_add_epi32(%s, %s));\n'%(v,v,term(k,j,'W2' in F)))
                    else:
                        o.append('            %s += %s;\n'%(v,term(k,j,'W1' in F)))
        if blk==0:
            o.append(red('a','s0','s2','W5' in F)); o.append(red('b','s1','s3','W5' in F)); o.append(red('sum','a','b','W3' in F)); o.append(outp('sum'))
        else:
            o.append(red('e','t0','t2','W7' in F)); o.append(red('f','t1','t3','W7' in F)); o.append(red('sum2','e','f','W4' in F)); o.append(outp('sum2'))
    o.append('            ++in;\n        }\n        break;\n')
o.append('    }\n}\n')
open(sys.argv[1],'w').write(''.join(o))
