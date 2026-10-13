// CWorldMap::GetNextRegionOnRouteTo() -- no stack arguments in retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8A920, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov ecx,[ecx+48h] ; test ecx,ecx ; jne F ; mov eax,-131 ; ret ; F: jmp Router::Route
// engine/CWorldMap.h: the route object lives at +0x48. Without one the sentinel -131 (0xFFFFFF7D,
// "no region") is returned; otherwise the query is forwarded by tail call to the router's
// no-argument __thiscall @ 0x00C8F050 (landed under a propagated label).
#include "engine/CWorldMap.h"

struct CRegionRouterCalls {
    long Route();                           // 0x00C8F050
};

struct CWorldMap_Methods : CWorldMap {
    long GetNextRegionOnRouteTo();
};

long CWorldMap_Methods::GetNextRegionOnRouteTo()
{
    // `ret` (not `ret 8`): this body takes no stack arguments despite the manifest's (long, long)
    // prototype, and the tail-called router method takes none either.
    CRegionRouterCalls* router = *reinterpret_cast<CRegionRouterCalls**>(reinterpret_cast<unsigned char*>(this) + 0x48);
    if (router == 0)
        return -131;
    return router->Route();
}
