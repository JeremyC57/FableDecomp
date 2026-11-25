#pragma optimize("t", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
// Naming (NF423, 2026-11-25): the member release 0x009A4400 now binds its PDB spelling ??1?$CCountedPointer@VCBankFileAsyncData@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), called on the member through the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT); was the free stand-in ?DestructFirstMember_009a4400@@YIXPAX@Z. Object otherwise unchanged.
// Naming (NF423, 2026-11-25): the member release 0x006E46F0 now binds its PDB spelling ??1?$CCountedPointer@VCLipSyncSampleData@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), the same way; was the free stand-in ?DestructSecondMember_006e46f0@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
class CBankFileAsyncData;
class CLipSyncSampleData;
void* __fastcall CData_CompositeScalarDeletingDestructor(void* data, int, unsigned int flags)
{
    reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(static_cast<unsigned char*>(data) + 36)->CCountedPointer<CBankFileAsyncData>::~CCountedPointer();
    reinterpret_cast<CCountedPointer<CLipSyncSampleData>*>(static_cast<unsigned char*>(data) + 28)->CCountedPointer<CLipSyncSampleData>::~CCountedPointer();
    ((CResource*)(data))->CResource::~CResource();
    if (flags & 1)
        operator delete(data);
    return data;
}