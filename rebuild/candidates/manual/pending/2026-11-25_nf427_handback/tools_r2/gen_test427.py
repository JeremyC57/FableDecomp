#!/usr/bin/env python3
"""gen_test427.py <batch> [out_dir]: NF427 round 2. Regenerates each Part 2 Clone test (NF425 CLONE_INLINE form) on the shared
headers. The test takes the row's own prelude verbatim (its #defines, including the class's VIRTUAL_MODEL opt-in and its Needs,
and its #includes), so the base class comes from engine/<Base>.h and, where the row includes engine/<C>.h, C from that header.
No class body is defined in the test. The two members the test must reach without a class body are mapped by /alternatename onto
extern "C" __fastcall stand-ins (a __fastcall function with (ecx, edx, stack) parameters has the __thiscall frame):
  * the base copy constructor ??0<Base>@@QAE@ABV0@@Z (defined elsewhere in retail) -> @nf427_base_copy@12, which records this /
    other, counts the call and copies the base's bytes after the vptr (sizeof(<Base>) from the header);
  * the row's private/public virtual Clone (the leaf, defined in the row object) <- @nf427_clone@4, called by the test.
The old test's slot alternatenames (every vftable slot target the row object leaves unresolved -> one unused stub) are kept.
Checks are the NF425 ones: one allocation of sizeof(C) bytes; the base copy constructor runs once into the block from the source;
the block gets a vftable pointer; the bytes after the vptr equal the source's; 0 with no construction when new fails.
Out: <out_dir or batches/<b>/tests.r2>/<a>_test.cpp, and a TSV line per row on stdout."""
import os, re, sys
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
b = sys.argv[1]; B = A + '/batches/' + b
OUT = sys.argv[2] if len(sys.argv) > 2 else B + '/tests.r2'
os.makedirs(OUT, exist_ok=True)
for a in open(B + '/rows.txt').read().split():
    rt = open('%s/rows/%s.cpp' % (B, a)).read(); tt = open('%s/tests/%s_test.cpp' % (B, a)).read()
    leaf = re.search(r'^// Leaf:\s*(\S+)', rt, re.M).group(1)
    C = re.match(r'\?Clone@(\w+)@@', leaf).group(1)
    pre = [l for l in rt.splitlines() if l.startswith(('#define ', '#include '))]
    m = re.search(r'^class (\w+)\n\{\npublic:\n    \1\(const \1& other\);', tt, re.M)
    base = m.group(1)
    size = re.search(r'g_size == (0x[0-9A-Fa-f]+)', tt).group(1)
    alts = [l for l in tt.splitlines() if 'alternatename:' in l]
    # the old test's member-by-member copy check (padding is not copied): memcmp(d + off, s + off, n), base part first
    chk = re.findall(r'memcmp\(d \+ (\w+), s \+ \w+, ([^)]+)\) == 0', tt)
    body = re.search(r'friend int main\(void\);\n(.*?)^\};', tt, re.M | re.S).group(1)
    mem = [re.sub(r'/\*.*?\*/', '', l).strip().rstrip(';').split()[-1] for l in body.splitlines()
           if re.match(r'^    (?!virtual|friend)\w[^:(]*;$', l)]
    assert len(chk) == len(mem) + 1 and chk[0][0] == '4', (a, chk, mem)
    hasC = '#include "engine/%s.h"' % C in pre
    opt = [l.split()[1] for l in pre if l.endswith('_VIRTUAL_MODEL')]
    csize = 'sizeof(%s)' % C if hasC else size
    o = []
    o.append('// Behaviour test for 0x%s %s::Clone (NF425; NF427 round 2: on the shared headers): links the row object.' % (a.upper(), C))
    if hasC:
        o.append('// Classes: the row\'s prelude below (its #defines, including %s and its Needs, and its #includes) gives %s from' % (opt[0] if opt else 'the opt-ins', C))
        o.append('//   engine/%s.h and %s from engine/%s.h; no class body is defined here.' % (C, base, base))
    else:
        o.append('// Classes: the row\'s prelude below (its #defines and #includes) gives %s from engine/%s.h; %s is the row\'s own' % (base, base, C))
        o.append('//   class (%s), so the test names it only through its Clone symbol and the retail allocation size %s; no class body is defined here.'
                 % ('no engine/%s.h; ruling q1' % C if b == 'p2c' else 'engine/%s.h has a hand-written branch; ruling q2' % C, size))
    o.append('// Reached without a class body: the base copy constructor ??0%s@@QAE@ABV0@@Z and the row\'s virtual Clone are mapped by' % base)
    o.append('//   /alternatename onto extern "C" __fastcall stand-ins (ecx = this, edx unused, stack arguments popped by the callee: the')
    o.append('//   __thiscall frame); operator new is defined here.')
    o.append('// Checks: one allocation of %s bytes; the base copy constructor runs once into the block from the source; the block gets the' % csize)
    if mem:
        o.append('//   class\'s vftable pointer; the base part after the vptr equals the source\'s (by the constructor) and each own member (%s) is' % ', '.join(mem))
        o.append('//   copied by the inlined copy (padding is not copied)%s; 0 with no construction when new fails.' % (', at its offset in engine/%s.h' % C if hasC else ''))
    else:
        o.append('//   class\'s vftable pointer; the bytes after the vptr equal the source\'s (by the constructor: %s has no own members); 0 with' % C)
        o.append('//   no construction when new fails.')
    o += pre
    o += ['#include <stdio.h>', '#include <string.h>', '#include <stddef.h>', '']
    if hasC:
        o.append('typedef char nf427_size_check[sizeof(%s) == %s ? 1 : -1];   // retail operator new(%s)' % (C, size, size))
    o.append('static int g_ctors = 0; static const void* g_this = 0; static const void* g_other = 0;')
    o.append('extern "C" void* __fastcall nf427_base_copy(void* self, int edx_unused, const void* other)')
    o.append('{')
    o.append('    g_this = self; g_other = other; ++g_ctors;')
    o.append('    memcpy((unsigned char*)self + 4, (const unsigned char*)other + 4, sizeof(%s) - 4);' % base)
    o.append('    return self;')
    o.append('}')
    o.append('#pragma comment(linker, "/alternatename:??0%s@@QAE@ABV0@@Z=@nf427_base_copy@12")' % base)
    o.append('extern "C" CCreatureActionBase* __fastcall nf427_clone(const void* self);   // the row\'s Clone')
    o.append('#pragma comment(linker, "/alternatename:@nf427_clone@4=%s")' % leaf)
    o.append('// The row object emits the class\'s vftable (its inlined copy constructor stores it); the test never calls through it, so')
    o.append('// every slot target the object leaves unresolved is mapped onto one unused stub for the link.')
    o.append('extern "C" void nf425_unused_slot(void) { }')
    o += alts
    o.append('static unsigned int g_size = 0; static int g_news = 0, g_fail = 0;')
    o.append('static long g_block[%s / 4 + 4];' % csize)
    o.append('void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }')
    o.append('')
    o.append('int main(void)')
    o.append('{')
    o.append('    static long src_store[%s / 4 + 4];' % csize)
    o.append('    unsigned char* s = (unsigned char*)src_store;')
    o.append('    for (int i = 4; i < (int)%s; ++i) s[i] = (unsigned char)(i * 7 + 3);' % csize)
    o.append('    CCreatureActionBase* r = nf427_clone(src_store);')
    o.append('    unsigned char* d = (unsigned char*)g_block;')
    o.append('    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == %s && g_ctors == 1 && g_this == (void*)g_block' % csize)
    if not mem:
        o.append('        && g_other == (const void*)src_store && memcmp(d + 4, s + 4, %s - 4) == 0 && *(void**)d != 0;' % csize)
    else:
        o.append('        && g_other == (const void*)src_store && memcmp(d + 4, s + 4, sizeof(%s) - 4) == 0 && *(void**)d != 0' % base)
        for (off, n), mn in zip(chk[1:], mem):
            o.append('        && memcmp(d + %s, s + %s, %s) == 0%s   // %s' % (off, off, n, (' && offsetof(%s, %s) == %s' % (C, mn, off)) if hasC else '', mn))
        o[-1] = o[-1].replace('   // ', ';   // ', 1)
    o.append('    g_fail = 1;')
    o.append('    r = nf427_clone(src_store);')
    o.append('    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;')
    o.append('    if (!ok) {')
    o.append('        printf("%s_TEST FAIL news=%%d ctors=%%d size=%%u\\n", g_news, g_ctors, g_size);' % a)
    o.append('        return 1;')
    o.append('    }')
    o.append('    printf("%s_TEST PASS\\n");' % a)
    o.append('    return 0;')
    o.append('}')
    open('%s/%s_test.cpp' % (OUT, a), 'w').write('\n'.join(o) + '\n')
    print('%s\t%s\t%s\t%s\t%s\t%d' % (a, C, base, size, 'header' if hasC else 'row-class', len(alts)))
