#ifndef FABLE_LIBPK_CSOUNDEVENTSET_H
#define FABLE_LIBPK_CSOUNDEVENTSET_H
#include "libpk/PKCTRList.h"
#ifdef FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23
#include "libpk/PKCFrallocStatic.h"
#include "libpk/CPlaybackSlot.h"
#endif
#include "libpk/CSoundEventList.h"
#include "libpk/CHandleBasedMemoryPool_012b4098.h"
class CPooledResource;
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
class CSoundEvent;
#endif
struct CSoundEventManager;
// Native C22100's receiver is manager+2c and its pool is receiver+334.
// Native C26560 dispatches Add at receiver slot9; same genuine shared list protocol.
struct CSoundEventSet:public PKCTRList<CPooledResource> {
 CSoundEventSet();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 unsigned char Unrecovered08[0x1c0];
 CSoundEventList ListeningEvents;
#ifdef FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23
 // NativeC1C50B/C1C513 count atmanager+228; poolaccessC13700+10.
 unsigned char Unrecovered1d0[0x28];
 PKCFrallocStatic<CPlaybackSlot> Playbacks;
 unsigned char Unrecovered20c[0x128];
#else
 unsigned char Unrecovered1d0[0x164];
#endif
 CHandleBasedMemoryPool_012b4098 Pool;
 long AllocateHandle(CSoundEventManager*,long,long);
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
 // C21E20 searches child list+d8 then this via each genuine key slot17.
 CSoundEvent* FindInChildThenSelf(long);
#endif
};
#endif
