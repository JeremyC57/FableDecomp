// std::swap(pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>& a, pair<...>& b) -- exchange two quest-persistence entries (key string + quest name + persist script) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B7800, 148 bytes, /Gr `__fastcall` (a in ecx -> edi, b in edx -> ebx), plain `ret`.
// Identity: the quest-persistence vector map's sort TU (neighbours: the landed pair<CCharString,
//   NQuestManager::CRuntimeQuestPersistence::CPersistInfo> constructors 0x004B7490 / 0x004B7650 and its __copy_backward
//   0x004B78D0); callers are the out-of-line iter_swap 0x004B86A0 (a bare `jmp`) and the partition step 0x004BA0D3.
//   Ego_r.pdb CPersistInfo (8 bytes): CCharString QuestName +0, CCharString PersistScript +4.
// Callees: CCharString copy ctor 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over STLport's pair (user copy constructor, implicit
//   assignment): three string copies, three assignments per direction, the temporary destroyed in reverse order.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                // Ego_r.pdb, 8 bytes
    public:
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

typedef pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> TQuestPersistPair;
template void __fastcall swap<TQuestPersistPair>(TQuestPersistPair&, TQuestPersistPair&);
}