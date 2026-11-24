#pragma optimize("s",on)
// CPersistContext_TransferableComponent<CDefIndex>::TransferIn  @ 005cbe9d
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent@VCDefIndex@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z
// __thiscall private virtual void TransferIn(CPersistContext&, CStringParser&)
// Identity (NF385): retail vftable 0x01253F18 (RTTI CPersistContext_TransferableComponent<CDefIndex>) slot 1 = FableWin slot 1
// ?TransferIn@?$CPersistContext_TransferableComponent@VCDefIndex@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z; both PDB name sets
// (definition_manager.obj; Ego_r 29 B = retail); the callee 0x00A3E0D0 is CPersistTraits<CDefIndex>::TransferIn (landed self).
// `this` is the shared engine/CPersistContext_TransferableComponent.h model; the callee keeps its landed table spelling (NAME_FIXES 0x00A3E0D0).

class CPersistContext;
class CStringParser;

// Helper temp object constructed on the stack (single dword, zero-inited),
// then a __fastcall method invoked on it with (persist, parser, value).
struct TransferHelper {
    unsigned int m_dword;
    // out-of-line __fastcall method -> emitted as a direct call
    void Apply(CPersistContext* ctx, CStringParser* parser, unsigned int value);
};

class CDefIndex;
#include "engine/CPersistContext_TransferableComponent.h"

template <> void CPersistContext_TransferableComponent<CDefIndex>::TransferIn(CPersistContext& ctx, CStringParser& parser)
{
    TransferHelper tmp;
    tmp.m_dword = 0;
    tmp.Apply(&ctx, &parser, (unsigned int)this->Object);
}
