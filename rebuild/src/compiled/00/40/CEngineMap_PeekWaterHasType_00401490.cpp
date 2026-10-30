// CEngineMap::PeekWaterHasType(int type) -- true when the water query for `type` reports nothing
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401490, 16 bytes. Callees: wcscmp 0x00BFE94C (MSVCR71 import thunk).
//
// Thin predicate over the map's water lookup. The underlying query returns a code for the requested
// water type and this member reports whether that code is zero. Two callers in the map code use it as a
// quick yes/no before doing heavier water work.
//
// Matching: `type` is forwarded unchanged and the result is folded with `test eax,eax; sete al`.
class CEngineMap;
// The callee at 0x00BFE94C is the MSVCR71 wcscmp import thunk (it was landed as an invented Water_QueryType):
// both register arguments are wide strings and the predicate is "the two compare equal".
extern "C" int __cdecl wcscmp(const unsigned short* a, const unsigned short* b);

bool __fastcall CEngineMap_PeekWaterHasType(CEngineMap* self, int type)
{
    return wcscmp((const unsigned short*)self, (const unsigned short*)type) == 0;
}
