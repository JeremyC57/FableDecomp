#!/usr/bin/env python3
"""mk7tests.py: NF445 linked behaviour tests (over engine/CSalRender.h) for the seven NF444 binder rows whose HEAD tests modelled the old
CVtable_012beda4 / CCodaObjectBase locally. Each test copies the row's own class declaration (a stand-in class with no shared header),
stubs the out-of-line members the row object references, and is linked against the row object (vt.py third argument)."""
import re,sys
B=sys.argv[1]
PRE='''#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"
#include <cstdio>
#include <cstdlib>
static int g_baseCtors = 0, g_baseDtors = 0, g_dtors = 0, g_frees = 0, g_order = 0, g_baseAt = 0, g_derivedAt = 0, g_clear = 0;
static void* g_lastFree = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { return std::malloc(n); }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; g_lastFree = p; std::free(p); }
CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { ++g_baseCtors; }
CSalRender::~CSalRender() { ++g_baseDtors; g_baseAt = ++g_order; }
void CSalRender::ClearFilterHistory() { }
'''
def decl(a,cls,stop):
    s=open(f'{B}/rows/{a}.cpp').read()
    s=s[s.index('#include "engine/CSalRender.h"'):]; s=s[s.index('\n')+1:]
    return s[:s.index(stop)] if stop else s
def stubs(cls,dtor=True,clear=True):
    t=''
    if dtor: t+=f'{cls}::~{cls}() {{ ++g_dtors; g_derivedAt = ++g_order; }}\n'
    if clear: t+=f'void {cls}::ClearFilterHistory() {{ ++g_clear; }}\n'
    t+=f'void {cls}::Process(const short*, int*(&)[4]) {{ }}\nvoid {cls}::SetGain(float, const float*, bool) {{ }}\n'
    t+=f'void {cls}::SetPosition(float, float, float, float, const float*, bool, bool) {{ }}\n'
    return t
