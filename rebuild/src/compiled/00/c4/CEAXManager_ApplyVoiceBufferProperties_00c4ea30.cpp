// CEAXManager::ApplyVoiceBufferProperties(CEAXVoice* voice, unsigned long version, unsigned long id, void* data, unsigned long size) -- push a voice's cached EAX 3.0 buffer properties down to its audio object: clamp the direct / room / obstruction / occlusion / exclusion levels to [-10000, 0] mB, the LF ratios to [0, 1] and the room-rolloff and air-absorption factors to [0, 10], keep the clamped block on the voice and hand it to the object (a 3D object takes direct, direct HF and the whole block, a 2D one only direct and room); 1 when applied, 0 for a voice without an object, a property with no data or another EAX version
// Leaf: ?ApplyVoiceBufferProperties@CEAXManager@@QAEJPAUCEAXVoice@@KKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4EA30, 1182 bytes (one function, `ret 14h` at 0x00C4EECB; GetVoiceBufferProperty
// 0x00C4EED0 follows), thiscall `ret 14h` with `this` unused (as GetVoiceBufferProperty).
// Identity: its only caller is the EAX property setter 0x00C4F600 (SetEAXProperty, called by the landed QueueCommit
// 0x00C51A20): after a DSPROPSETID_EAX30_BufferProperties value changed it calls this at 0x00C5130E with the manager in ecx
// and (voice, 2, id, data, size). The voice is the 0x228-byte EAX property block CAudioObject::Open 0x00C42DD0 allocates
// (+0x1A8 the EAX30BUFFERPROPERTIES, +0x1F0 its CEAXBuffer, +0x21C the owning audio object); slot 1 of the audio object
// (vtable 0x012B5A88) is Is3D. Descriptive name (libpk/CODA, no PDB/RTTI); the catalog's "CTCLook_ProcessHeadTurn" is a
// propagated guess.
// Callees (all change-tested setters that raise a dirty bit): 3D object (landed class CLibpkObject1D8) SetDirect 0x00C415A0
// (+0x19C), SetDirectHF 0x00C415C0 (+0x1A0), SetEAXParameters 0x00C41E40 (the ten values at +0x1A4..+0x1C8); 2D object
// (C2DAudioObject, its Open is 0x00C449C0) SetDirect 0x00C44860 (+0x100) and SetRoom 0x00C44880 (+0x104). The landed rows
// carry propagated names (NAME_FIXES.tsv). The constants 1.0f / 0.0f / 10.0f are 0x0122DED8 / 0x0122DEDC / 0x01237F0C.
// Shape: each kept value is clamped by an if/else that stores both copies in each arm (the in-range arm copies the raw
// bits, the out-of-range arm picks the bound; a `?:` stored once and copied puts the value through the x87 stack); the
// owner's type is re-tested for each cast as a checked downcast (`neg al / sbb / and`), once for the 3D path and per call
// on the 2D path. A bool result keeps it in bl (retail: edi, `mov eax, edi`).
#include "engine/EAXNativeTypes.h"

extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

// v clamped to [lo, hi] (the bound on the side it fell out of).
#define EAX_CLAMP(v, lo, hi) (((v) <= (hi) && (v) >= (lo)) ? (v) : (((v) > (hi)) ? (hi) : (lo)))
// Store v clamped to [lo, hi] into both the voice's applied copy and the parameter block.
#define EAX_CLAMP_TO(applied, parameter, v, lo, hi) \
    if ((v) <= (hi) && (v) >= (lo)) { applied = (v); parameter = (v); } \
    else { applied = ((v) > (hi)) ? (hi) : (lo); parameter = ((v) > (hi)) ? (hi) : (lo); }

// The occlusion parameter block an audio object takes (ten values; layout of the 3D object's +0x1A4 copy).
struct SAudioEAXParameters {
    long  lRoom;                                        // +0x00
    long  lRoomHF;                                      // +0x04
    long  lObstruction;                                 // +0x08
    float flObstructionLFRatio;                         // +0x0c
    long  lOcclusion;                                   // +0x10
    float flOcclusionLFRatio;                           // +0x14
    long  lExclusion;                                   // +0x18
    float flExclusionLFRatio;                           // +0x1c
    float flRoomRolloffFactor;                          // +0x20
    float flAirAbsorptionFactor;                        // +0x24
};

