"""Read-only download of pinned header/tool dependencies for the two supplied rows."""
import concurrent.futures, hashlib, json, pathlib, re, urllib.request
S=pathlib.Path(__file__).resolve().parent
REV='015db27900e473067d2f540770aa9a42b16271aa'
BASE='https://raw.githubusercontent.com/JeremyC57/FableDecomp/'+REV+'/'
def get(path):
    target=S/'support'/path
    if target.exists(): return path,target.read_bytes(),None
    try:
        data=urllib.request.urlopen(BASE+path,timeout=40).read()
        target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
        return path,data,None
    except Exception as ex: return path,None,str(ex)
roots=['docs/pipeline/VC71_CODEGEN_RESEARCH.md','docs/pipeline/C2_ALLOCATOR_PROBE.md',
       'docs/pipeline/GOTCHAS.md','docs/pipeline/EXTRA_RULES.md',
       'tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py']
includes=set()
for f in (S/'input').rglob('*'):
    if f.suffix in ('.h','.cpp'):
        includes.update(re.findall(r'^\s*#\s*include\s*"([^"]+)"',f.read_text(),re.M))
pending=set(roots)|{'rebuild/include/'+x for x in includes};seen=set();errors={}
while pending:
    batch=sorted(pending);pending=set()
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        for path,data,error in pool.map(get,batch):
            seen.add(path)
            if error: errors[path]=error;continue
            if path.endswith('.h'):
                for inc in re.findall(r'^\s*#\s*include\s*"([^"]+)"',data.decode('utf8',errors='replace'),re.M):
                    # Repository uses include-root paths; local siblings are resolved later if needed.
                    child='rebuild/include/'+inc
                    if child not in seen:pending.add(child)
    print('read',len(seen),'pending',len(pending),'errors',len(errors),flush=True)
(S/'support'/'FETCH.json').write_text(json.dumps({'revision':REV,'errors':errors,'sha256':{str(f.relative_to(S/'support')):hashlib.sha256(f.read_bytes()).hexdigest()for f in (S/'support').rglob('*')if f.is_file()}},indent=2))
print(json.dumps(errors,indent=2))
