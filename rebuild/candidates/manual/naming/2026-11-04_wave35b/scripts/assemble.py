# assemble.py: copy canonical generated sources/tests into overlay/, write final/all.tsv (mkpay format) and RENAMES.tsv
import json,os,shutil,sys
W=os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R='/home/user/FableDecomp'
FAMS='tstruct3 lvlalloc lvltstruct frameloop plainctor requal lvlctor entctor minent man'.split()
os.makedirs(W+'/final',exist_ok=True)
allr=[];ren=[]
for f in FAMS:
    for l in open('%s/gen/%s/gen.tsv'%(W,f)):
        a=l.rstrip('\n').split('\t')
        if a[1]!='OK': continue
        d=json.loads(a[2])
        so=W+'/overlay/rebuild/src/compiled/'+d['new_src']; to=W+'/overlay/rebuild/tests/'+d['new_test']
        os.makedirs(os.path.dirname(so),exist_ok=True); os.makedirs(os.path.dirname(to),exist_ok=True)
        sp=d['path'] if os.path.isabs(d['path']) else os.path.normpath(W+'/scripts/'+d['path'])
        shutil.copyfile(sp,so)
        tp=d.get('test_path')
        if tp:
            tp=tp if os.path.isabs(tp) else (W+'/gen/'+tp if os.path.exists(W+'/gen/'+tp) else os.path.normpath(W+'/scripts/'+tp))
            shutil.copyfile(tp,to)
        else:
            shutil.copyfile(R+'/rebuild/tests/'+d['old_test'],to)
        allr.append((a[0],so,d['leaf'],d['module'],to,f))
        ren.append((a[0],d['old_src'],d['new_src'],d['old_test'],d['new_test'],d['module']))
allr.sort(); ren.sort()
open(W+'/final/all.tsv','w').write(''.join('\t'.join(x[:5])+'\n' for x in allr))
open(W+'/final/fam.tsv','w').write(''.join(x[0]+'\t'+x[5]+'\n' for x in allr))
open(W+'/RENAMES.tsv','w').write('address\told_source\tnew_source\told_test\tnew_test\tnew_module\n'+''.join('\t'.join(x)+'\n' for x in ren))
print(len(allr))
