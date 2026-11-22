// 008008b0 CTCPreCalculatedNavigationRoute::LoadRoute
// Leaf: ?LoadRoute@CTCPreCalculatedNavigationRoute@@AAEXAAVCPersistContext@@J@Z
// Flags: none
#define FABLE_RETAIL_CTCPRECALCULATEDNAVIGATIONROUTE_LOAD_PASS26
#define FABLE_RETAIL_CWORLDMAP_LOCAL_TO_WORLD_VECTOR_PASS26
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_VECTOR_DEFAULT_PASS26
#define FABLE_RETAIL_CPERSISTTRAITS_ROUTE_BYTE_READER_PASS26
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_LONG_BINARY_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#include "engine/CThing.h"
#include "engine/CTCPreCalculatedNavigationRoute.h"
#include "engine/CWorldMap.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

#include "engine/CPersistContext.h"
#include "engine/fable_script_text.h"

void CTCPreCalculatedNavigationRoute::LoadRoute(CPersistContext& context, long version)
{
    long map_index = GetParentThing().GetCurrentMapNumber();
    long number_of_steps_on_route;
    context.Transfer("NumberOfStepsOnRoute", number_of_steps_on_route);
    Route.clear();
    Route.reserve(number_of_steps_on_route);
    for (long c1 = 0; c1 < number_of_steps_on_route; ++c1) {
        C2DVector position;
        context.Transfer("NavPosition" + GFIntToCharString(c1), position);
        if (version >= 2)
            position = PeekWorldMap().ConvertLocalCoordsToWorldMapCoords(map_index, position);
        unsigned char layer;
        context.Transfer("NavLayer" + GFIntToCharString(c1), layer);
        Route.push_back(std::make_pair(position, layer));
    }
}