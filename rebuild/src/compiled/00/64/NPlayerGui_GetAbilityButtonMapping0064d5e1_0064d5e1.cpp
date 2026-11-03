#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::GetAbilityButtonMapping_0064d5e1(std::vector<long>&) -- reset the mapping to the fixed button order 3, 1, 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D5E1, length 78 bytes.
// Callees: vector<long>::erase(first, last) @ 0x00440590 (the inlined clear(): `push [esi+4] ; push [esi]`), vector<long>::push_back(const long&)
// @ 0x00485FF3 three times. Frame-pointer size TU (`optimize("y",off)` + `("s",on)`); `this` is never read, so the body is a free
// `__stdcall` function with one argument (`ret 4`). Shape: each push_back's temporary is homed in the dead argument slot
// (`mov [ebp+8],imm ; lea eax,[ebp+8]`), the vector pointer stays in esi. The sibling @ 0x0064D593 writes a different order; the
// manifest label "CDrawAbilityButtons::GetAbilityButtonMapping" names the family (two PC/pad mappings).
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
template <> class vector<long, allocator<long> > { public:   // STLport vector<long> (12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    long* erase(long* first, long* last);          // 0x00440590
    void push_back(const long& value);             // 0x00485FF3
    void clear() { erase(m_pFirst, m_pLast); }
};
}

void __stdcall GetAbilityButtonMapping_0064d5e1(std::vector<long>& mapping)
{
    mapping.clear();
    mapping.push_back(3);
    mapping.push_back(1);
    mapping.push_back(2);
}