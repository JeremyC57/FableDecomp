# callcensus.py <target> <Class> <Member>: for every landed row whose code calls the target (evidence/xsites_*.tsv E8/E9 sites), the
# memacc.py declaration lines of Class::Member in that row's source (access, const, text) -- or NODECL (shared header / other class).
import sys,subprocess,glob,re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF305'
t,C,M=sys.argv[1:4]
rows=set()
for f in glob.glob(A+'/evidence/xsites_*.tsv'):
    for l in open(f):
        c=l.rstrip('\n').split('\t')
        if len(c)==4 and c[0]==t and c[3]!='-' and c[3][8:9]=='L': rows.add(c[3][:8])
for r in sorted(rows):
    src=subprocess.run(['bash',A+'/tools/srcof.sh',r],capture_output=True,text=True).stdout.strip()
    if not src: print(r,'NOSRC'); continue
    o=subprocess.run(['python3',A+'/tools/memacc.py',C,M,src],capture_output=True,text=True).stdout.strip()
    inc=re.findall(r'#\s*include\s*"(engine/[^"]+)"',open(src,encoding='utf-8',errors='replace').read())
    print(r, ('\n    '.join(o.splitlines()) if o else 'NODECL incl='+','.join(inc)[:150]))
