# h30_cst.py: naming wave 30, family cst (scope 1, queue row 24): rows that bind ~CScriptThing 0x004AA840 under a non-retail spelling
#  take the shared engine/CScriptThing.h retail branch (FABLE_RETAIL_NSCRIPT: class CScriptThing : CBaseClass, vtable 0x01238C8C, virtual
#  ~CScriptThing -> ??1CScriptThing@@UAE@XZ, Ego_r.pdb + FableWin.pdb). Part 1: the local non-virtual `class CScriptThing { ~CScriptThing(); }`
#  stand-ins (??1CScriptThing@@QAE@XZ) whose bodies use no member of the local layout.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
INC = ('#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)\n'
       '#define FABLE_RETAIL_NSCRIPT\n'
       '#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840\n')
NOTE = ('the local non-virtual CScriptThing stand-in (??1CScriptThing@@QAE@XZ) is replaced by engine/CScriptThing.h\'s retail\n'
        '//   class: ~CScriptThing 0x004AA840 is virtual (RTTI CScriptThing : CBaseClass; Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ)')
def local_block(a, inc=INC, extra=''):
    p, rel, t = he._rw(a, 'src')
    if '#include "engine/CScriptThing.h"                         // : CBaseClass' in t: print('already', a, 'src'); return
    m = re.search(r'\nclass CScriptThing \{[^\n]*\n(?:public:\n)?(?:    [^\n]*\n)+\};\n', t)
    assert m, a
    blk = m.group(0)
    assert '~CScriptThing();' in blk, a
    t = t[:m.start()] + '\n' + inc + extra + t[m.end():]
    he._w(p, t); print('ok', a, 'src')
for a in ('00cbeed2', '00cbef30', '00cbefc1', '00cbf9de', '00d7ae80', '00dbde40', '00dbf2d0', '00df36e0'):
    local_block(a)
    note(a, 'cst', NOTE)
# 0x00D81820: the local virtual-slot stand-in (v00.. / CScriptThingImpl pointee) -> the header; the pointee is CScriptThing (PImp is
#  CCountedPointer<CScriptThing>); the ability argument takes the PDB's EHeroAbility.
a = '00d81820'
p, rel, t = he._rw(a, 'src')
if 'struct CScriptThingImpl;\nclass CScriptThing {' in t:
    i = t.index('struct CScriptThingImpl;\nclass CScriptThing {'); j = t.index('inline bool CScriptThing::IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }\n')
    t = t[:i] + INC + t[j + len('inline bool CScriptThing::IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }\n'):]
    he._w(p, t); print('ok', a, 'src')
else: print('already', a, 'src')
he.edit(a, 'src', 'Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")', 'Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO")')
note(a, 'cst', 'the local virtual-slot CScriptThing stand-in (non-virtual ??1CScriptThing@@QAE@XZ) and its CScriptThingImpl pointee are\n'
     '//   replaced by engine/CScriptThing.h\'s retail class (virtual ~CScriptThing 0x004AA840, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ;\n'
     '//   the pointee is CScriptThing); MsgIsHitBySpecialAbilityFrom takes the PDB\'s EHeroAbility')
# 0x00EEB7B0: the VSLOTS stand-in and its CScriptThingImplementation pointee -> the header; the inline alive test reads PImp.Data and calls the
#  pointee's slot 75 IsAlive (the 0x004AB130 body).
a = '00eeb7b0'
p, rel, t = he._rw(a, 'src')
if 'struct CScriptThingImplementation;\n\nclass CScriptThing {' in t:
    i = t.index('struct CScriptThingImplementation;\n\nclass CScriptThing {'); j = t.index('__forceinline bool IsThingHandleAlive(')
    t = t[:i] + INC + '\n' + t[j:]
    t = t.replace('    return thing.PImp_Data != 0 && thing.PImp_Data->IsThingAlive();', '    return thing.PImp.Data != 0 && thing.PImp.Data->IsAlive();')
    i = t.index('#define VSLOTS_THING'); j = t.index('\n\n', i)
    t = t[:i] + t[j + 1:]
    he._w(p, t); print('ok', a, 'src')
else: print('already', a, 'src')
note(a, 'cst', 'the local VSLOTS CScriptThing stand-in (non-virtual ??1CScriptThing@@QAE@XZ) and its CScriptThingImplementation pointee are\n'
     '//   replaced by engine/CScriptThing.h\'s retail class (virtual ~CScriptThing 0x004AA840, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ;\n'
     '//   the pointee is CScriptThing, whose slot 75 IsAlive the inline alive test calls)')
