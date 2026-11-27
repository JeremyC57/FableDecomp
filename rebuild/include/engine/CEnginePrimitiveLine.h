#ifndef FABLE_ENGINE_CENGINEPRIMITIVELINE_H
#define FABLE_ENGINE_CENGINEPRIMITIVELINE_H
#if defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT) || defined(FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG)
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineRenderEnums.h"
#endif
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
class CEnginePrimitiveLine:public CEnginePrimitive {
public:
#ifdef FABLE_RETAIL_DEBUG_BONE_PASS29
 CEnginePrimitiveLine() : CEnginePrimitive(ENGINE_PRIMITIVE_LINE,0,(EEngineInterpolationSet)0), UseZBuffer(true) {}
#endif
#if defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT) || defined(FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG)
 // Original donor01d06250: base(TYPE_LINE,id,set), Start/End/Colour copy ctors, then these scalar initialisers.
 // Real visible body reads every reference input; no writes through input references.
 CEnginePrimitiveLine(unsigned __int64 id,const C3DVector& start,const C3DVector& end,const CRGBColour& colour,float width,EEnginePrimitiveBlendMode blend_mode,unsigned long bank_index,bool use_z,EEngineInterpolationSet interpolation_set,EEnginePrimitiveBlendOp blend_op)
 :CEnginePrimitive(ENGINE_PRIMITIVE_LINE,id,interpolation_set),Start(start),End(end),Colour(colour),Width(width),BankIndex(bank_index),BlendMode(blend_mode),BlendOp(blend_op),UseZBuffer(use_z){}
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR)
 // Retail opt-in (p25N, row 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering): the constructor is an inline header
 // function in the original (Ego_d.pdb holds it once, under tc_graphic_appearance.obj; FableWin.pdb emits its COMDAT in several objects;
 // retail keeps one out-of-line copy, 0x004C3090, landed). Plain inline and visible: c2 still calls it out of line (as retail does at
 // every 0x00833C00 site), but with the body in view it copies the end point's X / Y through ecx / edx and stores Z first.
 // Needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR (the original three-argument base constructor; the id is unused).
 CEnginePrimitiveLine(unsigned __int64 id,const C3DVector& start,const C3DVector& end,const CRGBColour& colour,float width,EEnginePrimitiveBlendMode blend_mode,unsigned long bank_index,bool use_z,EEngineInterpolationSet interpolation_set,EEnginePrimitiveBlendOp blend_op)
 :CEnginePrimitive((EEnginePrimitiveType)6,id,interpolation_set),Start(start),End(end),Colour(colour),Width(width),BankIndex(bank_index),BlendMode(blend_mode),BlendOp(blend_op),UseZBuffer(use_z){}
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE)
 // Retail opt-in (wf3, row 0x00761980): the inline constructor's real body (landed 0x004C3090: type 6, interpolation set, the three
 // cleared channel bytes, Start/End/Colour copy constructors, then the scalars in declaration order) VISIBLE to the caller but kept
 // out of line, as retail's CTCShapeManager::DrawShapes calls 0x004C3090 while the same TU expands it in 0x007617A0.
 __declspec(noinline) CEnginePrimitiveLine(unsigned __int64 id,const C3DVector& start,const C3DVector& end,const CRGBColour& colour,float width,EEnginePrimitiveBlendMode blend_mode,unsigned long bank_index,bool use_z,EEngineInterpolationSet interpolation_set,EEnginePrimitiveBlendOp blend_op)
 :CEnginePrimitive((EEnginePrimitiveType)6,interpolation_set),Start(start),End(end),Colour(colour),Width(width),BankIndex(bank_index),BlendMode(blend_mode),BlendOp(blend_op),UseZBuffer(use_z){}
#else
 CEnginePrimitiveLine(unsigned __int64,const C3DVector&,const C3DVector&,const CRGBColour&,float,EEnginePrimitiveBlendMode,unsigned long,bool,EEngineInterpolationSet,EEnginePrimitiveBlendOp);
#endif
 C3DVector Start,End;
 CRGBColour Colour;
 float Width;
 unsigned long BankIndex;
 EEnginePrimitiveBlendMode BlendMode;
 EEnginePrimitiveBlendOp BlendOp;
 bool UseZBuffer;
};
FABLE_STATIC_ASSERT_AT(45, sizeof(CEnginePrimitiveLine)==60);
#endif
