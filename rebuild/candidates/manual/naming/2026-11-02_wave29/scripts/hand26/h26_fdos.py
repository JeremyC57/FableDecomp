# h26_fdos.py: naming wave 26, family fdos -- the CFileDataOutputStream(CAFile&) 0x00994400 binders (c693 queue row): CDiskFileWin32 derives
#  from CAFile (RTTI vtable 0x0122D06C: CDiskFileWin32 <- CAFile <- CBaseClassNonCopyable <- CBaseClass) and the stream takes the file by
#  reference (Ego_r.pdb + FableWin.pdb ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z); was CFileDataOutputStream(CDiskFileWin32*).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = ('// Naming wave 26 (family fdos): ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z 0x00994400 (Ego_r.pdb + FableWin.pdb; c693 queue) takes the\n'
        '//   file by reference; CDiskFileWin32 derives from CAFile (RTTI 0x0122D06C). Was CFileDataOutputStream(CDiskFileWin32*).')
for a, cafile in (('009a7720', None), ('009d4ce0', None), ('00a60aa0', 'static')):
    p, rel, t = he._rw(a, 'src')
    if 'CFileDataOutputStream(CAFile& file)' in t: print('already', a, 'src'); continue
    assert t.count('class CDiskFileWin32 : public CBaseClassNonCopyable { public:') == 1
    if cafile:
        t = t.replace('class CAFile {\npublic:\n', 'class CAFile : public CBaseClassNonCopyable {   // RTTI 0x0122D06C chain: CDiskFileWin32 <- CAFile\npublic:\n', 1)
        t = t.replace('class CDiskFileWin32 : public CBaseClassNonCopyable { public:', 'class CDiskFileWin32 : public CAFile { public:')
    else:
        t = t.replace('class CDiskFileWin32 : public CBaseClassNonCopyable { public:',
                      'class CAFile : public CBaseClassNonCopyable { };   // RTTI 0x0122D06C chain: CDiskFileWin32 <- CAFile <- CBaseClassNonCopyable\nclass CDiskFileWin32 : public CAFile { public:')
    assert t.count('    CFileDataOutputStream(CDiskFileWin32* file);   // 0x00994400') == 1
    t = t.replace('    CFileDataOutputStream(CDiskFileWin32* file);   // 0x00994400', '    CFileDataOutputStream(CAFile& file);           // 0x00994400')
    assert t.count('CFileDataOutputStream stream(&file);') == 1
    t = t.replace('CFileDataOutputStream stream(&file);', 'CFileDataOutputStream stream(file);')
    L = t.split('\n'); lf = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    L.insert((lf[0] + 1) if lf else 1, NOTE); he._w(p, '\n'.join(L)); print('ok', a, 'src')
