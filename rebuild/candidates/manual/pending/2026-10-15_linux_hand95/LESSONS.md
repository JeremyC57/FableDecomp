# Chunk 95 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `mov edx,ecx ; call f ; ... mov ecx,edx` (this kept in a scratch register across a call), or ecx simply NOT reloaded
  before a second member call, is NOT an LTCG gap: VC7.1 tracks the registers clobbered by functions compiled EARLIER IN
  THE SAME TRANSLATION UNIT. Define the callee's body above the function with `#pragma auto_inline(off)` / `(on)` around
  it (0x0059C6B0, 0x0059C6DD, 0x0059C710, 0x0059C747 with GetCategoryIndexForTradeType 0x0059B6F4; 0x00C265B0 with the
  `return m_state == 1;` test 0x00C265A0). Earlier rows parked as "LTCG callee register knowledge" are worth a retry.
- A virtual override whose code addresses the class with NEGATIVE offsets (`mov esi,[ecx-0C8h]`, `add ecx,-130h` before a
  base call) is an override of a virtual first declared in a SECONDARY base: model `struct D : Primary, Secondary` with
  the slot declared in Secondary and VC passes the sub-object as `this` (0x005FAF10, 0x0061A220).
- The VC6 swap-with-temporary release (`vector<T>().swap(v)`) only reproduces retail's interleaved `load first ; store 0 ;
  load last ; store 0 ; store 0` and its ebp/edi colouring when the swap goes through a std::swap-style
  `template<class T> void Swap(T& a, T& b) { T t = a; a = b; b = t; }`; a hand-written three-temporary swap hoists both
  loads (0x00CBC660). The temporary's _Tidy/destructor must be `__forceinline`.
- `std::for_each(first, last, std::mem_fun_ref(&T::F))` from an SGI /Gr module: declare `template<class It, class F> F
  __fastcall for_each(It, It, F)` with a functor that has a user constructor (hidden functor result: ecx = the `push ecx`
  frame slot, first in edx, last + the 4-byte member pointer on the stack) (0x005CAC8D).
- A secondary polymorphic base whose vtable is re-stored AFTER the derived class's member destructors and before the
  primary base's destructor tail jump is an empty INLINE user destructor `~CBase() {}` on a non-virtual-destructor base
  (0x005CD2FE).
- Under `optimize("s")` the out-parameter GetInterface helper (0x007BEA08 family) must be `__forceinline`; when retail
  ignores its bool and dereferences the component unconditionally, the local starts at 0 (`xor ebx,ebx` before the
  bitset call) (0x0059D9C2, 0x005C9962).
- A Dinkumware one-argument `resize(n)` is `resize(n, T())`: the default temporary is zeroed in member order (inline
  default constructor of the counted pointer in the middle) and only its non-trivial member is destroyed after the call;
  frame pointer + size = `optimize("y", off)` + `("s", on)` (0x005C2AA9).
- A function returning a call's result through a hidden-pointer CWideString keeps the caller's hidden pointer in edi and
  returns it (`mov eax,edi ; ret 4`) when the result is returned directly (`return Get(...);`) (0x005FAF10).

Review at landing:
- No row needed more than 4 grades (max 3: 0x00CBC660). Nothing parked.
- Local views next to existing engine headers: CTCInventoryTrade_Retail (engine/CTCInventoryTrade.h sits 0x2C bytes high
  at ItemSellCount/CategoryIdent and 0x50 high at PCurrentOpenTradeMenu: retail CTCInventoryBase is 0x130, vectors and
  maps 12 bytes, NotInStock list 4 bytes -- confirmed by the retail constructor 0x0059EB1C..0x0059EC3A); rows that use
  ParentThing derive from engine/CTCBase.h. CTCInventoryBase / Map / Quests / Stats / Weapons have no headers.
- 0x0059D9C2 / 0x005C9962 / 0x005FAA60 etc. model the CTCInventoryBase primary base as a 0x130-byte blob with the
  NHeroInformationScreens::CBase vptr at +0x130 (CTCInventoryStats' destructor proves the order).
- Catalog row 0x0061B530 holds two functions (0x0061B530 and 0x0061B560): re-cut before authoring.
- 0x00C265B0's class names (CPooledResource / CPooledResourceManager) are descriptive; the module's real class is unknown.
- Already-landed CTCInventoryBase_ProcessButtonAReleased_005bc649.cpp: 0x005BC649 is the CTCInventoryBase slot-41
  implementation (CTCInventoryClothing vtable 0x0124356C slot 41) that this chunk's 0x0061A140 calls as the base
  ProcessInventoryInputLeft -- the landed name "ProcessButtonAReleased" is probably the propagated label.
