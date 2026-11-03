# sibfind.py: landed sources whose preprocessed text changes under the chunk-734 mirror (only those need a regrade)
import subprocess,re,os,sys,concurrent.futures as cf
W='${S}/w22/chunk734/'
ST='${S}/codex12/hctool/stubinc'
os.chdir('/home/user/FableDecomp')
M="C3DBOUNDINGSPHERE C3DVECTOR CAFILE CBULLETTIMEMANAGER CCOMBATMANAGER CDRIVEMANAGER CENVIRONMENT CGAMEPLAYERINTERFACE NSCRIPT CIVCOUNTEDPOINTER CINITBASECLASS CMATRIX3X4 CNAVIGATORMANAGER CPHYSICSMESHOBJECT PLAYER CPLAYERGUI CQUESTMANAGER CREGION CRIGHTHANDEDSET CTCBASE CTHINGMANAGER CWORLD CWORLDMAP CMAINGAMECOMPONENT NRESOURCEDIRECTORYNAMES CUSERPROFILEMANAGER CCHARSTRING".split()
rx=re.compile(r'#\s*define\s+FABLE_RETAIL_(%s)\b'%'|'.join(M))
files=[]
for root,_,fs in os.walk('rebuild/src'):
    for f in fs:
        if f.endswith('.cpp'):
            p=os.path.join(root,f)
            if rx.search(open(p,errors='replace').read()): files.append(p)
def pp(inc,f):
    r=subprocess.run(['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-I',ST,'-I',inc,f],capture_output=True,text=True)
    return [re.sub(r'fable_static_assert_\d+','SA',l) for l in r.stdout.splitlines() if l.strip()], r.returncode
def chk(f):
    a,ra=pp('rebuild/include',f); b,rb=pp(W+'mirror',f)
    return f, a!=b, ra, rb
out=open(W+'sib/changed.txt','w')
with cf.ThreadPoolExecutor(8) as ex:
    for f,d,ra,rb in ex.map(chk,files):
        if d: out.write('%s\t%d\t%d\n'%(f,ra,rb)); out.flush()
print(len(files),'scanned')
