# h21_voice.py: naming wave 21, family voice -- DECISIONS_W20_RESOLVED item CPKVoiceParams: the 0x50-byte libpk voice play-parameter record
# gets one spelling, CPKVoiceParams (chunk 687), in its ctor 0x00C10A30 (was SNavigationRequest), its setters 0x00C20480 / 0x00C204A0 /
# 0x00C20420 (were CBrainOutput::SetAsPumpCombatActions stand-ins; float arguments as 0x00C62530 passes them: the sound's distance range
# minimum / maximum minus the listener distance -> SetMinDistance / SetMaxDistance, and 1.0f -> SetScale, descriptive), the callers
# 0x00C62530 / 0x00C621B0 / 0x00C37840 (CSoundPlayParams_00c37840) and the state rows 0x00C27C10 / 0x00C26680 (State).
# The table Play 0x00C13140 is called on: CSoundSystem+0x38 = the landed CSoundEventManager (ctor 0x00C11D00 stores vtable 0x012B2748;
# 0x00C13140 sits in its method range 0x00C10C90..0x00C13470 under the same library lock), so Play is ?Play@CSoundEventManager@@ -- the
# chunk-687 "CPKVoiceTable" (0x00C61CF0 / 0x00C61F60 / 0x00C61FB0: context+4 -> device+0x38) and the loop rows' CSoundSystem_00c3xxxx
# (owner->host->core+4->+0x38, 0x00C377E0) name the same object: respelled CSoundEventManager here (one class, one name).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_voice.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def P(a, txt):
    if a not in _X: he.put(a, 'src', txt)
def R(a, k, pat, rep):
    if a not in _X: he.resub(a, k, pat, rep)
