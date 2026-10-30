# sub-wave chunks 586-593 (hand): HE_OV=ov_chunk
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub, recode
# ---- 0x00C0DEA0 / 0x00C0DEC0 / 0x00C0DF10 CString::operator<< (coordinator decision, chunk 590; the defining rows emit
#      ??6CString@@QAIAAV0@ABV0@@Z / PBD / K, CBankProcessor::AddDriver 0x00C2E2B0 already calls them so) and CLoadLog::AddMessage
#      0x00C307E0 (?AddMessage@CLoadLog@@QAEAAVCString@@XZ, as the defining row 0x00C307E0 and 0x00C2E2B0): the CBankProcessor rows'
#      CDebugStream stand-in is the libpk CString.
DS = '''struct CDebugStream {                        // libpk message text
    char* m_pBuffer;
    CDebugStream& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};'''
CS = '''class CString {                              // libpk message text (0x00C0DEA0 / 0x00C0DEC0 / 0x00C0DF10 = CString::operator<<)
public:
    char* m_pchData;
    CString& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CString& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};'''
for a in ('00c2e190', '00c2e4d0', '00c2e5a0'):
    edit(a, 'src', DS, CS)
    edit(a, 'src', '''    CDebugStream& AddMessage();              // 0x00C307E0''', '''    CString& AddMessage();                   // 0x00C307E0 ?AddMessage@CLoadLog@@QAEAAVCString@@XZ''')
edit('00c2e7a0', 'src', '''    operator const char*() const { return m_pchData; }
};

struct CDebugStream {                        // libpk message text
    char* m_pBuffer;
    CDebugStream& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(const CString& text);    // 0x00C0DEA0
    CDebugStream& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};''', '''    operator const char*() const { return m_pchData; }
    CString& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CString& __fastcall operator<<(const CString& text);    // 0x00C0DEA0
    CString& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};''')
edit('00c2e7a0', 'src', '''    CDebugStream& AddMessage();              // 0x00C307E0''', '''    CString& AddMessage();                   // 0x00C307E0 ?AddMessage@CLoadLog@@QAEAAVCString@@XZ''')
edit('00c2e7a0', 'src', '''CDebugStream::operator<< 0x00C0DEC0''', '''CString::operator<< 0x00C0DEC0''')
# ---- CAudioSound / CAudioObject (chunk 592): one libpk object (vtable 0x012B5A88, no RTTI / PDB); its own member rows 0x00C43xxx and
#      CVoiceManager::Allocate / CVtable_012B5870::SetDefaultStateCallback spell it `class CAudioObject` (V): the voice rows' `struct
#      CAudioSound` parameter spelling becomes CAudioObject (the 0x00C5B300 row already casts one to the other).
import re
from he import _rw, _w
def rename_class(a, old, new, k='src', keep=False):   # keep: identifier only (class key unchanged)
    p, rel, t = _rw(a, k)
    out = []
    for line in t.split('\n'):
        i = line.find('//')
        code, com = (line, '') if i < 0 else (line[:i], line[i:])
        if line.lstrip().startswith('//'):
            out.append(line); continue
        if not keep:
            code = re.sub(r'\bstruct %s\s*\{' % old, 'class %s { public:' % new, code)
            code = re.sub(r'\bstruct %s\b' % old, 'class %s' % new, code)
        code = re.sub(r'\b%s\b' % old, new, code)
        out.append(code + com)
    n = '\n'.join(out)
    if n == t: print('already', a, k); return
    _w(p, n); print('ok', a, k)
edit('00c5b300', 'src', '''class CAudioObject {                                    // the same libpk object under its landed member spelling
public:
    void ReportQueueInfo();                             // 0x00C432F0
};

struct CAudioSound {
    unsigned char  _pad00[0x10];''', '''class CAudioObject {                                    // the libpk audio object (vtable 0x012B5A88; the voice rows' CAudioSound)
public:
    unsigned char  _pad00[0x10];''')
