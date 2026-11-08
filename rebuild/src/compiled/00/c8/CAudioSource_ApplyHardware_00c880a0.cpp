// CAudioSource::ApplyHardware(CAudioObject* stream, unsigned long flags) -- push a 3D sound's changed properties to its hardware OpenAL source: the EAX 3.0 direct level (the source's direct offset plus the distance x cone attenuation in millibels, clamped to [-10000, 1000]), direct HF and the listener HF reference, the room / room HF levels with a zero room-rolloff, the AL gain (sound x source gain, x the attenuation for caps bit 0x2000, x the context's master gain, at most 1), the AL pitch (retried at 80% while OpenAL rejects it as AL_INVALID_VALUE) and the AL position (z negated; only the direction without caps bit 0x2000), each only when its dirty bits are set
// Leaf: ?ApplyHardware@CAudioSource@@QAEXPAVCAudioObject@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C880A0, 1307 bytes (one function, `ret 8` at 0x00C885B8; the landed COggVorbisStream
// Retail-asm: 0x00C88103 `fld dword ptr [esp+10h]` + 0x00C88107 `fistp dword ptr [esp+14h]` -- the inline libpk FtoL rounding of AmplitudeToMillibels (libpk/FtoL.h)
// ctor 0x00C885C0 follows the 0xCC pad), thiscall.
// Identity: the landed caller CAudioSource::ApplySound 0x00C72E00 binds this name (hardware path: `IsHardware()` source,
// the sound when its slot-1 test holds); descriptive libpk/OpenAL name (no PDB/RTTI). The sound is the 3D audio object (landed
// class CLibpkObject1D8; members past +0xFC are read here only); slot 1 of the audio object is Is3D (CAudioObject::Open
// 0x00C42DD0 and this chunk's ApplyVoiceBufferProperties 0x00C4EA30; ApplySound spells it IsStreamed).
// Callees: the OpenAL wrapper g_AudioContext 0x0143BDB0 (landed CAudioContext model): EAXSet resolved through
// CAudioContext::GetProcAddress 0x00C4D740 into +0xF0 (as the landed EAXSet 0x00C558A0 / DisableReverb 0x00C71820), alSourcef
// +0x50 / alGetError +0xE0 / alSourcefv +0x54 resolved on first use through alGetProcAddress or the DLL export table
// (GetProcAddress IAT 0x0143FE44), the master gain float +0xF8 (0x0143BEA8). GUIDs: 0x012BEEA0 {A8FA6881-B476-11D3-BDB9-
// 00C0F02DDF87} = DSPROPSETID_EAX30_BufferProperties (the SAL module's copy, next to the landed SAL_EAX30_ListenerProperties
// 0x012BEE90). Constants 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 2000.0f 0x012A2AD4, 0.8f 0x01254DA8.
// Shape: the context's entry-point wrappers are inline, CAudioContext::GetProcAddress has its landed body in the TU and VC7.1
// expands it only inside the pitch retry loop (as retail); AmplitudeToMillibels (landed out of line at 0x00C40F50, a separate
// TU) is expanded here with its zero test first. The pitch retry computes the next pitch after alGetError (retail schedules
// the multiply ahead of the call); the length is x*x + (y*y + z*z) (retail adds y*y + z*z first).
#include "libpk/FtoL.h"
#pragma intrinsic(log10, sqrt)
extern "C" double __cdecl log10(double x);
extern "C" double __cdecl sqrt(double x);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

struct _GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern const _GUID SAL_EAX30_ListenerProperties;   // 0x012BEE90
extern const _GUID SAL_EAX30_BufferProperties;     // 0x012BEEA0


// OpenAL / EAX 3.0 ids (al.h, eax.h).
#define AL_PITCH                                0x1003
#define AL_POSITION                             0x1004
#define AL_GAIN                                 0x100a
#define AL_INVALID_VALUE                        0xa003
#define DSPROPERTY_EAXBUFFER_DIRECT             5
#define DSPROPERTY_EAXBUFFER_DIRECTHF           6
#define DSPROPERTY_EAXBUFFER_ROOM               7
#define DSPROPERTY_EAXBUFFER_ROOMHF             8
#define DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR  20
#define DSPROPERTY_EAXLISTENER_HFREFERENCE      22

typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef void (__cdecl* PFN_alSourcef)(unsigned long source, long param, float value);
typedef void (__cdecl* PFN_alSourcefv)(unsigned long source, long param, const float* values);
typedef long (__cdecl* PFN_alGetError)();
typedef long (__cdecl* PFN_EAXSet)(const _GUID* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

struct CAudioContext {
    void*                __vftable;
    void*                Module;                    // +0x04
    unsigned char        _pad08[0x48];
    PFN_alSourcef        pfnSourcef;                // +0x50
    PFN_alSourcefv       pfnSourcefv;               // +0x54
    unsigned char        _pad58[0x88];
    PFN_alGetError       pfnGetError;               // +0xe0
    void*                pfnIsExtensionPresent;     // +0xe4
    PFN_alGetProcAddress pfnGetProcAddress;         // +0xe8
    void*                pfnGetEnumValue;           // +0xec
    PFN_EAXSet           pfnEAXSet;                 // +0xf0
    void*                pfnEAXGet;                 // +0xf4
    float                MasterGain;                // +0xf8
    void*                Device;                    // +0xfc
    void*                Context;                   // +0x100
    bool                 UseALGetProcAddress;       // +0x104
    void* GetProcAddress(const char* name);         // 0x00C4D740
    long EAXSet(const _GUID* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size)
    {
        if (!pfnEAXSet)
            pfnEAXSet = (PFN_EAXSet)GetProcAddress("EAXSet");
        return pfnEAXSet(propertySetGuid, property, source, value, size);
    }
    void Sourcef(unsigned long source, long param, float value)
    {
        if (!pfnSourcef)
            pfnSourcef = (PFN_alSourcef)(UseALGetProcAddress ? GetProcAddress("alSourcef") : ::GetProcAddress(Module, "alSourcef"));
        pfnSourcef(source, param, value);
    }
    void Sourcefv(unsigned long source, long param, const float* values)
    {
        if (!pfnSourcefv)
            pfnSourcefv = (PFN_alSourcefv)(UseALGetProcAddress ? GetProcAddress("alSourcefv") : ::GetProcAddress(Module, "alSourcefv"));
        pfnSourcefv(source, param, values);
    }
    long GetError()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError") : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

inline void* CAudioContext::GetProcAddress(const char* name)
{
    if (!pfnGetProcAddress) {
        pfnGetProcAddress = (PFN_alGetProcAddress)::GetProcAddress(Module, "alGetProcAddress");
        if (UseALGetProcAddress)
            pfnGetProcAddress = (PFN_alGetProcAddress)pfnGetProcAddress("alGetProcAddress");
    }
    return pfnGetProcAddress(name);
}

inline long AmplitudeToMillibels(float amplitude)
{
    long result;
    if (amplitude == 0.0f)
        result = -10000;
    else {
        result = FtoL((float)log10(amplitude) * 2000.0f);
        if (result < -10000)
            result = -10000;
    }
    return result;
}

class CAudioObject {
public:
    virtual void v00();
    virtual bool Is3D() const;
    unsigned char  _pad04[8];
    unsigned long  m_caps;                          // +0x0c (bit 0x2000: attenuate through the AL gain)
    unsigned long  m_flags;                         // +0x10
    long           m_14;                            // +0x14
    float          m_gain;                          // +0x18
    long           m_volume;                        // +0x1c
    float          m_pitch;                         // +0x20
};
class CLibpkObject1D8 : public CAudioObject {
public:
    unsigned char  _pad24[0xfc - 0x24];
    float          m_position[3];                   // +0xfc
    unsigned char  _pad108[0x130 - 0x108];
    float          m_coneGain;                      // +0x130 linear attenuation factors of the 3D model
    float          m_distanceGain;                  // +0x134
    float          m_dopplerPitch;                  // +0x138 pitch factor
    long           m_directOffset;                  // +0x13c mB added to the attenuation
    long           m_directHF;                      // +0x140 mB
    float          m_directHFReference;             // +0x144 Hz (EAX listener HF reference)
    long           m_room;                          // +0x148 mB
    long           m_roomHF;                        // +0x14c mB
    float          m_roomHFReference;               // +0x150 Hz
};

struct CAudioSource {
    virtual void v00();
    unsigned char  _pad04[0x2c];
    float          m_gain;                          // +0x30
    unsigned char  _pad34[8];
    float          m_pitch;                         // +0x3c
    unsigned char  _pad40[0x18];
    unsigned long  SourceId;                        // +0x58
    void ApplyHardware(CAudioObject* stream, unsigned long flags);
};

void CAudioSource::ApplyHardware(CAudioObject* stream, unsigned long flags)
{
    CLibpkObject1D8* object = static_cast<CLibpkObject1D8*>(stream);
    // EAX direct path: on an EAX change (0x400000), or a volume change of a sound without caps bit 0x2000 (whose attenuation
    // then goes into the direct level instead of the AL gain).
    if ((flags & 0x400000) || ((flags & 0xa48002) && !(object->m_caps & 0x2000))) {
        long direct = object->m_directOffset + AmplitudeToMillibels(object->m_distanceGain * object->m_coneGain);
        if (direct > 1000)
            direct = 1000;
        else if (direct < -10000)
            direct = -10000;
        g_AudioContext.EAXSet(&SAL_EAX30_BufferProperties, DSPROPERTY_EAXBUFFER_DIRECT, SourceId, &direct, 4);
        if (flags & 0x400000) {
            g_AudioContext.EAXSet(&SAL_EAX30_BufferProperties, DSPROPERTY_EAXBUFFER_DIRECTHF, SourceId, &object->m_directHF, 4);
            g_AudioContext.EAXSet(&SAL_EAX30_ListenerProperties, DSPROPERTY_EAXLISTENER_HFREFERENCE, 0, &object->m_directHFReference, 4);
        }
    }
    // Room path.
    if (flags & 0x1000000) {
        float zero = 0.0f;
        g_AudioContext.EAXSet(&SAL_EAX30_BufferProperties, DSPROPERTY_EAXBUFFER_ROOM, SourceId, &object->m_room, 4);
        g_AudioContext.EAXSet(&SAL_EAX30_BufferProperties, DSPROPERTY_EAXBUFFER_ROOMHF, SourceId, &object->m_roomHF, 4);
        g_AudioContext.EAXSet(&SAL_EAX30_BufferProperties, DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR, SourceId, &zero, 4);
        g_AudioContext.EAXSet(&SAL_EAX30_ListenerProperties, DSPROPERTY_EAXLISTENER_HFREFERENCE, 0, &object->m_roomHFReference, 4);
    }
    // AL gain.
    if (flags & 0xa4800a) {
        float gain = object->m_gain * m_gain;
        if (object->m_caps & 0x2000)
        {
            gain *= object->m_distanceGain;
            gain *= object->m_coneGain;
        }
        gain *= g_AudioContext.MasterGain;
        if (gain > 1.0f)
            gain = 1.0f;
        g_AudioContext.Sourcef(SourceId, AL_GAIN, gain);
    }
    // AL pitch: clear the error state, then lower the pitch until OpenAL accepts it.
    if (flags & 0x10024) {
        g_AudioContext.GetError();
        float pitch = object->m_dopplerPitch * object->m_pitch * m_pitch;
        long error;
        do {
            if (!(pitch > 0.0f))
                break;
            g_AudioContext.Sourcef(SourceId, AL_PITCH, pitch);
            error = g_AudioContext.GetError();
            pitch *= 0.8f;
        } while (error == AL_INVALID_VALUE);
    }
    // AL position (z negated); without caps bit 0x2000 only the direction is kept (normalised; straight up when zero).
    if (flags & 0x8000) {
        float position[3];
        position[0] = object->m_position[0];
        position[1] = object->m_position[1];
        position[2] = -object->m_position[2];
        if (!(object->m_caps & 0x2000)) {
            float lengthSquared = object->m_position[0] * object->m_position[0]
                                + (object->m_position[1] * object->m_position[1] + object->m_position[2] * object->m_position[2]);
            if (lengthSquared != 0.0f) {
                float length = (float)sqrt(lengthSquared);
                if (length != 0.0f) {
                    float scale = 1.0f / length;
                    position[0] *= scale;
                    position[1] *= scale;
                    position[2] *= scale;
                } else
                    position[1] = 1.0f;
            } else
                position[1] = 1.0f;
        }
        g_AudioContext.Sourcefv(SourceId, AL_POSITION, position);
    }
}