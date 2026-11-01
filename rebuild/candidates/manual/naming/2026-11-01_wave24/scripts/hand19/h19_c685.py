# h19_c685.py: naming wave 19, family c685 -- the chunk 685 NAME_FIXES rows (QUEUE_W19_input.tsv) respelled in the landed callers (name-only):
#   ime:   ?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z (FableWin.pdb; the landed 0x00CB5180 leaf) in CIME::Shutdown 0x004051C0 / Initialise 0x00405650
#          (were ImeUI_Initialize), and the SDK callback pointer ImeUiCallback_DrawFans 0x0143CDFC in 0x00405650 (was g_ImeDrawFans);
#   amovie: the DirectShow base-class AMovieSetupRegisterFilter 0x00CAA4A0 has C linkage (_AMovieSetupRegisterFilter@12, the landed leaf) in
#          CBaseFilter::Register 0x00CAAEA0 / Unregister 0x00CAAF10 (were the C++ ?AMovieSetupRegisterFilter@@YG... spelling);
#   bone:  ?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@SIXPAX@Z 0x00AB3640 (the deleter the count block built at 0x00AB1FE0
#          stores: a CCountedPointer<C3DAnimationBuilder::CBoneInfo>) in the Reset row 0x00AB36B0 (was the free Destroy_00ab3640);
#   eax:   CEaxContext::SetSlotLoadEffect(unsigned long, SGuid) 0x00C674B0 (the landed leaf ?SetSlotLoadEffect@CEaxContext@@QAEJKUSGuid@@@Z) in the
#          slot-property dispatcher 0x00C689B0, whose own row is respelled to the same class under the CEaxContext::SetSlotProperty (the catalog name chunk 685 gave it)
#          (long slot, const SEaxSlotProperties&) beside the landed SetSlotLock / SetSlotVolume / SetSlotFlags / SetSlotLoadEffect (file renamed).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
def _once(a, k, pat, repl, marker='naming wave 19'):
    p, rel, t = he._rw(a, k)
    if marker in t: print('already', a, k); return
    he.resub(a, k, pat, repl)
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_c685.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
for a in ('004051c0', '00405650'):
    if a in _X: continue
    he.resub(a, 'src', r'\bImeUI_Initialize\b', 'ImeUi_Initialize')
    _once(a, 'src', r'(ImeUi_Initialize\([^;]*\);\s*//\s*0?x?00CB5180)', r'\1 (FableWin.pdb ?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z; naming wave 19)')
a = '00405650'
if a not in _X:
    he.resub(a, 'src', r'\bg_ImeDrawFans\b', 'ImeUiCallback_DrawFans')
    _once(a, 'src', r'(ImeUiCallback_DrawFans\)\(const IMEUI_VERTEX\*,unsigned int\); //0143CDFC)', r'\1 (SDK ImeUi.cpp callback pointer, chunk 685; naming wave 19)')
for a in ('00caaea0', '00caaf10'):
    if a in _X: continue
    he.resub(a, 'src', r'(?m)^long __stdcall AMovieSetupRegisterFilter\(', 'extern "C" long __stdcall AMovieSetupRegisterFilter(')
    _once(a, 'src', r'(AMovieSetupRegisterFilter\([^;]*\);\s*// 0x00CAA4A0)', r'\1 (_AMovieSetupRegisterFilter@12: the DirectShow base classes\' C-linkage dllsetup.cpp entry, the landed leaf; naming wave 19)')
a = '00ab36b0'
if a not in _X:
    he.edit(a, 'src', 'void __fastcall Destroy_00ab3640(void* object);   // 0x00AB3640: the per-type deleter stored in the block\n',
            'class C3DAnimationBuilder { public: class CBoneInfo; };\ntemplate <class T> class CCountedPointer { public:\n    static void __fastcall DeleteData(void* object);   // 0x00AB3640 ?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@SIXPAX@Z (Ego_r.pdb; the per-type deleter the count block stores, chunk 685)\n};\n')
    he.edit(a, 'src', 'm_pfnDestroy(Destroy_00ab3640)', 'm_pfnDestroy(CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData)')
    he.resub(a, 'src', r'destroy function \(0x00AB3640, called with the object in ecx\)', 'destroy function (0x00AB3640 CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData, called with the object in ecx)')
