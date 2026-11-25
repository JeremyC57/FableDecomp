#!/usr/bin/env python3
"""ast438.py <census tsv> <out jsonl> [shard k/n]: per row, libclang-parse the landed source (the nf.py precheck flags, -fno-wchar)
and record every main-file reference to a function whose MSVC mangled name is one of that row's census wrong spellings: call sites
(CALL_EXPR extent, callee MEMBER_REF_EXPR base extent or implicit this, argument extents) and non-call references (address taken).
Also records the referenced declaration's extent when it lies in the main file, and parse errors. One JSON line per row."""
import sys, os, re, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'pylib'))
import clang.cindex as ci
ci.Config.set_library_file('/usr/lib/llvm-18/lib/libclang-18.so.1')
R='/home/user/FableDecomp'; S=str(nf.S)
inp, outp = sys.argv[1:3]; k, n = (map(int, sys.argv[3].split('/')) if len(sys.argv) > 3 else (0, 1))
want=collections.defaultdict(set)
for l in open(inp):
    x=l.rstrip('\n').split('\t'); want[x[0]].add(x[2])
ARGS=['-x','c++','--target=i686-pc-windows-msvc','-fms-extensions','-fms-compatibility','-fms-compatibility-version=13.10','-w','-Xclang','-fno-wchar','-I'+R+'/rebuild/include','-isystem',S+'/wine/vc71/include']
idx=ci.Index.create()
def ext(c): e=c.extent; return [e.start.offset, e.end.offset]
done=set()
if os.path.exists(outp):
    for l in open(outp): done.add(json.loads(l)['row'])
fo=open(outp,'a')
cur=outp+'.cur'
if os.path.exists(cur):
    c=open(cur).read().strip()
    if c and c not in done: fo.write(json.dumps({'row':c,'err':'crash'})+'\n'); fo.flush(); done.add(c)
rows=sorted(want)
for i,a in enumerate(rows):
    if i % n != k or a in done: continue
    src=str(nf.src_of(a)); open(cur,'w').write(a)
    try: tu=idx.parse(src,args=ARGS,options=ci.TranslationUnit.PARSE_SKIP_FUNCTION_BODIES*0)
    except Exception as e: fo.write(json.dumps({'row':a,'err':'parse:'+str(e)})+'\n'); fo.flush(); continue
    errs=[str(d) for d in tu.diagnostics if d.severity>=3][:5]
    hits=[]; W=want[a]
    def walk(c, parent_call=None):
        for ch in c.get_children():
            if ch.location.file and ch.location.file.name!=src: continue
            if ch.kind in (ci.CursorKind.CALL_EXPR,ci.CursorKind.DECL_REF_EXPR,ci.CursorKind.MEMBER_REF_EXPR):
                ref=ch.referenced
                if ref is not None:
                    try: mn=ref.mangled_name
                    except Exception: mn=None
                    if mn in W:
                        h={'kind':ch.kind.name,'sym':mn,'ext':ext(ch),'spell':ch.spelling,
                           'decl_in_main': bool(ref.location.file and ref.location.file.name==src),'decl_ext':ext(ref) if ref.location.file and ref.location.file.name==src else None,
                           'refkind':ref.kind.name}
                        if ch.kind==ci.CursorKind.CALL_EXPR:
                            kids=list(ch.get_children()); h['args']=[ext(x) for x in ch.get_arguments()]
                            callee=kids[0] if kids else None
                            while callee is not None and callee.kind==ci.CursorKind.UNEXPOSED_EXPR and list(callee.get_children()): callee=list(callee.get_children())[0]
                            if callee is not None:
                                h['callee_kind']=callee.kind.name; h['callee_ext']=ext(callee)
                                ck=list(callee.get_children()); h['base_ext']=ext(ck[0]) if (callee.kind==ci.CursorKind.MEMBER_REF_EXPR and ck) else None
                        hits.append(h)
            walk(ch)
    walk(tu.cursor)
    fo.write(json.dumps({'row':a,'src':src,'errs':errs,'hits':hits})+'\n'); fo.flush()
