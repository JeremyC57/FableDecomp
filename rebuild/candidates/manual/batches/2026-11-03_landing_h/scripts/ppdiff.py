# ppdiff.py <mirror> <file>: unified diff of the preprocessed token lines (tree vs mirror), system headers stubbed
import sys,subprocess,os,difflib,re
mir,f=sys.argv[1],sys.argv[2]; stub=os.path.join(os.environ['S'],'stubinc')
def run(inc):
    a=['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-DWIN32','-I',inc,'-I',inc+'/stlport','-I',stub,f]
    return [re.sub(r'\s+',' ',l).strip() for l in subprocess.run(a,capture_output=True,text=True,errors='replace').stdout.splitlines() if l.strip()]
for l in difflib.unified_diff(run(mir),run('rebuild/include'),lineterm='',n=0):
    if l.startswith(('+','-')) and not l.startswith(('+++','---')): print(l[:200])
