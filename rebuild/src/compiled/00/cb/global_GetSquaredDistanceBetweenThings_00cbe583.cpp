// 00cbe583 GetSquaredDistanceBetweenThings
// Leaf: ?GetSquaredDistanceBetweenThings@@YIMABVCScriptThing@@0@Z
// Identity: both PDB name sets; neighbour distance helpers and two GetPos virtual slot6 calls.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
float FABLE_FASTCALL GetSquaredDistanceBetweenThings(const CScriptThing& a,const CScriptThing& b)
{
    return (a.GetPos()-b.GetPos()).GetSquaredMagnitude();
}