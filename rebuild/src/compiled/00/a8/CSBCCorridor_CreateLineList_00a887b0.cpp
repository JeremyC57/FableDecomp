// Leaf: ?CreateLineList@CSBCCorridor@@UAEXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Flags: /Gr
#define FABLE_RETAIL_SB_CORRIDOR_PASS29
#define FABLE_RETAIL_CSB
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CSBCCorridor.h"

void CSBCCorridor::CreateLineList(std::vector<C3DVector>& line_list)
{
    for (std::list<CWall>::iterator wall = Walls.begin(); wall != Walls.end(); ++wall) {
        C3DVector start;
        C3DVector end;
        start.X = wall->Point1.X;
        start.Y = wall->Point1.Y;
        start.Z = 0.0f;
        end.X = wall->Point2.X;
        end.Y = wall->Point2.Y;
        end.Z = 0.0f;
        line_list.push_back(start);
        line_list.push_back(end);
    }
}
