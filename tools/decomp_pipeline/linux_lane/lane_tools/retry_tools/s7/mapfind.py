# mapfind.py <addr> <suffix> <MapClass> <headerMember> <KeyType> <IterType> <iterInit>
# Adds a same-TU STLport map::find body (lower_bound walk over raw node offsets: parent +4, left +8, right +0xc, key +0x10)
# after the declaration's class. iterInit: "ctor" (Iter((Node*)y)), "field:<name>" (default-construct then set field),
# "ptr" (the iterator is a node pointer).
import re,sys
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf,mcls,hdr,ktype,itype,init=sys.argv[1:8]
src=open(f"{W}/src/{a}.cpp",errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
m=re.search(r"^[ \t]*[\w:<>* ]+?\bfind\s*\(\s*const\s+[\w: ]+&\s*\w*\s*\)\s*;[^\n]*$",src,re.M)
if not m: print("NODECL"); sys.exit(1)
# end of the enclosing (outermost) class
depth=0;i=m.start()
starts=[]
while i>0:
    i-=1
    if src[i]=="}": depth+=1
    elif src[i]=="{":
        if depth==0: starts.append(i)
        else: depth-=1
oc=starts[-1] if starts else None
# use the outermost non-namespace: approximate with the last brace found that is preceded by struct/class
for st in starts[::-1]:
    if re.search(r"(struct|class)\s+\w+[^;{}]*$",src[:st]): oc=st; break
depth=0;j=oc
while j<len(src):
    if src[j]=="{": depth+=1
    elif src[j]=="}":
        depth-=1
        if depth==0: break
    j+=1
end=src.find(";",j)+1
if init=="ctor": mk=f"return {itype}(({ 'void*' })y);"
if init=="ctor": mk="return %s((%s)y);"%(itype, "NODE_T*")
ret={"ctor":None}.get(init)
if init.startswith("field:"):
    f=init.split(":")[1]; body_ret=f"{itype} it;\n    *(void**)&it.{f} = y;\n    return it;"
elif init=="ctor":
    body_ret=f"{itype} it(0);\n    *(void**)&it = y;\n    return it;"
elif init=="ptr":
    body_ret=f"return ({itype})y;"
body=f"""
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation (lower_bound walk from the root, reading the key through its const
// reference; node links parent +4 / left +8 / right +0xc, key +0x10), then the end() test.
__declspec(noinline) {itype} {mcls}::find(const {ktype}& key)
{{
    unsigned char* h = (unsigned char*){hdr};
    unsigned char* y = h;
    unsigned char* x = *(unsigned char**)(h + 4);
    while (x != 0) {{
        if (!(*(const {ktype}*)(x + 0x10) < key)) {{
            y = x;
            x = *(unsigned char**)(x + 8);
        }} else
            x = *(unsigned char**)(x + 0xc);
    }}
    if (y == h || key < *(const {ktype}*)(y + 0x10))
        y = h;
    {body_ret}
}}
#pragma auto_inline(on)"""
open(f"{W}/var/{a}_{suf}.cpp","w").write(src[:end]+body+src[end:])
print(f"var/{a}_{suf}.cpp")