edit('00c5b300', 'src', '''    bool IsLooping() const { return (m_flags >> 8) & 1; }
};''', '''    bool IsLooping() const { return (m_flags >> 8) & 1; }
    void ReportQueueInfo();                             // 0x00C432F0
};''')
edit('00c5b300', 'src', '''    ((CAudioObject*)sound)->ReportQueueInfo();''', '''    sound->ReportQueueInfo();''')
for a in ('00c5b300', '00c72fe0', '00c59bc0', '00c5b250', '00c72930', '00c5a1d0', '00c72af0', '00c59720'):
    rename_class(a, 'CAudioSound', 'CAudioObject')
edit('00c5b300', 'src', '// Leaf: ?AdvanceBuffer@CSoftwareVoice@@QAEXPAUCAudioSound@@@Z', '// Leaf: ?AdvanceBuffer@CSoftwareVoice@@QAEXPAVCAudioObject@@@Z')
edit('00c5a1d0', 'src', '// Leaf: ?Initialise@CVtable_012B5C60@@UAEJPAUCAudioSound@@@Z', '// Leaf: ?Initialise@CVtable_012B5C60@@UAEJPAVCAudioObject@@@Z')
# the 0x00C5A1D0 test links the row and defines the base slot 1: same spelling there (the old one left LINK_FAIL)
edit('00c5a1d0', 'tst', 'struct CAudioSound;', 'class CAudioObject;')
resub('00c5a1d0', 'tst', r'\bCAudioSound\b', 'CAudioObject')
edit('00c5a1d0', 'src', '// CVtable_012B5C60::Initialise(CAudioSound* sound)', '// CVtable_012B5C60::Initialise(CAudioObject* sound)')
edit('00c5a1d0', 'src', '?Initialise@CVtable_012B5C10@@UAEJPAUCAudioSound@@@Z). Descriptive', '?Initialise@CVtable_012B5C10@@UAEJPAVCAudioObject@@@Z). Descriptive')
edit('00c59bc0', 'src', '// Leaf: ?BeginSound@CAudioSource@@QAEXPAUCAudioSound@@@Z', '// Leaf: ?BeginSound@CAudioSource@@QAEXPAVCAudioObject@@@Z')
edit('00c59720', 'src', '// Leaf: ?Play@CCodaVoice@@UAEJPAUCAudioSound@@@Z', '// Leaf: ?Play@CCodaVoice@@UAEJPAVCAudioObject@@@Z')
edit('00c5b250', 'src', '// Leaf: ?StartSound@CSoftwareVoice@@UAEJPAUCAudioSound@@_J@Z', '// Leaf: ?StartSound@CSoftwareVoice@@UAEJPAVCAudioObject@@_J@Z')
# ---- 0x00C596E0 ?Initialise@CVtable_012B5C10@@UAEJPAVCAudioObject@@@Z (chunk 586): slot 1 of the libpk voice base vtable 0x012B5C10;
#      the derived slot-1 overrides CVtable_012B5C60::Initialise 0x00C5A1D0 (`jmp 0x00C596E0`) and CAudioSource::Initialise 0x00C72930
#      call it as the base Initialise (both already bind this spelling); the CEnvironmentSpellEffectInterface::ClearScreenFilter label
#      was propagated. The base's vptr sits at +0 (the fields keep their offsets).
recode('00c596e0', 'src', 'struct CScreenFilterSource_00c596e0 {', '''class CAudioObject { public:                           // the libpk audio object (the sound the voice plays)
    unsigned char _pad[0xf4];
    void*         m_pRecord;                       // +0xf4
};

class CVtable_012B5C10 { public:                      // the libpk voice base, vtable 0x012B5C10 (no RTTI / PDB)
    virtual ~CVtable_012B5C10();                   // slot 0 (0x00C3F3D0)
    virtual long Initialise(CAudioObject* sound);  // slot 1
    unsigned char _pad04[0x8];
    long          m_filterA;                       // +0x0c
    unsigned char _pad10[0x1c];
    long          m_filterB;                       // +0x2c
    unsigned char _pad30[0x20];
    void*         m_pFilterRecord;                 // +0x50
};

long CVtable_012B5C10::Initialise(CAudioObject* sound)
{
    m_filterB = 0;
    m_filterA = 0;
    if (sound)
        m_pFilterRecord = sound->m_pRecord;
    else
        m_pFilterRecord = 0;
    return 0;
}
''', line1='// CVtable_012B5C10::Initialise(CAudioObject* sound) -- the voice base slot 1: clear the words at +0x2c and +0xc and take the sound\'s record at +0xf4 (or null) into +0x50; returns 0',
    leaf='?Initialise@CVtable_012B5C10@@UAEJPAVCAudioObject@@@Z',
    extra_comment='// Identity (naming wave 11): slot 1 of vtable 0x012B5C10; CVtable_012B5C60::Initialise 0x00C5A1D0 tail-jumps here and CAudioSource::Initialise 0x00C72930 calls it first (the ClearScreenFilter label was propagated). Descriptive libpk names.')