# Part 2: rows that included the flattened engine/CScriptThing.h and derived an invented second view (ScriptThingCopy, NScript::ScriptThing,
#  Row00e15500::Handle) only to get the out-of-line copy 0x004ABE90 / destructor 0x004AA840 that the retail class declares itself: the
#  header's retail branch, the view dropped. A flattened engine/CGameScriptInterface.h the row uses moves above the opt-in define.
def flat_to_retail(a, view_rx, view_name, gsi_first=False):
    p, rel, t = he._rw(a, 'src')
    if '#include "engine/CScriptThing.h"                         // : CBaseClass' in t: print('already', a, 'src'); return
    gsi = '#include "engine/CGameScriptInterface.h"\n'
    if gsi_first:
        assert t.count(gsi) == 1, a
        t = t.replace(gsi, '')
    assert t.count('#include "engine/CScriptThing.h"\n') == 1, a
    t = t.replace('#include "engine/CScriptThing.h"\n', (gsi if gsi_first else '') + INC)
    m = re.search(view_rx, t); assert m, a
    t = t[:m.start()] + t[m.end():]
    t = re.sub(r'(?<!::)(?<!\w)' + view_name + r'\b', 'CScriptThing', t)
    he._w(p, t); print('ok', a, 'src')
NOTE2 = ('the invented second view %s (??1%s) over the flattened engine/CScriptThing.h is dropped: engine/CScriptThing.h\'s retail\n'
         '//   class declares the out-of-line copy 0x004ABE90 and the virtual ~CScriptThing 0x004AA840 itself (Ego_r.pdb / FableWin.pdb\n'
         '//   ??0CScriptThing@@QAE@ABV0@@Z / ??1CScriptThing@@UAE@XZ)')
for a in ('00d2c530', '00e11bb0'):   # 0x00CDFF50 / 0x00D864B0: deferred (ScriptMethods projection / local enum definitions; whole-row re-author)
    flat_to_retail(a, r'struct ScriptThingCopy\s*:\s*CScriptThing\s*\{[^\n]*\};[^\n]*\n', 'ScriptThingCopy', gsi_first=True)
    note(a, 'cst', NOTE2 % ('ScriptThingCopy', 'ScriptThingCopy@@QAE@XZ'))
# 0x00D9C4A0: NScript::ScriptThing (an invented derived view whose MsgIsHitBy called slot 21 through a member-pointer union over the flattened
#  vfptr) -> the retail class, whose MsgIsHitBy is virtual slot 21 (+0x54) itself.
a = '00d9c4a0'
flat_to_retail(a, r'struct ScriptThing : ::CScriptThing \{\n(?:    [^\n]*\n)+\};\n', 'ScriptThing', gsi_first=True)
he.edit(a, 'src', 'union ThingSlot { void* Address; bool (CScriptThing::*MsgIsHitBy)(const CCharString&) const; };\n', '')
he.edit(a, 'src', 'FABLE_STATIC_ASSERT(sizeof(ThingSlot)==4);\n', '')
he.edit(a, 'src', '__forceinline bool CScriptThing::MsgIsHitBy(const CCharString& name) const { return (this->*static_cast<const ThingSlot*>(__vftable)[21].MsgIsHitBy)(name); }\n', '')
note(a, 'cst', NOTE2 % ('NScript::ScriptThing', 'ScriptThing@NScript@@QAE@XZ') + ';\n//   its MsgIsHitBy is the retail class\'s virtual slot 21 (was a member-pointer call through the flattened vfptr)')
# 0x00E15500: Row00e15500::Handle (an invented derived view of the flattened CScriptThing: default ctor 0x006E7B40, copy, dtor) -> the retail class.
a = '00e15500'
flat_to_retail(a, r'struct Handle:CScriptThing\{Handle\(\);Handle\(const Handle&\);~Handle\(\);\};', 'Handle')
note(a, 'cst', NOTE2 % ('Row00e15500::Handle', 'Handle@Row00e15500@@QAE@XZ'))
# 0x00E1E530: Row00e1e530::Handle {virtual ~Handle; Thing* object; Control* control} is CScriptThing {vptr; CCountedPointer<CScriptThing> PImp}
#  (Thing = the pointee CScriptThing, Control = CCPPointerInfo): the retail class from the header.
a = '00e1e530'
p, rel, t = he._rw(a, 'src')
if '#include "engine/CScriptThing.h"' not in t:
    i = t.index('namespace Row00e1e530 {'); t = t[:i] + INC + t[i:]
    for rx in (r'struct Thing\{virtual void V0\(\)=0;[^\n]*\};\n', r'struct Control\{int refs;[^\n]*\};\n', r'struct Handle\{virtual ~Handle\(\);[^\n]*\};\n'):
        m = re.search(rx, t); assert m, rx; t = t[:m.start()] + t[m.end():]
    t = re.sub(r'(?<!::)(?<!\w)Handle\b', 'CScriptThing', t)
    assert t.count('trader.object&&trader.object->IsAlive()') == 1
    t = t.replace('trader.object&&trader.object->IsAlive()', 'trader.PImp.Data&&trader.PImp.Data->IsAlive()')
    he._w(p, t); print('ok', a, 'src')
else: print('already', a, 'src')
note(a, 'cst', 'the invented Row00e1e530::Handle {virtual ~Handle; Thing* object; Control* control} (??1Handle@Row00e1e530@@UAE@XZ) is\n'
     '//   engine/CScriptThing.h\'s retail class {vptr; CCountedPointer<CScriptThing> PImp} (virtual ~CScriptThing 0x004AA840, Ego_r.pdb /\n'
     '//   FableWin.pdb ??1CScriptThing@@UAE@XZ); Thing was the pointee CScriptThing (slot 75 IsAlive), Control the CCPPointerInfo')
