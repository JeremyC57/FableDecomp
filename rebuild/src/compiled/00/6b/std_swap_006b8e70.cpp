// std::swap(_Tp& a, _Tp& b) -- (_Tp = pair<unsigned long, CCountedPointer<CFaction> >) exchange two vector-map entries through a by-value temporary, keeping the counted-pointer reference counts balanced
// Leaf: ??$swap@U?$pair@KV?$CCountedPointer@VCFaction@@@@@std@@@std@@YIXAAU?$pair@KV?$CCountedPointer@VCFaction@@@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8E70, 195 bytes, /Gr `__fastcall` (a in ecx -> esi, b in edx -> edi), plain `ret`.
// Identity (proven): std::swap<std::pair<unsigned long, CCountedPointer<CFaction> > >; evidence: the __unguarded_partition 0x006BA100 (call at 0x006BA13A) and the iter_swap stub 0x006B9EC0 (`jmp`) are its only callers; down.py: sort 0x006BB140 -> introsort_loop 0x006BB040 -> 0x006BA100 -> this; the sort is reached from 0x006B97E0 = CFactionManager::CFactionManager(const CWorld&) (zeroes Factions at +0, then its inline SortIfDirty; Ego_r.pdb Factions +0, CVectorMap<unsigned long, CCountedPointer<CFaction> >); FableWin.pdb has ??$swap@KV?$CCountedPointer@VCFaction@@@@@std@@.
// Label: the manifest's "swap<CCountedPointer<CContainmentVolume>_>" is propagated (family H047: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callees: operator delete 0x00BFE9BC (thrice: the releases inside both assignments and the temporary's destructor), and
//   the counted block's DeleteFunc through `call [reg+4]`.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over STLport's pair (user copy constructor, implicit
//   assignment) and CCountedPointer's copy constructor / assignment (landed 0x00448380 body) / destructor; FableWin.pdb
//   names this pair's swap through its own STLport's pair overload, retail emits the generic one (landed 0x00566870 model).
class CCPPointerInfo { public:                                    // CCountedPointer's shared block
    long  RefCount;                                        // +0x0
    void  (__fastcall* DeleteFunc)(void* data);            // +0x4
    void* Data;                                            // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x0
    CCPPointerInfo* Info;                                  // +0x4
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* const              data = o.Data;
        CCPPointerInfo* const info = o.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CFaction;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
typedef pair<unsigned long, CCountedPointer<CFaction> > _Elem;
template void __fastcall swap<_Elem>(_Elem&, _Elem&);
} // namespace std