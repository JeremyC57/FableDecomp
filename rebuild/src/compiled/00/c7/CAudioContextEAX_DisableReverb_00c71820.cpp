// CAudioContextEAX::DisableReverb() -- vtable slot 2 of the libpk EAX 3.0 software mix context: mute the EAX listener room (EAXSet DSPROPSETID_EAX30_ListenerProperties, property 5 = ROOM, source 0, -10000 mB, 4 bytes), resolving EAXSet through alGetProcAddress on first use, and clear the enabled byte (+4)
// Leaf: ?DisableReverb@CAudioContextEAX@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71820, 69 bytes (+ int3 padding), thiscall, plain `ret`; slot 2 of the
//   vtable 0x012BEDF4 (word at 0x012BEDFC; ctor 0x00C71710 / dtor 0x00C71CF0 / WriteOutput 0x00C71750 landed as
//   CAudioContextEAX). The landed destructor runs the same body inline.
// Callees: CAudioContext::GetProcAddress @ 0x00C4D740 on the global context 0x0143BDB0 ("EAXSet" @ 0x012B8B80), the
//   resolved entry point (cdecl, `add esp,14h`) cached in the context's pfnEAXSet (+0xF0 = 0x0143BEA0). GUID @ 0x012BEE90.
// Name: descriptive (libpk, no PDB/RTTI; the manifest label `_gapscan::sub_00c71820` is a placeholder). The context and
//   its EAXSet slot use the landed CAudioContext model (0x00C558A0 EAXSet out of line: here it is written inline, its
//   result unused); the landed destructor spells the same targets g_pfnEAXSet_0143bea0 / CProcResolver_00c71cf0::Resolve.
// Shape: the -10000 value is a frame local (the `push ecx` slot, stored before the branch); the entry point is called
//   through eax straight after the resolve.
typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef long (__cdecl* PFN_EAXSet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern const GUID DSPROPSETID_EAX30_ListenerProperties;   // 0x012BEE90

struct CAudioContext {                              // the OpenAL wrapper (landed model of 0x00C558A0; global 0x0143BDB0)
    void*                __vftable;
    void*                Module;                    // +0x04
    unsigned char        _pad08[0xe0];
    PFN_alGetProcAddress pfnGetProcAddress;         // +0xe8
    unsigned char        _padec[4];
    PFN_EAXSet           pfnEAXSet;                 // +0xf0 (0x0143BEA0)
    void* GetProcAddress(const char* name);         // 0x00C4D740
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

class CAudioContextBase {                           // vtable 0x012B95C4 (ctor 0x00C6AD70)
public:
    virtual void v00();
};
class CAudioContextEAX : public CAudioContextBase { // vtable 0x012BEDF4
public:
    virtual void v01();
    virtual void DisableReverb();                   // slot 2 (0x00C71820)
    bool m_enabled;                                 // +0x04
};

void CAudioContextEAX::DisableReverb()
{
    long room = -10000;
    if (!g_AudioContext.pfnEAXSet)
        g_AudioContext.pfnEAXSet = (PFN_EAXSet)g_AudioContext.GetProcAddress("EAXSet");
    g_AudioContext.pfnEAXSet(&DSPROPSETID_EAX30_ListenerProperties, 5, 0, &room, 4);
    m_enabled = false;
}