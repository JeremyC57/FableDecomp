# reset_body.py <addr> <suffix>: add the landed CCountedPointer::Reset body (0x00905FD0 shape) after the CCountedPointer struct
import re,sys
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]
s=open(f"{W}/src/{a}.cpp").read()
s="\n".join(l for l in s.split("\n") if not l.startswith("// Grade:"))
m=re.search(r"^[ \t]*void Reset\((\w+)\* object\);[^\n]*$",s,re.M)
T=m.group(1)
i=s.rfind("struct CCountedPointer",0,m.start())
j=s.find("\n};",m.end())+3
body=f"""
void __fastcall DestroyCountedObject_s7(void* object);   // the per-type deleter stored in the count block
#pragma auto_inline(off)
// Same-TU body of CCountedPointer::Reset (the landed 0x00905FD0 shape): release the old count block, adopt the object
// with a fresh one.
__declspec(noinline) void CCountedPointer::Reset({T}* object)
{{
    if (m_pCount) {{
        --m_pCount->m_references;
        if (m_pCount->m_references == 0) {{
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }}
    }}
    m_pCount = 0;
    m_pObject = object;
    if (object) {{
        SCountBlock* block = (SCountBlock*)operator new(sizeof(SCountBlock));
        if (block) {{
            block->m_references = 1;
            block->m_pfnDestroy = DestroyCountedObject_s7;
            block->m_pObject = m_pObject;
        }}
        m_pCount = block;
    }}
}}
#pragma auto_inline(on)"""
open(f"{W}/var/{a}_{suf}.cpp","w").write(s[:j]+body+s[j:])
