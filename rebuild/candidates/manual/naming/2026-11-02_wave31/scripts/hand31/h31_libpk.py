# h31_libpk.py: naming wave 31, family libpk (DECISIONS_W30 D7 DECIDED): libpk has no PDB or RTTI, so the 0x00C74450 class keeps the
#  vtable-address convention: CVtable_012bf050 (class key `class`; its dtor stores vtable 0x012BF050, slot 0 the deleting dtor 0x00C74480:
#  virtual, ??1CVtable_012bf050@@UAE@XZ). The own row (was the non-virtual ??1CVtable_012BF050@@QAE@XZ over a placeholder v00 slot), the ctor
#  0x00C74420 (was ??0CDspObject@@QAE@XZ) and the 19 other reverb rows that derive from or name the base (were ??1CDspObject@@UAE@XZ / the
#  CDspObject class) take the one spelling. The class-wide statics 0x0143CCDC / 0x0143CCE0 get one spelling across the two base rows:
#  s_pSharedTable (void*: the dtor frees it through FreeSharedTable(void*)) / s_refCount (were also s_pOps (CDspOps*) / s_refCount of CDspObject).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
BIND = '00c69660 00c69680 00c69720 00c69740 00c697a0 00c697c0 00c69880 00c698a0 00c698d0 00c69930 00c69990 00c6a280 00c6a2e0 00c6a340 00c6a650 00c6a780 00c75530 00c75980 00c75a00 00c775a0'.split()
for a in BIND:
    he.resub(a, 'src', r'\bCDspObject\b', 'CVtable_012bf050')
    note(a, 'libpk', 'the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:\n'
         '//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)')
a = '00c74420'
he.edit(a, 'src', 'struct CDspOps;\n\nstruct CDspObject {\n', 'class CDspObject { public:\n')
he.edit(a, 'src', '    static CDspOps* s_pOps;                // 0x0143CCDC\n', '    static void* s_pSharedTable;           // 0x0143CCDC\n')
he.edit(a, 'src', '    if (!s_pOps)\n', '    if (!s_pSharedTable)\n')
he.resub(a, 'src', r'\bCDspObject\b', 'CVtable_012bf050')
note(a, 'libpk', 'the class is CVtable_012bf050 (DECISIONS_W30 D7: libpk has no PDB / RTTI; vtable 0x012BF050) -- own row\n'
     '//   ??0CVtable_012bf050@@QAE@XZ (was ??0CDspObject@@QAE@XZ); the static 0x0143CCDC is s_pSharedTable, the dtor 0x00C74450\'s spelling\n'
     '//   (was s_pOps / CDspOps*)', leaf='??0CVtable_012bf050@@QAE@XZ')
a = '00c74450'
he.edit(a, 'src', '''struct CVtable_012BF050 {
    virtual void v00();
    ~CVtable_012BF050();
''', '''class CVtable_012BF050 { public:                  // vtable 0x012BF050 (slot 0: the deleting dtor 0x00C74480)
    virtual ~CVtable_012BF050();
''')
he.resub(a, 'src', r'\bCVtable_012BF050\b', 'CVtable_012bf050')
note(a, 'libpk', 'DECISIONS_W30 D7: the class is CVtable_012bf050 (class key `class`) and its destructor virtual (it stores vtable\n'
     '//   0x012BF050, whose slot 0 is the deleting dtor 0x00C74480): ??1CVtable_012bf050@@UAE@XZ, the spelling the reverb rows call (were\n'
     '//   ??1CVtable_012BF050@@QAE@XZ over a placeholder slot v00, and ??1CDspObject@@UAE@XZ in the callers)', leaf='??1CVtable_012bf050@@UAE@XZ')
# 0x00C77440: CVtable_012bf0c8's class key is `class` (DECISIONS_W30 D7); the bare Leaf becomes the emitted ??0CVtable_012bf0c8@@QAE@XZ.
a = '00c77440'
he.edit(a, 'src', 'struct CVtable_012bf0c8 : CVtable_012bf050 {\n', 'class CVtable_012bf0c8 : public CVtable_012bf050 { public:\n')
note(a, 'libpk', 'DECISIONS_W30 D7: class key `class` for CVtable_012bf0c8 (was struct); bare Leaf "??0CVtable_012bf0c8" -> the emitted symbol',
     leaf='??0CVtable_012bf0c8@@QAE@XZ')