a = '00c689b0'
if a not in _X:
    p, rel, t = he._rw(a, 'src')
    if 'SetSlotProperty' not in t:
        t = t.replace('// CSkillTable_00c67470::SetSkillPropertiesEx(long, const SSkillPropertiesEx&) -- set every property of skill record `index` in turn (index property, GUID, value, flag), stopping at the first error\n',
                      '// CEaxContext::SetSlotProperty(long slot, const SEaxSlotProperties&) -- EAX 4.0 FX-slot property dispatcher: set every property of slot `slot` in turn (the lock, the guidLoadEffect, the volume, the flags), stopping at the first error\n'
                      '// Leaf: ?SetSlotProperty@CEaxContext@@QAEJJABUSEaxSlotProperties@@@Z\n'
                      '// Name (naming wave 19, chunk 685): the object is the landed CEaxContext (the slot setters SetSlotLock 0x00C67470 / SetSlotLoadEffect 0x00C674B0 /\n'
                      '//   SetSlotVolume 0x00C67720 / SetSlotFlags 0x00C67430 share its `this`); descriptive member name (libpk, no PDB); landed as the propagated\n'
                      '//   CSkillTable_00c67470::SetSkillPropertiesEx (the skill-table reading of the EAX slot table).\n')
        t = t.replace('struct GUID {', 'struct SGuid {                                     // GUID, 16 bytes (the landed 0x00C674B0 spelling)')
        t = re.sub(r'\bGUID\b(?! \+| by| argument| /)', 'SGuid', t)
        t = t.replace('SSkillRecord_00c67470', 'SEaxSlotRecord').replace('SSkillPropertiesEx', 'SEaxSlotProperties').replace('CSkillTable_00c67470', 'CEaxContext')
        t = t.replace('SetSkillPropertiesEx', 'SetSlotProperty')
        t = t.replace('long SetSkillIndexProperty(long index, long value)   // 0x00C67470', 'long SetSlotLock(unsigned long slot, long lock)   // 0x00C67470 (the landed ?SetSlotLock@CEaxContext@@QAEJKJ@Z, inlined here)')
        t = t.replace('        if (index == 0 || index == 1) {\n            m_lastError = -1;\n            return -3;\n        }\n        SSkillRecord_00c67470* record = &m_skills[index];\n        if (value != record->m_property)\n            record->m_property = value;',
                      '        if (slot == 0 || slot == 1) {\n            m_lastError = -1;\n            return -3;\n        }\n        SEaxSlotRecord* record = &m_slots[slot];\n        if (lock != record->m_lock)\n            record->m_lock = lock;')
        t = t.replace('SEaxSlotRecord* record = &m_skills[index];\n        if (value != record->m_property)\n            record->m_property = value;', 'SEaxSlotRecord* record = &m_slots[slot];\n        if (lock != record->m_lock)\n            record->m_lock = lock;')
        _m = re.search(r'long SetSlotLock\(unsigned long slot, long lock\)[^\n]*\n    \{.*?\n    \}', t, re.S)
        if _m:
            _b = _m.group(0); _b2 = re.sub(r'\bindex\b', 'slot', _b); _b2 = re.sub(r'\bvalue\b', 'lock', _b2); _b2 = _b2.replace('m_property', 'm_lock').replace('m_skills', 'm_slots').replace('SSkillRecord_00c67470', 'SEaxSlotRecord')
            t = t.replace(_b, _b2)
        t = t.replace('long SetSkillGUID(long index, GUID guid);      // 0x00C674B0', 'long SetSlotLoadEffect(unsigned long slot, SGuid effect);   // 0x00C674B0 (the landed ?SetSlotLoadEffect@CEaxContext@@QAEJKUSGuid@@@Z)')
        t = t.replace('long SetSkillGUID(long index, SGuid guid);      // 0x00C674B0', 'long SetSlotLoadEffect(unsigned long slot, SGuid effect);   // 0x00C674B0 (the landed ?SetSlotLoadEffect@CEaxContext@@QAEJKUSGuid@@@Z)')
        t = t.replace('long SetSkillValue(long index, long value);    // 0x00C67720', 'long SetSlotVolume(unsigned long slot, long volume);   // 0x00C67720 (the landed ?SetSlotVolume@CEaxContext@@QAEJKJ@Z)')
        t = t.replace('long SetSkillFlag(long index, long flag);      // 0x00C67430', 'long SetSlotFlags(unsigned long slot, unsigned long flags);   // 0x00C67430 (the landed ?SetSlotFlags@CEaxContext@@QAEJKK@Z)')
        t = t.replace('SetSkillIndexProperty(index, properties.Property)', 'SetSlotLock(index, properties.Lock)').replace('SetSkillGUID(index, properties.Guid)', 'SetSlotLoadEffect(index, properties.Guid)')
        t = t.replace('SetSkillValue(index, properties.Value)', 'SetSlotVolume(index, properties.Volume)').replace('SetSkillFlag(index, properties.Flag)', 'SetSlotFlags(index, properties.Flags)')
        t = t.replace('    long Value;                                    // +0x10\n    long Property;                                 // +0x14\n    long Flag;                                     // +0x18', '    long Volume;                                   // +0x10\n    long Lock;                                     // +0x14\n    long Flags;                                    // +0x18')
        t = t.replace('    long          m_property;                      // +0x00', '    long          m_lock;                          // +0x00')
        t = t.replace('SEaxSlotRecord m_skills[1];', 'SEaxSlotRecord m_slots[1]; ')
        t = t.replace('skill record', 'slot record').replace('the class is the skill table named by address in the landed sibling 0x00C67470 (same `this` layout)', 'the class is the EAX context of the landed slot setters (same `this` layout)')
        t = t.replace('SetSkillGUID(long, SGuid) @ 0x00C674B0', 'SetSlotLoadEffect(unsigned long, SGuid) @ 0x00C674B0').replace('SetSkillValue(long, long) @ 0x00C67720', 'SetSlotVolume @ 0x00C67720').replace('SetSkillFlag(long, long) @ 0x00C67430', 'SetSlotFlags @ 0x00C67430')
        t = t.replace('CSkillTable::SetSkillIndexProperty(long, long) (0x00C67470, inlined here', 'CEaxContext::SetSlotLock (0x00C67470, inlined here')
        t = t.replace('//   CEaxContext::SetSlotProperty (the skill-table reading', '//   CSkillTable_00c67470::SetSkillPropertiesEx (the skill-table reading')
        he._w(p, t); print('ok', a, 'src')
    else: print('already', a, 'src')
    he.resub(a, 'tst', r'SetSkillPropertiesEx', 'SetSlotProperty')
    he.resub(a, 'tst', r'CSkillTable', 'CEaxContext')
