#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEBASE_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEBASE_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/CBaseClassNonCopyable.h"
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class CSimpleMemoryPool;
class CEngineSubPrimitiveBase:public CBaseClassNonCopyable {
public:
 CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent):Parent(parent),NextSubPrimitive(0){}
 virtual ~CEngineSubPrimitiveBase();
 static void* __fastcall operator new(unsigned int size);
 static void __fastcall operator delete(void*);
private:
 unsigned long RenderFlags;
 unsigned char Alpha;
 CEngineInternalPrimitiveBase* Parent;
 CEngineSubPrimitiveBase* NextSubPrimitive;
 CEnginePrimitiveManager* Manager;
 static CSimpleMemoryPool* MemoryPool;
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveBase)==24);
#endif