# ---- 0x0143E514 / 0x0143CF08 the ImeUi allocator callbacks (chunk 591): DirectX SDK ImeUi.cpp ImeUiCallback_Malloc / ImeUiCallback_Free,
#      the spelling the ImeUi rows 0x00CB2310 / 0x00CB2670 bind; CIME's constructor 0x00405650 stores Callback_malloc / Callback_free there.
edit('00405650', 'src', '''extern void* (FABLE_FASTCALL *g_ImeMalloc)(unsigned int); //0143E514
extern void (FABLE_FASTCALL *g_ImeFree)(void*); //0143CF08''', '''extern void* (FABLE_FASTCALL *ImeUiCallback_Malloc)(unsigned int); //0143E514 (DirectX SDK ImeUi.cpp; as 0x00CB2310 / 0x00CB2670)
extern void (FABLE_FASTCALL *ImeUiCallback_Free)(void*); //0143CF08 (DirectX SDK ImeUi.cpp; as 0x00CB2310)''')
edit('00405650', 'src', '''  g_ImeMalloc=Callback_malloc;
  g_ImeFree=Callback_free;''', '''  ImeUiCallback_Malloc=Callback_malloc;
  ImeUiCallback_Free=Callback_free;''')
# ---- 0x00CE4B70 (chunk 590 recheck) = ?BoastHobbeCavePerpertratorKilled@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z: Ego_r.pdb
#      member order UseNoHealthPotions (retail 0x00CE4600) < DarkwoodSaveAllTraders < DarkwoodPickpocketedAllTraders < MadBomberNoBombsExplode
#      < HobbeCavePerpertratorKilled < HobbeCaveInnocentSacrificed < HobbeCaveBoyUnharmed < BanditCampKillNoBandits < BreakSiegeNoAssistance
#      (retail 0x00CE5050): retail has exactly seven functions between (0x00CE4770 / 48A0 / 4A40 / 4B70 / 4C50 / 4DF0 / 4F20, the same
#      short/long size pattern as the PDB), and the body tests the master flag HobbeCavePerpertratorKilled. The six global-scope
#      CHeroBoastsScript rows of this model nest in NScript and call the retail CScriptBase::IsActiveThreadTerminating (0x00CB7940,
#      Ego_r ?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ, as the landed siblings 0x00CE40D0 / 0x00CE4600 bind it).
def boast(a, meth, new=None):
    new = new or meth
    edit(a, 'src', '''#include "fable_string.h"
''', '''#include "fable_string.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"                  // retail CScriptBase (0x40 bytes, vtable 0x012C1648)
''')
    edit(a, 'src', '''struct CHeroBoastsScript {
 unsigned char ScriptBaseState[0x40]; GameMethods* Game; SunnyvaleMasterData* MasterScriptData;
 bool IsActiveThreadTerminating()const; //00CB7940: active thread+2C, thread terminating byte+5
 void %s(CCharString quest);''' % meth, '''namespace NScript {
struct CHeroBoastsScript : public CScriptBase {  // IsActiveThreadTerminating 0x00CB7940 is CScriptBase's (active thread+2C, terminating byte+5)
 GameMethods* Game; SunnyvaleMasterData* MasterScriptData;
 void %s(CCharString quest);''' % new)
    edit(a, 'src', '''FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);''', '''}   // namespace NScript
using NScript::CHeroBoastsScript;
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);''')
    edit(a, 'src', 'void CHeroBoastsScript::%s(CCharString quest){' % meth, 'void NScript::CHeroBoastsScript::%s(CCharString quest){' % new)
    edit(a, 'src', '// Leaf: %s\n' % meth, '// Leaf: ?%s@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z\n' % new)
