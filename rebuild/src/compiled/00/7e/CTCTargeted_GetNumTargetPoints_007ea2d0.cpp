// CTCTargeted::GetNumTargetPoints @ 0x007ea2d0
// long __fastcall GetNumTargetPoints(CTCTargeted* this)

struct Inner {
    char pad[0x64];
    Inner* sub; // +0x64
    // external __fastcall method at 0x4bde90: (this, void*, float) -> long
};

// Data symbol (NF415, 2026-11-25): the first argument was the bare VA 0x13bac50; it is the const CCharString
//   TARGETING_CURSOR_POSITION_NAME @ 0x013BAC50 (Ego_r.pdb + FableWin.pdb `dynamic initializer for 'TARGETING_CURSOR_POSITION_NAME'';
//   reloc_bindings ?TARGETING_CURSOR_POSITION_NAME@@3VCCharString@@B; the landed 0x007EA490 / 0x008539C0 / 0x00661B40 use it).
class CCharString;
extern const CCharString TARGETING_CURSOR_POSITION_NAME;   // 0x013BAC50

class CTCTargeted { public:
    void* v;      // +0
    Inner* obj;   // +4
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004BDE90 ?GetNoHelperPoints@CTCGraphicAppearance@NGraphicAppearance@@QBEJABVCCharString@@M@Z (was ?Query@Inner@@QAEJPAXM@Z)
namespace NGraphicAppearance { class CTCGraphicAppearance { public: long GetNoHelperPoints(CCharString const&, float) const; }; }
long __fastcall CTCTargeted_GetNumTargetPoints(CTCTargeted* targeted)
{
    Inner* o = targeted->obj->sub;
    if (o)
        return reinterpret_cast<const NGraphicAppearance::CTCGraphicAppearance*>(o)->GetNoHelperPoints(*reinterpret_cast<CCharString const*>((void*)&TARGETING_CURSOR_POSITION_NAME), 1.0f);
    return 0;
}