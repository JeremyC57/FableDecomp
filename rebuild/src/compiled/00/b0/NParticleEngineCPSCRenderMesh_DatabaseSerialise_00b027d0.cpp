// NParticleEngine::CPSCRenderMesh::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17: load/save the render-mesh component through the emitter database
// Leaf: ?DatabaseSerialise@CPSCRenderMesh@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B027D0, 5313 bytes, thiscall, `ret 8`.
// Identity: FableWin.pdb ?DatabaseSerialise@CPSCRenderMesh@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z,
//   slot 17 of vtable 0x0129F96C.
// Shape: saving, the mesh / trail bank indices go out as their symbol names (CSymbolMap::FindFirstSymbolWithVal 0x009E50C0 on the
//   "EMeshType2" / "EEngineGraphic" groups); loading, the names come back through SymbolExists 0x009E51C0 / GetSymbolSLONG 0x009E5120 and
//   RenderSizeParamName through CCRC::Calc 0x004014A0; old data with StartMeshAlpha / MidMeshAlpha / EndMeshAlpha builds white colours with
//   that alpha. Then the bools, the packed integers (defaults from CParticleEmitterManager::GetDefaultBlendMode / GetDefaultBlendOp,
//   inline) and the packed floats: unpacked (steps -> range, rounded to 2 places with GFRoundFloatNear), transferred, packed back.
// Callees: CPersistContext::Transfer overloads (CCharString, unsigned long, bool, long, float, CRGBColour), the CSymbolMap members
//   above, CCRC::Calc, GetParticleEmitterManager 0x00AEAAF0, EmptyCCharString.
// Header overlay: engine/CSymbolMap.h behind FABLE_RETAIL_CSYMBOLMAP (nested CSymbolInfo, FindFirstSymbolWithVal / SymbolExists /
//   GetSymbolSLONG declarations; default path unchanged).
// Retail-asm: 0x00B032BD `fistp dword ptr [esp+20h]` (and 34 more: GFRoundFloatNear and the pack step) -- GFFloatToLongNear, shared
//   helper engine/GFFloatToLongNear.h; VC7.1 emits no bare dword fistp from C++.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CRGBCOLOUR
#include "fable_string.h"
#include "engine/CSymbolMap.h"
#include "engine/CPersistContext.h"
#include "engine/CRGBColour.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
inline float __fastcall GFRoundFloatNear(float value, long numDecimalPlaces)
{
    float scale = 1.0f;
    for (long i = numDecimalPlaces; i != 0; --i)
        scale *= 10.0f;
    return (float)GFFloatToLongNear(value * scale) / scale;
}
static __forceinline float UnpackParam(unsigned long packed, float steps, float minValue, float maxValue)
{
    float unit = (float)packed * (1.0f / steps);
    return GFRoundFloatNear(unit * (maxValue - minValue) + minValue, 2);
}
static __forceinline unsigned long PackParam(float value, float minValue, float maxValue, float steps)
{
    float normalised = (GFRoundFloatNear(value, 2) - minValue) * (1.0f / (maxValue - minValue));
    return GFFloatToLongNear(normalised * steps);
}
static __forceinline void TransferParam(CPersistContext& context, const char* name, float& value, float defaultValue)
{
    context.Transfer(name, value, defaultValue);
}
namespace NParticleEngine {
class CParticleEmitterManager { public:
    const long GetDefaultBlendMode() const { return 2; }
    const long GetDefaultBlendOp() const { return 0; }
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();    // 0x00AEAAF0
class CEmitterDatabaseSerialiseInfo { public:
    const CSymbolMap& GraphicSymbols;
    const CSymbolMap& MeshSymbols;
};
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) = 0;
};
class CPSCRenderMesh : public CPSCBase {
public:
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour TrailStartColour;                        // +0x14
    CRGBColour TrailMidColour;                          // +0x18
    CRGBColour TrailEndColour;                          // +0x1c
    CRGBColour StartColour;                             // +0x20
    CRGBColour MidColour;                               // +0x24
    CRGBColour EndColour;                               // +0x28
    unsigned long BlendMode : 3;                        // +0x2c
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;
    unsigned long TrailLengthInteger : 12;              // +0x30
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long StartRenderSizeX : 11;                // +0x34
    unsigned long StartRenderSizeY : 11;
    unsigned long StartRenderSizeZ : 11;                // +0x38
    unsigned long CentredOnPos : 1;
    unsigned long AlphaFadeEnable : 1;
    unsigned long EndRenderSizeX : 11;
    unsigned long EndRenderSizeY : 11;                  // +0x3c
    unsigned long EndRenderSizeZ : 11;
    unsigned long SizeFadeEnable : 1;
    unsigned long FlickerEnable : 1;
    unsigned long TrailUseStartColour : 1;
    unsigned long TrailUseMidColour : 1;
    unsigned long TrailUseEndColour : 1;
    unsigned long UseRenderSizeParam : 1;
    unsigned long FlickerBias : 8;                      // +0x40
    unsigned long FlickerSpeed : 12;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long UseStartColour : 1;
    unsigned long SizeFadeMinimum : 7;                  // +0x44
    unsigned long UseMidColour : 1;
    unsigned long TrailWidth : 10;
    unsigned long UseEndColour : 1;
    unsigned long RenderSizeParam;                      // +0x48
    unsigned long PrimHandle[2];                        // +0x4c CEnginePrimitiveHandle
    long ChildPrimIndex;                                // +0x54
    unsigned long TrailPrimHandle[2];                   // +0x58
    long ChildTrailPrimIndex;                           // +0x60
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
#define TRANSFER_BOOL(name, member, def) { bool value = member; context.Transfer(name, value, def); member = value; }
#define TRANSFER_LONG(name, member, def) { long value = member; context.Transfer(name, value, def); member = value; }
void CPSCRenderMesh::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    if (context.IsSaving()) {
        CCharString bankName;
        const CSymbolMap::CSymbolInfo* symbol = info.MeshSymbols.FindFirstSymbolWithVal("EMeshType2", BankIndex);
        if (symbol)
            bankName = symbol->EntryName;
        context.Transfer("BankIndexName", bankName, EmptyCCharString());
        CCharString trailName;
        symbol = info.GraphicSymbols.FindFirstSymbolWithVal("EEngineGraphic", TrailBankIndex);
        if (symbol)
            trailName = symbol->EntryName;
        context.Transfer("TrailName", trailName, EmptyCCharString());
        context.Transfer("RenderSizeParam", RenderSizeParam, 0UL);
        TRANSFER_BOOL("UseStartColour", UseStartColour, true);
        TRANSFER_BOOL("UseMidColour", UseMidColour, false);
        TRANSFER_BOOL("UseEndColour", UseEndColour, false);
        context.Transfer("StartColour", StartColour, CRGBColour(255, 255, 255, 255));
        context.Transfer("MidColour", MidColour, CRGBColour(255, 255, 255, 255));
        context.Transfer("EndColour", EndColour, CRGBColour(255, 255, 255, 255));
    } else {
        CCharString bankName;
        context.Transfer("BankIndexName", bankName, EmptyCCharString());
        if (info.MeshSymbols.SymbolExists(bankName))
            BankIndex = info.MeshSymbols.GetSymbolSLONG(bankName);
        else
            BankIndex = 0;
        CCharString trailName;
        context.Transfer("TrailName", trailName, EmptyCCharString());
        if (info.GraphicSymbols.SymbolExists(trailName))
            TrailBankIndex = info.GraphicSymbols.GetSymbolSLONG(trailName);
        else
            TrailBankIndex = 0;
        CCharString renderSizeParamName;
        context.Transfer("RenderSizeParam", renderSizeParamName, EmptyCCharString());
        RenderSizeParam = CCRC::Calc(0, renderSizeParamName.GetAsCharArray(), renderSizeParamName.GetLength());
        long alpha;
        context.Transfer("StartMeshAlpha", alpha, -1L);
        if (alpha != -1) {
            StartColour = CRGBColour(255, 255, 255, (unsigned char)alpha);
            UseStartColour = true;
            context.Transfer("MidMeshAlpha", alpha, 255L);
            UseMidColour = true;
            MidColour = CRGBColour(255, 255, 255, (unsigned char)alpha);
            context.Transfer("EndMeshAlpha", alpha, 255L);
            EndColour = CRGBColour(255, 255, 255, (unsigned char)alpha);
            UseEndColour = true;
        } else {
            TRANSFER_BOOL("UseStartColour", UseStartColour, true);
            TRANSFER_BOOL("UseMidColour", UseMidColour, false);
            TRANSFER_BOOL("UseEndColour", UseEndColour, false);
            context.Transfer("StartColour", StartColour, CRGBColour(255, 255, 255, 255));
            context.Transfer("MidColour", MidColour, CRGBColour(255, 255, 255, 255));
            context.Transfer("EndColour", EndColour, CRGBColour(255, 255, 255, 255));
        }
    }
    context.Transfer("TrailStartColour", TrailStartColour, CRGBColour(255, 255, 255, 255));
    context.Transfer("TrailMidColour", TrailMidColour, CRGBColour(255, 255, 255, 255));
    context.Transfer("TrailEndColour", TrailEndColour, CRGBColour(255, 255, 255, 255));
    TRANSFER_BOOL("UseRenderSizeParam", UseRenderSizeParam, false);
    TRANSFER_BOOL("CentredOnPos", CentredOnPos, false);
    TRANSFER_BOOL("AlphaFadeEnable", AlphaFadeEnable, false);
    TRANSFER_BOOL("SizeFadeEnable", SizeFadeEnable, false);
    TRANSFER_BOOL("FlickerEnable", FlickerEnable, false);
    TRANSFER_BOOL("TrailUseStartColour", TrailUseStartColour, true);
    TRANSFER_BOOL("TrailUseMidColour", TrailUseMidColour, false);
    TRANSFER_BOOL("TrailUseEndColour", TrailUseEndColour, false);
    TRANSFER_LONG("FadeInEndInteger", FadeInEndInteger, 0L);
    TRANSFER_LONG("FadeOutBeginInteger", FadeOutBeginInteger, 1000L);
    TRANSFER_LONG("FlickerMinAlphaInteger", FlickerMinAlphaInteger, 0L);
    TRANSFER_LONG("FlickerMinSizeInteger", FlickerMinSizeInteger, 0L);
    TRANSFER_LONG("TrailLengthInteger", TrailLengthInteger, 1000L);
    TRANSFER_LONG("BlendMode", BlendMode, GetParticleEmitterManager().GetDefaultBlendMode());
    TRANSFER_LONG("BlendOp", BlendOp, GetParticleEmitterManager().GetDefaultBlendOp());
    TRANSFER_LONG("TrailBlendMode", TrailBlendMode, GetParticleEmitterManager().GetDefaultBlendMode());
    TRANSFER_LONG("TrailBlendOp", TrailBlendOp, GetParticleEmitterManager().GetDefaultBlendOp());
    {
        float value = GFRoundFloatNear((float)AlphaFadeMinimum * (1.0f / 127.0f), 2);
        TransferParam(context, "AlphaFadeMinimum", value, 1.0f);
        AlphaFadeMinimum = PackParam(value, 0.0f, 1.0f, 127.0f);
    }
    {
        float value = UnpackParam(SizeFadeMinimum, 127.0f, 0.0f, 1.0f);
        TransferParam(context, "SizeFadeMinimum", value, 1.0f);
        SizeFadeMinimum = PackParam(value, 0.0f, 1.0f, 127.0f);
    }
    {
        float value = UnpackParam(StartRenderSizeX, 2047.0f, 0.0f, 20.0f);
        TransferParam(context, "StartRenderSize", value, 1.0f);
        StartRenderSizeX = PackParam(value, 0.0f, 20.0f, 2047.0f);
    }
    float defaultSize = UnpackParam(StartRenderSizeX, 2047.0f, 0.0f, 20.0f);
    {
        float value = UnpackParam(StartRenderSizeY, 2047.0f, 0.0f, 20.0f);
        TransferParam(context, "StartRenderSizeY", value, defaultSize);
        StartRenderSizeY = PackParam(value, 0.0f, 20.0f, 2047.0f);
    }
    {
        float value = UnpackParam(StartRenderSizeZ, 2047.0f, 0.0f, 20.0f);
        TransferParam(context, "StartRenderSizeZ", value, defaultSize);
        StartRenderSizeZ = PackParam(value, 0.0f, 20.0f, 2047.0f);
    }
    {
        float value = UnpackParam(EndRenderSizeX, 2047.0f, 0.0f, 20.0f);
        TransferParam(context, "EndRenderSize", value, 1.0f);
        EndRenderSizeX = PackParam(value, 0.0f, 20.0f, 2047.0f);
    }
    defaultSize = UnpackParam(EndRenderSizeX, 2047.0f, 0.0f, 20.0f);
    {
        float value = UnpackParam(EndRenderSizeY, 2047.0f, 0.0f, 20.0f);
        TransferParam(context, "EndRenderSizeY", value, defaultSize);
        EndRenderSizeY = PackParam(value, 0.0f, 20.0f, 2047.0f);
    }
    {
        float value = UnpackParam(EndRenderSizeZ, 2047.0f, 0.0f, 20.0f);
        TransferParam(context, "EndRenderSizeZ", value, defaultSize);
        EndRenderSizeZ = PackParam(value, 0.0f, 20.0f, 2047.0f);
    }
    {
        float value = UnpackParam(FlickerSpeed, 4095.0f, 0.0f, 30.0f);
        TransferParam(context, "FlickerSpeed", value, 15.0f);
        FlickerSpeed = PackParam(value, 0.0f, 30.0f, 4095.0f);
    }
    {
        float value = UnpackParam(FlickerBias, 255.0f, -1.0f, 1.0f);
        TransferParam(context, "FlickerBias", value, 0.0f);
        FlickerBias = PackParam(value, -1.0f, 1.0f, 255.0f);
    }
    {
        float value = UnpackParam(TrailWidth, 1023.0f, 0.0f, 10.0f);
        TransferParam(context, "TrailWidth", value, 0.5f);
        TrailWidth = PackParam(value, 0.0f, 10.0f, 1023.0f);
    }
}
}