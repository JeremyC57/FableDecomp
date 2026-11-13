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
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
 // Opt-in (chunk 2060, row 0x00BA5260 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive): FableWin.pdb inline accessors
 // ?PeekRenderFlags@CEngineSubPrimitiveBase@@QBEKXZ, ?PeekAlpha@CEngineSubPrimitiveBase@@QBEEXZ, ?GetParentBase@CEngineSubPrimitiveBase@@QAEPAVCEngineInternalPrimitiveBase@@XZ,
 // ?PeekParentBase@...@@QBEPBV... (9 debug instructions each = one member read). The derived ?GetParent@CEngineSubPrimitiveStaticMesh@@ is
 // also 9 instructions (no call to GetParentBase), so the data are protected for the derived sub-primitives.
 unsigned long PeekRenderFlags()const{return RenderFlags;}
 unsigned char PeekAlpha()const{return Alpha;}
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ALPHAASFLOAT
 // Nested opt-in (bwf1, row 0x00BB3E80): FableWin.pdb ?GetAlphaAsFloat@CEngineSubPrimitiveBase@@QBEMXZ (the byte alpha times 1/255,
 // the float pool 0x01231724 as the colour channels; retail 0x00BB40EB..0x00BB4110).
 float GetAlphaAsFloat()const{return Alpha*(1.0f/255.0f);}
#endif
 CEngineInternalPrimitiveBase* GetParentBase(){return Parent;}
 const CEngineInternalPrimitiveBase* PeekParentBase()const{return Parent;}
protected:
#else
private:
#endif
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
