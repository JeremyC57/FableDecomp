# Chunk 628 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- PDB reference spellings change register colouring, not just mangling. 0x0058400A's owner went to the dead `this` register (ebx)
  only once the callees took references: ctor `(CThingCreatureBase&, ...)` called with `*creature`, and `SetCurrentAction(const
  CCreatureActionBase&)`. The pointer stand-ins put it in the dead item register (esi). Respell callee parameters to the PDB form
  before treating a register swap as a lane gap.
- A block layout that no direct `if` spelling gives (0x005DAC50: retail lays the conditional restart arm first and the straight
  -1 arm last) comes from a NAMED bool condition: `bool canAim = !a || b; if (canAim) {...} else {...}`. VC7.1 normalises every
  direct `||`/`&&` condition to put the straight-line arm first.
- The double-local x87 rule also covers a length spilled across a call (0x00C418F0). `double length = v.GetLength(); float r =
  (float)length;` (GetLength returning `(float)sqrt(...)`) gives `fsqrt ; fstp st(3) ; fstp st(0) ; fstp st(0) ; fstp [slot]`. A plain
  float local spills straight after fsqrt.
- A float function whose integer arm copies the int to a frame word and `fild`s it AFTER a local's destructor uses the class's
  inline by-value accessor: `return (float)item.GetInteger();` (0x009BA650, PDB ?GetInteger@CParsedItem@@QBEJXZ). VC7.1 copies
  an inline's return value into a temporary. Direct member access converts before the dtor.
- The "compiled-out check" (`mov [dead arg slot],eax ; xor al,al ; jne`) after a GetPItem null test is the typed intelligent-pointer
  getter `CIntelligentPointer<T>::Get()` = `(GetPItem() && IsThingOfType<T>((T*)GetPItem())) ? (T*)GetPItem() : 0` with
  `IsThingOfType<T>(const CThing* const& t) { unsigned long mask = T::ThingTypeMask; return (t->ThingTypeFlags & mask) == mask; }`
  and `ThingTypeMask = 0` for CThing (0x006AF3F0; the 2026-10-31 sec2c "not reproduced by any inline shape" note is superseded).
  The same pattern sits in 0x006AF320 / 0x006AF370 / 0x006AF2D0 / 0x006AF4E0.
- Frame-slot assignment follows DECLARATION order of the spilled locals (0x009350C0): with an inlined out-parameter lookup, the
  first-declared spilled local takes the out-local's slot, the second a fresh slot, the third the inline key's slot.
- In a CEnginePrimitive-derived ctor, the base's CEnginePrimitiveThemeChannelDesc as a member with an inline zeroing ctor keeps the
  next member's `lea ecx,[esi+0Ch]` after the type/interpolation stores (0x00493AC2). Flat bytes let VC hoist it right after `mov esi,ecx`.

Review at landing:
- 0x005DAC50 and 0x0058400A use local CDefPointeeBase / CCreatureActionBase models next to existing shared headers (see NOTES.md
  "Header debt"; the shared CDefPointer.h costs 0x005DAC50 its match).
- Landed CAIStateGroupPerformAction_Update_009352c0.cpp declares Fired as `?Fired@CAIStateGroup_ScriptingBase@@QAEXXZ`. The PDB
  symbol is the protected template member (NAME_FIXES.tsv).
- Landed CTCWeapon siblings (e.g. 0x005DAD60) model `this` as a derived `CTCWeapon_Getters`, which mangles their members as
  CTCWeapon_Getters::. The `#define CTCWeapon CTCWeaponLayout` pattern used here gives the real CTCWeapon:: symbol.
- Rows 0x009350C0, 0x006AF3F0, 0x0061A9D0, 0x009F18F0 and 0x00C11520 have now had more than 4 grades in total, across the earlier
  sweeps plus this chunk.
