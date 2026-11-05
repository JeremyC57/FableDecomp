# survey_cthing.py OUT [CLASS]: scan every landed row source for a LOCAL definition of CLASS (default CThing): a `class|struct CLASS [: bases] {` block.
# Writes OUT/<CLASS>_rows.tsv (address, source, includes_header, bases, n_data_members, block_lines, gettc_variant, pack) and
# OUT/<CLASS>_gettc_variants.tsv (variant sha1_8, count, example, normalised GetTC/PeekTC template text) and OUT/<CLASS>_members.tsv
# (address, member declaration lines with their offset comments) so the offset map can be designed from real rows.
import sys,os,re,hashlib,collections
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, ROOT
OUT=sys.argv[1]; CLS=sys.argv[2] if len(sys.argv)>2 else 'CThing'; os.makedirs(OUT,exist_ok=True)
ent=ps1_entries()
rx=re.compile(r'(?<![\w:])(class|struct)\s+'+CLS+r'\b\s*(?:/\*.*?\*/\s*)?(?::\s*([^{;]*))?\{')
def block(txt,start):
    d=0;i=start
    while i<len(txt):
        c=txt[i]
        if c=='{': d+=1
        elif c=='}':
            d-=1
            if d==0: return txt[start:i+1]
        i+=1
    return txt[start:]
def norm(s): return re.sub(r'\s+',' ',re.sub(r'//[^\n]*','',s)).strip()
rows=[];var=collections.defaultdict(list);mem=open(OUT+'/%s_members.tsv'%CLS,'w')
for a,e in sorted(ent.items()):
    src=e.get('Source','')
    if not src.endswith('.cpp'): continue
    p=ROOT+'/rebuild/src/compiled/'+src
    try: txt=open(p,errors='replace').read()
    except Exception: continue
    inc='engine/%s.h'%CLS in txt
    m=None
    for mm in rx.finditer(txt):
        # skip template<...> specialisations and 'class CThing;' forward decls (no brace): rx needs the brace
        m=mm; break
    if not m:
        if inc: rows.append((a,src,'1','','0','0','','','hdr_only'))
        continue
    b=block(txt,m.end()-1)
    # GetTC / PeekTC templates inside the block
    g=re.findall(r'template\s*<[^>]*>\s*[^;{]*?\b(?:GetTC|PeekTC)\b[^{;]*\{',b)
    gt=[]
    for gm in re.finditer(r'template\s*<[^>]*>\s*[^;{]*?\b(?:GetTC|PeekTC)\b[^{;]*\{',b):
        gt.append(norm(block(b,gm.end()-1)))
    key=hashlib.sha1('\n'.join(gt).encode()).hexdigest()[:8] if gt else ''
    if gt: var[key].append((a,'\n'.join(gt)))
    pack='pack1' if re.search(r'pack\s*\(\s*push\s*,\s*1\s*\)',txt[max(0,m.start()-200):m.start()]) else ''
    data=[]
    for l in b.split('\n'):
        s=l.strip()
        if not s or s.startswith('//') or s.startswith('#') or '(' in s.split('//')[0]: continue
        if re.match(r'^(public|private|protected):',s) or s in ('{','}','};'): continue
        if ';' in s: data.append(s)
    rows.append((a,src,'1' if inc else '0',norm(m.group(2) or ''),str(len(data)),str(b.count('\n')+1),key,pack,'local'))
    for d in data: mem.write(a+'\t'+d[:200]+'\n')
mem.close()
with open(OUT+'/%s_rows.tsv'%CLS,'w') as o:
    o.write('address\tsource\tincludes_header\tbases\tn_data_members\tblock_lines\tgettc_variant\tpack\tkind\n')
    for r in rows: o.write('\t'.join(r)+'\n')
with open(OUT+'/%s_gettc_variants.tsv'%CLS,'w') as o:
    o.write('variant\tcount\texample\ttext\n')
    for k,v in sorted(var.items(),key=lambda kv:-len(kv[1])): o.write('%s\t%d\t%s\t%s\n'%(k,len(v),v[0][0],v[0][1].replace('\n',' || ')[:1500]))
c=collections.Counter(r[8] for r in rows); print(CLS,dict(c),'gettc variants',len(var))
