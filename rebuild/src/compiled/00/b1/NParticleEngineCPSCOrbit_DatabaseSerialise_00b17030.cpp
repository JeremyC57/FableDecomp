// 00b17030 NParticleEngine::CPSCOrbit::DatabaseSerialise
// Leaf: ?DatabaseSerialise@CPSCOrbit@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: none
// Original FableWin direct call signatures independently confirm only Expand/CycleFlag/CycleTime use explicit defaults.
// Retail: Fable.exe00b17030,991bytes,ret8.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/NParticleEngine_CPSCOrbit.h"
CCharString FABLE_FASTCALL GFIntToCharString(long);
namespace NParticleEngine {
void CPSCOrbit::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    if (context.IsSaving()) {
        context.Transfer("CentreParam", CentreParam);
    } else {
        CCharString temp;
        context.Transfer("CentreParam", temp);
        CentreParam = temp.GetCRC();
    }
    for (long c_orbit = 0; c_orbit < 3; ++c_orbit) {
        COrbitSettings& orbit=Orbits[c_orbit];
        CCharString postfix = GFIntToCharString(c_orbit);
        orbit.RotateSpeed *= 1.0f / 15.0f;
        orbit.RotateSpeedRandom *= 1.0f / 15.0f;
        context.Transfer("Type" + postfix, orbit.Type);
        context.Transfer("Enabled" + postfix, orbit.Enabled);
        context.Transfer("Radius" + postfix, orbit.Radius);
        context.Transfer((const char*)("Expand" + postfix), orbit.Expand,0.0f);
        context.Transfer((const char*)("CycleFlag" + postfix), orbit.CycleFlag,false);
        context.Transfer((const char*)("CycleTime" + postfix), orbit.CycleTime, 5.0f);
        context.Transfer("SqueezeScale" + postfix, orbit.SqueezeScale);
        context.Transfer("SqueezeAngle" + postfix, orbit.SqueezeAngle);
        context.Transfer("RotateSpeed" + postfix, orbit.RotateSpeed);
        context.Transfer("RotateStart" + postfix, orbit.RotateStart);
        context.Transfer("RotateSpeedRandom" + postfix, orbit.RotateSpeedRandom);
        context.Transfer("RotateStartRandom" + postfix, orbit.RotateStartRandom);
        orbit.RotateSpeed *= 15.0f;
        orbit.RotateSpeedRandom *= 15.0f;
    }
}
}