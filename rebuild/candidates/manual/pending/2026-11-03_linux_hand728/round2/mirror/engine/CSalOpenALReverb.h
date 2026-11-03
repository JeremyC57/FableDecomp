#ifndef FABLE_CSAL_OPENAL_REVERB_H
#define FABLE_CSAL_OPENAL_REVERB_H
#include "CodaEnvironment.h"
class CSalPlatform;
// Layout: SDK class names and hierarchy, native vtable012b95c4 has eight slots.
// Retail omits the later SDK's CCodaObject list base on this mix-only hierarchy.
class CSalReverb:public CCodaObjectBase {
public:
 virtual ~CSalReverb();
 virtual int Create(CSalPlatform*);
 virtual void Release();
 virtual bool IsSoftware()const;
 virtual void AddSwInput(const int*);
 virtual void Update(const CCodaEnvironment*)=0;
 virtual void Process()=0;
protected:bool m_bActive;
};
class CSalBaseReverb:public CSalReverb {
public:
 virtual ~CSalBaseReverb();
 virtual void GetFrame(short*);
protected:
 // Native ctor00c6ad70 points +21c at the aligned mix buffer inside this storage.
 unsigned char Opaque008[0x214];
 int* m_pMixBuffer;
};
// Layout: native ctor00c71710 adds no data; factory allocates0x220 bytes.
class CSalOpenALReverb:public CSalBaseReverb {
public:
 virtual ~CSalOpenALReverb();
 virtual int Create(CSalPlatform*);
 virtual void Release();
 virtual void Update(const CCodaEnvironment*);
 virtual void Process();
 virtual void GetFrame(short*);
};
#endif
