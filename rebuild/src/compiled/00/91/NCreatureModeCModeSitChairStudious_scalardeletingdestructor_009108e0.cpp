#pragma optimize("t", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x00739DF0 now binds its PDB spelling ??1CAnimFilterOf3Groups@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?DestructMember_00739df0@@YIXPAX@Z.
class CAnimFilterOf3Groups { public: virtual ~CAnimFilterOf3Groups(); };
// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x008D71D0 ??1CModeSitBase@NCreatureMode@@UAE@XZ (was ?CModeSitChairStudious_Destruct@@YIXPAX@Z)
namespace NCreatureMode { class CModeSitBase { public: virtual ~CModeSitBase(); }; }
void* __fastcall CModeSitChairStudious_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    reinterpret_cast<CAnimFilterOf3Groups*>(static_cast<unsigned char*>(mode) + 44)->CAnimFilterOf3Groups::~CAnimFilterOf3Groups();
    reinterpret_cast<NCreatureMode::CModeSitBase*>(mode)->NCreatureMode::CModeSitBase::~CModeSitBase();
    if (flags & 1)
        operator delete(mode);
    return mode;
}