# --- the ctor own row
P('00c10a30', '''// CPKVoiceParams::CPKVoiceParams() -- constructor of the 0x50-byte libpk voice play-parameter record: message-list owner, zero words, two 1.0f scales (volume, pitch), an empty event record, zero tail
// Leaf: ??0CPKVoiceParams@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10A30, length 57 bytes. No vtable.
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687): CPKVoiceParams, the record CAmbientEvent::Trigger 0x00C61D10, CAmbientTrigger::Play
//   0x00C62530, CAmbientChannel::Update 0x00C621B0 and the sound loop's Update 0x00C37840 build on the stack and hand to CSoundEventManager::Play
//   0x00C13140 (sound, 0, &params); its setters are 0x00C20480 / 0x00C204A0 / 0x00C20420. Was landed as SNavigationRequest (the manifest label
//   "CScriptModuleBase::CScriptModuleBase" is propagated from an unrelated body). Descriptive (libpk, no PDB); member names kept.
// Callees: CMessageListOwner::CMessageListOwner @ 0x00C203A0 (on +0), CNavigationLayer::CNavigationLayer
// @ 0x00C20390 (on +0x30) -- both landed (0x00C203A0, 0x00C20390), out of line.
// Shape: initialiser list in declaration order around the two member constructors; the integer zeros share
// edi, the two 1.0f scales share eax (`mov eax,3F800000h`), the word at +0x44 is `mov [esi+44h],di`.
// The same record is built inline on the stack by the landed 0x00C12D50 / 0x00C13310 and embedded at +0x24 of the 0x00C266E0 task
// object; callers 0x00C12EED, 0x00C61DAD.
struct CMessageListOwner {                    // 0x1c bytes
    unsigned char _body[0x1c];
    CMessageListOwner();                      // 0x00C203A0
};

struct CNavigationLayer {                     // 0x10 bytes
    void* m_pFirst;
    void* m_pSecond;
    void* m_pThird;
    unsigned char m_flag;
    CNavigationLayer();                       // 0x00C20390
};

struct CPKVoiceParams {                       // 0x50 bytes
    CMessageListOwner m_messages;             // +0x00
    long              m_1c;                   // +0x1c
    long              m_20;                   // +0x20
    float             m_scaleX;               // +0x24 (volume 1.0f)
    float             m_scaleY;               // +0x28 (pitch 1.0f)
    long              m_2c;                   // +0x2c
    CNavigationLayer  m_layer;                // +0x30
    long              m_40;                   // +0x40
    short             m_44;                   // +0x44
    long              m_48;                   // +0x48
    long              m_4c;                   // +0x4c
    CPKVoiceParams();
};

CPKVoiceParams::CPKVoiceParams()
    : m_1c(0), m_20(0), m_scaleX(1.0f), m_scaleY(1.0f), m_2c(0), m_40(0), m_44(0), m_48(0), m_4c(0)
{
}
''')
R('00c10a30', 'tst', r'\bSNavigationRequest\b', 'CPKVoiceParams')
# --- the setters' own rows
def setter(a, name, bit, callee, cname, what):
    P(a, '''// CPKVoiceParams::%s(float value) -- %s: store the value through the record's +8 slot (%s) and set flag bit %s of the record's first word
// Leaf: ?%s@CPKVoiceParams@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s (thiscall, `ret 4`).
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687): a setter of the libpk voice play-parameter record CPKVoiceParams (ctor 0x00C10A30);
//   CAmbientTrigger::Play 0x00C62530 passes %s (`fstp [esp]`: a float). Was landed as a CBrainOutput::SetAsPumpCombatActions stand-in
//   (propagated). Descriptive (libpk, no PDB): the slot callee %s is named by its address (the landed callers bind it under several
//   stand-in spellings).
struct CPKParamSlot_00c1bb80 {               // the record's +8 word
    void %s(float value);              // %s (thiscall: the slot in ecx, the value on the stack)
};

struct CPKVoiceParams {
    unsigned long         m_flags;           // +0x00
    long                  m_4;               // +0x04
    CPKParamSlot_00c1bb80 m_slot8;           // +0x08
    void %s(float value);
};

void CPKVoiceParams::%s(float value)
{
    m_slot8.%s(value);
    m_flags |= %s;
}
''' % (name, what, callee, bit, name, a.upper(), what, callee, cname, callee, name, name, cname, bit))
setter('00c20480', 'SetMinDistance', '4', '0x00C1BB80', 'Set_00c1bb80', "the sound's minimum distance (range minimum minus the listener distance)")
setter('00c204a0', 'SetMaxDistance', '8', '0x00C1BBA0', 'Set_00c1bba0', "the sound's maximum distance (range maximum minus the listener distance)")
setter('00c20420', 'SetScale', '0x200', '0x00C1BC70', 'Set_00c1bc70', 'a scale of 1.0f')
# --- the Play own row
P('00c13140', '''// CSoundEventManager::Play(long sound, long flags, const CPKVoiceParams* params) -- EH-framed: under a scoped lock over the library lock @ 0x0143BB14, the entry found out of line with (sound, flags, params, 0) (@ 0x00C12CD0) yields its +0xc word (the voice handle); -1 when nothing was found
// Leaf: ?Play@CSoundEventManager@@QAEJJJPBUCPKVoiceParams@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13140, oracle length 122 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; ...
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687): the play request CAmbientTrigger::Play 0x00C62530, CAmbientChannel::Update
//   0x00C621B0, the sound loop's Update 0x00C37840 and CAmbientEvent::Trigger 0x00C61D10 call with (sound, 0, &params) on the object at
//   CSoundSystem+0x38 -- the landed CSoundEventManager (ctor 0x00C11D00, vtable 0x012B2748; this body sits in its method range
//   0x00C10C90..0x00C13470 under the same library lock). The manifest label CTCHeroExperience::GetBlockParryReward is propagated.
//   Descriptive (libpk, no PDB); the lookup callee 0x00C12CD0 is named by its address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CPKVoiceParams;

struct CSoundEventEntry_00c13140 {
    unsigned char _pad[0xc];
    long m_handle;                         // +0xc
};

struct CSoundEventManager {
    CSoundEventEntry_00c13140* FindEntry_00c12cd0(long sound, long flags, const CPKVoiceParams* params, long mode);   // 0x00C12CD0
    long Play(long sound, long flags, const CPKVoiceParams* params);
};

long CSoundEventManager::Play(long sound, long flags, const CPKVoiceParams* params)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CSoundEventEntry_00c13140* entry = FindEntry_00c12cd0(sound, flags, params, 0);
    long handle = entry ? entry->m_handle : -1;
    return handle;
}
''')
# --- the callers: the params record, the table and the setters
for a in ('00c62530', '00c621b0', '00c37840'):
    for k in ('src', 'tst'):
        R(a, k, r'CSoundPlayParams_00c37840(?![A-Za-z0-9_])', 'CPKVoiceParams')
        R(a, k, r'CSoundSystem_00c37840(?![A-Za-z0-9_])', 'CSoundEventManager')
        R(a, k, r'\bPlay_00c13140\b', 'Play')
        R(a, k, r'\bSet2_00c20480\b', 'SetMinDistance')
        R(a, k, r'\bSet3_00c204a0\b', 'SetMaxDistance')
        R(a, k, r'\bSet9_00c20420\b', 'SetScale')
