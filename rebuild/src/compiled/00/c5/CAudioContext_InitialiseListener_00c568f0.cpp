// CAudioContext::InitialiseListener() -- after the OpenAL context is up: alDistanceModel(AL_NONE), listener AL_POSITION and AL_VELOCITY to the origin, then deferred EAX 3.0 listener ROOM = -10000 mB and ROOMROLLOFFFACTOR = 0; every OpenAL/EAX entry point is resolved on first use through the inlined lazy wrappers
// Leaf: ?InitialiseListener@CAudioContext@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C568F0, 334 bytes, __fastcall, no arguments, plain `ret`, frame 8 (the two EAX values).
// Identity: libpk's OpenAL wrapper class (descriptive CAudioContext, the landed family 0x00C558A0..0x00C57730; vtable
//   0x012B8C18): its only caller is the context-open routine 0x00C56A40 (OpenDevice 0x00C55AA0, CreateContext 0x00C55B00,
//   SupportsEAX3 0x00C562F0, then this call). The three inlined wrappers are the landed CAudioContext::DistanceModel
//   0x00C56150 ("alDistanceModel" 0x012B8C6C, slot +0xD4), Listener3f 0x00C560F0 ("alListener3f" 0x012B8C5C, slot +0x98) and
//   EAXSet 0x00C558A0 ("EAXSet" 0x012B8B80, slot +0xF0, alGetProcAddress only). 0x1004 / 0x1006 are AL_POSITION / AL_VELOCITY;
//   the GUID 0x012BEE90 is DSPROPSETID_EAX30_ListenerProperties (bytes a8fa6882-b476-11d3-bdb9-00c0f02ddf87), properties
//   0x80000005 / 0x80000018 are DSPROPERTY_EAXLISTENER_ROOM / _ROOMROLLOFFFACTOR | DEFERRED (EAX 3.0 eax.h).
//   Method name descriptive (libpk has no PDB). Manifest label "CAudioContext_ConfigureListener" is a guess, replaced.
// Callees: CAudioContext::GetProcAddress 0x00C4D740 (alGetProcAddress route), GetProcAddress (IAT 0x0143FE44, hoisted in edi).
// Shape: the wrappers are defined (not declared) in the TU, as in retail's, so VC7.1 inlines all five calls.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
extern const GUID DSPROPSETID_EAX30_ListenerProperties;   // 0x012BEE90

typedef void (__cdecl* PFN_alDistanceModel)(long value);
typedef void (__cdecl* PFN_alListener3f)(long param, float v1, float v2, float v3);
typedef long (__cdecl* PFN_EAXSet)(const GUID* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

#define AL_NONE                                0
#define AL_POSITION                            0x1004
#define AL_VELOCITY                            0x1006
#define DSPROPERTY_EAXLISTENER_ROOM            5
#define DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR 24
#define DSPROPERTY_EAXLISTENER_DEFERRED        0x80000000

class CPKObject {                                        // vtable 0x012B5868: the libpk root (inline virtual destructor)
public:
    virtual ~CPKObject() { }
};

// Retail OpenAL wrapper (layout of the landed CAudioContext rows): DLL module +0x04, lazily resolved entry points, the
// alGetProcAddress routing flag +0x104.
class CAudioContext : public CPKObject {
public:
    void*               Module;                          // +0x04
    unsigned char       _pad0[0x90];
    PFN_alListener3f    pfnListener3f;                   // +0x98
    unsigned char       _pad1[0x38];
    PFN_alDistanceModel pfnDistanceModel;                // +0xd4
    unsigned char       _pad2[0x18];
    PFN_EAXSet          pfnEAXSet;                       // +0xf0
    unsigned char       _pad3[0x10];
    bool                UseALGetProcAddress;             // +0x104

    void* GetProcAddress(const char* name);              // 0x00C4D740
    void DistanceModel(long value);                      // 0x00C56150
    void Listener3f(long param, float v1, float v2, float v3);   // 0x00C560F0
    long EAXSet(const GUID* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);   // 0x00C558A0
    void InitialiseListener();                           // 0x00C568F0
};

void CAudioContext::DistanceModel(long value)
{
    if (!pfnDistanceModel)
        pfnDistanceModel = (PFN_alDistanceModel)(UseALGetProcAddress ? GetProcAddress("alDistanceModel")
                                                                     : ::GetProcAddress(Module, "alDistanceModel"));
    pfnDistanceModel(value);
}

void CAudioContext::Listener3f(long param, float v1, float v2, float v3)
{
    if (!pfnListener3f)
        pfnListener3f = (PFN_alListener3f)(UseALGetProcAddress ? GetProcAddress("alListener3f")
                                                               : ::GetProcAddress(Module, "alListener3f"));
    pfnListener3f(param, v1, v2, v3);
}

long CAudioContext::EAXSet(const GUID* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size)
{
    if (!pfnEAXSet)
        pfnEAXSet = (PFN_EAXSet)GetProcAddress("EAXSet");
    return pfnEAXSet(propertySetGuid, property, source, value, size);
}

void CAudioContext::InitialiseListener()
{
    DistanceModel(AL_NONE);
    Listener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    Listener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);

    long room = -10000;
    EAXSet(&DSPROPSETID_EAX30_ListenerProperties, DSPROPERTY_EAXLISTENER_ROOM | DSPROPERTY_EAXLISTENER_DEFERRED, 0, &room, sizeof(room));
    float roomRolloffFactor = 0.0f;
    EAXSet(&DSPROPSETID_EAX30_ListenerProperties, DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR | DSPROPERTY_EAXLISTENER_DEFERRED, 0, &roomRolloffFactor, sizeof(roomRolloffFactor));
}