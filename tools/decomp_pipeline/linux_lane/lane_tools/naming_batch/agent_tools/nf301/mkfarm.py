# mkfarm.py <dest> <overlay-dir>: symlink farm of the repo rebuild/include at <dest>; files present in <overlay-dir> (relative path,
# *.md skipped) are copied from the overlay instead of linked. dest must not exist (never overwrites, never copies onto a symlink).
import os,sys,shutil
R='/home/user/FableDecomp/rebuild/include'; d,ov=sys.argv[1],sys.argv[2]
assert not os.path.exists(d)
over=set()
for root,_,fs in os.walk(ov):
    for f in fs:
        if not f.endswith('.md'): over.add(os.path.relpath(os.path.join(root,f),ov))
for root,dirs,fs in os.walk(R):
    rel=os.path.relpath(root,R); os.makedirs(os.path.join(d,rel),exist_ok=True)
    for f in fs:
        r=os.path.normpath(os.path.join(rel,f))
        if r in over: shutil.copyfile(os.path.join(ov,r),os.path.join(d,r))
        else: os.symlink(os.path.join(root,f),os.path.join(d,r))
print('overlaid',sorted(over))
