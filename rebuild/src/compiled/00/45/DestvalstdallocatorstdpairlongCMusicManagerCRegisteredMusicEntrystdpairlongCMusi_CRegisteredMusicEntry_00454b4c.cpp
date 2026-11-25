struct Inner { void dtor(); };
// Leaf: ?Dest_val@@YIXPAUPair@@@Z
struct Pair { char pad[0x39]; Inner inner; void dtor(); };

// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00450BA0 ??1CThingComponentSet@@QAE@XZ (was ?dtor@Inner@@QAEXXZ)
//   0x00430300 ??1CParentDefClassBase@@UAE@XZ (was ?dtor@Pair@@QAEXXZ)
class CThingComponentSet { public: ~CThingComponentSet(); };
class CParentDefClassBase { public: virtual ~CParentDefClassBase(); };
void __fastcall Dest_val(Pair* self)
{
    reinterpret_cast<CThingComponentSet*>(&(self->inner))->CThingComponentSet::~CThingComponentSet();
    reinterpret_cast<CParentDefClassBase*>(self)->CParentDefClassBase::~CParentDefClassBase();
}