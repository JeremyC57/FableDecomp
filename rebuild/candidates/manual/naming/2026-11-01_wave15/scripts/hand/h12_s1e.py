# sub-wave 1e (hand): the CScriptThing / ScriptedThingBase ctor-dtor callers. HE_OV=<overlay>
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub, _rw, _w
# ---- 0x007E74D0 ??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ: the composite deleting-destructor rows destroy a
#      ScriptedThingBase member through a free `AutoTiny*MemberDestructor_007e74d0(void*)` stand-in; written as the qualified
#      (direct, non-virtual) destructor call on the member, as the landed 0x00ED0E30 does.
STB = '''#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBasePdb
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
class CScriptGameResourceObjectScriptedThingBase { public:   // ctor 0x007E72A0 (Ego_r.pdb; the member's own model)
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
};'''
DT = r'reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(\1)->CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase();'
for a in ('00e546a0', '00eb6310', '00eb6340', '00ecd5d0', '00d34440', '00eb62b0', '00eb62e0'):
    p, rel, t = _rw(a, 'src')
    if 'AutoTiny' not in t or '_007e74d0' not in t:
        print('already', a, 'src'); continue
    t2, n1 = re.subn(r'extern void __fastcall AutoTiny(?:Second)?MemberDestructor_007e74d0\(void\* member\);', lambda m: STB, t)
    t2, n2 = re.subn(r'AutoTiny(?:Second)?MemberDestructor_007e74d0\((static_cast<unsigned char\*>\(self\) \+ \d+)\);', DT, t2)
    if n1 != 1 or n2 != 1: raise SystemExit('COUNT %s %d %d' % (a, n1, n2))
    _w(p, t2); print('ok', a, 'src')
# ---- 0x007E72A0 ??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ: the two tiny constructor rows
edit('00cd2e2d', 'src', '''struct CMember_007e72a0Member {
    CMember_007e72a0Member();                        // 0x007E72A0
};''', '''class CScriptGameResourceObjectScriptedThingBase { public:   // Ego_r.pdb (default constructor 0x007E72A0)
    CScriptGameResourceObjectScriptedThingBase();               // 0x007E72A0
};''')
edit('00cd2e2d', 'src', '    CMember_007e72a0Member m_string;                 // +0x0', '    CScriptGameResourceObjectScriptedThingBase m_string;   // +0x0')
resub('00cd2e2d', 'src', r'\bCMember_007e72a0::CMember_007e72a0\b', 'CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase')
resub('00cd2e2d', 'src', r'\bCMember_007e72a0\b', 'CScriptGameResourceObjectScriptedThingBase')
edit('00cd2b78', 'src', '''struct CClass_00cd2b78 {
    unsigned char _body[4];
    void Initialise();                     // 0x007E72A0
    CClass_00cd2b78(const CAllocator&);
};

CClass_00cd2b78::CClass_00cd2b78(const CAllocator&)
{
    Initialise();
}''', '''class CScriptGameResourceObjectScriptedThingBase { public:   // Ego_r.pdb (default constructor 0x007E72A0)
    CScriptGameResourceObjectScriptedThingBase();               // 0x007E72A0
    unsigned char _body[4];
};

struct CClass_00cd2b78 : CScriptGameResourceObjectScriptedThingBase {
    CClass_00cd2b78(const CAllocator&);
};

CClass_00cd2b78::CClass_00cd2b78(const CAllocator&)
{
}''')
edit('00cd2b78', 'src', '-- constructor: initialises through the out-of-line helper @ 0x007E72A0;', '-- constructor: its CScriptGameResourceObjectScriptedThingBase base is built by the out-of-line constructor @ 0x007E72A0;')
# ---- 0x006E7B40 ??0CScriptThing@@QAE@XZ / 0x004AA840 ??1CScriptThing@@UAE@XZ (engine/CScriptThing.h, Ego_r + FableWin; vtable
#      0x01238C8C slot 0 is the destructor): the four rows that wrapped their local CScriptThing in `namespace N_006e7b40` (so their
#      constructor / destructor named ??0 / ??1CScriptThing@N_006e7b40@@) drop the wrapper; where the destructor was not virtual it
#      takes vtable slot 0 (one padding slot less, every other slot keeps its index).
for a in ('00da6d50', '00da7830', '00da7d30', '00ee8870'):
    resub(a, 'src', r'namespace N_006e7b40 \{ struct CScriptThing; \} using N_006e7b40::CScriptThing;', 'class CScriptThing;')
    resub(a, 'src', r'namespace N_006e7b40 \{ struct CScriptThing : (CBase|CBaseClass) \{', r'class CScriptThing : public \1 { public:')
    resub(a, 'src', r'namespace N_006e7b40 \{ struct CScriptThing \{', 'class CScriptThing { public:')
    resub(a, 'src', r'\}; \} using N_006e7b40::CScriptThing;', '};', 1)
edit('00da6d50', 'src', '    VPAD64(ta) VPAD8(tb) VPAD2(tc) VPAD1(td)           // slots 0..74', '    virtual ~CScriptThing();                           // slot 0, 0x004AA840\n    VPAD64(ta) VPAD8(tb) VPAD2(tc)                     // slots 1..74')
edit('00da6d50', 'src', '    ~CScriptThing();                                   // 0x004AA840\n', '')
edit('00da7830', 'src', '    VPAD16(ta) VPAD4(tb) VPAD1(tc)                     // slots 0..20', '    virtual ~CScriptThing();                           // slot 0, 0x004AA840\n    VPAD16(ta) VPAD4(tb)                               // slots 1..20')
edit('00da7830', 'src', '    ~CScriptThing();                                   // 0x004AA840\n}', '}')
