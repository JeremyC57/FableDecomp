# hdrflag311.py: NF299 Q4 HDRFLAG check over work/census.txt: for each (entry, witness) stand-in line, does the witness's HEAD source
# #include a shared header (engine/<klass>.h, klass = ENTRIES column 5) whose code lines name the PDB leaf? Prints HDRFLAG lines (open those).
import re,subprocess,os,collections
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'; R='/home/user/FableDecomp/rebuild/'
L=open(A+'/ENTRIES.tsv').read().splitlines()
sys_path=__import__('sys').path; sys_path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as MD
def leaf(p):
    try: d=MD.demangle(p); return d['name'].split('<')[0] if d.get('ok') else None
    except Exception: return None
seen=set()
for l in open(A+'/work/census.txt'):
    if not l.startswith('#'): continue
    c=l.rstrip('\n').split('\t'); n=int(c[0][1:]); w=c[2]
    for wi in w.split(','):
        if not re.fullmatch(r'[0-9a-f]{8}',wi) or (n,wi) in seen: continue
        seen.add((n,wi)); e=L[n].split('\t'); kl=e[4].split('::')[-1].split('<')[0]; lf=leaf(e[6])
        h=R+'include/engine/%s.h'%kl
        if not lf or not os.path.exists(h): continue
        hl=[x.split('//')[0] for x in open(h,encoding='utf-8',errors='replace')]
        if not any(re.search(r'\b%s\b'%re.escape(lf),x) for x in hl): continue
        f=subprocess.run(['bash',A+'/tools/srcof.sh',wi],capture_output=True,text=True).stdout.strip()
        if not f: continue
        t=open(f,encoding='utf-8',errors='replace').read()
        if re.search(r'#\s*include\s*"engine/%s\.h"'%re.escape(kl),t): print('HDRFLAG\t#%d\t%s\t%s\tengine/%s.h\t%s'%(n,e[0],wi,kl,c[3][:40]))
