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
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_RENDERER_FRIEND
 // Opt-in (chunk 1014, row 0x00B84720 CEngineSubPrimitiveRenderer::AddSubPrimitive): the renderer stores the manager straight into
 // +0x14 (`mov [esi+14h],ecx`); neither PDB has a Manager setter, so the renderer is a friend. No layout change.
 friend class CEngineSubPrimitiveRenderer;
#endif
 static CSimpleMemoryPool* MemoryPool;
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveBase)==24);
#endif
