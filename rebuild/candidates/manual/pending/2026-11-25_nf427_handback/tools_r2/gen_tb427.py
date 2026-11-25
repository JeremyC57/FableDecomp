#!/usr/bin/env python3
"""gen_tb427.py <t>: NF427 round 2 Task B. Ports each landed NF425 behaviour test of test-only batch <t> (HEAD payload
work/p_<t>h/payload.json, i.e. the repo copies) onto the shared headers, test text only (the row is untouched):
  * every local class body is removed (the old tests modelled the class, its base, or both, with local bodies);
  * the members the old test defined (the copy constructor ??0X@@QAE@ABV0@@Z and/or X::operator= ??4X@@QAEAAV0@ABV0@@Z, the
    callees of the row) become extern "C" __fastcall stand-ins with the same body, mapped by /alternatename from the mangled name
    the OLD test object defines (read from its symbol table, not constructed). A __fastcall (ecx, edx, stack) function has the
    __thiscall frame (ecx = this, edx unused, stack arguments popped by the callee);
  * the call of the row's function (src->X::Clone() / dst->X::Copy(src)) goes through an extern "C" __fastcall stand-in mapped by
    /alternatename onto the row's leaf (its // Leaf: line), so no class declaration is needed to reach a private/protected member;
  * pointers to removed classes become void*; sizes, offsets and the printf / pass lines are kept verbatim (the retail literals);
  * the classes come from the shared headers: the row's prelude (#defines and #includes) verbatim, plus engine/<C>.h for the
    row's class C when that header exists and the test compiles with it under the row's prelude (/Od /W3, as vland compiles tests).
Out: r2/<t>/tests/<a>_test.cpp, work/tb/<t>_gen.tsv (address, class, stand-ins, engine/<C>.h included yes/no/none, notes)."""
import os, re, sys, json, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L; V = L.V
A = L.A; t = sys.argv[1]
P = json.load(open('%s/work/p_%sh/payload.json' % (A, t)))['result']['authored']
OUT = '%s/r2/%s/tests' % (A, t); os.makedirs(OUT, exist_ok=True)
W = '%s/work/tb/%s' % (A, t); os.makedirs(W, exist_ok=True)
only = set(sys.argv[2:])
def cl_od(src, obj):
    cmd = ([V.WINE] if V.WINE else []) + [str(V.CL_EXE)] + V.tool_args(['/nologo', '/c', '/Od', '/W3', '/Fo' + obj, src])
    try: r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=L.E, timeout=120)
    except subprocess.TimeoutExpired: return 99, 'error C9999: cl TIMEOUT (120 s)'
    return r.returncode, r.stdout + r.stderr
