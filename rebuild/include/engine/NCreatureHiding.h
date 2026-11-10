#ifndef FABLE_ENGINE_NCREATUREHIDING_H
#define FABLE_ENGINE_NCREATUREHIDING_H
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
class C3DVector;class CWorldMap;class CThingSearchTools;class CTCShapeManager;
// Both original PDBs, native0074a5d0, original025127e0.
namespace NCreatureHiding {
void FABLE_FASTCALL GetValidPositions(std::vector<C3DVector>&,const CWorldMap&,const CThingSearchTools&,const C3DVector&,float,bool,const CTCShapeManager*);
}
#endif