def main_(a,body): return 'int main()\n{\n'+body+f'''    if (!ok) {{ std::printf("{a}_TEST FAIL\\n"); return 1; }}
    std::printf("{a}_TEST PASS\\n");
    return 0;
}}
'''
HRTF='''template <int N> static bool AlignedInside(const SAlignedFloats<N>& f)
{
    const unsigned char* p = (const unsigned char*)f.Aligned;
    return ((unsigned long)p & 15) == 0 && p >= f.Raw && p + N * 4 <= f.Raw + sizeof(f.Raw);
}
'''
def hrtf_body(cls,n):
    return f'''    bool ok = true;
    for (int round = 0; round < 3; ++round) {{
        {cls}* obj = new {cls};                                  // heap objects land at varying 16-byte phases
        const unsigned char* audio = (const unsigned char*)obj->m_pAudio;
        ok = ok && g_baseCtors == round + 1 && ((unsigned long)audio & 15) == 0 && audio >= obj->m_audioBuffer + 0x80 &&
             audio < obj->m_audioBuffer + 0x90 && AlignedInside<{n}>(obj->Block0.A) && AlignedInside<{n}>(obj->Block0.B) &&
             AlignedInside<{n}>(obj->Block1.A) && AlignedInside<{n}>(obj->Block1.B) &&
             obj->PFront == &obj->Block0 && obj->PBack == &obj->Block1 &&
             obj->Count0 == 0 && obj->Count1 == 0 && obj->Count2 == 0 && obj->Count3 == 0 &&
             obj->Block0.GapA[3] == 0 && obj->Block1.B.Raw[5] == 0 && obj->Block1.GapB[15] == 0;
        int clears = g_clear; obj->ClearFilterHistory(); ok = ok && g_clear == clears + 1;   // the row stored this class's vtable
        CSalRender* base = obj; delete base;                                                // ??_G: derived, then base dtor, then free
        ok = ok && g_dtors == round + 1 && g_baseDtors == round + 1 && g_lastFree == (void*)obj;
    }}
'''
T={}
H='// Behaviour test (NF445, linked against the row object over engine/CSalRender.h; replaces the HEAD self-contained mirror) of '
T['00c70120']=(H+'''the HRTF
// renderer constructor 0x00C70120 (CVtable_012bedcc): the CSalRender base is constructed once, m_pAudio is 16-byte aligned inside
// m_audioBuffer + 0x80, the four block pointers are aligned inside their raw buffers, both blocks are otherwise zero, front / back point at
// the blocks, the counters are zero, and the object carries this class's vtable.
'''+PRE+decl('00c70120','CVtable_012bedcc','CVtable_012bedcc::CVtable_012bedcc()')+stubs('CVtable_012bedcc')+HRTF+main_('00c70120',hrtf_body('CVtable_012bedcc',64)))
T['00c70b00']=(H+'''the HRTF
// renderer constructor 0x00C70B00 (CVtable_012bede0, SSE2 twin of 0x00C70120 with 160-float blocks): as 0x00C70120.
'''+PRE+decl('00c70b00','CVtable_012bede0','CVtable_012bede0::CVtable_012bede0()')+stubs('CVtable_012bede0')+HRTF+main_('00c70b00',hrtf_body('CVtable_012bede0',160)))
T['00c6e760']=(H+'''the
// disabled-renderer constructor 0x00C6E760 (CVtable_012bedb8): the CSalRender base is constructed once and the current / previous
// gain-ramp pointers point at the first / second record; the object carries this class's vtable (Process override).
'''+PRE+decl('00c6e760','CVtable_012bedb8','CVtable_012bedb8::CVtable_012bedb8()')+stubs('CVtable_012bedb8',clear=False).replace('void CVtable_012bedb8::Process(const short*, int*(&)[4]) { }','void CVtable_012bedb8::Process(const short*, int*(&)[4]) { ++g_clear; }')+main_('00c6e760','''    CVtable_012bedb8* e = new CVtable_012bedb8;
    int* out[4] = { 0, 0, 0, 0 };
    e->Process(0, out);
    bool ok = g_baseCtors == 1 && e->m_pCurrent == &e->m_first && e->m_pPrevious == &e->m_second && (void*)&e->m_second > (void*)&e->m_first
              && g_clear == 1;
    CSalRender* base = e; delete base;
    ok = ok && g_dtors == 1 && g_baseDtors == 1 && g_lastFree == (void*)e;
'''))
for a,cls in (('00c701d0','CVtable_012bedcc'),('00c70bb0','CVtable_012bede0')):
    d=decl(a,cls,f'{cls}::~{cls}()')
    T[a]=(H+f'''the HRTF
// renderer destructor {a.upper().replace('00C','0x00C')} ({cls}): destroying the renderer through a base pointer runs the derived destructor (the row) and
// then the CSalRender base destructor (0x00C6E630) once, and frees the object once.
'''.replace('0X','0x')+PRE.replace('#define FABLE_RETAIL_CSALRENDER_MEMBERS\n','').replace('CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { ++g_baseCtors; }\n','')+d+stubs(cls,dtor=False)+main_(a,f'''    {cls}* r = new {cls};
    r->ClearFilterHistory();
    CCodaObjectBase* p = r; delete p;
    bool ok = g_clear == 1 && g_baseDtors == 1 && g_frees == 1 && g_lastFree == (void*)r;
'''))
for a,cls in (('00c70a30','CVtable_012bedcc'),('00c71400','CVtable_012bede0')):
    d=decl(a,cls,'namespace {')
    T[a]=(H+f'''the scalar
// deleting destructor {a} of {cls} (slot 0 of its vtable): delete through a base pointer runs the derived destructor, then the
// CSalRender base destructor, each once and in that order, then frees the object once through the class allocator (CodaFree).
'''+PRE+d+stubs(cls)+main_(a,f'''    {cls}* r = new {cls};
    CCodaObjectBase* p = r; delete p;
    bool ok = g_baseCtors == 1 && g_dtors == 1 && g_baseDtors == 1 && g_derivedAt == 1 && g_baseAt == 2 && g_frees == 1 && g_lastFree == (void*)r;
'''))
for a,t in T.items(): open(f'{B}/tests/{a}_test.cpp','w').write(t); print(a,len(t))
