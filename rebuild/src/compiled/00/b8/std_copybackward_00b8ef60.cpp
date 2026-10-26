// std::__copy_backward(pair* first, pair* last, pair* result, const random_access_iterator_tag&, int*) -- (pair<unsigned long, CCountedPointer<CVertexBufferWin32> >) assign last-first elements back to front into the range ending at result; returns the new result
// Leaf: ??$__copy_backward@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@PAU12@H@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8EF60, 149 bytes, `__fastcall` (first in ecx, last in edx; result, the tag reference and the null distance pointer on the stack, `ret 0Ch`).
// Identity (proven): std::__copy_backward<pair<unsigned long, CCountedPointer<CVertexBufferWin32> >*, pair<...>*, int> (STLport 4.6 _algobase.h; the PDB builds are Dinkumware); evidence: its caller is the 12-byte-element vector insert 0x00B8F950. That insert's only caller, 0x00B8DCD0, first searches the vector at this+0x24054 (`add ecx, 24054h`, _M_finish at +0x24058). That is CEngineOcclusionManager::FlareVertexBuffers, vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >: retail keeps Ego_r's fixed arrays up to FlareTests +0x24024 and gives the four 12-byte containers after it 0x24024, 0x24030, 0x2403C, 0x24048, so FlareVertexBuffers is at +0x24054. FableWin.pdb ?SelectFlareTestVertexBuffer@CEngineOcclusionManager@@AAEXK@Z; the second caller, 0x00B8F160, is an unreferenced copy_backward wrapper
// Label: the manifest's "_Move_backward<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBas" is propagated and wrong (a 16-byte element; family J051: five byte-identical bodies, one per 12-byte element type).
// Callees: the DeleteFunc of the count block (`call [Info+4]`, Data in ecx) and operator delete 0x00BFE9BC, from the inlined CCountedPointer assignment.
// Body: `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;` -- the count (/12) kept in the dead
//   result argument slot, each pair assignment the implicit one (key, then CCountedPointer::operator= = ShareData).
// Layout: pair<K, CCountedPointer<V> > is 12 bytes {first +0, second.Data +4, second.Info +8}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
// Model: the landed 0x00A802A0 body (CNavConnectorNode's instance) on the real STLport signature and agent950's landed CCountedPointer.
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
class CVertexBufferWin32;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}   // namespace std
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef pair<unsigned long, CCountedPointer<CVertexBufferWin32> > _Entry;
template _Entry* __fastcall __copy_backward<_Entry*, _Entry*, int>(_Entry*, _Entry*, _Entry*, const random_access_iterator_tag&, int*);
}   // namespace std