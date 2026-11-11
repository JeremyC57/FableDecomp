#ifndef FABLE_ENGINE_CTCNOISEDEF_H
#define FABLE_ENGINE_CTCNOISEDEF_H
#include "engine/CSubDefClassBase.h"
#ifdef _MSC_VER
enum ENoiseType;
#else
enum ENoiseType{};
#endif
class CPersistContext;
// Layout: complete original 0x1780d, both PDBs; size52.
class CTCNoiseDef:public CSubDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 ENoiseType Type;
 float LifeTime;
 float DistanceCanBeHeardFrom;
 virtual void Transfer(CPersistContext & p0);
 CTCNoiseDef(const CTCNoiseDef & p0);
 CTCNoiseDef();
 virtual ~CTCNoiseDef();
 CTCNoiseDef & operator=(const CTCNoiseDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCNoiseDef)==52);
#endif