CLS_RE = re.compile(r'^(?:class|struct) (\w+)(?: : public \w+)?\n\{\n.*?^\};\n', re.M | re.S)
DEF_RE = re.compile(r'^(?:(\w+)(&?) )?(\w+)::(\w+|operator=)\(const (\w+)& other\) \{ (.*) \}$', re.M)
log = []
for x in P:
    a = x['address']
    if only and a not in only: continue
    old = x['test_cpp']; row = x['source_cpp']; patt = x['pass_pattern']
    leaf = re.search(r'^// Leaf:\s*(\S+)', row, re.M).group(1)
    m = re.match(r'\?(Clone|Copy)@(\w+)@@[A-Z]([AB])E', leaf); meth, C = m.group(1), m.group(2)
    # symbols the old test object defines
    f0 = '%s/%s_old.cpp' % (W, a); open(f0, 'w').write(old)
    rc, o = cl_od(f0, f0[:-4] + '.obj')
    if rc: log.append([a, C, '-', '-', 'OLD_TCC']); continue
    dfn, und = L.syms(f0[:-4] + '.obj')
    if leaf not in und: log.append([a, C, '-', '-', 'LEAF_NOT_UND']); continue
    new = old; stubs = []; notes = []
    # 1. member definitions -> stand-ins
    for d in list(DEF_RE.finditer(old)):
        ret, amp, X, M, X2, body = d.groups()
        kind = 'ctor' if M == X else 'assign'
        pref = '??0%s@@' % X if kind == 'ctor' else '??4%s@@' % X
        syms = [s for s in dfn if s.startswith(pref)]
        if len(syms) != 1: notes.append('NOSYM %s' % pref); continue
        nm = 'nf427_copy_ctor' if kind == 'ctor' else 'nf427_assign'
        b = body.replace('memcpy(_base, other._base, ', 'memcpy((unsigned char*)self + 4, (const unsigned char*)other + 4, ')
        b = re.sub(r'\bthis\b', 'self', b).replace('&other', 'other').replace('return *self;', 'return self;')
        if 'return self;' not in b: b += ' return self;'
        stub = ('extern "C" void* __fastcall %s(void* self, int edx_unused, const void* other) { %s }   // %s::%s\n'
                '#pragma comment(linker, "/alternatename:%s=@%s@12")' % (nm, b, X, 'operator=' if kind == 'assign' else X, syms[0], nm))
        new = new.replace(d.group(0), stub); stubs.append(syms[0])
    # 2. class bodies out
    removed = [mm.group(1) for mm in CLS_RE.finditer(new)]
    new = CLS_RE.sub('', new)
    # 3. calls of the row's function
    if meth == 'Clone':
        new, k = re.subn(r'\b(\w+)->%s::Clone\(\)' % re.escape(C), r'nf427_clone(\1)', new)
        decl = ('extern "C" CCreatureActionBase* __fastcall nf427_clone(const void* self);   // the row\'s %s::Clone\n'
                '#pragma comment(linker, "/alternatename:@nf427_clone@4=%s")' % (C, leaf))
    else:
        new, k = re.subn(r'\b(\w+)->%s::Copy\((\w+)\)' % re.escape(C), r'nf427_copy(\1, 0, \2)', new)
        decl = ('extern "C" void __fastcall nf427_copy(void* self, int edx_unused, const void* other);   // the row\'s %s::Copy\n'
                '#pragma comment(linker, "/alternatename:@nf427_copy@12=%s")' % (C, leaf))
    if not k: log.append([a, C, '-', '-', 'NOCALL']); continue
    # 4. pointers to removed classes -> void*
    for X in removed:
        new = re.sub(r'^(\s+)(const )?%s\* (\w+) = \((?:const )?%s\*\)(.*);$' % (X, X), r'\1\2void* \3 = \4;', new, flags=re.M)
    left = [X for X in removed if re.search(r'\b%s\b' % X, re.sub(r'//[^\n]*', '', new).replace('class %s;' % X, ''))]
    new = re.sub(r'^friend int main\(void\);\n', '', new, flags=re.M)
    # 5. header block and prelude
    pre = [l for l in row.splitlines() if l.startswith(('#define ', '#include '))]
    lines = new.splitlines()
    k0 = next(i for i, l in enumerate(lines) if l.startswith('#include'))
    head = lines[:k0]; rest = lines[k0:]
    # insert the stand-in declaration for the row's call before int main
    km = next(i for i, l in enumerate(rest) if l.startswith('int main'))
    rest = rest[:km] + decl.split('\n') + [''] + rest[km:]
    hdrC = 'engine/%s.h' % C
    hasH = any(os.path.exists(r + '/' + hdrC) for r in [os.environ.get('FABLE_HDR_OVERLAY', ''), L.R + '/rebuild/include'] if r)
    note = ['// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row\'s',
            '//   prelude below, verbatim%s). Removed local bodies: %s. The members the test supplies (%s) and the row\'s %s::%s are'
            % (', plus ' + hdrC if hasH else '', ', '.join(removed) or '-', ', '.join(stubs) or '-', C, meth),
            '//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments',
            '//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.']
    inc = 'no'
    for withC in ([True, False] if hasH and ('#include "%s"' % hdrC) not in pre else [False]):
        body = head + note + pre + (['#include "%s"' % hdrC] if withC else []) + rest
        if not withC and hasH and ('#include "%s"' % hdrC) not in pre:
            body[len(head) + 1] = body[len(head) + 1].replace(', plus ' + hdrC, '')
            body.insert(len(head) + len(note), '//   engine/%s.h is not included: it does not compile under the row\'s prelude (header debt, T3).' % C)
        txt = '\n'.join(body) + '\n'
        f1 = '%s/%s_test.cpp' % (W, a); open(f1, 'w').write(txt)
        rc, o = cl_od(f1, f1[:-4] + '.obj')
        if rc == 0: inc = 'yes' if withC else ('prelude' if ('#include "%s"' % hdrC) in pre else ('no-compile' if hasH else 'none')); break
        notes.append('TCC%s %s' % ('+H' if withC else '', ' '.join(re.findall(r'error C\d+[^\n]{0,70}', o)[:2])))
    else:
        log.append([a, C, ';'.join(stubs), '-', 'TCC ' + ' | '.join(notes)]); continue
    if patt not in txt: notes.append('PATT_MISSING')
    pass   # (a removed class still named in code is fine when it compiles: its header declares it)
    rd = V.shared_header_redefinitions(txt)
    if rd: notes.append('SHARED %s' % rd)
    if re.search(r'\b(?:class|struct)\s+\w+\s*(?::[^{;()]*)?\{', V._strip_comments_and_strings(txt)): notes.append('BODY_LEFT')
    open('%s/%s_test.cpp' % (OUT, a), 'w').write(txt)
    log.append([a, C, ';'.join(stubs) or '-', inc, ' '.join(n for n in notes if not n.startswith('TCC+H')) or 'OK'])
with open('%s/work/tb/%s_gen.tsv' % (A, t), 'w') as fo:
    fo.write('address\tclass\tstand_ins\tengine_header\tnote\n')
    for r in log: fo.write('\t'.join(r) + '\n')
import collections
print(t, collections.Counter(r[4].split()[0] for r in log), collections.Counter(r[3] for r in log))
