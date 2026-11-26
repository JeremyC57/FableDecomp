// CAIStateGroup_OpenAndCloseShop::CAIStateGroup_OpenAndCloseShop() -- base CAIStateGroupBase() then this class's vtable
// Leaf: ??0CAIStateGroup_OpenAndCloseShop@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FE750, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBase::CAIStateGroupBase ; mov dword ptr [esi],offset CAIStateGroup_OpenAndCloseShop::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base constructor @ 0x008FD255
// (??0CAIStateGroupBase@@QAE@XZ) runs, then the compiler stores this class's vtable (0x0128AC9C) and returns `this`.
// NF474 (r467f rework of NF467 b3; r459a vftable port): no engine header exists for this class, so it is modelled here (r462a) on the
//   shared engine/CAIStateGroupBase.h retail view, with its Ego_r.pdb members and size 0x18 (struct_layouts_egor.tsv; retail Alloc
//   0x00901C2B pushes the same size before operator new) and the overrides of retail's table in their PDB spelling; so the emitted
//   ??_7CAIStateGroup_OpenAndCloseShop@@6B@ has retail's 14 slots at retail's targets (was a one-slot 0x14-byte local model, 4 bytes).
// TU mode: size-optimised, as retail's TU was (r467c): the ??_GCAIStateGroup_OpenAndCloseShop@@UAEPAXI@Z the full table brings is then 28 bytes,
//   equal to retail 0x00901C37 (under /O2 it is 30 bytes and differs), and the implicit ??1CAIStateGroupBase 19 bytes = 0x008FD288.
//   The ctor's own bytes are the same in both modes.
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"

class CAIStateGroup_OpenAndCloseShop : public CAIStateGroupBase {     // vftable 0x0128AC9C
public:
    CAIStateGroup_OpenAndCloseShop();
    virtual bool InitCreaturePosition();                                   // slot 3 0x00926B30 ?InitCreaturePosition@CAIStateGroup_OpenAndCloseShop@@UAE_NXZ (Ego_r.pdb + FableWin.pdb)
    virtual bool IsRunnable();                                             // slot 11 0x00926C50 ?IsRunnable@CAIStateGroup_OpenAndCloseShop@@UAE_NXZ (Ego_r.pdb + FableWin.pdb)
    virtual void Update();                                                 // slot 12 0x00926ED0 ?Update@CAIStateGroup_OpenAndCloseShop@@UAEXXZ (Ego_r.pdb + FableWin.pdb)
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature); // slot 13 0x00926A10 ?InitStateGroup@CAIStateGroup_OpenAndCloseShop@@UAEXABVCWorld@@AAVCThingAICreature@@@Z (Ego_r.pdb + FableWin.pdb)
    bool          GoToInsideFace;                        // +0x14
    bool          Lock;                                  // +0x15
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_OpenAndCloseShop) == 0x18);

CAIStateGroup_OpenAndCloseShop::CAIStateGroup_OpenAndCloseShop()
    : CAIStateGroupBase()
{
}
