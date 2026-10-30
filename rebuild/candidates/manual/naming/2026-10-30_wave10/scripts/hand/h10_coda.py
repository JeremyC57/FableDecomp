# sub-wave 586 (hand part): the CODA C API wrapper 0x00C35CA0 is `extern "C" long __stdcall CODA_IsSourcePlaying` (chunk 586:
# LogStatus 0x00C27760 gates "CODA reports source IS playing(" on it; body tests source+0x10 & 0x217 like CODA_GetSourcePosition
# 0x00C35CC0; 0x00C278BE tests the full eax, so long). 0x00C35CC0 `_CODA_GetSourcePosition@4` is bound from 00c276b0.
# 0x00C596E0 is the voice base's slot 1 ?Initialise@CVtable_012B5C10@@UAEJPAUCAudioSound@@@Z (bound from 00c72930); the row
# 0x00C5A1D0 is slot 1 of the derived voice vtable 0x012B5C60 (also slot 1 of 0x012B5CD0, which inherits it): it sets three
# members and tail-jumps to the base Initialise.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
# the defining row
edit('00c35ca0', 'src', 'class CEngine { public: unsigned char pad[0x10]; unsigned int flags; };\n\nbool __stdcall Is3DSceneRenderingEnabled(CEngine* engine)\n{\n    return (engine->flags & 0x217) != 0;\n}',
     '// CODA_IsSourcePlaying(source) -- the CODA C API wrapper: nonzero when the source\'s +0x10 state flags have any of 0x217\n'
     '// Leaf: _CODA_IsSourcePlaying@4\n'
     '// Name (chunk 586): LogStatus 0x00C27760 gates its "CODA reports source IS playing(" line on this call; a `__stdcall` CODA API\n'
     '// wrapper (ret 4) like CODA_GetSourcePosition 0x00C35CC0 (& 0x216); 0x00C278BE tests the full eax, so it returns long.\n'
     'struct CODASource { unsigned char pad[0x10]; unsigned int flags; };\n\n'
     'extern "C" long __stdcall CODA_IsSourcePlaying(CODASource* source)\n{\n    return (source->flags & 0x217) != 0;\n}')
# callers
edit('00c27760', 'src', 'long __stdcall IsPlaying_00c27760(void* source);           // 0x00C35CA0: +0x10 flags & 0x217\nunsigned long __stdcall GetPosition_00c27760(void* source);   // 0x00C35CC0',
     'extern "C" long __stdcall CODA_IsSourcePlaying(void* source);             // 0x00C35CA0: +0x10 flags & 0x217\nextern "C" unsigned long __stdcall CODA_GetSourcePosition(void* source);   // 0x00C35CC0')
edit('00c27760', 'src', 'if (IsPlaying_00c27760(m_pSource)) {', 'if (CODA_IsSourcePlaying(m_pSource)) {')
edit('00c49650', 'src', 'extern "C" long __stdcall IsPlaying_00c49650(void* source);                 // 0x00C35CA0 (non-throwing)',
     'extern "C" long __stdcall CODA_IsSourcePlaying(void* source);               // 0x00C35CA0 (non-throwing)')
