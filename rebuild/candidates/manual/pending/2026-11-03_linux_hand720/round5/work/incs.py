import re,os,sys
# approximate MSVC include resolution: including file's dir, then overlay, then rebuild/include
ovl=sys.argv[1]; root='rebuild/include'; src=sys.argv[2]
seen=set(); order=[]
def res(name,cur):
    for d in [os.path.dirname(cur),ovl,root]:
        p=os.path.normpath(os.path.join(d,name))
        if os.path.exists(p): return p
def walk(f):
    if f in seen: return
    seen.add(f); order.append(f)
    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"',open(f,errors='replace').read(),re.M):
        p=res(m.group(1),f)
        if p: walk(p)
walk(src)
for f in order:
    if 'fable_string.h' in open(f,errors='replace').read() and 'include' in f: print(f)