boast('00ce4b70', 'BoastHobbeCaveKillPerpetrator', 'BoastHobbeCavePerpertratorKilled')
edit('00ce4b70', 'src', '''// CHeroBoastsScript::BoastHobbeCaveKillPerpetrator(CCharString) -- wait for quest/boast outcome''', '''// NScript::CHeroBoastsScript::BoastHobbeCavePerpertratorKilled(CCharString) -- wait for quest/boast outcome
// Identity (naming wave 11): Ego_r.pdb member order (seven retail functions between BoastUseNoHealthPotions 0x00CE4600 and BoastBreakSiegeNoAssistance 0x00CE5050, fifth = HobbeCavePerpertratorKilled) + the master flag the body tests.''')
for a, m in (('00ce5620', 'BoastHobbeContestKillMoreHobbes'), ('00ce56e0', 'BoastGraveyardKillAllInnocents'), ('00ce5e10', 'BoastBountyHuntDecapitation'),
             ('00ce6a40', 'BoastFireHeartFreeAllPrisoners'), ('00ce6b10', 'BoastFireHeartKillAllPrisoners')):
    boast(a, m)
# ---- 0x00C355E0 (chunk 589: two spellings): the defining row's ??0CWorkerThread_00c355e0@@QAE@PAXPBUSThreadParams_00c355e0@@@Z (also
#      bound by CTimerHost::CTimerHost 0x00C25B10) wins; the CAsyncDecoder ctor 0x00C48640 spells the same worker / parameter record so.
edit('00c48640', 'src', '''struct SThreadParams_00c48640 {            // 8 bytes: stack size and flags
    long m_stackSize;
    long m_flags;
};

struct CAsyncDecoder_00c48640;
struct CWorkerThread_00c48640 {            // 12 bytes
    unsigned char _body[0xc];
    CWorkerThread_00c48640(CAsyncDecoder_00c48640* owner, const SThreadParams_00c48640* params);   // 0x00C355E0
};''', '''struct SThreadParams_00c355e0 {            // 8 bytes: stack size and priority (the 0x00C355E0 row's record)
    long m_stackSize;
    long m_flags;
};

struct CWorkerThread_00c355e0 {            // 12 bytes (the 0x00C355E0 row's spelling, as CTimerHost 0x00C25B10)
    unsigned char _body[0xc];
    CWorkerThread_00c355e0(void* owner, const SThreadParams_00c355e0* params);   // 0x00C355E0
};''')
edit('00c48640', 'src', '''    CWorkerThread_00c48640* m_pThread;     // +0x38''', '''    CWorkerThread_00c355e0* m_pThread;     // +0x38''')
edit('00c48640', 'src', '''    SThreadParams_00c48640 params;''', '''    SThreadParams_00c355e0 params;''')
edit('00c48640', 'src', '''    m_pThread = new CWorkerThread_00c48640(this, &params);''', '''    m_pThread = new CWorkerThread_00c355e0(this, &params);''')
# ---- 0x00C39BA0..0x00C39CE0 the CActivationReceptor part destructors (chunk 587): one name per address -- the owner's spelling
#      (~CActivationReceptor_00c38b20 0x00C38B20 and its ctor 0x00C39130 name them by the part's offset in the receptor) replaces the
#      rows' own CPair_<addr> placeholders.
for a, n in (('00c39ba0', 'CReceptorBase_00c38b20'), ('00c39bf0', 'CPart2C_00c38b20'), ('00c39c40', 'CPart60_00c38b20'),
             ('00c39c90', 'CPart94_00c38b20'), ('00c39ce0', 'CPartCC_00c38b20')):
    rename_class(a, 'CPair_%s_Inner' % a, n + '_Inner', keep=True)   # struct kept: the parts are aggregates (no parameter
    rename_class(a, 'CPair_%s' % a, n, keep=True)                    # mangles them), a class key would make ~Inner private
    edit(a, 'src', '// CPair_%s::~CPair_%s() -- EH-framed' % (a, a), '// %s::~%s() -- EH-framed' % (n, n))
