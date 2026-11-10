#ifndef FABLE_ENGINE_CENGINEPRIMITIVEOCCLUSIONINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVEOCCLUSIONINFO_H
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CArray.h"
class CEnginePrimitiveOcclusionInfo:public CIVCountedPointeeBase {public:CEnginePrimitiveOcclusionInfo();virtual ~CEnginePrimitiveOcclusionInfo();private:long InvisibleFrames;CArray<unsigned long> OcclusionTestHandles;bool Seen;unsigned long TestFrame;};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveOcclusionInfo)==0x20);
#endif
