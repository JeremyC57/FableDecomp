// std::pair<CCharString,CPersistInfo>::~pair() -- implicit destructor of a quest persist-script entry: the info's persist script and quest name, then the key string
// Leaf: ??1?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B6A80, 31 bytes, thiscall, `jmp` tail; no call or address reference (an
//   unreferenced COMDAT: VC7.1 emits an implicit destructor's `??1` when the TU destroys the object by name; retail kept it).
// Identity (proven): (1) TU: 0x004B6A80 sits in the CQuestManager TU (LoadGameState 0x004B6380 / SaveGameState 0x004B64E0
//   before it, CTBaseSingletonNoDefaultAlloc<CQuestManager>'s `??_G` 0x004B6AA0 right after it -- Ego_r.pdb has that `??_G`
//   at 0x004BC770 in the same TU, which holds the pair<CCharString,CRuntimeQuestPersistence::CPersistInfo> instantiations
//   of CRuntimeQuestPersistence's two CVectorMaps PersistScripts / MasterDataPersistScripts, and no CSymbolMap one);
//   the TU's landed STLport helpers run over the same 12-byte pair (__copy 0x004B84F0, called by ClearQuestPersistScript
//   0x004B39B0 / CRuntimeQuestPersistence::ClearAll 0x004B41F0). (2) Placement: as in chunk 656 (~TeleportData 0x00CD4550
//   vs _Destroy 0x00CD4FA0), the implicit `??1` sits with the TU's class special members and its STLport `_Destroy`
//   twin 0x004B86B0 sits among the STLport helpers; the CSymbolInfo pair's pair of bodies is in the animation-events TU
//   (0x006FB460 / 0x006FB8D0), and a same-name COMDAT exists once per image, so this TU's pair is not the CSymbolInfo one.
//   The manifest label (_Dest_val<..pair<CCharString,CSymbolMap::CSymbolInfo>..>) is propagated.
// Layout: Ego_r.pdb CPersistInfo (8) = CCharString QuestName +0, PersistScript +4; the pair is 12 bytes (key +0, info +4).
// Callees: CCharString::~CCharString 0x0099EAE0 (x3).
// Shape: the implicit pair destructor emitted through a qualified explicit destructor call (helper below, not a retail
//   function); the inlined ~CPersistInfo keeps `&second` in edi (PersistScript [edi+4], QuestName [edi]), the key is the tail jmp.
#include "fable_string.h"      // CCharString::~CCharString @ 0x0099EAE0

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                 // Ego_r.pdb, 8 bytes
    public:
        CCharString QuestName;                           // +0x00
        CCharString PersistScript;                       // +0x04
    };
};
}

namespace std {
template <class _T1, class _T2>
struct pair {                                            // STLport 4.6 stl/_pair.h
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2>
    pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
};
template <class _Tp>
inline void _Destroy(_Tp* __pointer)                     // STLport 4.6 stl/_construct.h
{
    __pointer;
    __pointer->~_Tp();
}
}

typedef std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> TPersistEntry;

void TUHarness_DestroyPersistEntry(TPersistEntry* entry)    // emits the implicit destructor; not part of the retail body
{
    entry->TPersistEntry::~TPersistEntry();
}