# delchk.py <target> <member>: every landed exe caller row of target (E8/E9 sites, tools/xsites.py) and each one's declaration lines of
# member (lines naming `member(` that end in `;` outside comments), plus rows that #include another row source; absolute refs count.
import sys,subprocess,re,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
t,m=sys.argv[1],sys.argv[2]
x=subprocess.run(['python3',A+'/tools/xsites.py',t],capture_output=True,text=True).stdout.splitlines()
open(A+'/evidence/xsites_%s.tsv'%t,'w').write('\n'.join(x)+'\n')
rows={}; nolist=0
for l in x:
    c=l.split('\t')
    if c[3]=='-': nolist+=1; continue
    rows.setdefault(c[3][:8],c[3][8:9])
ab=subprocess.run(['python3',A+'/tools/absrefs.py',t],capture_output=True,text=True).stdout.strip().splitlines()[-1]
print('## %s %s: %d sites, %d rows, %d in no row; %s'%(t,m,len(x),len(rows),nolist,ab))
for r,k in sorted(rows.items()):
    if k!='L': print('  %s not landed'%r); continue
    src=subprocess.run(['bash',A+'/tools/srcof.sh',r],capture_output=True,text=True).stdout.strip()
    if not src: print('  %s NOSRC'%r); continue
    txt=open(src,encoding='utf-8',errors='replace').read()
    d=[ln.split('//')[0].strip() for ln in txt.splitlines() if re.search(r'\b%s\s*\('%re.escape(m),ln.split('//')[0]) and ln.split('//')[0].rstrip().endswith(';') and not re.search(r'[=.>]\s*%s\s*\('%re.escape(m),ln.split('//')[0])]
    inc=re.findall(r'#\s*include\s*"([^"]*(?:engine|src)/[^"]+)"',txt)
    print('  %s %s'%(r,(' || '.join(d)[:260]) if d else 'NODECL incl='+','.join(i.split('/')[-1] for i in inc)[:200]))
