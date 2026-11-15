// CTCCreatureGenerator::DrawDebugInfo(const CEnginePrimitivePrepareInfo&) -- draw counts, spawn-position links and vertical markers through persistent handles.
// Grade: RELOCATION_MATCH --1290/1290bytes; original parent and carried constructors independently audited.
// Retail: assigned0074BC30 lies16bytes into whole0074BC20..0074C12A,1290bytes, ret4.
// Leaf: ?DrawDebugInfo@CTCCreatureGenerator@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /d2inlT62
// Inline-budget: /d2inlT62 -- smallest exact threshold:61 keeps number formatting calls;62 expands them while retaining two line-constructor calls. Plain-inline1418 expands all four line constructors.
// Identity: original both-PDB member signature, real CTCCreatureGenerator fields and native debug/count/primitive calls.
#define FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CIENGINE
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CEnginePrimitiveText.h"
#include "engine/CIEngine.h"
#include "engine/CThing.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFIntToWideString.h"

extern bool DrawCreatureGeneratorDebug;

void CTCCreatureGenerator::DrawDebugInfo(const CEnginePrimitivePrepareInfo& info)
{
    if (!DrawCreatureGeneratorDebug)
        return;
    C3DVector centre=GetParentThing().GetPos();
    DebugPrimitives.resize((HiddenPositions.size()+OpenPositions.size())*2+1);
    CWideString label=L"Points (Open/Hidden): "+GFIntToWideString(OpenPositions.size())+L" / "+GFIntToWideString(HiddenPositions.size());
    C3DVector labelPos=centre+C3DVector(0,0,2);
    const CFontBank& font=PeekComponent().PeekDebugFont();
    unsigned int index=0;
    info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
        CEnginePrimitiveText(GetParentThing().GetUniqueID(),labelPos,1.0f,label,font,CRGBColour(255,255,255),true,ENGINE_INTERPOLATION_SET_DEFAULT),0);
    for (std::vector<C3DVector>::iterator it=OpenPositions.begin();it!=OpenPositions.end();++it) {
        CRGBColour openColour(0,255,0);
        CRGBColour openTopColour(128,255,128);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),centre,*it,openColour,0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),*it,*it+C3DVector(0,0,1),openTopColour,0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
    }
    for (std::vector<C3DVector>::iterator it=HiddenPositions.begin();it!=HiddenPositions.end();++it) {
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),centre,*it,CRGBColour(255,0,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),*it,*it+C3DVector(0,0,1),CRGBColour(255,128,128),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
    }
    for (std::vector<CEnginePrimitiveHandle>::iterator it=DebugPrimitives.begin()+index;it!=DebugPrimitives.end();++it)
        it->Clear();
}