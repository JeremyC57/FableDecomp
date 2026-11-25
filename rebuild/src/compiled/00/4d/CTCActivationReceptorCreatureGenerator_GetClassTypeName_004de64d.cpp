// CTCActivationReceptorCreatureGenerator::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActivationReceptorCreatureGenerator::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActivationReceptorCreatureGenerator@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE64D, 16 bytes: mov ecx,[esp+4]; call CTCActivationReceptorCreatureGenerator::StatGetClassTypeName 0x004D4339; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActivationReceptorCreatureGenerator@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012413EC (RTTI .?AVCTCActivationReceptorCreatureGenerator@@) slot 22 (0x01241444 -> 0x004DE64D) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4339, the landed CTCActivationReceptorCreatureGenerator::StatGetClassTypeName (?StatGetClassTypeName@CTCActivationReceptorCreatureGenerator@@SI?AVCCharString@@XZ; literal "CTCActivationReceptorCreatureGenerator").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActivationReceptorCreatureGenerator_GetClassTypeName@4 calling the made-up ?Init_004d4339@T@@QAEXXZ.
// Header: no engine header defines CTCActivationReceptorCreatureGenerator; a local class over engine/CTCBase.h, its nearest RTTI ancestor whose header reaches the
//   polymorphic CTCBase (rtti_bases.tsv: .?AVCTCActivationReceptorCreatureGenerator@@ : CTCActivationReceptorBase : CTCBase; engine/CTCActivationReceptorBase.h is a
//   generated flat layout that does not derive from CTCBase, in either branch).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCActivationReceptorCreatureGenerator : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4339
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActivationReceptorCreatureGenerator::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
