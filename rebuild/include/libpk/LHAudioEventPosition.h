// Native LHAudio types; descriptive spellings, see NAMING_EVIDENCE.md.
#ifndef FABLE_RETAIL_LHAUDIO_EVENT_POSITION_H
#define FABLE_RETAIL_LHAUDIO_EVENT_POSITION_H
class CSoundEventManager;
struct CSoundPositionVector { float X, Y, Z; };
struct CSoundPosition {
    CSoundPositionVector Position;
    bool Relative;
    CSoundPosition(); // 00c20390
};
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
private:
    unsigned char UnmodelledMembers[0xb0];
};
class CSoundEventSet {
    unsigned char UnmodelledMembers[0x788];
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
private:
    unsigned char UnmodelledPrefix[0x28];
    CSoundEventSet EventSet; // +0x2c, complete native embedded extent
    unsigned char UnmodelledTail[0xdc]; // +0x7b4 through +0x88f
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char LHAudioManagerSize[(sizeof(CSoundEventManager)==0x890)?1:-1];
typedef char LHAudioEventSize[(sizeof(CSoundEvent)==0xb4)?1:-1];
typedef char LHAudioSetSize[(sizeof(CSoundEventSet)==0x788)?1:-1];
typedef char LHAudioPositionSize[(sizeof(CSoundPosition)==16)?1:-1];
#endif
#endif
