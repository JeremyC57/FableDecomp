# T28 (pass-12 queued_correct_open): STLport vector takes its allocator parameter; _M_insert_overflow is protected
# (Ego_r ?_M_insert_overflow@?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@IAEXPAVCScriptThing@@ABV3@ABU__false_type@2@I_N@Z);
# 0x00568850 = std::vector<float>::push_back (CRetailVector stand-in respelled).
A = "rebuild/src/compiled/00/8a/STLvector_pushback_008adf90.cpp"
B = "rebuild/src/compiled/00/d7/NScriptCQHangingTreeGoodScript_StoreGuardsHealth_00d77340.cpp"
EDITS = [
    (A, "template <class _Tp>\nclass vector {\npublic:", "template <class _Tp> class allocator;\n\ntemplate <class _Tp, class _Alloc = allocator<_Tp> >\nclass vector {\npublic:"),
    (A, "    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);\n    void push_back(const _Tp& __x);\n",
        "    void push_back(const _Tp& __x);\nprotected:\n    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);\n"),
    (A, "template <class _Tp>\n__declspec(noinline) void vector<_Tp>::_M_insert_overflow(", "template <class _Tp, class _Alloc>\n__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow("),
    (A, "template <class _Tp>\nvoid vector<_Tp>::push_back(", "template <class _Tp, class _Alloc>\nvoid vector<_Tp, _Alloc>::push_back("),
    (B, "template <class T> struct CRetailVector {        // retail vector: begin, end, capacity",
        "namespace std {\ntemplate <class T> class allocator;\ntemplate <class T, class A = allocator<T> > class vector { public:   // STLport vector: begin, end, capacity"),
    (B, "    void push_back(const T& value);              // 0x00568850 (float instance)\n};\n", "    void push_back(const T& value);              // 0x00568850 (float instance)\n};\n}\n"),
    (B, "CRetailVector<", "std::vector<", 2),
]