edit('00c49650', 'src', 'if (IsPlaying_00c49650(m_pSource))', 'if (CODA_IsSourcePlaying(m_pSource))')
# 0x00C5A1D0
edit('00c5a1d0', 'src', '''// CTCStealth::OnCreate  retail 0x00c5a1d0
// xor eax,eax; [ecx+0x1e8]=0x1000; [ecx+0x1ec]=0; [ecx+0xd8]=0; jmp base::OnCreate

class CTCStealth { public:
    char pad_d8[0xd8];
    unsigned int f_d8;      // +0xd8
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;     // +0x1e8
    unsigned int f_1ec;     // +0x1ec
};

// base-class OnCreate, tail-called
void __fastcall CTCStealth_base_OnCreate(void* self);

void __fastcall CTCStealth_OnCreate(CTCStealth* self)
{
    self->f_1e8 = 0x1000;
    self->f_1ec = 0;
    self->f_d8 = 0;
    CTCStealth_base_OnCreate(self);
}''', '''// CVtable_012B5C60::Initialise(CAudioSound* sound)  retail 0x00c5a1d0 -- slot 1 of the derived libpk voice vtable 0x012B5C60
// (and of 0x012B5CD0, which inherits it; was catalogued CTCStealth::OnCreate, a propagated label)
// Leaf: ?Initialise@CVtable_012B5C60@@UAEJPAUCAudioSound@@@Z
// xor eax,eax; [ecx+0x1e8]=0x1000; [ecx+0x1ec]=0; [ecx+0xd8]=0; jmp CVtable_012B5C10::Initialise 0x00C596E0 (chunk 586: the
// voice base's slot 1, ?Initialise@CVtable_012B5C10@@UAEJPAUCAudioSound@@@Z). Descriptive class names (libpk has no PDB / RTTI).

struct CAudioSound;
struct CVtable_012B5C10 {                                // the libpk voice base, vtable 0x012B5C10 (no RTTI)
    virtual ~CVtable_012B5C10();                         // slot 0
    virtual long Initialise(CAudioSound* sound);         // slot 1 (0x00C596E0)
};
struct CVtable_012B5C60 : CVtable_012B5C10 {             // derived voice, vtable 0x012B5C60
    char pad_04[0xd8 - 4];
    unsigned int f_d8;      // +0xd8
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;     // +0x1e8
    unsigned int f_1ec;     // +0x1ec
    virtual long Initialise(CAudioSound* sound);         // slot 1 (0x00C5A1D0)
};

long CVtable_012B5C60::Initialise(CAudioSound* sound)
{
    f_1e8 = 0x1000;
    f_1ec = 0;
    f_d8 = 0;
    return CVtable_012B5C10::Initialise(sound);
}''')
edit('00c27760', 'src', 'GetPosition_00c27760(m_pSource)', 'CODA_GetSourcePosition(m_pSource)')
# the row's behaviour test links the row: it now drives the member through a derived object (same checks, same pass line)
edit('00c5a1d0', 'tst', '''class CTCStealth { public:
    char pad_d8[0xd8];
    unsigned int f_d8;
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;
    unsigned int f_1ec;
};

static int g_base_called = 0;
void __fastcall CTCStealth_base_OnCreate(void* self) { g_base_called = 1; }

void __fastcall CTCStealth_OnCreate(CTCStealth* self);

int main()
{
    CTCStealth obj;
    obj.f_d8 = 0xDEAD;
    obj.f_1e8 = 0xBEEF;
    obj.f_1ec = 0xCAFE;
    CTCStealth_OnCreate(&obj);
    if (obj.f_1e8 == 0x1000 && obj.f_1ec == 0 && obj.f_d8 == 0 && g_base_called == 1)''', '''struct CAudioSound;
struct CVtable_012B5C10 {
    virtual ~CVtable_012B5C10();
    virtual long Initialise(CAudioSound* sound);
};
struct CVtable_012B5C60 : CVtable_012B5C10 {
    char pad_04[0xd8 - 4];
    unsigned int f_d8;
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;
    unsigned int f_1ec;
    virtual long Initialise(CAudioSound* sound);
};

static int g_base_called = 0;
static CAudioSound* g_base_sound = 0;
CVtable_012B5C10::~CVtable_012B5C10() { }
long CVtable_012B5C10::Initialise(CAudioSound* sound) { g_base_called = 1; g_base_sound = sound; return 0; }

int main()
{
    CVtable_012B5C60 obj;
    obj.f_d8 = 0xDEAD;
    obj.f_1e8 = 0xBEEF;
    obj.f_1ec = 0xCAFE;
    long r = obj.Initialise((CAudioSound*)0x1234);
    if (r == 0 && obj.f_1e8 == 0x1000 && obj.f_1ec == 0 && obj.f_d8 == 0 && g_base_called == 1 && g_base_sound == (CAudioSound*)0x1234)''')
