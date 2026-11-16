#ifndef FABLE_LIBPK_CSOUNDEVENTLIST_H
#define FABLE_LIBPK_CSOUNDEVENTLIST_H
#include "libpk/PKCTRList.h"
class CSoundEvent;
class CSampleRef;
// Descriptive concrete keyed list. Shared family slots0..15 and native C1128B slot17.
class CSoundEventList:public PKCTRList<CSoundEvent> {
public:
 CSoundEventList();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(CSoundEventList*);
 virtual CSoundEvent* FindByKey(const CSampleRef*);
};
#endif
