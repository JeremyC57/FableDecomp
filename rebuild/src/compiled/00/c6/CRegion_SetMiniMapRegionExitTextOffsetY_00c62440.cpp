// Vendor naming V2 (plan phase 1, one address, one name): 0x00C623F0 _FableRet_c623f0@4 (was _FableResolve_00c62440@4)
#include "rebuild_abi.h"
// CRegion::SetMiniMapRegionExitTextOffsetY @ 0x00c62440
// mov eax,[esp+4]; push eax; call helper; mov ecx,[eax+4]; mov eax,[ecx+0x38]; ret 4
// Resolve a handle via a __cdecl helper, then read a nested field.
struct CInner { char pad00[0x38]; long m_val; };
struct COuter { char pad00[4]; CInner *m_inner; };
extern "C" COuter * FABLE_STDCALL FableRet_c623f0(long handle);
class CRegion { public: long SetMiniMapRegionExitTextOffsetY(long handle); };
long CRegion::SetMiniMapRegionExitTextOffsetY(long handle)
{
    COuter *o = FableRet_c623f0(handle);
    return o->m_inner->m_val;
}