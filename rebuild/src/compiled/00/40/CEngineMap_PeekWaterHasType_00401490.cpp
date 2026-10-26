// CEngineMap::PeekWaterHasType(int type) -- true when the water query for `type` reports nothing
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401490, 16 bytes. Callees: Water_QueryType (tail of the map's water lookup).
//
// Thin predicate over the map's water lookup. The underlying query returns a code for the requested
// water type and this member reports whether that code is zero. Two callers in the map code use it as a
// quick yes/no before doing heavier water work.
//
// Matching: `type` is forwarded unchanged and the result is folded with `test eax,eax; sete al`.
class CEngineMap;
// The map's water lookup; a zero result is what this predicate reports as "true".
extern int __cdecl Water_QueryType(CEngineMap* self, int type);

bool __fastcall CEngineMap_PeekWaterHasType(CEngineMap* self, int type)
{
    return Water_QueryType(self, type) == 0;
}
