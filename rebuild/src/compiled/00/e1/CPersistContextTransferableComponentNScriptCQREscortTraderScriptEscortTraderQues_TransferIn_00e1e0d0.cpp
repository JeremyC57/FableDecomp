// CPersistContext_TransferableComponent<NScript::CQR_EscortTraderScript::EscortTraderQuestStage>::TransferIn -- parse the enum as a long through CPersistTraitsSLONG::TransferIn and store it through Object
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent@W4EscortTraderQuestStage@CQR_EscortTraderScript@NScript@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1E0D0, 32 bytes, thiscall `ret 8`: Object (+4) into esi before the call; the long is homed in the parser's dead argument slot
//   (`lea eax,[esp+0Ch]`); CPersistTraitsSLONG::TransferIn 0x009E2590 is __fastcall (context ecx, parser edx, &value pushed).
// Naming wave 35c: slot 1 of the RTTI-less component vtable 0x012E149C (slot 0 TransferOut / slot 1 TransferIn, the pair of this
//   instantiation; the vtable dword is the only reference to 0x00E1E0D0); FableWin.pdb ?TransferIn@?$CPersistContext_TransferableComponent@W4EscortTraderQuestStage@CQR_EscortTraderScript@NScript@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z.
//   Was a generated stand-in CTransferableLong_009e2590::TransferIn (old file unattributed_TransferInCTransferableLong009e2590_00e1e0d0.cpp).
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
class CQR_EscortTraderScript {
public:
    enum EscortTraderQuestStage { };
};
}

template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    T* object = Object;                                  // read before the call (retail loads [ecx+4] first)
    long value;
    CPersistTraitsSLONG::TransferIn(context, parser, value);
    *object = (T)value;
}

template void CPersistContext_TransferableComponent<NScript::CQR_EscortTraderScript::EscortTraderQuestStage>::TransferIn(CPersistContext& context, CStringParser& parser);
