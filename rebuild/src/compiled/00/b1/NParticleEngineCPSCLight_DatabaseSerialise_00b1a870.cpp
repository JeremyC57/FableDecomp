// NParticleEngine::CPSCLight::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) -- persist named light settings and user points.
// Grade: RELOCATION_MATCH_PENDING_MAIN_AUDIT -- 1310 bytes; normal speed-TU vector COMDAT differs from selected size-TU copy.
// COMDAT: the same header under native /Os /Oy- independently reproduces selected C3DVector transfer153 and its input34.
// Leaf: ?DatabaseSerialise@CPSCLight@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Retail: Fable.exe 0x00B1A870..0x00B1AD8E, 1310 bytes, virtual thiscall, ret 8.
// Layout: original Ego_r CPSCLight TPI0x60784 fields (native108, donor112), shared CPSCBase/handle/CArray storage.
// Shape: original repeated bool locals and long size; visible genuine persistence bodies remove an unnecessary bool load.
// Behavior: preserve the original pointer advance in the user-point key, so later keys are suffixes of LightUserPointsArray.
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CPSCBase.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPSCLight.h"
namespace NParticleEngine {
void CPSCLight::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    {
        bool temp = LightEnabled;
        context.Transfer("LightEnabled", temp);
        LightEnabled = temp;
    }
    if (context.IsSaving()) {
        context.Transfer("LightPositionParam", LightPositionParam);
    } else {
        CCharString temp;
        context.Transfer("LightPositionParam", temp);
        LightPositionParam = temp.GetCRC();
    }
    {
        bool temp = LightUseLifeSecs;
        context.Transfer("LightUseLifeSecs", temp);
        LightUseLifeSecs = temp;
    }
    context.Transfer("LightLifeSecs", LightLifeSecs);
    {
        bool temp = LightRespawns;
        context.Transfer("LightRespawns", temp);
        LightRespawns = temp;
    }
    context.Transfer("LightRespawnDelaySecs", LightRespawnDelaySecs);
    context.Transfer("LightStartRenderWorldRadius", LightStartRenderWorldRadius);
    context.Transfer("LightEndRenderWorldRadius", LightEndRenderWorldRadius);
    context.Transfer("LightAttenuationFactor", LightAttenuationFactor);
    context.Transfer("LightFadeInEndInteger", LightFadeInEndInteger);
    context.Transfer("LightFadeOutBeginInteger", LightFadeOutBeginInteger);
    {
        bool temp = LightWorldRadiusFadeEnable;
        context.Transfer("LightWorldRadiusFadeEnable", temp);
        LightWorldRadiusFadeEnable = temp;
    }
    context.Transfer("LightWorldRadiusFadeMinimum", LightWorldRadiusFadeMinimum);
    {
        bool temp = LightUseFadeColour;
        context.Transfer("LightUseFadeColour", temp);
        LightUseFadeColour = temp;
    }
    {
        bool temp = LightColourFadeEnable;
        context.Transfer("LightColourFadeEnable", temp);
        LightColourFadeEnable = temp;
    }
    context.Transfer("LightColourFadeMinimum", LightColourFadeMinimum, CRGBColour(0, 0, 0, 0xff));
    {
        bool temp = LightUseStartColour;
        context.Transfer("LightUseStartColour", temp);
        LightUseStartColour = temp;
    }
    {
        bool temp = LightUseMidColour;
        context.Transfer("LightUseMidColour", temp);
        LightUseMidColour = temp;
    }
    {
        bool temp = LightUseEndColour;
        context.Transfer("LightUseEndColour", temp);
        LightUseEndColour = temp;
    }
    context.Transfer("LightStartColour", LightStartColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightMidColour", LightMidColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightEndColour", LightEndColour, CRGBColour(0, 0, 0, 0xff));
    {
        bool temp = LightUseTimelineSecs;
        context.Transfer("LightUseTimelineSecs", temp);
        LightUseTimelineSecs = temp;
    }
    context.Transfer("LightStartTime", LightStartTime);
    context.Transfer("LightTimelineSecs", LightTimelineSecs);
    long size = (long)LightUserPointsArray.size();
    context.Transfer("LightUserPointsArraySize", size);
    LightUserPointsArray.resize(size);
    long i = 0;
    if (i < size) {
        C3DVector zero(0.0f, 0.0f, 0.0f);
        do {
            context.Transfer("LightUserPointsArray" + i, LightUserPointsArray[i], zero);
        } while (++i < size);
    }
}
}