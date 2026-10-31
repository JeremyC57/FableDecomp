# Chunk 629 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A null CScriptThing built by `mov ecx,<result> ; call 0x008CFF40` is `return CGameScriptThing::CreateNullThing();` (bound static __fastcall), not `return CScriptThing();` -- check the callee's binding before modelling an out-of-line "ctor" (0x008930F0).
- End iterators of two std::map members held in named locals (`const_iterator bEnd = other.m.end();` first) move the `other` pointer load to the top as retail does; the ebx/ebp header assignment still does not follow declaration order (0x00AD8AF0).
- The inline `World->GetThingSearchTools()` reference accessor (landed 0x00894490 form) loads the tools pointer straight into ecx where a raw member chain goes through edx (0x008930F0).
- A POD element with a `T DefaultVal` traits overlays the resize T() temporary as retail, but VC7.1 value-initialises a POD T() through a late block zero (ecx), not the CSE'd zero register a `= 0` ctor gives; the two levers conflict (0x00619560).
- VC7.1 canonicalises `p + n`, `n + p`, `&p[n]`, `p[n]` and char-offset forms to one ADD order; a SIB base/index swap on a CSE'd scaled index is not reachable from the address spelling (0x006D8300).
- A real CDefPointer local whose address goes to GetDef re-reads `.Object` at every use (202 v 185); retail's single esi load means the def object was read into a raw pointer local once (0x00408820).
- /G5, /G6, /G7 and /GB leave VC7.1 RTM's store-vs-push scheduling unchanged (0x009D5D30).

Review at landing:
- All ten rows needed more than 4 grades across sweep + this chunk; none is a MATCH. Residues are same-length scheduling / register / block-order misses.
- 0x00A01B50 CBaseIntelligentPointer::GetPItem: landed sources use dozens of spellings (Ego_r.pdb is `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`; 114+ landed declarations return CThing*, two bindings `QBEPAVCThing@@XZ`) -- a sweep, not a per-row fix.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound has many bound stand-in spellings (lower_bound@Vec, HeroMap, MapKind, AstmMap...).
- 0x013B85F3 is bound as both `g_bAggressiveModeAvailable` and `g_aggressiveEnabled`.
- Landed CTCHeroExperience siblings (e.g. 0x006D77C0 AddExperience, 0x006D6F60 SpendExperience) model the class locally next to engine/CTCHeroExperience.h; 0x006D6F60's own header says 61v59 while landed.
- The parked 2026-10-15_linux_hand185 CBankFileAsyncData sources call the base ctor as `CBase::CBase` (wrong symbol); the differ/ versions here use ??0CBaseClassNonCopyable@@IAE@XZ.
