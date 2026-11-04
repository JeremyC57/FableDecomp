// CSoundMap::operator=(const CSoundMap& other) -- member-wise assignment: each CVectorMap's vector (STLport operator=, out of line), its empty comparator and its Dirty flag
// Leaf: ??4CSoundMap@@QAEAAV0@ABV0@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CFAD, 49 bytes, thiscall `ret 4` (Ego_r.pdb ??4CSoundMap@@QAEAAV0@ABV0@@Z).
// Callees: std::vector<std::pair<unsigned long, CSoundPair> >::operator= 0x0045CFDE, std::vector<std::pair<unsigned long,
//   CDefString> >::operator= 0x0045D183 (STLport 4.6 _vector.c, out of line in this TU).
// Layout: Ego_r.pdb CSoundMap = { CVectorMap<unsigned long, CSoundPair> SoundMap +0, CVectorMap<unsigned long, CDefString>
//   CriteriaMap }: retail CVectorMap is 0x10 (12-byte vector, Compare +0xc, Dirty +0xd), so CriteriaMap is at +0x10.
// Shape: a size-optimised TU (the unaligned entry 0x0045CFAD): optimize("s"). The CVectorMap assignment is expanded at both
//   member sites (FABLE_RETAIL_CVECTORMAP_ASSIGN, __forceinline: the implicit one is a called 33-byte COMDAT under /Os), the
//   second map's addresses formed once (`add edi,10h` / `lea ebx,[esi+10h]`), the last Dirty byte copied through al before
//   the pops (the non-inline /O2 forms copy it through cl after `mov eax,esi`).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CVECTORMAP_ASSIGN
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CVectorMap.h"
#include "engine/CDefString.h"
class CSoundPair { public: long FirstSound, LastSound; };   // Ego_r.pdb, 8 bytes
class CSoundMap {
public:
    CVectorMap<unsigned long, CSoundPair> SoundMap;       // +0x00
    CVectorMap<unsigned long, CDefString> CriteriaMap;    // +0x10
    CSoundMap& operator=(const CSoundMap& other);
};

CSoundMap& CSoundMap::operator=(const CSoundMap& other)
{
    SoundMap = other.SoundMap;
    CriteriaMap = other.CriteriaMap;
    return *this;
}