# ---- 0x00C54BD0 (chunk 592 "misnamed Reset"): its callee 0x00C5CE40 is CAudioRingBuffer::Clear (zeroes +4..+0x1C of the ring, vtable
#      0x012B8FC4; CSoftwareVoice::AdvanceBuffer 0x00C5B300 binds ?Clear@CAudioRingBuffer@@QAEXXZ), not CGameflowAssistanceScript::Init;
#      the owner is the voice layout of that row (ring +0xB4, current buffer +0x0C). No caller / vtable reference: the method name stays.
edit('00c54bd0', 'src', '''struct CGameflowAssistanceScript {
    void Init();                                   // 0x00C5CE40
};

struct CAssistanceOwner_00c54bd0 {
    unsigned char              _pad00[0xc];
    long                       m_c;                // +0x0c
    unsigned char              _pad10[0xa4];
    CGameflowAssistanceScript  m_script;           // +0xb4
    void Reset();
};

void CAssistanceOwner_00c54bd0::Reset()
{
    m_script.Init();
    m_c = 0;
}''', '''class CAudioRingBuffer { public:                    // vtable 0x012B8FC4 (as CSoftwareVoice::AdvanceBuffer 0x00C5B300)
    void Clear();                                  // 0x00C5CE40 ?Clear@CAudioRingBuffer@@QAEXXZ
};

struct CVoice_00c54bd0 {                           // the libpk voice layout of CSoftwareVoice::AdvanceBuffer 0x00C5B300
    unsigned char              _pad00[0xc];
    long                       m_pBuffer;          // +0x0c
    unsigned char              _pad10[0xa4];
    CAudioRingBuffer           m_ring;             // +0xb4
    void Reset();
};

void CVoice_00c54bd0::Reset()
{
    m_ring.Clear();
    m_pBuffer = 0;
}''')
edit('00c54bd0', 'src', '''// CAssistanceOwner_00c54bd0::Reset() -- re-initialise the assistance script member at +0xb4 (0x00C5CE40) and clear the dword at +0xc''', '''// CVoice_00c54bd0::Reset() -- clear the voice's ring buffer at +0xb4 (CAudioRingBuffer::Clear 0x00C5CE40) and its current buffer at +0xc''')
edit('00c54bd0', 'src', '''// Callees: 0x00C5CE40 (labelled CGameflowAssistanceScript::Init, thiscall on the member).''', '''// Callees: CAudioRingBuffer::Clear 0x00C5CE40 (the CGameflowAssistanceScript::Init label was propagated).''')
# ---- CEngineScene render family (chunk 588: 0x00B97B60 / 0x00B92790 / 0x00B91760 / 0x00B93F40 / 0x00B94060): Ego_r.pdb spells the
#      layer EEngineRenderLayer everywhere (RenderScene / RenderSceneShadowBuffer / RenderSceneAndCheckVisibility / RenderEntireScene,
#      the grid's RenderCellClipped* / RenderCellAll, the private RenderPrimitiveList / RenderPrimitiveListToShadowBuffer and private
#      CEngineSceneGrid::GetCellRenderMask) and the per-layer stats as std::map<EEngineRenderLayer, CEngineSceneRenderStats>
#      (RenderSceneOcclusion 0x00B94180 already binds all of these spellings). Source order RenderScene 0x00B93F40 / ShadowBuffer
#      0x00B94060 / Occlusion 0x00B94180 / AndCheckVisibility 0x00B942A0 / RenderEntireScene 0x00B943F0 = Ego_r order.
MAPDECL = '''namespace std {
template <class T> struct less;
template <class T> class allocator;
template <class T1, class T2> struct pair;
template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    void* m_header; unsigned long m_count;
    T& operator[](const Key& key);              // 0x00B97B60 (stand-in body below)
    T& FindOrInsert(Key key);                   // descriptive: the tree lookup/insert the real body performs
};
}
typedef std::map<EEngineRenderLayer, CEngineSceneRenderStats> CRenderStatsMap;'''
OLDMAP = '''struct CRenderStatsMap {
    void* m_header; unsigned long m_count;
    CEngineSceneRenderStats& operator[](const long& key);   // 0x00B97B60 (stand-in body below)
    CEngineSceneRenderStats& FindOrInsert(long key);         // descriptive: the tree lookup/insert the real body performs
};'''
def scene(a, old, new, leaf, extra=()):
    edit(a, 'src', 'class CEngineSceneRenderStats;\n', 'class CEngineSceneRenderStats;\nenum EEngineRenderLayer { };\nenum EEngineOcclusionLayer { };\n')
    edit(a, 'src', OLDMAP, MAPDECL)
    p, rel, t = _rw(a, 'src')
    t2 = re.sub(r'CEngineSceneRenderStats& CRenderStatsMap::operator\[\]\(const long& key\)\n\{\n(.*?)\n\}',
                lambda m: 'namespace std {\ntemplate <> CEngineSceneRenderStats& map<EEngineRenderLayer, CEngineSceneRenderStats>::operator[](const EEngineRenderLayer& key)\n{\n%s\n}\n}' % m.group(1),
                t, count=1, flags=re.S)
    for o, n in extra: t2 = t2.replace(o, n)
    t2 = t2.replace('void %s(long layer' % old, 'void %s(EEngineRenderLayer layer' % new).replace('void CEngineScene::%s(long layer' % old, 'void CEngineScene::%s(EEngineRenderLayer layer' % new)
    if '\npublic:\n    void %s(EEngineRenderLayer layer' % new not in t2:
        t2 = t2.replace('\n    void %s(EEngineRenderLayer layer' % new, '\npublic:\n    void %s(EEngineRenderLayer layer' % new, 1)
    L = t2.split('\n')
    lf = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if lf: L[lf[0]] = '// Leaf: ' + leaf
    else: L.insert(1, '// Leaf: ' + leaf)
    if L[0].startswith('// CEngineScene::%s(' % old): L[0] = L[0].replace('CEngineScene::%s(' % old, 'CEngineScene::%s(' % new, 1)
    t2 = '\n'.join(L)
    if t2 != t: _w(p, t2); print('ok', a, 'scene')
    else: print('already', a, 'scene')
