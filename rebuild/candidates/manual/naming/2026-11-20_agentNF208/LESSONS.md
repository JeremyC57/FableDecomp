# NF208 lessons (one line each)
- A row whose same-TU stand-in overflow body exists only for the tag slot (GOTCHAS b48) ports onto the shared vector with FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE plus the owner row's own inline macros: the visible body keeps the slot and its pick-any copy is cmpbody-equal to the owner (five rows, one form each after the first).
- Declaration-only is not a free substitute for a visible noinline body: 0x008AE5E0 went 115 -> 92 bytes (the tag got its own push ecx word).
- Build the owner object once (work/owner_<addr>.obj) and cmpbody every new object against it; also compare the side COMDATs (push_back) across the set, since a stand-in owner row cannot be the reference.
- A test on the shared headers can set a protected base member through a pointer to member formed in a derived helper (`&Derived::Member` has type `T Base::*`); private members (CVectorMap::Dirty) cannot be reached, so drop that check rather than add a model.
- vland's TCC_FAIL / LINK_FAIL give no message: reproduce with a /Od compile + link of the test (tools/tcc.py) before regrading.
