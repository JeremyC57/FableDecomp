# Chunk 345 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `fabs` magnitude compare with retail's `fxch st(1) ; fxch st(1)` pair: write `(float)fabs(a) > (float)fabs(b)` (intrinsic fabs); the double compare drops the fxch pair (CComponent::CanScroll 0x00530690).
- A `movzx eax,al` before a folded `shr eax,4 ; and eax,2` comes from inline `bool` accessors `return (flags & mask) != 0;`; masked tests, bitfields and shift forms drop the movzx (CText::GetAlignment 0x0054FFF0).
- A global component's member read between inlined string reads and the CRC call needs an inline const getter (`g->GetBank()->GetTagFromCRC(text.GetCRC())`); a direct member read lands after Calc, a named local is hoisted too early (CText::SetValue 0x0054F370 / 0x0054F410).
- Overloaded virtuals are laid out in reverse declaration order: declare SetValue(char), SetValue(wide), SetValue(tag, wide) to get slots 145/144/143 (0x0054F370).
- An in-class inline helper (CCharString::GetCRC) is inlined at its first call site and called out of line (0x00404310) at the second in the same function (0x0054F370).
- STLport `find` with the input_iterator_tag homed in the live value argument slot: a real same-TU `__declspec(noinline) __fastcall` SGI `__find` body with an unnamed tag parameter under `#pragma auto_inline(off)` (CComponent::PositionChildDying 0x00531B40).
- Inline STLport list push_back keeps the callee-saved push at entry only with a declared iterator copy constructor and a `create_node` helper (`if (data != 0) data->Construct(x)`) (0x00531E50).
- `find_if(...) != end()` where retail reloads `_M_finish` after the call: evaluate the search into a named local first (`T* found = find_if(...); if (found != end())`); the inline expression lets VC7.1 reuse the pre-call `last` register (NCameraModes::CBase::AddSubComponent 0x0084E810).
- STLport `back()` must be the real `iterator tmp = end(); --tmp; return *tmp;`: it fixes the last node before a following call (`Strengths.back() + exp->GetStatExperience(0)`), the direct `_M_node->_M_prev->_M_data` form loads it after the call (SExpSpendUndo::RevertExperiencePoints 0x0056F4BA).
- A size-TU function that spills into its dead argument slot via `[ebp+8]` after `push ebp ; mov ebp,esp` is `#pragma optimize("y", off)` + `("s", on)` (0x0056F4BA); its frameless neighbours (0x0056E93D, 0x0056F597, 0x00579C35) match under `("s", on)` alone.
- `and dword ptr [esi+4],0` for a zero store in an unaligned constructor = `#pragma optimize("s", on)` (0x005B1A7B).
- A pointer argument that retail keeps in the register its source struct dies in (edi) needs a named local (`T* data = verts.Data;`); the direct member argument picks edx and shifts later temporaries (SaveVerts 0x00BFD0B0).
- A CDefVirtual model needs a slot-0 filler so Destroy is slot 1 (`call [edx+4]`) (CComponent::IsPCStyle 0x00530740).
- A by-value class parameter with a user destructor is destroyed by the callee (`lea ecx,[esp+8] ; call ~T` before `ret`) (CComponent::SetDeletion 0x00531E90).
- Constructors with `ret 4` and an unread argument (0x005B1A7B, 0x00527DE0, 0x009F0900, 0x009A9900) take a `(long unused)` parameter, as the landed CTargetAndThing_0078e700.

Review at landing:
- 0x005305A0 used 5 grades (one over the limit of 4) before parking.
- Byte twins in this chunk need distinct symbols: 0x0078E3A0 / 0x007939D0 are different nested classes (CTCDivineWrath / CTCUnholyPower::PerEffectInfo), but 0x0050F090 / 0x005102F0 (CTwoLayerPolyMap_<addr>) and 0x009A7670 / 0x009AA210 (CBankInfo / CBankInfo_009aa210) are address-suffixed copies of one shape; decide whether the lander wants a different naming for same-class twins.
- Local base models next to existing engine headers: 0x0056E93D / 0x0056F4BA / 0x0056F597 / 0x00579C35 use a local `CTCHeroExperience_Calls` call view because engine/CTCHeroExperience.h is data-only (no methods), as the landed TU-mate 0x005724CD does. No engine header exists for NUISystem::CComponent's retail layout (PDB layout is 0x30 larger), CTCDivineWrath, CTCUnholyPower, CBankInfo, SExpSpendUndo, LHHttpHeaders.
- 0x0049CFE0 / 0x005B1A7B / 0x00527DE0 / 0x009F0900 have no callers or data references and no PDB-unique owner: descriptive, address-suffixed names.
- Already-landed sources with errors: 0x0042ABCA is landed as CActiveFile::OnReadFinished (rebuild/src/compiled/00/42/CActiveFile_OnReadFinished_0042abca.cpp) but is std::list<CComponent*>::~list (called by SetDeletion 0x00531E90 on its by-value CDeletion); 0x0042B687 is landed as global LinkedList_PushFront but is list<long>::push_back (inserts before the header, i.e. at the end); the landed CBufferedReader seek 0x00C4AA40 writes the file name literal as `".\\\\PKCBufferedReader.cpp"` (two backslashes) while retail holds `.\PKCBufferedReader.cpp` (relocations mask it).
- Catalog row 0x00462FAC holds two functions (5-byte thunk + 51-byte body at 0x00462FB1, absent from the manifest).
