# mkt.py <addr> <tail.cpp>: test/<addr>.cpp = header + src/<addr>.cpp verbatim (comments kept) + tail (stubs + main); then run tall.py
import sys,os,subprocess
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,tail=sys.argv[1],sys.argv[2]
src=open(f"{W}/src/{a}.cpp").read()
import re as _re
_seen=set()
def _inline(text):
    def rep(m):
        name=m.group(1); path="/home/user/FableDecomp/rebuild/include/"+name
        if name in _seen: return "// (inlined above: "+name+")"
        if not os.path.exists(path): return m.group(0)
        _seen.add(name)
        return "// ---- inlined from rebuild/include/"+name+" ----\n"+_inline(open(path).read())+"\n// ---- end "+name+" ----"
    return _re.sub(r'^#include "([^"]+)"[^\n]*$',rep,text,flags=_re.M)
src=_inline(src)
name=src.splitlines()[0].lstrip("/ ").split(" -- ")[0]
head=f"""// Behaviour test for {a}: src/{a}.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. {name}
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do {{ if (!(c)) {{ ++fails; printf("FAIL %d: %s\\n", __LINE__, #c); }} }} while (0)
"""
body=open(tail).read().replace("@ADDR@",a)
open(f"{W}/test/{a}.cpp","w").write(head+"// ---- source (verbatim) ----\n"+src+"\n// ---- stubs and driver ----\n"+body)
