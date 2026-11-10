#ifndef FABLE_CSAL_VOICE_H
#define FABLE_CSAL_VOICE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CCodaObject.h"
#include "engine/CodaSourceTypes.h"
class CCodaSource;struct _GUID;
class CSalVoice:public CCodaObject {
public: virtual int Create(const CCodaSource *);
public: virtual int SetBuffer(const CCodaSource *);
public: virtual bool IsVoice() const;
public: virtual bool IsSoftware() const;
public: virtual bool Is3D() const;
public: virtual void Release();
public: virtual unsigned int PlayBuffer(const CCodaSource *);
public: virtual unsigned int PlayQueue(CCodaSource *,CREFTIME );
public: virtual unsigned int Update(CCodaSource *,CREFTIME );
public: virtual unsigned int Resume(CREFTIME );
public: virtual bool Stop(CREFTIME );
public: virtual void ReleaseEnvelopes(CREFTIME );
public: virtual unsigned int GetPlayCursor() const;
public: virtual void SetPlayCursor(unsigned int );
public: virtual int EAXSet(const _GUID *,int ,void * ,int );
public: virtual int EAXGet(const _GUID *,int ,void * ,int );
public: virtual ~CSalVoice();
};
#endif
