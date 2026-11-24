import re, sys, shutil, os
sys.path.insert(0, 'tools'); import nf
a = '00995d30'; src = str(nf.src_of(a)); shutil.copy(src, 'orig/%s.cpp' % a)
t = open(src, encoding='utf-8').read()
reps = [("class CMemoryDataInputStream\n{ public:\n    void**   vtable;\n",
"class CDataInputStream\n{ public:\n    virtual void _v00();\n    virtual void SetPosition(unsigned long pos);         // slot 1: 0x00993BC0 ?SetPosition@CDataInputStream@@UAEXK@Z\n"
"    virtual unsigned long GetPosition() const;           // slot 2: 0x00405FA0 ?GetPosition@CDataInputStream@@UBEKXZ (labels_rtti_port; rtti-vtables CMemoryDataInputStream slot 2)\n};\n"
"class CMemoryDataInputStream : public CDataInputStream\n{ public:\n    virtual void _v03(); virtual void _v04(); virtual void _v05();\n    // +0x00: the vptr (was `void** vtable`; slot 2 was called through it as a __fastcall pointer)\n"),
("    CCharString ReadNullTerminatedString();\n", "    virtual CCharString ReadNullTerminatedString();   // slot 6: rtti-vtables CMemoryDataInputStream slot 6; PDB UAE\n"),
("typedef long (__fastcall *GetPositionFn)(CMemoryDataInputStream*);\n", ""),
("    long curPos = ((GetPositionFn)vtable[2])(this);\n", "    long curPos = GetPosition();\n"),
("        __int64 newPosWide = ((GetPositionFn)vtable[2])(this) + (unsigned long)consumed;\n", "        __int64 newPosWide = GetPosition() + (unsigned long)consumed;\n")]
for o, n in reps:
    assert t.count(o) == 1, o[:50]; t = t.replace(o, n)
P = [e for e in nf.entries() if e[0] == a][0][3]
t = '// Leaf: ' + P + '\n' + t
open('rows/%s.cpp' % a, 'w', encoding='utf-8').write(t); print('OK')
