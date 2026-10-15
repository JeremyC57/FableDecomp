// CSoundEmitterLH::CActiveSound::CActiveSound(long handle, CSoundEmitterSource* source, float volume) -- CBaseClassNonCopyable(), own vtable, state 2, the handle, zero counters, the volume, playing, an empty name, the source
// Leaf: ??0CActiveSound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB910, length 71 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (landed as "CBase"), CCharString::CCharString()
// @ 0x0099E4B0.
// Identity: vtable 0x0129E67C = CSoundEmitterLH::CActiveSound (rtti-vtables.tsv; its deleting destructor
// 0x00AAC7F0 destroys the string at +0x1c); the manifest's "CTCCarrying::CTCCarrying" label is propagated
// and rejected. The only caller (0x00AAC27C) does `new (0x24 bytes) CActiveSound(result, sound, volume)`
// with a non-negative play result, the sound object and a float. No PDB layout: member names are descriptive.
// Shape: init list in declaration order; the scheduler hoists the two argument stores above the vtable
// store, integer zeros share eax, and the name's out-of-line constructor runs before the last member's
// store. The float argument is copied through ecx.
#include "fable_string.h"

struct CSoundEmitterSource;

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                        // 0x0099A2F0
};

struct CSoundEmitterLH {
    struct CActiveSound : CBaseClassNonCopyable {   // vtable 0x0129E67C, 0x24 bytes
        long                 m_state;               // +0x04
        long                 m_handle;              // +0x08
        long                 m_elapsed;             // +0x0c
        float                m_volume;              // +0x10
        bool                 m_playing;             // +0x14
        bool                 m_stopping;            // +0x15
        long                 m_flags;               // +0x18
        CCharString          m_name;                // +0x1c
        CSoundEmitterSource* m_pSource;             // +0x20
        virtual void v00();
        CActiveSound(long handle, CSoundEmitterSource* source, float volume);
    };
};

CSoundEmitterLH::CActiveSound::CActiveSound(long handle, CSoundEmitterSource* source, float volume)
    : m_state(2)
    , m_handle(handle)
    , m_elapsed(0)
    , m_volume(volume)
    , m_playing(true)
    , m_stopping(false)
    , m_flags(0)
    , m_name()
    , m_pSource(source)
{
}