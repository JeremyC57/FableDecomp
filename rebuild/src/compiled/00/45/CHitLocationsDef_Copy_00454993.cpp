// NF48 (round 7): the +0x28 member is Ego_r.pdb CHitLocationsDef::HitLocations, a STLport 4.6.2 std::vector<long> from the shared
// stlport/fable_containers.h (was the stand-in CSub::SubCopy); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares vector<long>::operator=
// (0x00454886) out of line.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
class CHitLocationsDef { public:
    unsigned char _base[0x28];                          // the CDefClassBase part (retail 0x25 packed, padded)
    std::vector<long> HitLocations;                     // +0x28 (Ego_r.pdb CHitLocationsDef::HitLocations)
};

void __fastcall CHitLocationsDef_Copy(CHitLocationsDef* self, int /*edx*/, const CHitLocationsDef* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->HitLocations = src->HitLocations;
}