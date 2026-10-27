// std::__median(const pair<CCharString, CCountedPointer<CEntityScriptBindingBase>>& a, const pair<...>& b, const pair<...>& c, CCompareString<CCountedPointer<CEntityScriptBindingBase>> comp) -- the median of three pairs by their CCharString key
// Leaf: ??$__median@U?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@std@@YIABU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@ABU10@00V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00cba620, 94 bytes, /Gr `__fastcall` (a in ecx, b in edx, c and the comparator on the stack), `ret 8`.
// Identity (proven): called (four times) by the introsort loop 0x00CBDAD0 over 12-byte {CCharString, CCountedPointer<CEntityScriptBindingBase>} pairs (its TU's swap 0x00CBA8E0 and operator= 0x00CBA700 are bound to pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >); Ego_r.pdb sorts that pair with CCompareString<CCountedPointer<CEntityScriptBindingBase> >.
// Callee: CCharString::operator<(const CCharString&) const 0x009E6530 (Ego_r.pdb ??MCCharString@@QBE_NABV0@@Z; the comparator inline).
// Body: STLport 4.6 __median (the nested comp(a, b) / comp(b, c) / comp(a, c) ladder returning a reference).
class CCharString {
public:
    void* PStringData;
    bool operator<(const CCharString& other) const;        // 0x009E6530
};
class CEntityScriptBindingBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
};
}
template <class T> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, T>& a, const std::pair<CCharString, T>& b) const { return a.first < b.first; }
};
namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template const pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >& __fastcall __median(const pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >&, const pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >&, const pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >&, CCompareString<CCountedPointer<CEntityScriptBindingBase> >);
}