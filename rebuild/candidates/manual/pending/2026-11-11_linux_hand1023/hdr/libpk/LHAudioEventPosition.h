// Native LHAudio types; descriptive spellings, see NAMING_EVIDENCE.md.
#ifndef FABLE_RETAIL_LHAUDIO_EVENT_POSITION_H
#define FABLE_RETAIL_LHAUDIO_EVENT_POSITION_H
class CSoundEventManager;
struct CSoundPositionVector { float X, Y, Z; };
#ifdef FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
#include "engine/C3DVector.h"
#include "libpk/PKCFrallocStatic.h"
#endif
struct CSoundPosition {
#ifdef FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
    C3DVector Position;      // the engine vector (same 12-byte X/Y/Z layout): 0x00C1C54C hands it to SwapYZ(const C3DVector&)
#else
    CSoundPositionVector Position;
#endif
    bool Relative;
    CSoundPosition(); // 00c20390
};
#ifdef FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
// The 16-byte position holder at CSoundEvent +0x38 (landed descriptive name CQuadHolder; GetValue 0x00C34600 returns the held
// value by hidden pointer, `ret 4`).
struct CQuadHolder {
    CSoundPosition Value;                                                // +0x00
    CSoundPosition GetValue() const;                                     // 0x00C34600
};
#endif
class CSoundEvent {
public:
    virtual void UnresolvedSlot0(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot1(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot2(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot3(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot4(); // Uncalled signature is not recovered.
    virtual void SetPosition(CSoundEventManager*, const CSoundPosition*); // 00c212f0
    virtual void UnresolvedSlot6(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot7(); // Uncalled signature is not recovered.
    virtual bool IsPositionRelative(CSoundEventManager*); // 00c21330
    virtual void UnresolvedSlot9(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot10(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot11(); // Uncalled signature is not recovered.
#ifdef FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
    // Retail opt-in (chunk 1023, row 0x00C1C460): the event's position holder at +0x38 (0x00C1C544 `lea ecx,[eax+38h]` ; call
    // CQuadHolder::GetValue 0x00C34600, a 16-byte CSoundPosition by hidden pointer) and the pointer at +0x48 the update requires
    // non-null (0x00C1C529). Descriptive member names; no PDB covers libpk.
    unsigned char UnmodelledMembers04[0x34];
public:
    CQuadHolder PositionHolder;                                          // +0x38
    void* PPlayingData;                                                  // +0x48
private:
    unsigned char UnmodelledMembers4C[0x68];
#else
private:
    unsigned char UnmodelledMembers[0xb0];
#endif
};
#ifdef FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
// The event's playback slot (0x2c bytes; the landed PKCFrallocStatic<CPlaybackSlot>::Get 0x00C13700 element): the event at +4
// (TracePlaybacks 0x00C127C0), the CODA source handle at +0xC (0x00C1C5C7 -> CODA_SetSourcePosition) and the positional flag at
// +0x25 (0x00C1C534). Descriptive member names.
struct CPlaybackSlot {
    long Unmodelled00;
    CSoundEvent* PEvent;                                                 // +0x04
    long Unmodelled08;
    long SourceHandle;                                                   // +0x0c
    unsigned char Unmodelled10[0x15];
    bool Positional;                                                     // +0x25
    unsigned char Unmodelled26[0x6];
};
#endif
class CSoundEventSet {
#ifdef FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
    unsigned char UnmodelledMembers[0x1f8];
public:
    PKCFrallocStatic<CPlaybackSlot> Playbacks;                           // +0x1f8 (manager +0x224; count +0x228)
private:
    unsigned char UnmodelledMembers20C[0x57c];
#else
    unsigned char UnmodelledMembers[0x788];
#endif
public:
    CSoundEvent* FindInChildThenSelf(long); // 00c21e20
};
class CSoundEventManager {
public:
    virtual void UnresolvedSlot0(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot1(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot2(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot3(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot4(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot5(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot6(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot7(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot8(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot9(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot10(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot11(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot12(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot13(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot14(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot15(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot16(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot17(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot18(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot19(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot20(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot21(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot22(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot23(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot24(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot25(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot26(); // Uncalled signature is not recovered.
    virtual void SetEventPosition(long key, const CSoundPositionVector& position); // 00c11520
    virtual void UnresolvedSlot28(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot29(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot30(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot31(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot32(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot33(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot34(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot35(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot36(); // Uncalled signature is not recovered.
#ifdef FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
    // Retail opt-in (chunk 1023, row 0x00C1C460): the listener position / front / top at +0x08 / +0x14 / +0x20 (converted by SwapYZ
    // at 0x00C1C4A4 / 0x00C1C4B0 / 0x00C1C4BC; the landed CAudioListener::Update 0x00C1C280 takes the same three in that order), the
    // event set public for its playback pool, and the listener-valid flag at +0x885 (0x00C1C48D). Descriptive names.
    unsigned char UnmodelledPrefix[0x4];
public:
    C3DVector ListenerPosition;                                          // +0x08
    C3DVector ListenerFront;                                             // +0x14
    C3DVector ListenerTop;                                               // +0x20
    CSoundEventSet EventSet;                                             // +0x2c, complete native embedded extent
private:
    unsigned char UnmodelledTail7B4[0xd1];
public:
    bool ListenerValid;                                                  // +0x885
private:
    unsigned char UnmodelledTail886[0xa];
#else
private:
    unsigned char UnmodelledPrefix[0x28];
    CSoundEventSet EventSet; // +0x2c, complete native embedded extent
    unsigned char UnmodelledTail[0xdc]; // +0x7b4 through +0x88f
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char LHAudioManagerSize[(sizeof(CSoundEventManager)==0x890)?1:-1];
typedef char LHAudioEventSize[(sizeof(CSoundEvent)==0xb4)?1:-1];
typedef char LHAudioSetSize[(sizeof(CSoundEventSet)==0x788)?1:-1];
typedef char LHAudioPositionSize[(sizeof(CSoundPosition)==16)?1:-1];
#endif
#endif
