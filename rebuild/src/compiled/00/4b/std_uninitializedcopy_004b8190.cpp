// std::__uninitialized_copy(CQuestManager::CActivateQuestInfo* first, CQuestManager::CActivateQuestInfo* last, CQuestManager::CActivateQuestInfo* result, const __false_type&) -- copy-construct [first, last) into raw storage at result
// Leaf: ??$__uninitialized_copy@PAVCActivateQuestInfo@CQuestManager@@PAV12@@std@@YIPAVCActivateQuestInfo@CQuestManager@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8190, 64 bytes, `__fastcall` (first ecx, last edx), `ret 8`; returns the end of the run.
// Identity (proven): bound ??$__uninitialized_copy@PAVCActivateQuestInfo@CQuestManager@@PAV12@@std@@...; its callers are the landed vector<CQuestManager::CActivateQuestInfo>::_M_insert_overflow 0x004BB2E0 (prefix and suffix moves); Ego_r.pdb CActivateQuestInfo {CCharString QuestName, CScriptInfo* PScriptInfo, bool GameScript, bool LoadResources}.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the element's implicit copy constructor inline).
// Shape (family M083, representative 0x004B8190): STLport `for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur,
//   *__first); return __cur;` -- the placement new's null test, the string copy, then the scalar members; speed TU.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CCharString {                                         // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
class CScriptInfo;
class CQuestManager {
public:
    class CActivateQuestInfo {                            // Ego_r.pdb, 12 bytes
    public:
        CCharString QuestName;                            // +0x0
        CScriptInfo* PScriptInfo;                         // +0x4
        bool GameScript;                                  // +0x8
        bool LoadResources;                               // +0x9
    };
};
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
typedef ::CQuestManager::CActivateQuestInfo _E;
template _E* __fastcall __uninitialized_copy<_E*, _E*>(_E*, _E*, _E*, const __false_type&);
}