NOTE = '// Naming (naming wave 21, DECISIONS_W20_RESOLVED): the play-parameter record is CPKVoiceParams (ctor 0x00C10A30; setters SetMinDistance 0x00C20480 / SetMaxDistance 0x00C204A0 / SetScale 0x00C20420, descriptive) and the table Play 0x00C13140 is called on is the landed CSoundEventManager (CSoundSystem+0x38, vtable 0x012B2748).'
def note(a, k='src', txt=NOTE):
    if a in _X: return
    p, rel, t = he._rw(a, k)
    if txt in t: print('already', a, k); return
    m = re.search(r'^// Leaf: [^\n]*\n', t, re.M)
    i = m.end() if m else (t.index('\n') + 1)
    he._w(p, t[:i] + txt + '\n' + t[i:]); print('ok', a, k)
for a in ('00c62530', '00c621b0', '00c37840'): note(a)
# --- the sound loop rows: one class CSoundLoop_00c377f0, the event manager accessor
for a in ('00c37840', '00c377e0', '00c377f0', '00c37930'):
    for k in ('src', 'tst'):
        R(a, k, r'CSoundSystem_00c37(7f0|840|930)(?![A-Za-z0-9_])', 'CSoundEventManager')
        R(a, k, r'CSoundLoop_00c37(840|930)(?![A-Za-z0-9_])', 'CSoundLoop_00c377f0')
        R(a, k, r'\bGetSoundSystem_00c377e0\b', 'GetEventManager')
        R(a, k, r'\bGetSoundSystem\b', 'GetEventManager')
        R(a, k, r'\bDropFinishedVoice_00c377f0\b', 'DropFinishedVoice')
LNOTE = '// Naming (naming wave 21): the record is one class, CSoundLoop_00c377f0 (DropFinishedVoice 0x00C377F0, GetEventManager 0x00C377E0, Update 0x00C37840, dtor 0x00C37930), and the object its accessor reaches (owner -> host -> core+4 -> +0x38) is the landed CSoundEventManager (CSoundSystem+0x38), not a second sound-system class.'
for a in ('00c377e0', '00c377f0', '00c37930'): note(a, 'src', LNOTE)
# --- the CPKVoiceTable rows (0x00C61CF0 own row and its two callers)
for a in ('00c61cf0', '00c61f60', '00c61fb0'):
    for k in ('src', 'tst'):
        R(a, k, r'\bclass CPKVoiceTable\b', 'struct CSoundEventManager')
        R(a, k, r'CPKVoiceTable(?![A-Za-z0-9_])', 'CSoundEventManager')
        R(a, k, r'\bGetTable\b', 'GetEventManager')
TNOTE = '// Naming (naming wave 21): the "voice table" (context+4 -> device+0x38) is CSoundSystem+0x38, the landed CSoundEventManager (ctor 0x00C11D00, vtable 0x012B2748; Play 0x00C13140): CPKVoiceTable / GetTable -> CSoundEventManager / GetEventManager (one class, one name).'
for a in ('00c61cf0', '00c61f60', '00c61fb0'): note(a, 'src', TNOTE)
# --- the state rows
for a in ('00c27c10', '00c26680'):
    R(a, 'src', r'\bstruct State\b', 'struct CPKVoiceParams')
    R(a, 'src', r'\bState::', 'CPKVoiceParams::')
    R(a, 'src', r'\bState\b', 'CPKVoiceParams')
    R(a, 'tst', r'\bState\b', 'CPKVoiceParams')
a = '00c27c10'
if a not in _X:
    he.edit(a, 'src', 'void Set2_00c20480(unsigned);void Set3_00c204a0(unsigned);', 'void SetMinDistance(float);void SetMaxDistance(float);')
    he.edit(a, 'src', 'void Set9_00c20420(unsigned);', 'void SetScale(float);')
    he.edit(a, 'src', 'if(s->flags0&4)Set2_00c20480(s->values[3]);if(s->flags0&8)Set3_00c204a0(s->values[4]);', 'if(s->flags0&4)SetMinDistance(*(const float*)&s->values[3]);if(s->flags0&8)SetMaxDistance(*(const float*)&s->values[4]);')
    he.resub(a, 'src', r'Set9_00c20420\(([^)]*)\)', r'SetScale(*(const float*)&\1)')
    he.edit(a, 'src', '// Layout: retail receiver fields', '// Naming (naming wave 21): the receiver is the libpk CPKVoiceParams record (ctor 0x00C10A30; SetMinDistance 0x00C20480 / SetMaxDistance 0x00C204A0 / SetScale 0x00C20420 take floats: the stored words are passed as their bit patterns); the other setters keep their address names.\n// Layout: retail receiver fields')
