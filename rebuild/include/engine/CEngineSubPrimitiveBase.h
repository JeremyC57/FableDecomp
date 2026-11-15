#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEBASE_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEBASE_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/CBaseClassNonCopyable.h"
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class CSimpleMemoryPool;
class CEngineSubPrimitiveBase:public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_CTOR_OUTOFLINE
 // Retail opt-in (agentC40, row 0x00BBDDD0): the constructor as a declaration, for a TU that calls the out-of-line copy 0x00B98A50
 // (landed; `push esi ; mov ecx,eax ; call 0x00B98A50` at 0x00BBE04D) instead of expanding it. Default unchanged.
 CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent);
#else
 CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent):Parent(parent),NextSubPrimitive(0){}
#endif
 #ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
 // Original FableWin module904 dtor has no own work; only the real base dtor call.
 // Evidence: SUBPRIMITIVE_DTOR_LOCALS.json; donor02E831E0/02E83240; retail deleting dtor00B9C130.
 // Implicit virtual destructor: native00B9C130 forwards straight to the real base.
#else
#ifndef FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS
 virtual ~CEngineSubPrimitiveBase();
#endif
#endif
 static void* __fastcall operator new(unsigned int size);
 static void __fastcall operator delete(void*);
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
 // Retail opt-in (chunk 2045, row 0x00BB8EF0): FableWin.pdb ?GetParentBase@CEngineSubPrimitiveBase@@QAEPAVCEngineInternalPrimitiveBase@@XZ
 // and ?PeekParentBase@CEngineSubPrimitiveBase@@QBEPBVCEngineInternalPrimitiveBase@@XZ, inline in retail (0x00BB8F0A `mov ebp,[eax+0Ch]`).
 CEngineInternalPrimitiveBase* GetParentBase() { return Parent; }
 const CEngineInternalPrimitiveBase* PeekParentBase() const { return Parent; }
#endif
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
 // Opt-in (chunk 2060, row 0x00BA5260; agentC2 row 0x00BB3E80): FableWin.pdb inline accessors ?PeekRenderFlags@CEngineSubPrimitiveBase@@QBEKXZ,
 // ?PeekAlpha@CEngineSubPrimitiveBase@@QBEEXZ, ?GetParentBase@ / ?PeekParentBase@ (9 debug instructions each = one member read); the data
 // are protected for the derived sub-primitives (FableWin ?GetParent@CEngineSubPrimitiveStaticMesh@@ is 9 instructions too).
 unsigned long PeekRenderFlags()const{return RenderFlags;}
 unsigned char PeekAlpha()const{return Alpha;}
#ifndef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
 CEngineInternalPrimitiveBase* GetParentBase(){return Parent;}
 const CEngineInternalPrimitiveBase* PeekParentBase()const{return Parent;}
#endif
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
FABLE_STATIC_ASSERT_AT(33, sizeof(CEngineSubPrimitiveBase)==24);
#endif
