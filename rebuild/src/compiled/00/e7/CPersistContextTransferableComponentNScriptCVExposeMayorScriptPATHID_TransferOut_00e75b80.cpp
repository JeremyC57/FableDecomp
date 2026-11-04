// CPersistContext_TransferableComponent<NScript::CV_ExposeMayorScript::PATH_ID>::TransferOut -- write the enum as a long through CPersistTraitsSLONG::TransferOut
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@W4PATH_ID@CV_ExposeMayorScript@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E75B80, 30 bytes, thiscall `ret 8`: the value is widened into a `push ecx` local and passed by const reference to the
//   __fastcall CPersistTraitsSLONG::TransferOut 0x009E2560 (context ecx, text edx, &value pushed).
// Naming wave 35c: slot 0 of the RTTI-less component vtable 0x012E976C (slot 0 TransferOut / slot 1 TransferIn, the pair of this
//   instantiation; the vtable dword is the only reference to 0x00E75B80); FableWin.pdb ?TransferOut@?$CPersistContext_TransferableComponent@W4PATH_ID@CV_ExposeMayorScript@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z.
//   Was a generated stand-in CTransferableLong_009e2560::TransferOut (old file unattributed_TransferOutCTransferableLong009e2560_00e75b80.cpp).
// Model: engine/CPersistContext_TransferableComponent.h ({vptr, T* Object +4}; private virtuals); the enum traits are inline and forward to
//   the long traits, whose Ego_r.pdb / FableWin.pdb static __fastcall members ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@
//   AAVCStringParser@@AAJ@Z / ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z are bound to 0x009E2590 /
//   0x009E2560 by the landed 0x009BB110 / 0x00566360. The enum is declared in its script class with no enumerators (only its
//   W4 mangling matters here).
#include "engine/CPersistContext_TransferableComponent.h"
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferIn(CPersistContext& context, CStringParser& parser, long& value);         // 0x009E2590
    static void __fastcall TransferOut(CPersistContext& context, CCharString& out, const long& value);       // 0x009E2560
};
namespace NScript {
class CV_ExposeMayorScript {
public:
    enum PATH_ID { };
};
}

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    long value = *Object;
    CPersistTraitsSLONG::TransferOut(context, out, value);
}

template void CPersistContext_TransferableComponent<NScript::CV_ExposeMayorScript::PATH_ID>::TransferOut(CPersistContext& context, CCharString& out);
