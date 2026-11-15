import json,csv,re,sys,os
X,OUT=sys.argv[1],sys.argv[2]
rows=[r for r in csv.DictReader(open(X+'/STATUS.tsv'),delimiter='\t') if r['status']=='RELOCATION_MATCH']
def demangle(m):
    if m.startswith('.text'): return None
    s=m[1:]; ctor=dtor=False
    if s.startswith('?0'): ctor=True; s=s[2:]
    elif s.startswith('?1'): dtor=True; s=s[2:]
    elif s.startswith('?$'):  # template function
        nm=s[2:].split('@')[0]; return '_global',nm
    parts=[]; i=0
    first=None
    if not (ctor or dtor):
        first=s.split('@')[0]; s=s[len(first)+1:]
    while s and not s.startswith('@'):
        if s.startswith('?$'):
            nm=s[2:].split('@')[0]; parts.append(nm)
            # skip template args crudely: cut to the scope terminator after matching '@@'
            depth=0; j=2+len(nm)+1
            # find '@@' that closes template args at depth 0 (approximate: first '@std@@' or '@@')
            k=s.find('@@',j); s=s[k+2:] if k>=0 else ''
            # remaining scope pieces follow
            continue
        p=s.split('@')[0]; parts.append(p); s=s[len(p)+1:]
    scope='::'.join(reversed(parts)) or '_global'
    cls=parts[0] if parts else ''
    if ctor: return scope, cls
    if dtor: return scope, '~'+cls
    return scope, first
au=[]; ora=[]; skipped=[]
for r in rows:
    a=r['address'][2:].lower(); d=json.load(open(X+f'/rows/{a}/ROW.json')); b=json.load(open(X+f'/rows/{a}/BUILD.json'))
    T=json.load(open(X+'/validation/TEST_CONFIG.json')).get(a,{})
    if a=='004a1c41': skipped.append((a,'catch funclet inside the 0x004A1840 extent; landed with its parent')); continue
    src=open(X+f'/rows/{a}/{a}.cpp',encoding='utf-8').read(); tst=open(X+f'/rows/{a}/{a}_test.cpp',encoding='utf-8').read()
    land=(d.get('corrected_start') or a).lower()
    length=int(r['retail_size'])
    leaf=re.search(r'^// Leaf: *(\S+)',src,re.M).group(1)
    e={'address':land,'source_cpp':src,'test_cpp':tst,'pass_pattern':T['expected_stdout'].strip(),'flags':b['flags']}
    if a in ('01214fe0','01215000'):
        e['funclet_offset']=0
        par=d.get('dependency_parent')
        if a=='01215000':
            src=src.replace('#include "parent_00c3db80.cpp"','#include "../src/compiled/00/c3/CCodaContext_Create3DSource_00c3db80.cpp"   // the landed parent 0x00C3DB80 (no code of its own)')
        else:
            ptxt=open(X+f'/rows/{a}/parent_00c3da60.cpp',encoding='utf-8').read()
            src=src.replace('#include "parent_00c3da60.cpp"','// The parent 0x00C3DA60 CCodaContext::Create2DSource (not landed yet) follows verbatim; only its EH section is graded here.\n'+ptxt)
        e['source_cpp']=src
        mod,nm='_global','Unwind'+a
    else:
        mod,nm=demangle(r['assigned_name'])
    e['module']=mod; e['name']=nm
    assert '#include "parent' not in e['source_cpp']
    au.append(e); ora.append((land,mod+'::'+nm,length))
json.dump({'result':{'authored':au}},open(OUT+'/payload.json','w'),indent=1)
with open(OUT+'/rows.tsv','w') as f:
    f.write('address\tmodule\tname\tlength\n')
    for e,(l,n,ln) in zip(au,ora): f.write(f"{l}\t{e['module']}\t{e['name']}\t{ln}\n")
print(len(au),'rows; skipped',skipped)
for e in au: print(e['address'],e['module'],e['name'],e['flags'],e.get('funclet_offset',''))
