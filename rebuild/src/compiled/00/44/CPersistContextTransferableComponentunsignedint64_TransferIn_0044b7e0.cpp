// CPersistContext_TransferableComponent<unsigned __int64>::TransferIn(CPersistContext&, CStringParser&) -- reads a 64-bit integer with CStringParser::ReadAsUHUGE and stores it through the target pointer
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent@_K@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z
// Identity (NF385): retail vftable 0x01231C80 (RTTI CPersistContext_TransferableComponent<unsigned __int64>) slot 1 = FableWin slot 1
// ?TransferIn@?$CPersistContext_TransferableComponent@_K@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z; both PDB name sets
// (player_manager.obj; Ego_r 22 B = retail). `this` is the shared engine/CPersistContext_TransferableComponent.h model.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044B7E0, 22 bytes (+10 int3 padding).
// Callee: 0x009BA5B0 CStringParser::ReadAsUHUGE (__fastcall, result in edx:eax).
//
// Persist READ hook (vtable slot 1) of a 64-bit component: the target pointer at +0x4 is read before
// the call (named local, kept in esi), the parser (second argument) is `this` of the reader, and the
// edx:eax result is stored as two dwords.
class CPersistContext;

class CStringParser {
public:
    unsigned __int64 ReadAsUHUGE();   // 0x009BA5B0 (Ego_r.pdb ?ReadAsUHUGE@CStringParser@@QAE_KXZ)
private:                    // 0x009BA5B0
};

#include "engine/CPersistContext_TransferableComponent.h"

template <> void CPersistContext_TransferableComponent<unsigned __int64>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    unsigned __int64* target = Object;
    *target = parser.ReadAsUHUGE();
}
