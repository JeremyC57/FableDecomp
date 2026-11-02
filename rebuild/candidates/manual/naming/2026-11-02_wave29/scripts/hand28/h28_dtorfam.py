# h28_dtorfam.py: naming wave 28, families sfb / esb (c696 queue): the free __fastcall stand-ins bound at the chunk 696 destructors are
#  respelled to the destructors themselves, called qualified (a direct thiscall: `this` in ecx as the stand-in had it):
#  sfb: ?AutoTinyCompositeDestructor@@YIXPAX@Z (150 rows, the CSpawnedFunc<T> deleting destructors) -> ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0
#       (chunk 696: ctor 0x00CDD450 stores vtable 0x012C2F9C, whose ??_G 0x00CDD4A0 calls it; Ego_r.pdb + FableWin.pdb);
#  esb: ?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z (41 rows) -> ??1CEntityScriptBase@@UAE@XZ 0x00F35B40 (chunk 696: stores vtable
#       0x012C3224, engine/CEntityScriptBase.h's class; the tail of every entity-script destructor).
#  Usage: h28_dtorfam.py <fam> (rows are read from idx/full.jsonl: every row naming the stand-in symbol).
import os, sys, json, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
FAM = {
 'sfb': ('?AutoTinyCompositeDestructor@@YIXPAX@Z', 'AutoTinyCompositeDestructor', 'CSpawnedFuncBase', '0x00CDD4C0', 'vtable 0x012C2F9C',
         '??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0 (chunk 696: the ctor 0x00CDD450 stores vtable\n//   0x012C2F9C, whose ??_G 0x00CDD4A0 calls it; Ego_r.pdb + FableWin.pdb)'),
 'esb': ('?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z', 'AutoTinyCompositeDestructor_00f35b40', 'CEntityScriptBase', '0x00F35B40', 'vtable 0x012C3224',
         '??1CEntityScriptBase@@UAE@XZ 0x00F35B40 (chunk 696: stores vtable 0x012C3224, the\n//   engine/CEntityScriptBase.h class; Ego_r.pdb + FableWin.pdb)'),
}
fam = sys.argv[1]
sym, fn, cls, addr, vt, why = FAM[fam]
IX = os.path.join(he.W, 'idx', 'full.jsonl')
rows = sorted(json.loads(l)['a'] for l in open(IX) if sym in l)
skip = set(l.split()[0] for l in open(os.path.join(he.W, 'runs', 'excl_%s.txt' % fam))) if os.path.exists(os.path.join(he.W, 'runs', 'excl_%s.txt' % fam)) else set()
for a in rows:
    if a in skip: continue
    he.edit(a, 'src', 'extern void __fastcall %s(void* self);' % fn,
            'class %s { public: virtual ~%s(); };   // %s (%s)' % (cls, cls, addr, vt))
    he.resub(a, 'src', r'\b%s\(([^;]*)\);' % re.escape(fn), r'((%s*)(\1))->%s::~%s();' % (cls, cls, cls), 1)
    note(a, fam, 'the base destructor is %s, called qualified. Was the free\n//   __fastcall %s stand-in.' % (why, sym))
