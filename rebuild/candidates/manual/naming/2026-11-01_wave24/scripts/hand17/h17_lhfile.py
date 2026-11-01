# h17_lhfile.py: naming wave 17, family own -- the CLHFileManager slot 0 / slot 1 factories (0x00AAB7D0 / 0x00AAB7A0) call the CLHFile
# constructors c676 proved: ??0CLHFile@@QAE@PAVCLHFileManager@@PBD@Z (0x00ADD380, narrow name) and ??0CLHFile@@QAE@PAVCLHFileManager@@PBG@Z
# (0x00ADD4A0, wide name); the invented SFileSpec_* reference parameters are those name pointers, and CLHFile / CLHFileManager are
# classes (RTTI .?AVCLHFile@@, as the landed constructor rows declare them). CreateFile0/1 stay descriptive (no PDB covers the LH
# file manager). Name-only.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import _rw, _w
for a, slot, ch, other, ctor, sym in (('00aab7a0', 1, 'char', '00aab7d0', '0x00ADD380', '??0CLHFile@@QAE@PAVCLHFileManager@@PBD@Z'),
                                      ('00aab7d0', 0, 'unsigned short', '00aab7a0', '0x00ADD4A0', '??0CLHFile@@QAE@PAVCLHFileManager@@PBG@Z')):
    p, rel, t = _rw(a, 'src')
    if 'SFileSpec' not in t: print('already', a); continue
    s = t.index('struct SFileSpec_'); e = t.index('CLHFile* CLHFileManager::CreateFile')
    model = ('class CLHFileManager;\n'
             'class CLHFile {                                    // 0x14 bytes, vtable 0x0129F094 (RTTI .?AVCLHFile@@ : LHA::CFile)\n'
             'public:\n'
             '    unsigned char _body[0x14];\n'
             '    CLHFile(CLHFileManager* manager, const %s* fileName);%s// %s (%s)\n'
             '};\n'
             'class CLHFileManager {                             // vtable 0x0129D2B4\n'
             'public:\n'
             '    virtual CLHFile* CreateFile0(const unsigned short* fileName);   // slot 0, 0x00AAB7D0\n'
             '    virtual CLHFile* CreateFile1(const char* fileName);             // slot 1, 0x00AAB7A0\n'
             '};\n\n') % (ch, ' ' * (12 if ch == 'char' else 2), ctor, sym)
    body = ('CLHFile* CLHFileManager::CreateFile%d(const %s* fileName)\n{\n    return new CLHFile(this, fileName);\n}' % (slot, ch))
    hdr = t[:s].replace('(const SFileSpec_%s& spec)' % a, '(const %s* fileName)' % ch).replace('from this manager and the spec', 'from this manager and the file name')
    hdr += ('// Naming wave 17: the constructor is c676\'s %s; the landed SFileSpec_%s reference parameter is that\n'
            '//   %s file-name pointer (name-only: the bytes are unchanged).\n') % (sym, a, 'narrow' if ch == 'char' else 'wide')
    _w(p, hdr + model + body + t[t.index('}', e) + 1:]); print('ok', a, 'src')
