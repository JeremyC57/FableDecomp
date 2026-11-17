#ifndef FABLE_LIBPK_CSOUNDEVENTSET_H
#define FABLE_LIBPK_CSOUNDEVENTSET_H
#include "libpk/PKCTRList.h"
#ifdef FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23
#include "libpk/PKCFrallocStatic.h"
#include "libpk/CPlaybackSlot.h"
#endif
#include "libpk/CSoundEventList.h"
#ifdef FABLE_RETAIL_CSOUNDEVENTSET_EVENTPOOL
#include "libpk/PKCFrallocStatic.h"
class CSoundEvent;
#endif
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
#ifdef FABLE_RETAIL_CSOUNDEVENTSET_EVENTPOOL
 // NF63 (round 7, row 0x00C120A0): the fixed event pool at set+0x30 (manager+0x5C; count +0x60, accessor
 // PKCFrallocStatic<CSoundEvent>::Get 0x00C13620, 0xB4-byte events), as the landed CSoundEventManager rows 0x00C123B0 / 0x00C125E0 /
 // 0x00C12680 / 0x00C12720 read it. Same 0x1C0 bytes as Unrecovered08.
 unsigned char Unrecovered08[0x28];
 PKCFrallocStatic<CSoundEvent> Events;
 unsigned char Unrecovered44[0x1c0-0x28-0x14];
#else
 unsigned char Unrecovered08[0x1c0];
#endif
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
#ifdef FABLE_RETAIL_CSOUNDEVENTSET_FINDWITHFIELD
 // NF63 (round 7, rows 0x00C12460 / 0x00C12520): the keyed finds the manager's StopEventsWithField10 / StopEventsWithField24 repeat
 // until null (each builds a key visitor, vtable 0x012B3848 at 0x00C21FA0, and runs it through slot 10 ForEach); descriptive names
 // after their callers (libpk has no PDB). Declarations only.
 CPooledResource* FindWithField24(void*);                            // 0x00C21FA0
 CPooledResource* FindWithField10(void*);                            // 0x00C21FF0
#endif
#ifdef FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
 // C21E20 searches child list+d8 then this via each genuine key slot17.
 CSoundEvent* FindInChildThenSelf(long);
#endif
};
#endif
