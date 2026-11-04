# annot_conflicts.py <BINDINGS.tsv>...: mark CONFLICT lines that the pre-wave base object emits too
import glob,subprocess,sys
W='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b'
for fn in sys.argv[1:]:
    L=open(fn).read().rstrip('\n').split('\n')
    for i,l in enumerate(L):
        a=l.split('\t')
        if a[0]!='CONFLICT': continue
        o=glob.glob(W+'/base/rk_part_*/work/*/*/%s/%s.obj'%(a[3],a[3]))
        t=subprocess.run(['objdump','-t',o[0]],capture_output=True,text=True).stdout if o else ''
        L[i]=l+('; pre-existing: the landed (pre-wave-35b) source of %s emits the same symbol for the same target; not introduced here, not flagged by vland (vtable); do not apply'%a[3] if a[1] in t else '; NEW')
    open(fn,'w').write('\n'.join(L)+'\n')
