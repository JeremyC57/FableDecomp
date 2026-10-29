from he import edit
P = '// 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ / 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ: protected (PDB)'
for a in ('004fe2a0', '009d0380'):
    import re
    from he import path
    p, rel = path(a, 'src'); t = open(p).read()
    m = re.search(r'struct CBaseClassNonCopyable \{\n(    CBaseClassNonCopyable\(\);[^\n]*\n    virtual ~CBaseClassNonCopyable\(\);[^\n]*\n)\};', t)
    edit(a, 'src', m.group(0), 'class CBaseClassNonCopyable {\nprotected:\n' + m.group(1) + '};')
for a in ('00afbbd0', '00b017f0'):
    edit(a, 'src', 'class CBaseClassNonCopyable {                      // vtable 0x0129A7C4\npublic:\n    virtual ~CBaseClassNonCopyable();              // 0x0099A300\n',
         'class CBaseClassNonCopyable {                      // vtable 0x0129A7C4\nprotected:\n    virtual ~CBaseClassNonCopyable();              // 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ (protected)\n')
for k in ('src', 'tst'):
    from he import path
    p, rel = path('00bdedd0', k); t = open(p).read()
    import re
    m = re.search(r'class CBaseClassNonCopyable \{\npublic:\n(    CBaseClassNonCopyable\(\); +// 0x0099A2F0\n    virtual ~CBaseClassNonCopyable\(\); +// 0x0099A300\n)', t)
    if m: edit('00bdedd0', k, m.group(0), 'class CBaseClassNonCopyable {\nprotected:\n' + m.group(1))
    else: print('nomatch 00bdedd0', k)
edit('00b1ffc0', 'src', 'class CBaseClassNonCopyable {\npublic:\n    virtual void v000();\n    ~CBaseClassNonCopyable();                      // 0x0099A300\n',
     'class CBaseClassNonCopyable {\nprotected:\n    virtual ~CBaseClassNonCopyable();              // 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ (slot 0: the virtual dtor)\n')