RPL = ('''    void RenderPrimitiveList(CEngineInternalPrimitiveBase* list, long layer, const CEngineCamera& camera,''',
       '''private:
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* list, EEngineRenderLayer layer, const CEngineCamera& camera,''')
scene('00b93f40', 'RenderLayerCells', 'RenderScene', '?RenderScene@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@@Z', (RPL,
      ('void RenderCellClipped(unsigned long x, unsigned long y, long layer,', 'void RenderCellClipped(unsigned long x, unsigned long y, EEngineRenderLayer layer,'),
      ('map<long, CEngineSceneRenderStats>::operator[](const long&) @ 0x00B97B60', 'map<EEngineRenderLayer, CEngineSceneRenderStats>::operator[] @ 0x00B97B60'),
      ('Only RenderLayerCells is graded.', 'Only RenderScene is graded.')))
scene('00b94060', 'RenderLayerCellsShadow', 'RenderSceneShadowBuffer', '?RenderSceneShadowBuffer@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCShadowClipVolume@@@Z', (
      ('''    void RenderPrimitiveList(CEngineInternalPrimitiveBase* list, long layer, const CShadowClipVolume& volume,''',
       '''private:
    void RenderPrimitiveListToShadowBuffer(CEngineInternalPrimitiveBase* list, EEngineRenderLayer layer, const CShadowClipVolume& volume,'''),
      ('    RenderPrimitiveList(PrimitiveListA, layer, volume, stats);\n    RenderPrimitiveList(PrimitiveListB, layer, volume, stats);',
       '    RenderPrimitiveListToShadowBuffer(PrimitiveListA, layer, volume, stats);\n    RenderPrimitiveListToShadowBuffer(PrimitiveListB, layer, volume, stats);'),
      ('void RenderCellClippedShadowBuffer(unsigned long x, unsigned long y, long layer,', 'void RenderCellClippedShadowBuffer(unsigned long x, unsigned long y, EEngineRenderLayer layer,'),
      ('Only RenderLayerCellsShadow is graded.', 'Only RenderSceneShadowBuffer is graded.')))
