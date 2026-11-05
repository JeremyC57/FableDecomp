# survey_cthing3.py ROWS.tsv OUT.tsv : for every local-CThing row, the members of the local CThing block (data and methods, with
# the `// +0xNN` offset comment when present) and which of them the rest of the TU references (`->name`, `.name`, or bare `name`
# when the row defines a CThing member function). Aggregates name -> (rows declaring, rows referencing) so the field map / accessor
# map can be designed on evidence. Writes OUT.tsv (address, referenced members: name@offset,...) and prints the top names.
import sys,re,collections,csv
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ROOT
sys.path.insert(0,'/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/plan/p7/tools')
from wavelib import block,class_rx
rx=class_rx("CThing")
rows=[r for r in csv.DictReader(open(sys.argv[1]),delimiter='\t') if r['kind']=='local']
decl=collections.Counter(); refd=collections.Counter(); o=open(sys.argv[2],'w'); o.write('address\tbases\tmembers_referenced\tmembers_unreferenced\n')
KW={'if','for','while','return','else','switch','case','const','static','virtual','inline','public','private','protected','class','struct','enum','template','typedef','unsigned','signed','char','short','long','int','bool','float','double','void','true','false','this','sizeof','operator'}
for r in rows:
    txt=open(ROOT+'/rebuild/src/compiled/'+r['source'],errors='replace').read()
    m=rx.search(txt)
    if not m: continue
    b=block(txt,m.end()-1); rest=txt[:m.start()]+txt[m.end()-1+len(b):]
    bnc=re.sub(r'//[^\n]*','',b)
    names={}
    for line in b.split('\n'):
        code=line.split('//')[0]; off=re.search(r'//\s*\+?(0x[0-9a-fA-F]+)',line); off=off.group(1).lower() if off else ''
        code=re.sub(r'\b(public|private|protected)\s*:','',code)
        for mm in re.finditer(r'\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?:[;,:=]|\()',code):
            n=mm.group(1)
            if n in KW or n=='CThing' or re.match(r'^(v\d+|_pad\w*|pad\w*|_beforeFlags)$',n): continue
            # a data member or method name: the identifier right before ; , = : ( or [..]
            names.setdefault(n,off)
    own=bool(re.search(r'\bCThing::\w+\s*\(',rest))
    used=[]; unused=[]
    for n,off in names.items():
        if re.search(r'(->|\.)\s*'+re.escape(n)+r'\b',rest) or (own and re.search(r'(?<![\w>.])'+re.escape(n)+r'\s*[\(\[;&|=!<>+\-*/,)]',rest)): used.append(n+('@'+off if off else ''))
        else: unused.append(n+('@'+off if off else ''))
    for n in names: decl[n]+=1
    for u in used: refd[u.split('@')[0]]+=1
    o.write('\t'.join([r['address'],r['bases'],','.join(used),','.join(unused)])+'\n')
print('top referenced members (rows referencing / rows declaring):')
for n,c in refd.most_common(90): print('  %5d / %5d  %s'%(c,decl[n],n))
