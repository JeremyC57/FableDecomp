# vstubs.py <src>: print empty definitions for every `virtual void vNN();` filler declared in the TU's classes (namespace-aware)
import re,sys
src=open(sys.argv[1]).read()
# strip comments
src=re.sub(r"//[^\n]*","",src)
out=[]; ns=[]; stack=[]
i=0; tokens=re.finditer(r"namespace\s+(\w+)\s*\{|(?:struct|class)\s+(\w+)\s*(?::[^{;]*)?\{|\{|\}|virtual\s+void\s+(v\d+)\s*\(\s*\)\s*;",src)
for m in tokens:
    t=m.group(0)
    if m.group(1): stack.append(("ns",m.group(1)))
    elif m.group(2): stack.append(("cls",m.group(2)))
    elif t=="{": stack.append(("blk",None))
    elif t=="}":
        if stack: stack.pop()
    elif m.group(3):
        names=[n for k,n in stack if k in("ns","cls")]
        if stack and stack[-1][0]=="cls":
            out.append("void "+"::".join(names)+"::"+m.group(3)+"() {}")
print("\n".join(out))