scene('00b942a0', 'RenderSceneAndCheckVisibility', 'RenderSceneAndCheckVisibility', '?RenderSceneAndCheckVisibility@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@W4EEngineOcclusionLayer@@_N@Z', (RPL,
      ('bool RenderCellClippedAndCheckVisibility(unsigned long x, unsigned long y, long layer,', 'bool RenderCellClippedAndCheckVisibility(unsigned long x, unsigned long y, EEngineRenderLayer layer,'),
      ('void RenderCellAndCheck(long layer,', 'void RenderCellAndCheck(EEngineRenderLayer layer,'),
      ('const CEngineCamera& camera, long occlusion, bool flag)', 'const CEngineCamera& camera, EEngineOcclusionLayer occlusion, bool flag)'),
      ('long occlusion, bool flag, bool& visible);', 'EEngineOcclusionLayer occlusion, bool flag, bool& visible);'),
      ('                            long occlusion, bool flag)', '                            EEngineOcclusionLayer occlusion, bool flag)')))
scene('00b943f0', 'RenderEntireScene', 'RenderEntireScene', '?RenderEntireScene@CEngineScene@@QAEXW4EEngineRenderLayer@@@Z', (
      ('void RenderCellAll(unsigned long x, unsigned long y, long layer,', 'void RenderCellAll(unsigned long x, unsigned long y, EEngineRenderLayer layer,'),))
GCM = ('    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);   // 0x00B91DC0',
       'private:\n    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);   // 0x00B91DC0 ?GetCellRenderMask@CEngineSceneGrid@@AAEKKK@Z (Ego_r: private)\npublic:')
edit('00b92500', 'src', GCM[0], GCM[1])
edit_all('00b92500', 'src', 'RenderCellClipped(unsigned long x, unsigned long y, long layer,', 'RenderCellClipped(unsigned long x, unsigned long y, EEngineRenderLayer layer,', 2)
edit('00b92500', 'src', 'class CEngineSceneRenderStats;\n', 'class CEngineSceneRenderStats;\nenum EEngineRenderLayer { };\n')
edit('00b92500', 'src', '// Leaf: RenderCellClipped\n', '// Leaf: ?RenderCellClipped@CEngineSceneGrid@@QAEXKKW4EEngineRenderLayer@@ABVCEngineCamera@@_NAAVCEngineSceneRenderStats@@@Z\n')
edit('00b92790', 'src', GCM[0], GCM[1])
edit_all('00b92790', 'src', 'RenderCellClippedOcclusion(unsigned long x, unsigned long y, long layer,', 'RenderCellClippedOcclusion(unsigned long x, unsigned long y, EEngineRenderLayer layer,', 2)
edit('00b92790', 'src', 'class CEngineSceneRenderStats;\n', 'class CEngineSceneRenderStats;\nenum EEngineRenderLayer { };\n')
edit('00b92790', 'src', '// Leaf: RenderCellClippedOcclusion\n', '// Leaf: ?RenderCellClippedOcclusion@CEngineSceneGrid@@QAE_NKKW4EEngineRenderLayer@@KABVCEngineCamera@@_NAAVCEngineSceneRenderStats@@@Z\n')
edit('00b92660', 'src', ' unsigned long GetCellRenderMask(unsigned long,unsigned long); //00B91DC0',
     'private: unsigned long GetCellRenderMask(unsigned long,unsigned long); public: //00B91DC0 (Ego_r: private)')
edit('00b92c20', 'src', ' unsigned long GetCellRenderMask(unsigned long,unsigned long); //00b91dc0',
     'private: unsigned long GetCellRenderMask(unsigned long,unsigned long); public: //00b91dc0 (Ego_r: private)')
