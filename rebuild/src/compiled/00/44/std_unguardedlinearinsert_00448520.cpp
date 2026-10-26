// std::__unguarded_linear_insert(EGameAction* last, pair val, CKeyPairCompareLess comp) -- (pair<EGameAction, CCountedPointer<const CInputTypeBase> >) shift each element whose key is above val's one slot up, then store val in the hole
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@U12@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@std@@YIXPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@U10@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00448520, 186 bytes, `__fastcall` (last in ecx; the 12-byte val and the empty 4-byte comparator on the stack, `ret 10h`).
// Identity (proven): std::__unguarded_linear_insert<pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, pair<...>, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> > > (FableWin.pdb name set); evidence: its callers, the unguarded insertion sort 0x00448910 and the linear insert 0x00448E90, sit under sort 0x004495D0. CGamePlayerInterface::SetControlScheme 0x00446EF0 calls that sort on this+0x7A8 (`lea ebx, [esi+7A8h]`, sorted when the Dirty byte +0xD is set). That member is Ego_r.pdb CGamePlayerInterface::Controls at +1960, CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> >; FableWin.pdb's Dinkumware build has _Insertion_sort1 over the same pair; the key compare is signed (jl/jge), and Ego_r.pdb has exactly five CVectorMap<signed key, CCountedPointer<X> > members (CGamePlayerInterface::Controls, CCachedVolumes::NavigationContainmentVolumes, CRegionConnectionGraph::GraphNodes, CNavQuadTree::ConnectorNodes, CParticleEmitterDatabase::Emitters), one per J050 row
// Label: the manifest's "Vector_DestroyRange" is invented/propagated (family J050: five byte-identical bodies, one per element type; retail has no identical-code folding).
// Callees: the DeleteFunc of the count block (`call [Info+4]`, Data in ecx) and operator delete 0x00BFE9BC, both from the inlined CCountedPointer assignment and destructor.
// Body: STLport 4.6 _algo.c `__next = __last; --__next; while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; } *__last = __val;`,
//   the comparator comparing the keys only (signed), each pair assignment the implicit one (key, then CCountedPointer::operator= = ShareData), then ~val.
// Layout: pair<K, CCountedPointer<V> > is 12 bytes {first +0, second.Data +4, second.Info +8}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
// Model: agent950's landed CCountedPointer (0x00A99110: ReleaseData / ShareData) and the landed 0x00448910 caller model.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0
    void (__fastcall* DeleteFunc)(void* data);              // +4, Data in ecx
    void* Data;                                             // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};
enum EGameAction { };                                      // Ego_r.pdb (enumerators not needed)
class CInputTypeBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}   // namespace std
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const std::pair<K, V>& a, const K& key) const { return a.first < key; }
};
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
typedef pair<EGameAction, CCountedPointer<const CInputTypeBase> > _Entry;
template void __fastcall __unguarded_linear_insert<_Entry*, _Entry, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> > >(_Entry*, _Entry, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> >);
}   // namespace std