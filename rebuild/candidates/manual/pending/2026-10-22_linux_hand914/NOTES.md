# Hand chunk 914 (CTCTavernGameCardBlackjack .. _global): 29 of 33 byte-exact

The brief's template says "of 40"; this boundary chunk holds 33 rows (`$S/batch/chunk914_oracle.tsv`). All rows were
graded against the chunk oracle only, via q914.py (adapted from agent911's q911.py), because older mis-cut oracle rows
shadow these addresses in the shared q.py.

## What matched (29)
- **CTCTavernGameCardBlackjack** 0x007DA3A0 vfunc_42: `CDefPointer def; GetGameDef(def); return CCharString("", -1).ToWideString();`
  - The def pointer releases the long way: an MI base CRefCounted{DecRef, GetRefCount} plus CDefVirtual{Destroy slot 1}, and the dtor does `ReleaseTemporary(); m_p = 0;`.
- **CBaseVideoRenderer** 0x00CA5400 RecordFrameLateness (labelled CTextureRenderer::vfunc_45): the DirectShow strmbase renbase.cpp body with FRAMES_TO_IGNORE 1, byte-exact.
  - The vptr is modelled as a plain `void*` member, because a real vptr in a class with `__int64` members pads to 8 and shifts every member.
- **CThing**
  - 0x004CA010 ConstructFromParams: a local model at the retail offsets.
    - The GetString results and the AddTC("CTCEditor"/"CTCMapwho") names are named CCharString locals, each in its own `{}` block.
    - The default section name comes from an inline accessor that copies global 0x013BAB0C.
    - The player clamp is `== 5 || < 0 || >= 4 -> 4`.
  - 0x004CD1E0 vfunc_30 is `return DrawIsDrawable();` (slot 27). 0x004CD220 and 0x004CD230 (vfunc_34/35) are `return false;`.
- **CThingFilter_IsValid** 0x0043E430 operator(): a no-argument forwarder to virtual slot 1.
- **CThingSwitch** 0x0083CD70 vfunc_19 = WorldUpdate: the base update, then every fifth frame (staggered by UniqueID, `__aullrem`) `switch (GetSwitchDef()->TriggerType)` over 5 handlers.
  - The 220 bytes include the jump table.
  - The def temporary in the switch condition keeps the dtor's null store; a `long type = ...` local loses it.
- **CWideString** 0x0099B1D0 operator!=(wchar_t): `return !(*this == c);` over an inline operator== that tests the data block first, then a `{c, 0}` buffer compared with wcscmp.
- **_global** (all named through `// Leaf:` lines):
  - 0x00410CB0 CopyTrivialTagged, 0x00410D40 CopyTrivial and 0x00410D60 CopyTrivialBackward: STLport `__copy_trivial` / `__copy_trivial_backward`, `__fastcall` (the /Gr algorithm TU), memmove.
  - 0x00410D10 DestroyRangeWideString: a `__fastcall` `_Destroy_Range` loop calling ~CWideString.
  - 0x0043E440, 0x0043E450 and 0x0043E460: member getters at +0x2EC, +0x2FC and +0x1C8.
  - 0x00449BF0 and 0x00449C10: CPlayerManager::GetMainPlayerHeroCreature(long unused), two byte-identical copies.
    - Written as `GetPlayer(MainPlayer)->GetHeroCreature()` and bound to two distinct leaf names.
    - Local model: MainPlayer at +0x1C behind a 12-byte retail vector.
  - 0x004CD2A0 and 0x004CD2B0: the CPairVectorWithFlag ctor and copy ctor.
    - The ctor zeroes the vector and sets `m_flag(false)` at +0xD; +0xC stays uninitialised.
    - The copy ctor calls the vector copy 0x004CC290, then copies the flag.
  - 0x004CD2D0: list push_back as `insert(end(), x)`. insert returns through a hidden pointer, and the iterator needs a user copy ctor.
  - 0x004CD2F0: vector<8-byte POD>::push_back. The empty `__true_type` tag has a user ctor, so no byte is stored; growth goes through `_M_insert_overflow` 0x004CCCE0.
  - 0x00567E90: STLport string `_M_deallocate_block`, written as `if (_M_start) free(_M_start);` (CRT free 0x00BFEA14).
  - 0x005D93B0 and 0x005D9430: CTCInventoryItem::GetTattooDef and GetHairCardDef.
    - These need a same-TU `#pragma auto_inline(off)` stand-in for GetSubDef<T> that writes only its out pointer; that stand-in is what fixed the release shapes.
  - 0x00619950 and 0x006199A0: rb-tree `_M_clone_node` (map<long, vector>) and `_M_create_node` (map<long, CRegion>): malloc 0x00BFEA0E plus a placement copy.
  - 0x00686090: CPointerVector::ClearElements, `for (i < size()) (*this)[i] = 0;` with `operator[]` written as `*(_M_start + n)`.
  - 0x006CA650: CPointerHolder::CPointerHolder(void*).

## Parked for Fable 5.1 (4)
- 0x00a0d1a0 CWStringParser::ReadAsString DIFFER(96v95): the only difference is the compiler `int3` after the noreturn `_CxxThrowException` call. The chunk oracle trims it as padding, but the next function starts right after it. | tried: probe/q914_int3.py with the int3 kept gives RELOCATION_MATCH | next: land against a 96-byte oracle (agent273 precedent: grader artefact only).
- 0x00a0d200 CWStringParser::ReadAsSymbol DIFFER(96v95): the same int3-after-throw oracle trim as ReadAsString. | tried: the 96-byte oracle gives RELOCATION_MATCH | next: land against a 96-byte oracle.
- 0x00567e80 _global::FUN_00567e80 (CRetailString::_M_append_dispatch) DIFFER(23v13): `push x3 ; call append ; ret 0Ch` where retail does `mov [esp+0Ch],&tag ; jmp append`. | tried: user-ctor tags, by-value tags, an iterator_category helper, `#pragma optimize` s/t/y, /Ox /O1 /Oa /Ow /Gr /EHsc | next: lane gap: VC7.1 sibling tail call with an address-taken temporary (possibly LTCG or a different compiler build).
- 0x004a1840 CWorld::Load DIFFER(1129v1129): same length and the same frame-slot order; three register picks differ.
  - The differences: `lea ecx,[ebp-50h]; push ecx` (GetOfflineData argument), the SetStaticMap chain `mov eax/ecx/ecx; mov edx,[ecx]; lea eax`, and `lea ecx,[ebp-20h]; push ecx` (the Format `this`). We pick eax/edx in all three.
  - tried: catch(...) changed to a named `catch (CBBBFileException& exception)` (read from FuncInfo 0x01369298; it fixed sub esp 0xA4); named locals for L"", the startup-WAD map pointer and the offline file name (these fixed the slot order); an inline GetEngine() versus a member chain versus a named local; /O1 /Os /G5 /G6 /G7 /Oy-; the QFE toolset.
  - next: advisor AdvB3 has the row; otherwise lane gap: register-allocation tie-break.

## Skipped
None. Every row was attempted.

## Label corrections
- 0x0043E430 CThingFilter_IsValid::operator() takes no CThing* argument; it is a generic slot-1 forwarder.
- 0x00CA5400 "CTextureRenderer::vfunc_45" is the inherited CBaseVideoRenderer::RecordFrameLateness (strmbase).
- 0x0083CD70 "CThingSwitch::vfunc_19" is CThingSwitch::WorldUpdate.
- 0x00567400 "CTCElectrocutedEffect::InsertStringRange" (propagated label) is the STLport basic_string range append.
- 0x004C7AB0 is CThing::GetWorld; 0x0049D870 is GFGetCurrentFrame; 0x00A37300 is CClassFactory::ReregisterAllocatedClass.
- 0x005EA000 is the tavern-game def getter (GetGameDef).
- 0x0049D770, 0x0049D610 and 0x0049DDD0 are the CWorld level-quest, WAD and static-map filename helpers.
- 0x006C20A0 is the map loader's IsBusy.
- The _global rows are named in their Leaf lines:
  - 0x005D93B0 / 0x005D9430: CTCInventoryItem GetTattooDef / GetHairCardDef.
  - 0x00449BF0 / 0x00449C10: CPlayerManager::GetMainPlayerHeroCreature.
  - 0x00410CB0 / 0x00410D40 / 0x00410D60 / 0x00410D10: STLport copy and destroy helpers.
