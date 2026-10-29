# REVERTED (not in compose6.py): the merged CBaseObject makes the derived views polymorphic-destructible and all 24 rows DIFFER (runs/bodtor1 vs bodtor_base); kept for the record, see DEFERRED.tsv.
# wave 6 family: CBaseObject::~CBaseObject 0x0099A430 is `protected: virtual` (Ego_r.pdb / FableWin.pdb
# ??1CBaseObject@@MAE@XZ; slot 0 of vtable 0x0129A7D8 is its scalar deleting destructor). The script-chunk rows carried a
# separate "destructor-only view" `struct CBaseObjectDtor { void* m_pIntelligentPointer; virtual ~CBaseObjectDtor(); }`
# (??1CBaseObjectDtor@@UAE@XZ, no such class in either PDB) beside a CBaseObject view whose slot 0 was a `b000` placeholder.
# Transform (idempotent): the placeholder slot 0 of `class CBaseObject` becomes the real destructor
#   protected: virtual ~CBaseObject(); public:   (test files: `{ }` body kept where the placeholder had one)
# the CBaseObjectDtor struct is deleted, every other `CBaseObjectDtor` spelling becomes `CBaseObject`, and a test's
# out-of-line `CBaseObjectDtor::~CBaseObjectDtor() { }` becomes `CBaseObject::~CBaseObject() { }`.
import re, sys
SLOT0 = re.compile(r'(class CBaseObject \{ public:[^\n]*\n[ \t]*)virtual void (?:b000|v00)\(\)[ \t]*(;|\{ \})')
DTORS = re.compile(r'struct CBaseObjectDtor \{[^\n]*\n(?:[^\n]*\n){1,3}?\};[ \t]*\n')
def fix(t):
    if 'CBaseObjectDtor' not in t: return None, 'no CBaseObjectDtor'
    m = SLOT0.search(t)
    if not m: return None, 'no CBaseObject slot-0 placeholder'
    body = ';' if m.group(2) == ';' else ' { }'
    t = t[:m.start()] + m.group(1) + 'protected: virtual ~CBaseObject()%s public:   // 0x0099A430 (slot 0; PDB protected virtual)' % body + t[m.end():]
    t, n = DTORS.subn('', t)
    if n != 1: return None, 'CBaseObjectDtor struct count %d' % n
    t = t.replace('CBaseObjectDtor::~CBaseObjectDtor()', 'CBaseObject::~CBaseObject()')
    t = t.replace('~CBaseObjectDtor', '~CBaseObject').replace('CBaseObjectDtor', 'CBaseObject')
    return t, 'ok'
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
