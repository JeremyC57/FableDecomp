import sys
# gen.py <out.cpp> flags... : StereoFIR generator
#  W1 += arg wrap, W2 k3/4 inner wrap, W3 sum unpack wrap, W4 sum2 unpack wrap, W5 a/b wrap, W7 e/f wrap
#  O<n> output-stage spelling variants
F=set(sys.argv[2:])
head=open(sys.argv[1].rsplit('/',1)[0]+'/head_r.txt' if '/' in sys.argv[1] else 'head_r.txt').read()
if 'IN' in F: head=head.replace('const Is16vec8* in = (const Is16vec8*)(source - 40);','const __m128i* in = (const __m128i*)(source - 40);')
if 'CO' in F: head=head.replace('const Is16vec8* c = (const Is16vec8*)phase;','const __m128i* c = (const __m128i*)phase;')
if 'H1' in F:
    head=head.replace('(signed short)(startGain + 3 * gainStep)','startGain + 3 * gainStep').replace('(signed short)(startGain + 2 * gainStep)','startGain + 2 * gainStep').replace('(signed short)(startGain + gainStep)','startGain + gainStep').replace('(signed short)startGain','startGain').replace('(signed short)(4 * gainStep)','4 * gainStep')
if 'H2' in F: head=head.replace('(signed short)pan','pan')
if 'H3' in F:
    i=head.index('    Is16vec8 panGain'); j=head.index('\n',i)+1; pl=head[i:j]; head=head[:i]+head[j:]
    k=head.index('    Is16vec8 gain('); head=head[:k]+pl+head[k:]
if 'H4' in F:
    head=head.replace('    Is16vec8 step(0, (signed short)(4 * gainStep), 0, (signed short)(4 * gainStep),\n                  0, (signed short)(4 * gainStep), 0, (signed short)(4 * gainStep));','    short delta = 4 * gainStep;\n    Is16vec8 step(0, delta, 0, delta, 0, delta, 0, delta);')
o=[head,'    switch (increment)\n    {\n']
def term(k,j,wrap):
    m='_mm_madd_epi16(in[%d], c[%d])'%(k,j)
    if wrap=='I': return 'I32vec4(%s)'%m
    return 'Is32vec4(%s)'%m if wrap else m
def red(name,x,y,wrap):
    lo='_mm_unpacklo_epi32(%s, %s)'%(x,y); hi='_mm_unpackhi_epi32(%s, %s)'%(x,y)
    if wrap: lo='Is32vec4(%s)'%lo; hi='Is32vec4(%s)'%hi
    return '            Is32vec4 %s = _mm_add_epi32(%s, %s);\n'%(name,lo,hi)
def outp(v):
    s='            %s >>= 15;\n'%v
    vv=('Is16vec8(%s)'%v) if ('G' in F or 'GA' in F) else v
    s+='            %s = Is32vec4(_mm_madd_epi16(%s, gain)) >> 14;\n'%(v,vv)
    s+='            *out1 = Is32vec4(_mm_add_epi32(*out1, %s));\n'%v
    s+='            ++out1;\n'
    s+='            gain = Is16vec8(_mm_adds_epi16(gain, step));\n'
    s+='            %s >>= 1;\n'%v
    vv=('Is16vec8(%s)'%v) if ('G' in F or 'GP' in F) else v
    s+='            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(%s, panGain)) >> 14));\n'%vv
    s+='            ++out2;\n'
    return s
for n in range(8):
    lp='for (unsigned long i = 0; i < count; ++i)'
    if 'L1' in F: lp='for (unsigned long i = count; i != 0; --i)'
    if 'L2' in F: lp='for (unsigned long i = count; i > 0; --i)'
    if 'L3' in F: lp='for (unsigned long i = 0; i != count; ++i)'
    o.append('    case %d:\n        %s\n        {\n'%(n,lp))
    for f in F:
        if f.startswith('D%d_'%n): 
            for q in range(int(f.split('_')[1])): o.append('            out1[%d] += Is32vec4(_mm_setzero_si128());\n'%(q+50))
    for blk,(rows,base) in enumerate([((4,5,6,7),1),((0,1,2,3),2)]):
        nm={r:('s%d' if blk==0 else 't%d')%rows.index(r) for r in rows}
        taps={}
        for r in rows:
            taps[r]=[(base-1+t,5*r+t) for t in range(4)] if r<n else [(base+t,5*r+1+t) for t in range(4)]
        started=set(); n0=min(k for r in rows for k,_ in taps[r])
        for k in sorted(set(k for r in rows for k,_ in taps[r])):
            for r in rows:
                for kk,j in taps[r]:
                    if kk!=k: continue
                    v=nm[r]
                    if r not in started:
                        o.append('            Is32vec4 %s = %s;\n'%(v,term(k,j,'W0' in F or ('W0a' in F and blk==0) or ('W0b' in F and blk==1)))); started.add(r)
                    elif blk==1 and k in (3,4):
                        o.append('            %s = Is32vec4(_mm_add_epi32(%s, %s));\n'%(v,v,term(k,j,'W2' in F or ('W2n' in F and r>=n))))
                    else:
                        o.append('            %s += %s;\n'%(v,term(k,j,'W1' in F or ('I' if 'W1i' in F and blk==0 else ('W1a' in F and blk==0) or (blk==0 and ('W1k%d'%(k-n0)) in F and not ('NC0' in F and n==0)) or (blk==0 and ('C0k%d'%(k-n0)) in F and n==0) or (blk==0 and ('X%d_%d'%(rows.index(r),k-n0)) in F) or (blk==1 and k==5 and 'W1b5' in F)) or ('W1b' in F and blk==1))))
        if blk==0:
            o.append(red('a','s0','s2','W5' in F or 'W5a' in F)); o.append(red('b','s1','s3','W5' in F or 'W5b' in F)); o.append(red('sum','a','b','W3' in F)); o.append(outp('sum'))
        else:
            o.append(red('e','t0','t2','W7' in F)); o.append(red('f','t1','t3','W7' in F)); o.append(red('sum2','e','f','W4' in F)); o.append(outp('sum2'))
    o.append('            ++in;\n        }\n        break;\n')
o.append('    }\n}\n')
open(sys.argv[1],'w').write(''.join(o))
