# Identity and layout evidence

Exact leaf: `?CloseInventory@CTCInventoryMagic@@UAEXXZ`.

Ego_r name set 00744B30 and FableWin 02555930 give the same public virtual member. The original FableWin body calls 01804620, a thunk to 0236AC40, then clears the member vector at donor offset +0x15c. Both PDBs identify the base member as `?CloseInventory@CTCInventoryBase@NInventory@@UAEXXZ` (Ego_r 00620440, FableWin 0236AC40).

Retail primary vtable 012668EC slot31 points to 0072DC40. The native constructor 0072DB90 establishes NInventory::CTCInventoryBase at zero, NHeroInformationScreens::CBase at +0x130 and InventoryMagicPrimitiveHandles at +0x134. The original type record names that vector (Ego_r donor offset +0x14c). The existing real shared bases plus the real 12-byte vector yield size 0x140. No opaque or invented members are used.

Retail 005BD597 is the NInventory base CloseInventory, also slot31 in base vtable 01253DBC. Stale alternative bindings are reported in NAME_FIXES.tsv; none are copied into this row. The vector erase call 00440D70 uses its existing exact canonical specialization.

The retail extent is 35 bytes, ending after the ret at 0072DC62. This is an ordinary complete virtual member, despite the input Unwind label. The two relocated operands were independently decoded from the executable and compared to the object's exact symbol table. See the selected history RELOCATION_AUDIT.json.

Behavior tests retain the complete method body and substitute traceable external callees: six combinations of empty/single/multiple handles and initial inventory open/closed state, followed by repeated close. They check ordering, release counts, capacity and unrelated state. Production shared-header size is checked by the real VC71 compilation. Both C++03 toolchains passed.