class CAudioObject {                                    // vtable 0x012B5A88
public:
    virtual ~CAudioObject();                            // slot 0
    virtual bool Is3D() const;                          // slot 1
};
class CLibpkObject1D8 : public CAudioObject {           // the 3D audio object
public:
    void SetDirect(long direct);                        // 0x00C415A0
    void SetDirectHF(long directHF);                    // 0x00C415C0
    void SetEAXParameters(const SAudioEAXParameters& parameters);   // 0x00C41E40
};
class C2DAudioObject : public CAudioObject {
public:
    void SetDirect(long direct);                        // 0x00C44860
    void SetRoom(long room);                            // 0x00C44880
};

class CEAXBuffer;
struct CEAXVoice {                                      // 0x228 bytes (CAudioObject::Open)
    unsigned char         _pad[0x1a8];
    EAX30BUFFERPROPERTIES m_eax3BufferProperties;       // +0x1a8 .. +0x1ef
    CEAXBuffer*           m_pEffect;                    // +0x1f0
    SAudioEAXParameters   m_applied;                    // +0x1f4 the clamped values last applied
    CAudioObject*         m_pOwner;                     // +0x21c
    unsigned char         _pad220[8];
};

struct CEAXManager {
    long ApplyVoiceBufferProperties(CEAXVoice* voice, unsigned long version, unsigned long id, void* data, unsigned long size);
};

inline CLibpkObject1D8* As3D(CAudioObject* object) { return object->Is3D() ? static_cast<CLibpkObject1D8*>(object) : 0; }
inline C2DAudioObject* As2D(CAudioObject* object) { return object->Is3D() ? 0 : static_cast<C2DAudioObject*>(object); }

long CEAXManager::ApplyVoiceBufferProperties(CEAXVoice* voice, unsigned long version, unsigned long id, void* data, unsigned long size)
{
    long result = 0;
    CAudioObject* owner = voice->m_pOwner;
    if (!owner)
        return 0;
    if (!id || data) {
        SAudioEAXParameters parameters;
        memset(&parameters, 0, sizeof(parameters));
        if (version == 2) {
            long direct = EAX_CLAMP(voice->m_eax3BufferProperties.lDirect, -10000, 0);
            long directHF = EAX_CLAMP(voice->m_eax3BufferProperties.lDirectHF, -10000, 0);
            EAX_CLAMP_TO(voice->m_applied.lRoom, parameters.lRoom, voice->m_eax3BufferProperties.lRoom, -10000, 0)
            EAX_CLAMP_TO(voice->m_applied.lRoomHF, parameters.lRoomHF, voice->m_eax3BufferProperties.lRoomHF, -10000, 0)
            EAX_CLAMP_TO(voice->m_applied.lObstruction, parameters.lObstruction, voice->m_eax3BufferProperties.lObstruction, -10000, 0)
            EAX_CLAMP_TO(voice->m_applied.flObstructionLFRatio, parameters.flObstructionLFRatio,
                         voice->m_eax3BufferProperties.flObstructionLFRatio, 0.0f, 1.0f)
            EAX_CLAMP_TO(voice->m_applied.lOcclusion, parameters.lOcclusion, voice->m_eax3BufferProperties.lOcclusion, -10000, 0)
            EAX_CLAMP_TO(voice->m_applied.flOcclusionLFRatio, parameters.flOcclusionLFRatio,
                         voice->m_eax3BufferProperties.flOcclusionLFRatio, 0.0f, 1.0f)
            EAX_CLAMP_TO(voice->m_applied.flRoomRolloffFactor, parameters.flRoomRolloffFactor,
                         voice->m_eax3BufferProperties.flRoomRolloffFactor, 0.0f, 10.0f)
            EAX_CLAMP_TO(voice->m_applied.flAirAbsorptionFactor, parameters.flAirAbsorptionFactor,
                         voice->m_eax3BufferProperties.flAirAbsorptionFactor, 0.0f, 10.0f)
            EAX_CLAMP_TO(voice->m_applied.lExclusion, parameters.lExclusion, voice->m_eax3BufferProperties.lExclusion, -10000, 0)
            EAX_CLAMP_TO(voice->m_applied.flExclusionLFRatio, parameters.flExclusionLFRatio,
                         voice->m_eax3BufferProperties.flExclusionLFRatio, 0.0f, 1.0f)
            result = 1;
            if (owner->Is3D()) {
                CLibpkObject1D8* object = As3D(owner);
                object->SetDirect(direct);
                object->SetDirectHF(directHF);
                object->SetEAXParameters(parameters);
            } else {
                As2D(owner)->SetDirect(direct);
                As2D(owner)->SetRoom(parameters.lRoom);
            }
        }
    }
    return result;
}