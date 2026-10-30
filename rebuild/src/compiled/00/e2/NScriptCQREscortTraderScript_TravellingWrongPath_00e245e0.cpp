// NScript::CQR_EscortTraderScript::TravellingWrongPath() -- is the escort off its trade route? A current region on the route's dynamic list is fine (and, coming from the previous region, that region is struck off the list); otherwise, once a wrong turn was already detected, a region on the static list is fine too; any other region flags a wrong turn and returns true
// Leaf: ?TravellingWrongPath@CQR_EscortTraderScript@NScript@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E245E0, 372 bytes, thiscall, plain `ret`, returns bool.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?TravellingWrongPath@CQR_EscortTraderScript@NScript@@QAE_NXZ (the class's one
//   bool member without a landed body: InFinalRegion 0x00E24210 and GetTraderRandomSpeechEnabled 0x00E24260 are landed next to
//   it); the only caller is CTraderToEscort::Main 0x00E20600 (site 0x00E22692, `this` = ParentClass), and every member the body
//   reads is a CQR_EscortTraderScript one at its retail offset (TradeRoute, RouteDynamic, RouteStatic, PreviousLoadedRegion,
//   CurrentLoadedRegion, WrongTurnDetected). Catalog label StringList_FindDuplicateEntry (descriptive, wrong).
// Callees: CBasicString<char>::Compare 0x00411570 (__fastcall, the inline CCharString::operator==), std::vector<CCharString>::erase
//   (iterator) 0x004B8770.
// Layout (retail = Ego_r.pdb - 0x14 before the STL members; 12-byte STLport vectors): WrongTurnDetected +0x5f, TradeRoute +0xc4
//   (ETradeRoutes), RouteDynamic[3] +0xc8, RouteStatic[3] +0xec, PreviousLoadedRegion +0x110, CurrentLoadedRegion +0x114.
// Shape: each scan is a `for (it = v[TradeRoute].begin(); it != v[TradeRoute].end(); ++it)` whose end is re-read through
//   TradeRoute every pass; the inline CCharString == of the landed MonitorRegionPresence 0x00D6CDC0 / CBasicString<char>.
//   Compare's body must be in the TU (auto_inline(off), the chunk 576 0x00F088E0 trick): VC7.1 then knows the compares write
//   no memory, keeps CurrentLoadedRegion's data pointer in ebx and reuses the pre-loop &RouteDynamic[TradeRoute] / end /
//   RouteDynamic-RouteStatic base in the frame (declared only: 363v372, every value reloaded after each call).
#pragma pack(push, 1)
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const CBasicString& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) ? false : true;
    }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
// The Compare body (landed 0x00411570) compiled in this TU under auto_inline(off), as the landed
// CQ_AmbushTradersScript::EntityCountDecrease 0x00F088E0 does: VC7.1 then sees that the comparison writes no memory.
#pragma auto_inline(off)
template <> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (!PStringData || !other.PStringData)
            return false;
        return PStringData->Data == other.PStringData->Data;
    }
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position);                // 0x004B8770 for CCharString
};
}
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQR_EscortTraderScript : public CScriptBase {
public:
    enum ETradeRoutes { };
    unsigned char _pad40[0x1f];                         // +0x40 Game, PMasterData, the flags before WrongTurnDetected
    bool          WrongTurnDetected;                    // +0x5f
    unsigned char _pad60[0x64];                         // +0x60 .. +0xc4
    ETradeRoutes  TradeRoute;                           // +0xc4
    std::vector<CCharString> RouteDynamic[3];           // +0xc8
    std::vector<CCharString> RouteStatic[3];            // +0xec
    CCharString   PreviousLoadedRegion;                 // +0x110
    CCharString   CurrentLoadedRegion;                  // +0x114
    bool TravellingWrongPath();
};

bool CQR_EscortTraderScript::TravellingWrongPath()
{
    std::vector<CCharString>::iterator it;
    for (it = RouteDynamic[TradeRoute].begin(); it != RouteDynamic[TradeRoute].end(); ++it) {
        if (*it == CurrentLoadedRegion) {
            for (it = RouteDynamic[TradeRoute].begin(); it != RouteDynamic[TradeRoute].end(); ++it) {
                if (*it == PreviousLoadedRegion) {
                    RouteDynamic[TradeRoute].erase(it);
                    break;
                }
            }
            WrongTurnDetected = false;
            return false;
        }
    }
    if (WrongTurnDetected) {
        for (it = RouteStatic[TradeRoute].begin(); it != RouteStatic[TradeRoute].end(); ++it) {
            if (*it == CurrentLoadedRegion) {
                WrongTurnDetected = false;
                return false;
            }
        }
    }
    WrongTurnDetected = true;
    return true;
}
}   // namespace NScript