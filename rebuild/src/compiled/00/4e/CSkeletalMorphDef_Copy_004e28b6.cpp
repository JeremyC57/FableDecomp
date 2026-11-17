// CSkeletalMorphDef::Copy 0x004e28b6
// NF48 (round 7): the +0x28 member is Ego_r.pdb CSkeletalMorphDef::SkeletalMorphs, the shared CArray<CDefString> (engine/CArray.h, a
// public STLport 4.6.2 std::vector<CDefString>; was the stand-in Sub::Copy); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares
// vector<CDefString>::operator= (0x0046078C) out of line, which CArray's implicit operator= calls.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "engine/CArray.h"
class CDefString;
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CSkeletalMorphDef { public:
    char pad[0x28];
    CArray<CDefString> SkeletalMorphs;   // +0x28 (Ego_r.pdb CSkeletalMorphDef::SkeletalMorphs)
};

void __fastcall CSkeletalMorphDef_Copy(CSkeletalMorphDef* self, void* edx, const char* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(src));
    self->SkeletalMorphs = ((const CSkeletalMorphDef*)src)->SkeletalMorphs;
}