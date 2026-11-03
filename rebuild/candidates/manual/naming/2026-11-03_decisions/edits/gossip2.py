A = "template <class _Pair> struct _Select1st { };          // STLport 4.6 _function.h (map's KeyOfValue)\n"
B = A + "template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {\n    _Value _M_value_field;                               // +0x10\n};\n"
EDITS = [("rebuild/src/compiled/00/8a/CTCAIScratchpad_GetGossip_008ae510.cpp", A, B),
         ("rebuild/src/compiled/00/8a/CTCAIScratchpad_GetGossipFactions_008ae970.cpp", A, B)]
