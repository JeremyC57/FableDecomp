# NF203 lessons (one line each)
- The generated `CRetailTree_erase<addr>` dtor pairs split into ~_Rb_tree / ~map by COMDAT run order (tree run, then map() / ~map); with FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY the three tree rows port in one grade each.
- A port's first grade should diff smart-pointer dtor / operator= shape before blaming the loop wording: 0x005DCF70 spent three forms on the loop while all three carried the default ~CIVCountedPointer null-store shape.
- A test that links the row's out-of-line callee can replace it with an explicit specialization (`template<> void TTree::_M_erase(...)`) placed before the row's explicit instantiation.
- Tests on shared headers that include fable_string.h need stub CCharString() / CWideString() ctors when a class's vtable names by-value string returns.
- Copy every mangled leaf from the object: a hand-built ~map leaf had @3@ where VC7.1 emits @4@.
