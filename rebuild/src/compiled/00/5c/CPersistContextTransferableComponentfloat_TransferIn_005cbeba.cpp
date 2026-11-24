#pragma optimize("s",on)
// CPersistContext_TransferableComponent<CDefIndex>::TransferOut  @ 005cbeba (was modelled as <float>::TransferIn)
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@VCDefIndex@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// __thiscall private virtual void TransferOut(CPersistContext&, CCharString&)
// Identity (NF385): retail vftable 0x01253F18 (RTTI CPersistContext_TransferableComponent<CDefIndex>) slot 0 = FableWin slot 0
// ?TransferOut@?$CPersistContext_TransferableComponent@VCDefIndex@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z; both PDB name sets
// (definition_manager.obj; Ego_r 29 B = retail); its callee 0x00A3E180 is CPersistTraits<CDefIndex>::TransferOut (table witness
// 0x0074CDD0, landed self). `this` is the shared engine/CPersistContext_TransferableComponent.h model. The callee keeps its HEAD
// stand-in symbol (its parameter is modelled as the parser's; the real callee takes CCharString&: NAME_FIXES 0x00A3E180), so the
// string argument goes through the stand-in's pointer type.

class CPersistContext;
class CStringParser;

// Helper temp object constructed on the stack (single dword, zero-inited),
// then a __fastcall method invoked on it with (persist, parser, value).
struct TransferHelper {
    unsigned int m_dword;
    // out-of-line __fastcall method -> emitted as a direct call
    void Apply_00a3e180(CPersistContext* ctx, CStringParser* parser, unsigned int value);
};

class CDefIndex;
#include "engine/CPersistContext_TransferableComponent.h"

template <> void CPersistContext_TransferableComponent<CDefIndex>::TransferOut(CPersistContext& ctx, CCharString& out)
{
    TransferHelper tmp;
    tmp.m_dword = 0;
    tmp.Apply_00a3e180(&ctx, (CStringParser*)&out, (unsigned int)this->Object);
}
