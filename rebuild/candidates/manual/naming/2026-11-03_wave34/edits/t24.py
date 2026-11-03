# T24: 0x00485FF3 is std::vector<long>::push_back(const long&) (Ego_r/FableWin ?push_back@?$vector@JV?$allocator@J@std@@@std@@QAEXABJ@Z),
# cx12 4329. The CLongVector / CRetailLongVector stand-ins became std::vector<long> by vecspec.py (same 12-byte members).
A = "rebuild/src/compiled/00/66/std_copyRbtreeiteratorlongbackinsertiteratorvectorlo_006611f7.cpp"
B = "rebuild/src/compiled/00/5e/CTCInventoryAbilities_AddLearnedExpression_005eedcf.cpp"
EDITS = [
    (A, "template <class _Tp> class vector {", "template <class _Tp> class allocator;\ntemplate <class _Tp, class _Alloc = allocator<_Tp> > class vector {"),
    (B, """struct FableAddLearned_005eedcf_sub
{
    void helper(long* parg);
};""", """namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:   // STLport vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& value);               // 0x00485FF3
};
}"""),
    (B, "    FableAddLearned_005eedcf_sub sub;   // +0x1bc", "    std::vector<long> m_1bc;            // +0x1bc"),
    (B, "    this->sub.helper(&arg);", "    m_1bc.push_back(arg);"),
]
