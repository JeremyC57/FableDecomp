# Hand chunk 377 (std::basic_string .. std::vector<CHeroMarriageInfo>): 36 of 40 byte-exact

9 MATCH + 27 RELOCATION_MATCH; every matched row has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).
The 4 parked rows also have tests (differ/*.test.cpp) that pass under both compilers.

## What matched (36)

**STLport strings and string algorithms (scoreboard / HTTP TU)**
- 0x566E60 `std::basic_string::basic_string()`: zero the `_String_base`, then allocate an 8-byte block holding only the terminator. Shape: STLport 4.5's out-of-class ctor verbatim, with `_String_base(allocator_type())`. The empty allocator has user-declared ctors, so the temporary costs no store.
- 0x566EA0 `std::operator!=(const string&, const char*)`: compare lengths, then the characters. Shape: STLport `size() == n && compare(..) == 0`, inlined under `!`. This produces a materialised 1/0 and then `sete` in both tails.
- 0x567120 `std::search(.., _Eq_traits)`: find a pattern in a character range. `__fastcall` (/Gr). Shape: STLport verbatim, including the general loop's two consecutive scans for `*first2`, and `__p1` spilled to the `push ecx` word.
- 0x5671C0 `std::__reverse(pair<long,CWideString>*, ..)`: swap the ends inward. Shape: `for (; first < last; ++first) iter_swap(first, --last)` with memberwise pair copy and assignment.
- 0x567240 `std::__find(list<CWideString>::iterator, ..)`: find over a list. Shape:
  - Uses the landed `CWideString::operator==` form (0x639BA0), with `if (compare) return false; return true;`.
  - The traits compare body is a same-TU `__declspec(noinline)`, which keeps val's block in esi.
- 0x5693B0 `std::basic_string::assign(const char*, const char*)`: range assign. Shape: STLport verbatim; the traits copy is `n == 0 ? s1 : memcpy(..)`, and the tag temporary for append takes the dead `l` slot.
- 0x569690 `std::operator+(const string&, const string&)`: reserve, append both, return by value. Shape:
  - `_Str __result(_Reserve_t(), n)` with a `__forceinline` `_Reserve_t` ctor.
  - The length error is `puts ; abort()` with `abort` declared noreturn, so the cold block ends in `jmp abort`.
  - VC7.1 has no NRVO here, so the return copies and frees the local.

**STLport containers**
- 0x567560 `std::vector<unsigned char>::vector(n, val, a)`: allocate and memset. Shape: `_Vector_base(n, a)` with `_STLP_alloc_proxy`, then `uninitialized_fill_n` reaching the one-byte `fill` -> `memset` intrinsic.
- 0x567790 `std::list<CClanInfo>::insert(pos, x)` and 0x567FC0 `std::list<CClanMember>::push_back(x)`: create a node, copy the record in, link it. Shape: the landed 0x5355C0 insert, with placement new testing `&node->_M_data`. The iterator needs user ctors for the hidden-pointer return.
- 0x569A80 `_Rb_tree::_M_erase` for the scoreboard map (kept as the landed `CRetailTree_erase00569a80::Erase` symbol): recurse right, destroy the entry vector inline, free the node, continue left.
- 0x569E30 `std::vector<string>::_M_insert_overflow` and 0x56A2D0 `vector<string>::push_back`: STLport verbatim. The tag-taking callees are same-TU noinline bodies that never read the tag, so every tag temporary lands on a dead or live argument slot (`atend` / `x`).
- 0x56A680 `std::map<long, vector<pair<long,CWideString>>>::operator[]`: lower_bound, then `insert(i, value_type(k, T()))` on a miss. Shape:
  - Built on the landed 0x47CB50 form.
  - The vector copy ctor (0x566CD0) and `insert_unique(hint)` (0x56A370) are same-TU `__declspec(noinline)` stand-ins.
  - Only with those does VC7.1 elide the `T()` dtor and cache the zero register. `#pragma auto_inline(off)` also un-inlines the pair ctor (200v188).
- 0x56D490 `std::vector<T*>::vector(const vector&)`: `_Vector_base(x.size(), a)`, then `__copy_trivial` into the new block. Matched on the first grade.
- 0x5796D1 `std::vector<CComponent*>::_M_allocate_and_copy`: allocate, then `__copy_trivial` (0x405BA0, `__fastcall`, declared only). Its leaf is the member-template symbol `??$_M_allocate_and_copy`.
- 0x579A4B `vector<ExperienceSpendInfo>::_M_insert_overflow` (12-byte record) and 0x58044A `vector<CHeroMarriageInfo>::_M_insert_overflow`: STLport verbatim. Shape:
  - Same-TU noinline `__uninitialized_copy` and `__uninitialized_fill_n` home the tag temporaries in dead argument slots.
  - Declared-only callees produce `[ebp-1]` temporaries and a larger frame.
  - `_M_clear` is inline: a destroy loop, then deallocate with a null test.
  - 0x58044A needs a local CHeroMarriageInfo with the user copy ctor (0x57A3A2).
- 0x579B3C `vector<ExperienceSpendInfo>::_M_fill_insert`: insert n copies in place, or grow through `_M_insert_overflow`. Same-TU noinline copy, fill_n, copy_backward and fill bodies; matched on the first grade.
- 0x579D18 / 0x579D55 `_Rb_tree<EHeroTrainableStatType|EHeroAbility, pair<const K, SExpSpendUndo>>::erase(iterator)`: `_Rebalance_for_erase` (0x42A34E), destroy the value, free the node, decrement the count. Shape:
  - The implicit non-trivial pair dtor is called through its `??_G` with 0 (0x5799DE / 0x5799FD).
  - Instantiate only `_Rb_tree`: a map wrapper emits a second `?erase@` symbol. The leaf is `?erase@?$_Rb_tree@`.

**STLport algorithms (`__fastcall`, verbatim)**
- 0x578C50 / 0x57F188 / 0x57F29E `std::__copy(.., random_access_iterator_tag, _Distance*)` over ExperienceSpendInfo (12 bytes), `pair<const CRegion*, C3DVector>` (16 bytes) and CHeroMarriageInfo: count-down loop with memberwise assignment.
- 0x578CF7 / 0x57F338 `std::__copy_backward`: the same, backwards (12 and 24 bytes).
- 0x578C8D `std::__uninitialized_fill_n(.., __false_type)`: placement new with its null test.
- 0x57F279 `std::fill<CHeroMarriageInfo*>`: element loop.
- All of these land as the STLport 4.x text with `__fastcall` templates; no levers were needed.

**Serialisation**
- 0x569B30 `GFSerialiseVectorBinaryIn<CQuestActivityRecord>`: read the count, resize, read each 4-byte record through its traits. Shape: the landed template (0x5C2AD8) without the size pragmas; the read-only `TransferBinaryIn` body is a same-TU noinline stand-in.
- 0x57F109 `GFSerialiseVectorBinaryOut<CHeroMarriageInfo>`: write the count, then each record through `CPersistTraits::TransferBinaryOut` (0x57A3CD). Shape: size TU, no frame; traits shaped as `{ T DefaultVal; TransferBinaryOut(CDataOutputStream*, const T*) }`.
- 0x57F78F `CPersistTraits<CHeroMarriageInfo>::TransferOut(context, out, value)`: append `"(" wife "," home "," state "," TRUE|FALSE ")"`. Shape:
  - Frame-pointer size TU, with `__forceinline` element traits (the size TU leaves plain inline members out of line).
  - The u64 traits copy into a local first, which gives `mov eax,[esi] ; push eax`.
  - The long traits pass a local copy to `TransferOutLong` (0x9E2560, `__fastcall`).
  - TRUE/FALSE pairing checked by hand against the object relocations: TRUE on the fall-through, FALSE on the `je` target, as in retail.

**Gameplay / UI**
- 0x569C50 `LHNetworkLib::LHHttpHeaderStatus::LHHttpHeaderStatus()`: build the header block and three strings, then reset code=-1, the strings to `""`, and port=0. Shape: members in declaration order with the retail STLport 0x48-byte layout (the generated header is the 0x8C Ego_r layout).
- 0x56ED74 `CTCInventoryExperience::GetExperienceStatColour(EHeroTrainableStatType)`: returns an RGBA colour.
  - Stats 0..2 give (1, .3, .3, 1); stats 3..5 give (1, 1, .3, 1); anything else gives (.3, .3, 1, 1).
  - Shape: size TU (`optimize("s", on)`) turns the 1.0f stores into `fld1 ; fstp`.
- 0x57A2D0 `float ceil(float)`: `(float)ceil((double)x)`. Shape: a speed TU (`sub esp,8`); under "s" it becomes `push ecx ; push ecx`.
- 0x57A57A `CTCHeroStats::GetStaminaPercentage() const`: Stamina / MaxStamina, or 0 when max <= 0. Retail offsets: +0x58 / +0x5C. Shape: `float f = (float)Stamina; f /= max; return f;` gives `fdiv st,st(1)`. `Stamina / max` gives `fidivr`, and a named quotient gives `fdivrp`.
- 0x57AD4C `CWideString __fastcall JustGetTheFrikkingText2(const CCharString&)`: look up the text tag's CRC in the data bank and return the entry's text, or the widened tag on a miss. Shape:
  - Frame-pointer size TU.
  - CWideString needs its user copy ctor and dtor; otherwise VC7.1 returns it in eax as a POD.
  - The first bank read is a named local; the second goes through an inline accessor so that it loads ecx.
  - CIVCountedPointer has a `__forceinline` dtor.

## Parked for Fable 5.1 (4)

- 00568e70 std::__uninitialized_copy DIFFER(149v148): ours ends with the 0xCC that VC7.1 emits after the final noreturn `_M_throw_length_error` call; the chunk row / q.py strip that byte as padding. Against the 149-byte row read straight from Fable.exe it is RELOCATION_MATCH. | tried: STLport verbatim; N+1 oracle grade (probe/n1.py) | next: lane gap: int3-after-noreturn oracle trim (land against an N+1 row)
- 00568f10 std::__uninitialized_fill_n DIFFER(143v142): same int3-after-noreturn trailing 0xCC; RELOCATION_MATCH against the 143-byte row. | tried: STLport verbatim; N+1 oracle grade | next: lane gap: int3-after-noreturn oracle trim (land against an N+1 row)
- 0056fbed YesCloseInventory DIFFER(114v112): only the final merge differs. Retail joins the hit path (`mov ecx,[eax+4]`) and the miss path (`mov ecx,[ebp-4]`) in ecx, then `mov eax,[ecx] ; push 1 ; call [eax+164h]`. Ours joins in eax, then `mov edx,[eax] ; push 1 ; mov ecx,eax ; call [edx+164h]`. | tried: IS_THING_ALIVE declared only (116) or as a same-TU noinline body (114); derived-to-base temporary; inline / static_cast GetInterface; PDB GetTC(T**) wrapper over a CTCBase* out local; value-returning GetTC; early return; outer-scope out local; long/bool slot argument; reference alias (about 13 grades) | next: model CThing::GetInterface as the real out-of-class template that a landed sibling in this TU uses (find it via xref of 0x40F020 callers), or as a member returning `CTCBase*&`, so the merge register becomes the `this` of the virtual call
- 0057ef86 GFLimitReturn<float> DIFFER(37v37): two scheduling spots. Retail loads `hi` (`mov edx,[esp+4]`) after the second `fld [ecx]`; ours loads it before. Retail's `> hi` arm is `jne L ; mov ecx,edx ; L: mov eax,ecx`; ours is `mov eax,edx ; je exit ; mov eax,ecx`. | tried: speed TU (38, separate `ret 4`), size TU if/if/return (37), ternary (37), a `const T* result` local (35) | next: try the double instantiation's landed form (if any) with `T` = float and an inline `GFLimit` helper, or `if (value < lo) return lo; return value > hi ? hi : value;` with the parameters reordered by reference aliases

## Skipped

None: every row holds one function (the bounds-checked lengths matched the scanner extents; the trailing differences between the chunk lengths and the oracle lengths are int3/nop padding).

## Label corrections

- 0x566E60 "Vector_InitializeEmpty" -> `std::basic_string::basic_string()` (STLport default ctor).
- 0x566EA0 "String_Compare" -> `std::operator!=(const string&, const char*)`. The landed caller 0x5615C0 names it operator==, but the body is the `!=` (sete-inverted) form.
- 0x567120 "StringSearch_FindPattern" -> `std::search(.., _Eq_traits<char_traits<char>>)` (STLport).
- 0x5671C0 "WideStringPair_SwapRange" -> `std::__reverse(pair<long,CWideString>*, .., random_access_iterator_tag)`.
- 0x567240 "DataList_FindEqualNode" -> `std::__find(list<CWideString>::iterator, .., input_iterator_tag)`.
- 0x567560 "Vector_Fill_Byte" -> `std::vector<unsigned char>::vector(size_type, const unsigned char&, const allocator&)`.
- 0x567790 "ClanMember_CreateNode" -> `std::list<CTCHeroOnlineScoreboard::CClanInfo>::insert`; 0x567FC0 "ClanLogo_CreateNode" -> `std::list<CClanMember>::push_back`.
- 0x568E70 "std_uninitialized_copy_range_strings" -> `std::__uninitialized_copy<const string*, string*>(.., __false_type)`; 0x568F10 "ElectrocutedEffect_FillFromRange" -> `std::__uninitialized_fill_n<string*, unsigned int, string>`.
- 0x5693B0 "Vector_Insert" -> `std::basic_string::assign(const char*, const char*)`.
- 0x569690 "CTCElectrocutedEffect_InitFromStringRange" -> `std::operator+(const string&, const string&)`.
- 0x569A80 "TreeNode_DestructWithCCharString" -> `_Rb_tree::_M_erase` for `map<long, vector<pair<long,CWideString>>>`. The element holds a CWideString, not a CCharString.
- 0x569B30 "GFSerialiseVectorBinaryIn<CRGBColour>" -> `GFSerialiseVectorBinaryIn<CQuestActivityRecord>` (the CRGBColour was propagated across byte-identical bodies).
- 0x569C50 "Transaction_InitializeVectors" -> `LHNetworkLib::LHHttpHeaderStatus::LHHttpHeaderStatus()` (Ego_r.pdb `??0LHHttpHeaderStatus@LHNetworkLib@@QAE@XZ`).
- 0x569E30 "Vector_ReallocateStructArray" -> `std::vector<string>::_M_insert_overflow`; 0x56A2D0 "CTCElectrocutedEffect_PushElement" -> `std::vector<string>::push_back`.
- 0x56A680 "Scoreboard_LookupOrCreateEntry" -> `std::map<long, vector<pair<long,CWideString>>>::operator[]`.
- 0x56D490 "Vector_CopyConstructor" -> `std::vector<T*>::vector(const vector&)`. T is a pointer element of the local menu map in CQuestManager::UpdateUI_QuestMenu, so the exact type is unknown.
- 0x56ED74 "SetColorRGBA" -> `CTCInventoryExperience::GetExperienceStatColour(EHeroTrainableStatType)`.
- 0x578C50 "_Move<CMusicSetEntry*,..>" -> `std::__copy<ExperienceSpendInfo*>`; 0x578C8D "_Fill_n<unsigned_long*,..>" -> `std::__uninitialized_fill_n<ExperienceSpendInfo*>`; 0x578CF7 "_Copy_backward<CBrainUpdateZone*,..>" -> `std::__copy_backward<ExperienceSpendInfo*>`.
- 0x5796D1 "Vector_AllocAndMove" -> `std::vector<NUISystem::CComponent*>::_M_allocate_and_copy` (CTCInventoryExperience::ExpSliders, retail +0x23C).
- 0x579A4B "Vector_InsertAndRealloc" -> `std::vector<CTCInventoryExperience::ExperienceSpendInfo>::_M_insert_overflow`; 0x579B3C "Vector_InsertRangeWithGrow" -> `..._M_fill_insert`.
- 0x579D18 "BuyStatTree_EraseAndFree" / 0x579D55 "BuyAbilityTree_EraseAndFree" -> `_Rb_tree<..., pair<const K, SExpSpendUndo>>::erase(iterator)` (UndoableStatBuys / UndoableAbilityBuys).
- 0x5799DE / 0x5799FD (callees) -> `pair<const K, SExpSpendUndo>::scalar deleting destructor`, not CLandscapeBackgroundPatch.
- 0x57A57A "WillEnergy_ComputePercentage" -> `CTCHeroStats::GetStaminaPercentage() const`.
- 0x57EF86 "GFLimitReturn<double>" -> `GFLimitReturn<float>` (the body uses `fld dword`).
- 0x57F188 "_Move<CMusicEntry*,..>" -> `std::__copy<pair<const CRegion*, C3DVector>*>` (CTCHeroStats::HousePositions).
- 0x57F279 "_Fill<CHeroMarriageInfo*,..>" -> `std::fill<CHeroMarriageInfo*>`; 0x57F29E "_Copy_impl<CDegradableInfo*,..>" -> `std::__copy<CHeroMarriageInfo*>`; 0x57F338 "_Copy_backward<..>" -> `std::__copy_backward<CHeroMarriageInfo*>`.
- 0x57F78F "Inventory_FormatItemTransferDetails" -> `CPersistTraits<CHeroMarriageInfo>::TransferOut(CPersistContext&, CCharString&, const CHeroMarriageInfo*) const`.
- 0x58044A "Vector_InsertRangeInit" -> `std::vector<CHeroMarriageInfo>::_M_insert_overflow`.
- 0x9E2560 (callee, catalogued as GetItemDescription) -> `TransferOutLong` (`__fastcall`).
- 0x405BA0 (callee, "Std_Move_Backward") -> `std::__copy_trivial` (`__fastcall`).
