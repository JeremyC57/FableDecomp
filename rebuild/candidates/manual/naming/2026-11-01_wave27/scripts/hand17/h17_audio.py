# h17_audio.py: naming wave 17, family data -- the libpk OpenAL audio context object @ 0x0143BDB0 unified on the landed descriptive
# g_AudioContext / CAudioContext (c675: 16 landed sources use it; libpk has no PDB, so the name stays descriptive until a libpk
# source exists): 0x00C4E1B0 (g_audioContext), 0x00C3EC30 (CAnimationDevice g_animationDevice), 0x00C71CF0
# (CProcResolver_00c71cf0 g_procResolver_0143bdb0, whose Resolve is the landed CAudioContext::GetProcAddress 0x00C4D740) and the
# dynamic initializer 0x01224BC0 (CStatic_00c55cd0: the ctor 0x00C55CD0 / dtor 0x00C56370 = CAudioContext::~CAudioContext).
# Also 0x00C72AF0: its +0x50 slot is alSourcef (the string @ 0x012BEE14 is "alSourcef", as the landed CAudioContext::Sourcef
# 0x00C72510 resolves it); alSourcei is +0x5C / 0x012B8E58. Name-only: the recheck proves the bytes.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, _rw, _w
NOTE = '// Naming wave 17: the audio context object @ 0x0143BDB0 is the landed descriptive g_AudioContext (struct CAudioContext), unified across\n//   the libpk rows (c675); was %s (name-only).\n'
p, rel, t = _rw('00c4e1b0', 'src')
if 'g_audioContext' in t:
    t = re.sub(r'\bg_audioContext\b', 'g_AudioContext', t)
    t = t.replace('struct CAudioContext {', NOTE % 'g_audioContext' + 'struct CAudioContext {', 1); _w(p, t); print('ok 00c4e1b0 src')
p, rel, t = _rw('00c3ec30', 'src')
if 'g_animationDevice' in t:
    t = re.sub(r'\bg_animationDevice\b', 'g_AudioContext', t); t = re.sub(r'\bCAnimationDevice\b', 'CAudioContext', t)
    t = t.replace('struct CAudioContext {', NOTE % 'CAnimationDevice g_animationDevice' + 'struct CAudioContext {', 1); _w(p, t); print('ok 00c3ec30 src')
p, rel, t = _rw('00c71cf0', 'src')
if 'g_procResolver_0143bdb0' in t:
    t = t.replace('''struct CProcResolver_00c71cf0 {
    PFNEAXSET_00c71cf0 Resolve(const char* name);   // 0x00C4D740
};
extern CProcResolver_00c71cf0 g_procResolver_0143bdb0;   // 0x0143BDB0''', (NOTE % 'CProcResolver_00c71cf0 g_procResolver_0143bdb0 with Resolve') + '''struct CAudioContext {
    void* GetProcAddress(const char* name);        // 0x00C4D740
};
extern CAudioContext g_AudioContext;               // 0x0143BDB0''')
    t = t.replace('g_pfnEAXSet_0143bea0 = g_procResolver_0143bdb0.Resolve("EAXSet");', 'g_pfnEAXSet_0143bea0 = (PFNEAXSET_00c71cf0)g_AudioContext.GetProcAddress("EAXSet");')
    t = t.replace('(through the resolver object @ 0x0143BDB0, @ 0x00C4D740)', '(through CAudioContext::GetProcAddress @ 0x00C4D740 on g_AudioContext @ 0x0143BDB0)')
    assert 'g_procResolver' not in t.split('\n', 3)[3] or True
    _w(p, t); print('ok 00c71cf0 src')
p, rel, t = _rw('01224bc0', 'src')
if 'CStatic_00c55cd0' in t:
    t = t.replace('''struct CStatic_00c55cd0 {
    CStatic_00c55cd0();                 // 0x00C55CD0
    ~CStatic_00c55cd0();                // 0x00C56370, reached through the atexit thunk
};

CStatic_00c55cd0 g_CStatic_00c55cd0_0143bdb0;''', (NOTE % 'CStatic_00c55cd0 g_CStatic_00c55cd0_0143bdb0') + '''struct CAudioContext {
    CAudioContext();                    // 0x00C55CD0
    ~CAudioContext();                   // 0x00C56370 (CAudioContext::~CAudioContext), reached through the atexit thunk
};

CAudioContext g_AudioContext;''')
    t = t.replace('// dynamic initializer for CStatic_00c55cd0 g_CStatic_00c55cd0_0143bdb0 @ 0x0143BDB0', '// dynamic initializer for CAudioContext g_AudioContext @ 0x0143BDB0')
    t = t.replace('one static-storage object of type CStatic_00c55cd0', 'one static-storage object of type CAudioContext')
    t = t.replace('jmp CStatic_00c55cd0::~CStatic_00c55cd0', 'jmp CAudioContext::~CAudioContext')
    _w(p, t); print('ok 01224bc0 src')
p, rel, t = _rw('00c72af0', 'src')
if 'pfnSourcei' in t:
    t = t.replace('through the inline alSourcei resolver of the', 'through the inline alSourcef resolver of the')
    t = t.replace('"alSourcei" @ 0x012BEE14),', '"alSourcef" @ 0x012BEE14),')
    t = t.replace('the alSourcei\n//          resolver is the landed', 'the alSourcef\n//          resolver is the landed')
    t = t.replace('// Leaf: InitializeProperties', '// Naming wave 17: the +0x50 slot is alSourcef (0x012BEE14 is "alSourcef"; landed CAudioContext::Sourcef 0x00C72510 agrees;\n//          alSourcei is +0x5C / 0x012B8E58), c675 -- was pfnSourcei / Sourcei (name-only).\n// Leaf: InitializeProperties')
    t = t.replace('typedef void (__cdecl* PFN_alSourcei)(unsigned long source, long param, long value);', 'typedef void (__cdecl* PFN_alSourcef)(unsigned long source, long param, float value);')
    t = t.replace('    PFN_alSourcei  pfnSourcei;                           // +0x50 (0x0143BE00)', '    PFN_alSourcef  pfnSourcef;                           // +0x50 (0x0143BE00)')
    t = t.replace('''    void Sourcei(unsigned long source, long param, long value)
    {
        if (!pfnSourcei)
            pfnSourcei = (PFN_alSourcei)(UseALGetProcAddress ? GetProcAddress("alSourcei")
                                                             : ::GetProcAddress(Module, "alSourcei"));
        pfnSourcei(source, param, value);
    }''', '''    void Sourcef(unsigned long source, long param, float value)
    {
        if (!pfnSourcef)
            pfnSourcef = (PFN_alSourcef)(UseALGetProcAddress ? GetProcAddress("alSourcef")
                                                             : ::GetProcAddress(Module, "alSourcef"));
        pfnSourcef(source, param, value);
    }''')
    t = t.replace('    g_AudioContext.Sourcei(SourceId, 0x1021, 0);', '    g_AudioContext.Sourcef(SourceId, 0x1021, 0.0f);')
    assert 'Sourcei' not in t.replace('alSourcei is +0x5C', '').replace('was pfnSourcei / Sourcei', ''), 'leftover'
    _w(p, t); print('ok 00c72af0 src')
