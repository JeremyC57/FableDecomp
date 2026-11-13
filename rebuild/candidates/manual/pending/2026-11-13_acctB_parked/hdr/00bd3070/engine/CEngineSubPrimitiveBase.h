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
 // Opt-in (chunk 4010 scout, row 0x00BD3070): FableWin.pdb inline accessors (no Ego_r.pdb / Fable.exe out-of-line copy; debug sizes:
 // PeekRenderFlags 9i/17b, PeekAlpha 9i/17b, GetParentBase 9i/17b, PeekParentBase 9i/17b, SetRenderFlags 10i/22b, SetAlpha 10i/22b,
 // GetAlphaAsFloat 14i/38b). RenderMeshPrimitive reads RenderFlags +0x04 (0x00BD33E5), Alpha +0x08 (0x00BD33E8, 0x00BD4599 x 1/255),
 // Parent +0x0c. GetAlphaAsFloat's body (Alpha * (1/255), 0x01231724) is FITTED, check with fwsize. No layout change.
 unsigned long PeekRenderFlags()const{return RenderFlags;}
 unsigned char PeekAlpha()const{return Alpha;}
 CEngineInternalPrimitiveBase* GetParentBase(){return Parent;}
 const CEngineInternalPrimitiveBase* PeekParentBase()const{return Parent;}
 void SetRenderFlags(unsigned long flags){RenderFlags=flags;}
 void SetAlpha(unsigned char alpha){Alpha=alpha;}
 float GetAlphaAsFloat()const{return Alpha*(1.0f/255.0f);}
#endif
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
