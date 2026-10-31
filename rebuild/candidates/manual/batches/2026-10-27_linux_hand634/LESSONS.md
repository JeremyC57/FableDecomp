# Chunk 634 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `frequency = pow(...); frequency *= reference;` (two statements) schedules an unsigned sample rate's `mov eax ; test eax,eax`
  before `fmul [reference]`; the one-expression product puts the fmul first. Second row confirming the SetHighPass 0x00C74BB0
  lesson: 0x00C74AB0 CEffectFilter::SetLowPass (parked 4 grades as "x87 scheduling", matched on the first try).
- VC7.1 canonicalises commutative x87 operands: the written order of `a*b`, `a+b` and of a three-term sum makes no difference
  (micro tests: `a.X*b.X + a.Y*b.Y + a.Z*b.Z` alone compiles z,y,x; all 16 xy/xz/yz spellings of 0x00A55F90 grade
  identically; X-first and Y-first condition sums of 0x006AFAF0 are identical). The chosen order moves when an unrelated local
  is added (0x00547050: a `float*` local flips the zoom multiply instead of the add). Do not burn grades on operand-order
  residues: park them as "x87 canonical operand order".
- A destructor with generic stand-in members (0x00C14F90) mis-schedules a `p = 0` store after `delete p`; the real member
  classes (the libpk list holders over a pure CPKList base with user dtors; a flag-patch class with a compiler-generated dtor over
  an inline virtual base dtor) give retail's store-after-next-test placement with plain `delete p; p = 0;`.
- The shared engine/CDefPointer.h model (ENullDef ctor) with CDefPointer<const T> locals and the real GetDef<T> / GetSubDef<T>
  bodies as same-TU noinline definitions reproduces the invented FindDef_* stand-in bodies byte for byte (0x0076C8A0): the
  stand-ins in parked/landed GetSubDef callers can be replaced without changing bytes.
- An inline member condition (`normal.LengthSquaredXY() > eps*eps`) instead of the same inline expression changes the x87 spill
  decisions downstream (0x006AFAF0: the dot product goes to the dead argument slot and is multiplied from memory, as retail).

Review at landing:
- vtable 0x012B29F8's class has four landed spellings (CTCDiceGame, CDataOwner_00c14a50, CSettingsProxy_012b29f8,
  PKCPlaybackTemplate); NAME_FIXES.tsv proposes PKCPlaybackTemplate (layout evidence; descriptive libpk).
- 0x00C19250 is bound under 17 spellings (CPKListImpl, CNode_*, CMap_*, CParamBlock...); 0x0129BA3C under ~30 epsilon names
  (the parked sources here keep g_epsilon_0129ba3c / g_floatEpsilon as found).
- 0x00686800 / 0x006869D0 / 0x00686A80 / 0x009ACCC0 / 0x009D49B0 / 0x006B93D0 / 0x0073ABA0 / 0x0097D580 / 0x0097EC00 / 0x0097E200 /
  0x008FCF40 carry many non-PDB spellings in reloc_bindings.tsv: PDB spellings in NAME_FIXES.tsv.
- 0x00C74AB0's caller CEffectPart260::Configure 0x00C783F0 binds it as ?SetShelf@CEffectFilter@@QAEXMMK@Z (wrong name and type).
- Local base models next to flattened headers: CTCBase (0x0076C8A0, 0x00946190's CAIStateGroupBase chain is local too).
- Rows retested past 4 grades only through one-pass family sweeps (0x00AC7200, 0x00C72D00, 0x00A55F90, 0x00547050).