edit('00b91dc0', 'src', '''    CArray<CEngineSceneGridCell*> GridCells;
    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);
};''', '''    CArray<CEngineSceneGridCell*> GridCells;
private:
    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);   // ?GetCellRenderMask@CEngineSceneGrid@@AAEKKK@Z (Ego_r: private)
};''')
edit('00b91760', 'src', '''class CEngineScene {
public:
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* primitive, EEngineRenderLayer layer,''', '''class CEngineScene {
private:                                        // Ego_r.pdb: private (AAE)
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* primitive, EEngineRenderLayer layer,''')
# 0x00B917E0 = ?RenderPrimitiveListToShadowBuffer@CEngineScene@@AAEXPAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@ABVCShadowClipVolume@@AAVCEngineSceneRenderStats@@@Z
#   (Ego_r.pdb: the only (primitive list, layer, CShadowClipVolume&, stats&) member; RenderSceneShadowBuffer 0x00B94060 calls 0x00B917E0 with
#   (list, layer, volume, stats); the manager's slot 8 = RenderPrimitiveToShadowBuffer(primitive)).
edit('00b917e0', 'src', '''class CEngineScene { public:
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* first, unsigned long layerMask, long a, long b);
};

void CEngineScene::RenderPrimitiveList(CEngineInternalPrimitiveBase* first, unsigned long layerMask, long a, long b)
{''', '''enum EEngineRenderLayer { };
class CShadowClipVolume;
class CEngineSceneRenderStats;
class CEngineScene {
private:                                        // Ego_r.pdb: private (AAE)
    void RenderPrimitiveListToShadowBuffer(CEngineInternalPrimitiveBase* first, EEngineRenderLayer layerMask,
                                           const CShadowClipVolume& volume, CEngineSceneRenderStats& stats);
};

void CEngineScene::RenderPrimitiveListToShadowBuffer(CEngineInternalPrimitiveBase* first, EEngineRenderLayer layerMask,
                                                     const CShadowClipVolume& volume, CEngineSceneRenderStats& stats)
{''')
edit('00b917e0', 'src', '''// CEngineScene::RenderPrimitiveList(CEngineInternalPrimitiveBase* first, unsigned long layerMask, long, long) -- walk''', '''// CEngineScene::RenderPrimitiveListToShadowBuffer(CEngineInternalPrimitiveBase* first, EEngineRenderLayer layer, const CShadowClipVolume&, CEngineSceneRenderStats&) -- walk
// Leaf: ?RenderPrimitiveListToShadowBuffer@CEngineScene@@AAEXPAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@ABVCShadowClipVolume@@AAVCEngineSceneRenderStats@@@Z
// Identity (naming wave 11): Ego_r.pdb's only (list, layer, CShadowClipVolume&, stats&) CEngineScene member; RenderSceneShadowBuffer 0x00B94060 calls it on both loose lists.''')
# 0x00B97B60 std::map<EEngineRenderLayer, CEngineSceneRenderStats>::operator[] (Ego_r.pdb exact symbol; RenderSceneOcclusion 0x00B94180 binds it)
edit('00b97b60', 'src', '''template CEngineSceneRenderStats& std::map<long, CEngineSceneRenderStats>::operator[](const long&);''', '''enum EEngineRenderLayer { };
template CEngineSceneRenderStats& std::map<EEngineRenderLayer, CEngineSceneRenderStats>::operator[](const EEngineRenderLayer&);''')
edit('00b97b60', 'src', '''// std::map::operator[](const long& k) -- STLport subscript of map<long, CEngineSceneRenderStats>:''', '''// std::map::operator[](const EEngineRenderLayer& k) -- STLport subscript of map<EEngineRenderLayer, CEngineSceneRenderStats> (Ego_r.pdb; CEngineScene::RenderStats):''')
edit('00b97b60', 'src', '''// Leaf: ??A?$map@JVCEngineSceneRenderStats@@''', '''// Leaf: ??A?$map@W4EEngineRenderLayer@@VCEngineSceneRenderStats@@''')
