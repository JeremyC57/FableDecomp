# h31_tcb.py: naming wave 31, family tcb (scope 1, DECISIONS_W30 D5 NOTED): engine/CTCBase.h gains the retail polymorphic branch
#  (opt-in FABLE_RETAIL_CTCBASE, as CScriptThing.h's FABLE_RETAIL_NSCRIPT; the flattened layout stays the landed default, so no
#  existing includer changes), then the three slot-13 rows' local CTCBase models move to it: the own row CTCBase::OnDie 0x006868E0,
#  CTCScriptedControl::OnDie 0x00713740 (whose local CTCBase padded slots 0-12 with invented v00..v12) and CTCKrakenTentacle::OnDie
#  0x007F0890 (RTTI: both derive from CTCBase).
import os, sys, re, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REL = 'rebuild/include/engine/CTCBase.h'
dst = os.path.join(W, os.environ.get('HE_OV', 'ov_hand'), REL)
new = open(os.path.join(W, 'hdr', 'CTCBase.h'), encoding='utf-8', newline='').read()
if os.path.exists(dst) and open(dst, encoding='utf-8', newline='').read() == new: print('already', REL)
else:
    os.makedirs(os.path.dirname(dst), exist_ok=True); open(dst, 'w', encoding='utf-8', newline='').write(new); print('ok', REL)
INC = ('#define FABLE_RETAIL_CTCBASE\n'
       '#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; slot 13 OnDie 0x006868E0\n')
# 0x006868E0: the own row
a = '006868e0'
he.edit(a, 'src', '''// Layout: CTCBase {vptr, ParentThing +0x04, ToBeRemoved +0x08, IsInGlobalUpdateTCs +0x09} (Ego_r.pdb, 0xc); only the slot is modelled.
class CTCBase { public:
    virtual void OnDie();   // slot 13
};
''', '''// Layout: engine/CTCBase.h's retail branch (CTCBase : CBaseClassNonCopyable, vtable 0x0125BE0C, OnDie slot 13).
''' + INC)
note(a, 'tcb', 'the local one-slot CTCBase is engine/CTCBase.h\'s retail branch (FABLE_RETAIL_CTCBASE; DECISIONS_W30 D5)')
# 0x007F0890
a = '007f0890'
he.edit(a, 'src', '''class CTCBase { public:
    virtual void OnDie();       // slot 13, 0x006868E0
};
''', INC)
note(a, 'tcb', 'the local one-slot CTCBase is engine/CTCBase.h\'s retail branch (FABLE_RETAIL_CTCBASE; DECISIONS_W30 D5;\n'
     '//   RTTI CTCKrakenTentacle : CTCBase)')
# 0x00713740
a = '00713740'
he.edit(a, 'src', '''class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual void OnDie();                // slot 13: 0x006868E0 (empty)
    CThing* ParentThing;                    // +0x04
    bool    ToBeRemoved;                    // +0x08
    bool    IsInGlobalUpdateTCs;            // +0x09
};
''', INC)
note(a, 'tcb', 'the local CTCBase (slots 0-12 padded with the invented v00..v12) is engine/CTCBase.h\'s retail branch\n'
     '//   (FABLE_RETAIL_CTCBASE; DECISIONS_W30 D5; RTTI CTCScriptedControl : CTCBase; ParentThing +0x04 from the header)')
