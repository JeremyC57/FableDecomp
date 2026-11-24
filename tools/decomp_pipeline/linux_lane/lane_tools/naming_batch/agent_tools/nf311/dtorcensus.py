# dtorcensus.py <xsites.tsv> <Class>: for every landed caller row of a dtor target, classify how its source declares ~Class:
# HDR (includes engine/<Class>.h; the macros it defines are listed), LOCALV (a local `virtual ~Class(`), LOCALNV (a local non-virtual
# `~Class(` declaration), NONE (no declaration seen: read by hand). Source-level census only (no compile).
import sys,re,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311'
xs,C=sys.argv[1],sys.argv[2]; rows=[]
for l in open(xs):
    c=l.rstrip('\n').split('\t')
    if len(c)==4 and c[3][8:9]=='L': rows.append(c[3][:8])
for r in sorted(set(rows)):
    f=subprocess.run(['bash',A+'/tools/srcof.sh',r],capture_output=True,text=True).stdout.strip()
    t=open(f,encoding='utf-8',errors='replace').read()
    code='\n'.join(x.split('//')[0] for x in t.splitlines())
    v='NONE'
    if re.search(r'virtual\s+~%s\s*\('%C,code): v='LOCALV'
    elif re.search(r'(?<![\w:])~%s\s*\(\s*\)\s*;'%C,code): v='LOCALNV'
    if re.search(r'#\s*include\s*"engine/%s\.h"'%C,code): v='HDR' if v=='NONE' else v+'+HDR'
    defs=sorted(set(re.findall(r'#\s*define\s+(FABLE_RETAIL_\w*(?:CPARENTDEFCLASSBASE|DEF_CLASS_BASE)\w*)',code)))
    print(r,v,','.join(defs),f.split('/')[-1][:70])
