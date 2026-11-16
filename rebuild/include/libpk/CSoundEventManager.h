#ifndef FABLE_LIBPK_CSOUNDEVENTMANAGER_H
#define FABLE_LIBPK_CSOUNDEVENTMANAGER_H
#include "libpk/CHandleBasedMemoryPool_012b4098.h"
#ifdef FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
#include "libpk/CFader.h"
#endif
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
#include "engine/C3DVector.h"
#endif
#include "rebuild_abi.h"
#include <stddef.h>
// Descriptive shared libpk type; existing00C26090 model and native00C262E0.
// Layout: native00C26260/00C262E0 address the embedded pool at+360.
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
#include "libpk/CSoundEventList.h"
class CSoundSample;
#endif
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#include "libpk/CSoundEventSet.h"
#if defined(FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23) || defined(FABLE_RETAIL_STREAMED_TYPED_MANAGER_PASS23)
#include "engine/C3DVector.h"
#endif
#include "libpk/CStreamSoundBank.h"
#include "libpk/CPKVoiceParams.h"
struct Input;
struct CSoundPosition;
class CSoundEvent;
// First15native virtual entries012B2748..012B2780; later unused entries omitted.
// Existing names absent from game PDBs; names/prototypes follow independent native bodies/callers.
struct CSoundEventManager {
 virtual void AddToWorld(void*);
 virtual void SetMember888(long);
 virtual void SetWorldMode(bool);
 virtual void SetMember7cc(long);
 virtual void* GetWorldSelection();
 virtual void* FindInWorld(void*);
 virtual void SetMember884(unsigned char);
 virtual void SetWorldGain(float);
 virtual float GetWorldGain();
 virtual long SetWorldEnabled(void*);
 virtual int IsWorldRunning();
 virtual long SetWorldState(void*);
 virtual void SetListenerFrame(const CSoundPosition*,const CSoundPosition*,const CSoundPosition*);
 virtual void SetListenerPosition(const CSoundPosition*);
 virtual long PlayStreamed(CStreamSoundBank*,long,unsigned long,const Input*);
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
 // Descriptive libpk ABI continuation. Native table012B2748 slots15..27;
 // no original library PDB names. Opaque resource selectors stay void pointers.
 // Evidence: position-row NAMING_EVIDENCE and native callee/dispatch records.
 virtual long PlaySample(void*,long,const Input*); // C12D50, ret12, calls PlayDriver.
 virtual long PlaySelectedResource(const void*,const Input*); // C133D0, ret8.
 virtual long FindPlayingSelectedResource(const void*,const void*); // C11AC0, ret8.
 virtual long FindPlayingSample(const void*,void*); // C119D0, ret8, keyed sample lookup.
 virtual void StopAllEvents(); // C123B0, event and pooled-resource walks.
 virtual void StopEventsWithField24(void*); // C12520, pointer identity filter.
 virtual void StopEventsWithField10(void*); // C12460, pointer identity filter.
 virtual void ResetEvent(long); // C11330 forwards event slot1, then world update.
 virtual void ResetEventPlayback(long); // C114B0 forwards event slot4.
 virtual void SetEventPan(long,float); // C115D0 forwards event slot7.
 virtual void SetEventGain(long,float); // C113B0 forwards event slot2.
 virtual void SetEventPlaybackParameter(long,float); // C11430 forwards event slot3.
 virtual void SetEventPosition(long,const C3DVector&); // C11520 forwards slots8 and5.
#endif
#if defined(FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23) || defined(FABLE_RETAIL_STREAMED_TYPED_MANAGER_PASS23)
 unsigned char Unrecovered04[4];
 C3DVector ListenerPosition,ListenerFront,ListenerTop;
#else
 unsigned char Unrecovered04[0x28];
#endif
 CSoundEventSet m_events;
#if defined(FABLE_RETAIL_STREAMED_TYPED_MANAGER_PASS23)
 // Combined independently proven streamed/listener fields; existing profiles unchanged.
 unsigned char Unrecovered7b0[8];
 long StreamOwner;
 unsigned char Unrecovered7bc[0xc9];
 bool ListenerValid;
 unsigned char Unrecovered886[6];
#elif defined(FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23)
 // NativeC1C48D ListenerValid885, same manager used by independentC1C280.
 unsigned char Unrecovered7b0[0xd5];
 bool ListenerValid;
 unsigned char Unrecovered886[6];
#else
 unsigned char Unrecovered7b0[8];
 long StreamOwner;
 unsigned char Unrecovered7bc[0xd0];
#endif
 long m_nextId;
 CSoundEvent* PlayDriver(void*,long,const CPKVoiceParams*,long);
};
#else
struct CSoundEventManager {
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
    // Layout: native C1127A/80 uses embedded keyed list+1f4; existing pool remains+360.
    unsigned char OpaqueManagerPrefix[0x1f4];
    CSoundEventList ListeningEvents;
    unsigned char OpaqueManagerMiddle[0x360-0x1fc];
    int CheckExisting(CSoundSample*,long,long,long,long,unsigned long);
#else
    unsigned char OpaqueManagerPrefix[0x360];
#endif
    CHandleBasedMemoryPool_012b4098 m_pool;
#ifdef FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
    // Opt-in (landing gm, row 0x00C11700): the fader at +0x7D8 (constructed by 0x00C11D00 with CFader::CFader 0x00C26010,
    //   destroyed by 0x00C13470) and its one forwarder. Descriptive names (libpk has no PDB or RTTI).
    unsigned char m_7b0[0x7d8 - 0x7b0];         // +0x7B0 (the two worker parts +0x7B4 / +0x7B8, resources +0x7BC, ...)
    CFader m_fader;                             // +0x7D8
    float GetFaderOutput() const;               // 0x00C11700
#endif
};
FABLE_STATIC_ASSERT_AT(12, offsetof(CSoundEventManager,m_pool)==0x360);
#ifdef FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
FABLE_STATIC_ASSERT_AT(24, offsetof(CSoundEventManager,m_7b0)==0x7b0);
FABLE_STATIC_ASSERT_AT(25, offsetof(CSoundEventManager,m_fader)==0x7d8);
#endif
#endif // STREAMED_PASS23
#endif
