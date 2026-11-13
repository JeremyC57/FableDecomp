"""pin_asserts.py <old-include-tree> <new-include-tree> <header-relpath>... : after a header edit, keep every FABLE_STATIC_ASSERT typedef
name as it was. The macro names its typedef fable_static_assert_<__LINE__>, and VC7.1's stack-slot order in some rows depends on those
names (row 0x007E2A60; GOTCHAS, landing fz), so a line shift is not neutral. Every assert whose line number changed between <old> and
<new> is respelled FABLE_STATIC_ASSERT_AT(<old line>, ...), which pins the old name whatever line it sits on. Rewrites <new>."""
import sys,re,difflib,io
old,new=sys.argv[1:3]; tot=0
for h in sys.argv[3:]:
    A=io.open(old+'/'+h,encoding='utf-8',newline='').read().split('\n'); B=io.open(new+'/'+h,encoding='utf-8',newline='').read().split('\n')
    sm=difflib.SequenceMatcher(None,A,B,autojunk=False); omap={}
    for tag,i1,i2,j1,j2 in sm.get_opcodes():
        if tag=='equal':
            for k in range(i2-i1): omap[j1+k]=i1+k
    n=0; warn=[]
    for j,l in enumerate(B):
        if 'FABLE_STATIC_ASSERT(' not in l: continue
        if j not in omap:
            warn.append('line %d: new or changed assert, not pinned: %s'%(j+1,l.strip()[:80])); continue
        if omap[j]==j: continue
        if l.count('FABLE_STATIC_ASSERT(')!=1 or not l.rstrip().rstrip('\r').endswith(');'):
            warn.append('line %d: unusual assert spelling, pin by hand: %s'%(j+1,l.strip()[:80])); continue
        B[j]=l.replace('FABLE_STATIC_ASSERT(','FABLE_STATIC_ASSERT_AT(%d, '%(omap[j]+1)); n+=1
    io.open(new+'/'+h,'w',encoding='utf-8',newline='').write('\n'.join(B)); tot+=n
    print('%-50s pinned %d'%(h,n)); [print('   !!',w) for w in warn]
print('total pinned',tot)
