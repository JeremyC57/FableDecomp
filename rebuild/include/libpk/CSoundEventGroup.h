#ifndef FABLE_LIBPK_CSOUNDEVENTGROUP_H
#define FABLE_LIBPK_CSOUNDEVENTGROUP_H
#include "rebuild_abi.h"
#include <stddef.h>
#ifdef FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23
#include "libpk/CSoundPosition.h"
#endif
struct CSoundEventManager;
class CSoundEventGroup;
// Descriptive audio-library identities: original names absent from all three PDBs.
// Native C20390 / C345B0: three coordinate words and a byte flag, sizeof16.
// Only used by the proven virtual prefix; no new implementation is supplied.
struct CSoundPosition;
#ifdef FABLE_RETAIL_CSOUNDEVENTGROUP
// Layout: native C21BA0 table012B3674; C23A1E array stride0xB4.
// The first nine virtual slots are declared; later unused slots are not modeled.
class CSoundEvent {
public:
    virtual long* GetIndexSlot();
    virtual void ResetLevel(CSoundEventManager* manager);
    virtual void SetUserGain(CSoundEventManager* manager,float gain);
    virtual void SetPlaybackParameter(CSoundEventManager* manager,float value);
    virtual void ResetPlaybackState(CSoundEventManager* manager);
    virtual void Set3DPosition(CSoundEventManager* manager,const CSoundPosition& position);
    virtual void SetPositionParameters(CSoundEventManager* manager,float first,float second);
    virtual void ApplyPan(CSoundEventManager* manager,float pan);
    virtual bool GetPositionFlag(CSoundEventManager* manager);
    unsigned char UnrecoveredState04[8];
    long Index;
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
    unsigned char UnrecoveredState10[4];
    unsigned long Priority;
    unsigned char UnrecoveredState18[16];
    long Pending;
    unsigned char UnrecoveredState2c[4];
    bool IsAudible(CSoundEventManager*);
    void Stop(CSoundEventManager*);
    void RestartPlayback(CSoundEventManager*);
#else
    unsigned char UnrecoveredState10[32];
#endif
    CSoundEventGroup* Group;
    CSoundEvent* Next;
#ifdef FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23
    CQuadHolder PositionHolder;
    void* PPlayingData;
    unsigned char UnrecoveredState4c[0x68];
#else
    unsigned char UnrecoveredState38[124];
#endif
};
// Layout: C240BF array stride12; ctorC29F50 vptr0, Index4=-1, First8=null.
// Slots012B40A4..012B40C4 correspond exactly to the concrete event prefix.
class CSoundEventGroup {
public:
    virtual long* GetIndexSlot();
    virtual void ResetLevel(CSoundEventManager* manager);
    virtual void SetUserGain(CSoundEventManager* manager,float gain);
    virtual void SetPlaybackParameter(CSoundEventManager* manager,float value);
    virtual void ResetPlaybackState(CSoundEventManager* manager);
    virtual void Set3DPosition(CSoundEventManager* manager,const CSoundPosition& position);
    virtual void SetPositionParameters(CSoundEventManager* manager,float first,float second);
    virtual void ApplyPan(CSoundEventManager* manager,float pan);
    virtual bool GetPositionFlag(CSoundEventManager* manager);
    long Index;
    CSoundEvent* First;
};
FABLE_STATIC_ASSERT_AT(48, sizeof(CSoundEvent)==0xB4);
FABLE_STATIC_ASSERT_AT(49, offsetof(CSoundEvent,Index)==0x0C);
FABLE_STATIC_ASSERT_AT(50, offsetof(CSoundEvent,Group)==0x30);
FABLE_STATIC_ASSERT_AT(51, offsetof(CSoundEvent,Next)==0x34);
FABLE_STATIC_ASSERT_AT(52, sizeof(CSoundEventGroup)==12);
FABLE_STATIC_ASSERT_AT(53, offsetof(CSoundEventGroup,Index)==4);
FABLE_STATIC_ASSERT_AT(54, offsetof(CSoundEventGroup,First)==8);
#endif
#endif