a = '00c26680'
if a not in _X:
    he.edit(a, 'src', '    void Set2_00c20480(unsigned int value);     // 0x00C20480\n    void Set3_00c204a0(unsigned int value);     // 0x00C204A0\n', '    void SetMinDistance(float value);           // 0x00C20480 (naming wave 21: CPKVoiceParams, float argument as 0x00C62530 passes it)\n    void SetMaxDistance(float value);           // 0x00C204A0\n')
    he.edit(a, 'src', '    m_state24.Set2_00c20480(a);\n    m_state24.Set3_00c204a0(b);\n', '    m_state24.SetMinDistance(*(const float*)&a);\n    m_state24.SetMaxDistance(*(const float*)&b);\n')
    he.resub(a, 'src', r'\bSet2_00c20480\b', 'SetMinDistance'); he.resub(a, 'src', r'\bSet3_00c204a0\b', 'SetMaxDistance')
    he.resub(a, 'tst', r'\bSet2_00c20480\b', 'SetMinDistance'); he.resub(a, 'tst', r'\bSet3_00c204a0\b', 'SetMaxDistance')
# the own row 0x00C61CF0 and its caller 0x00C61FB0: the emitter is the wave-20 CAmbientEmitter spelling (0x00C61F60), the class key U as the 20 landed bindings
for a in ('00c61cf0', '00c61fb0'):
    for k in ('src', 'tst'): R(a, k, r'CPKEmitter(?![A-Za-z0-9_])', 'CAmbientEmitter')
R('00c61cf0', 'src', r'// Leaf: \?GetEventManager@CPKVoiceHandle@@QAEPAVCSoundEventManager@@', '// Leaf: ?GetEventManager@CPKVoiceHandle@@QAEPAUCSoundEventManager@@')
# the 0x00C204A0 test relied on the source object for its free-function stand-in: a self-contained test of the new model
if '00c204a0' not in _X: he.put('00c204a0', 'tst', '''// Behaviour test for 0x00C204A0 CPKVoiceParams::SetMaxDistance(float): the value goes through the record's +8 slot (0x00C1BBA0) and flag bit 3 is set (OR, not assign)
#include <stdio.h>
static float g_last = 0.0f; static int g_called = 0;
struct CPKParamSlot_00c1bb80 { void Set_00c1bba0(float value); };
void CPKParamSlot_00c1bb80::Set_00c1bba0(float value) { g_last = value; ++g_called; }
struct CPKVoiceParams {
    unsigned long         m_flags;
    long                  m_4;
    CPKParamSlot_00c1bb80 m_slot8;
    void SetMaxDistance(float value);
};
void CPKVoiceParams::SetMaxDistance(float value) { m_slot8.Set_00c1bba0(value); m_flags |= 8; }
int main()
{
    CPKVoiceParams p; p.m_flags = 1; p.m_4 = 0;
    p.SetMaxDistance(12.5f);
    if (g_called == 1 && g_last == 12.5f && p.m_flags == 9) { printf("00c204a0_TEST PASS\\n"); return 0; }
    printf("00c204a0_TEST FAIL\\n"); return 1;
}
''')

# 0x00C20440 has one name: SetFilter_00c20440(long), as the landed callers 0x00C37840 / 0x00C621B0 spell it (the state row's Set5_00c20440(unsigned) was a second name)
if '00c27c10' not in _X:
    for k in ('src', 'tst'):
        he.resub('00c27c10', k, r'void Set5_00c20440\(unsigned\);', 'void SetFilter_00c20440(long);')
        he.resub('00c27c10', k, r'\bSet5_00c20440\(s->', 'SetFilter_00c20440((long)s->')
    he.resub('00c27c10', 'tst', r'void CPKVoiceParams::Set5_00c20440\(unsigned n\)\{seen\[5\]=n;', 'void CPKVoiceParams::SetFilter_00c20440(long n){seen[5]=(unsigned)n;')
# 0x00C44D30 ClearEvents: the emitter is the wave-20 CAmbientEmitter (0x00C44A90 GetVolume / 0x00C61F60); its call of CPKVoiceHandle::Release
# 0x00C61FB0 follows the own row's respelled CAmbientEmitter* parameter (one address, one name)
if '00c44d30' not in _X:
    for k in ('src', 'tst'): R('00c44d30', k, r'\bCPKEmitter\b', 'CAmbientEmitter')
