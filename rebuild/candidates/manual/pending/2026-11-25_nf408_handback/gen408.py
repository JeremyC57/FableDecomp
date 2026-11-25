# gen408.py <batch> <addr...>: NF407 tools/gen.py, extended (NF408) to (1) find the row anywhere under rebuild/src/compiled and
# (2) the namespaced shape (`namespace N { class C {...}; }` + `... N::C::StatGetClassTypeName()` out of line). The transform is
# gen.py's: the out-of-line definition becomes the in-class body (return statement and trailing comment unchanged), an
# anonymous-namespace scaffolding helper takes its address, and the `// Form` paragraph goes after the leading comment block.
# Writes batches/<batch>/rows/<a>.cpp, tests/<a>_test.cpp (HEAD copy), orig/rows, orig/tests (HEAD copies).
import sys, re, os, glob, shutil
R = '/home/user/FableDecomp'; W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
B = W + '/batches/' + sys.argv[1]
for d in ('rows', 'tests', 'orig/rows', 'orig/tests'): os.makedirs(B + '/' + d, exist_ok=True)
FORM = """// Form (NF408, 2026-11-25; NF407's mechanical re-form): the static is defined in the class body, so it is a pick-any COMDAT, and a
//   scaffolding helper takes its address so this object emits it (the nf86 / nf88 form, landing nf387 ruling; the nf406_b2 0x004D31C0
//   form). The landed binder CTCBase::RegisterTCs 0x004EE23F defines the same static in the class body (TC_REGISTRATION_INTERFACE,
//   pick-any), and retail's own definition is an in-class inline (RegisterTCs expands it at its first 41 registrations and calls the
//   out-of-line copy after that). The previous out-of-line (NoDuplicates) definition clashed with the binder's pick-any copy at link
//   (LNK4006, landing nf382 ruling).
"""
PLAIN = re.compile(r'^class (CTC\w+)(?: : public \w+)?\s*\{\s*\npublic:\n    static CCharString __fastcall StatGetClassTypeName\(\);\n\};\n\n'
                   r'CCharString __fastcall (CTC\w+)::StatGetClassTypeName\(\)\n\{\n    (return [^\n]*)\n\}\n?', re.M)
NS = re.compile(r'^namespace (N\w+) \{\nclass (CTC\w+)(?: : public \w+)?\s*\{\s*\npublic:\n    static CCharString __fastcall StatGetClassTypeName\(\);\n\};\n\}\n\n'
                r'CCharString __fastcall (N\w+)::(CTC\w+)::StatGetClassTypeName\(\)\n\{\n    (return [^\n]*)\n\}\n?', re.M)
def inbody(ret):
    return ('    static CCharString __fastcall StatGetClassTypeName() { ' + ret.split('   //')[0].rstrip() + ' }' +
            (('   //' + ret.split('   //', 1)[1]) if '   //' in ret else '') + '\n')
def helper(a, qual):
    return ('namespace {\n'
            '// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a\n'
            f'// pick-any COMDAT (retail 0x{a.upper()} is that out-of-line copy). Internal to this TU; never called.\n'
            'CCharString (__fastcall* ReferenceStatGetClassTypeName())()\n{\n'
            f'    return &{qual}::StatGetClassTypeName;\n' '}\n}\n')
for a in sys.argv[2:]:
    src = glob.glob(f'{R}/rebuild/src/compiled/*/*/*_{a}.cpp'); assert len(src) == 1, a; src = src[0]
    tst = src[:-4].replace('/src/compiled/', '/tests/') + '_test.cpp'
    t = open(src, encoding='utf-8').read()
    shutil.copy(src, f'{B}/orig/rows/{a}.cpp'); shutil.copy(tst, f'{B}/orig/tests/{a}_test.cpp'); shutil.copy(tst, f'{B}/tests/{a}_test.cpp')
    m = PLAIN.search(t); n = None if m else NS.search(t)
    if m:
        assert m.group(1) == m.group(2), a
        cls, ret, qual = m.group(1), m.group(3), m.group(1)
        decl = t[m.start():m.end()].split('\n')[0].rstrip().rstrip('{').rstrip()
        body = decl + '\n{\npublic:\n' + inbody(ret) + '};\n\n' + helper(a, qual)
        mm = m
    else:
        assert n and n.group(1) == n.group(3) and n.group(2) == n.group(4), a
        ns, cls, ret = n.group(1), n.group(2), n.group(5); qual = ns + '::' + cls
        decl = t[n.start():n.end()].split('\n')[1].rstrip().rstrip('{').rstrip()
        body = 'namespace ' + ns + ' {\n' + decl + '\n{\npublic:\n' + inbody(ret) + '};\n}\n\n' + helper(a, qual)
        mm = n
    head = t[:mm.start()]; tail = t[mm.end():]
    assert tail.strip() == '', (a, tail)
    L = head.split('\n'); k = 0
    while k < len(L) and L[k].startswith('//'): k += 1
    head = '\n'.join(L[:k]) + '\n' + FORM + '\n'.join(L[k:])
    open(f'{B}/rows/{a}.cpp', 'w', encoding='utf-8').write(head + body)
    print(a, qual, 'OK')
