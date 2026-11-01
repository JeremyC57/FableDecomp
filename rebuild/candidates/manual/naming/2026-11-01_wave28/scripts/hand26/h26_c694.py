# h26_c694.py: naming wave 26, family c694 -- the chunk 694 queue rows 0x00854310 / 0x005ED120 (appended to QUEUE_W26_input.tsv after
#  the chunk 694 landing): the NCameraModes script-module ??_G stand-ins call the two base destructors under row-local spellings.
#  0x00854310 is ??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ and 0x005ED120 ??1CScriptModuleBase@NCameraModes@@UAE@XZ
#  (Ego_r.pdb name set; Fable.exe RTTI: the base arrays of OpenChest / UseLamp / OpenGate put CScriptModuleSelfTerminateBase directly
#  under them, CScriptModuleBase under it; vtable 0x01255F74 .?AVCScriptModuleBase@NCameraModes@@). Name-only: each call stays a
#  direct (qualified) destructor call; the bases declare the virtual destructors and no data (the vptr is the implicit one).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
BASES = '''namespace NCameraModes {
class CScriptModuleBase { public:                 // vtable 0x01255F74 (.?AVCScriptModuleBase@NCameraModes@@)
    virtual ~CScriptModuleBase();                   // 0x005ED120
};
class CScriptModuleSelfTerminateBase : public CScriptModuleBase { public:   // RTTI: the direct base of OpenChest / UseLamp / OpenGate
    virtual ~CScriptModuleSelfTerminateBase();      // 0x00854310
};
}
'''
NOTE = ('// Naming wave 26 (family c694): the base destructor 0x%s is %s (Ego_r.pdb; Fable.exe RTTI base arrays);\n'
        '//   was %s.')
def leaf(a, fn):
    p, rel, t = he._rw(a, 'src')
    if '// Leaf: ' in t: return
    L = t.split('\n'); i = 0
    while i < len(L) and L[i].startswith('//'): i += 1
    L.insert(i, '// Leaf: ?%s@@' % fn); he._w(p, '\n'.join(L))
def note(a, txt):
    p, rel, t = he._rw(a, 'src'); L = t.split('\n')
    j = [k for k, x in enumerate(L) if x.startswith('// Leaf: ')][0]
    L.insert(j + 1, txt); he._w(p, '\n'.join(L))
# 0x00854500 OpenChest / 0x00854540 UseLamp: the base was CScriptModuleBase with a non-virtual ~ at 0x00854310
for a, fn in (('00854500', 'CScriptModuleOpenChest_CompositeScalarDeletingDestructor'), ('00854540', 'CScriptModuleUseLamp_CompositeScalarDeletingDestructor')):
    he.edit(a, 'src', '''namespace NCameraModes {
class CScriptModuleBase { public:                 // vtable 0x01255F74 (.?AVCScriptModuleBase@NCameraModes@@)
    void* _vptr;
    ~CScriptModuleBase();                   // 0x00854310
};
}
''', BASES)
    he.edit(a, 'src', 'struct CObject_%s : NCameraModes::CScriptModuleBase {' % a, 'struct CObject_%s : NCameraModes::CScriptModuleSelfTerminateBase {' % a)
    he.edit(a, 'src', '    object->NCameraModes::CScriptModuleBase::~CScriptModuleBase();',
            '    object->NCameraModes::CScriptModuleSelfTerminateBase::~CScriptModuleSelfTerminateBase();')
    leaf(a, fn)
    note(a, NOTE % ('00854310', '??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ', '??1CScriptModuleBase@NCameraModes@@QAE@XZ (the base one level too high)'))
# the three free __fastcall *_Destruct stand-ins: OpenGate 0x00854580 (0x00854310), GoToLookAt 0x00718810 / TransitionBetweenPoints
#  0x00854450 (0x005ED120)
for a, cls, var, base, tgt in (('00854580', 'OpenGate', 'gate', 'CScriptModuleSelfTerminateBase', '00854310'),
                               ('00718810', 'GoToLookAt', 'lookAt', 'CScriptModuleBase', '005ed120'),
                               ('00854450', 'TransitionBetweenPoints', 'points', 'CScriptModuleBase', '005ed120')):
    he.edit(a, 'src', 'extern void __fastcall CScriptModule%s_Destruct(void* %s);\n' % (cls, var), BASES)
    he.edit(a, 'src', '    CScriptModule%s_Destruct(%s);' % (cls, var),
            '    static_cast<NCameraModes::%s*>(%s)->NCameraModes::%s::~%s();' % (base, var, base, base))
    leaf(a, 'CScriptModule%s_CompositeScalarDeletingDestructor' % cls)
    note(a, NOTE % (tgt.upper(), '??1%s@NCameraModes@@UAE@XZ' % base, 'the free ?CScriptModule%s_Destruct@@YIXPAX@Z' % cls))
