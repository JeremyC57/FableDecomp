// ---- TU-local models (no engine header / no retail branch for these classes) ----
class CGameTimeManager {                                 // Ego_r.pdb CGameTimeManager (methods only)
public:
    bool IsDay() const;                                  // 0x006BB2C0 ?IsDay@CGameTimeManager@@QBE_NXZ
    bool IsTimeBetween(long start, long end) const;      // 0x006BB390 ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z (Ego_r / FableWin; reads Time +0x8)
};
class CTCLight : public CTCBase {                        // RTTI .?AVCTCLight@@ : CTCBase (interface TCI_LIGHT)
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_LIGHT; }   // FableWin ?StatGetInterfaceType@CTCLight@@SI..
    void SetActive(bool active);                         // 0x006412B0 ?SetActive@CTCLight@@QAEX_N@Z
};
enum EVillageTaskType { };                               // Ego_r.pdb CVillageDef::Tasks element type (enumerator names unproven)
class CVillageDef : public CSubDefClassBase {            // Ego_r.pdb CVillageDef (retail STLport offsets; HouseLights +0xCC/+0xD0)
public:
    std::vector<EVillageTaskType> Tasks;                 // +0x28
    CDefString     PatrolTrackName;                      // +0x34
    long           DefaultStanceToHero;                  // +0x38 EVillageStanceToHero
    long           SecondsSecuritySweepLasts;            // +0x3c
    long           WarehouseCrateDef;                    // +0x40
    std::vector<long> StoryStartTimes;                   // +0x44
    long           StoryDuration;                        // +0x50
    std::vector<long> SchoolStartTimes;                  // +0x54
    long           SchoolDuration;                       // +0x60
    std::vector<long> WorkStartTimes;                    // +0x64
    long           WorkDuration;                         // +0x70
    std::vector<long> CookStartTimes;                    // +0x74
    long           CookDuration;                         // +0x80
    std::vector<long> EatStartTimes;                     // +0x84
    long           EatDuration;                          // +0x90
    std::vector<long> TavernStartTimes;                  // +0x94
    long           TavernDuration;                       // +0xa0
    std::vector<long> GamingStartTimes;                  // +0xa4
    long           GamingDuration;                       // +0xb0
    long           SleepAdultStartTime;                  // +0xb4
    long           SleepAdultEndTime;                    // +0xb8
    long           SleepChildStartTime;                  // +0xbc
    long           SleepChildEndTime;                    // +0xc0
    long           SleepElderlyStartTime;                // +0xc4
    long           SleepElderlyEndTime;                  // +0xc8
    long           HouseLightsOnTime;                    // +0xcc
    long           HouseLightsOffTime;                   // +0xd0
};
// CThingObject: GetProperties is CThing-hierarchy virtual slot 48 (retail `call [eax+0C0h]`; FableWin ?GetProperties@CThingObject@@UBEKXZ).
// Header debt: no engine header models the CThing vtable (slots 3..47 of the retail chain), so the slots in between are
// positional placeholders here (CThingGameObject's retail branch already puts OnHit at slot 3).
class CThingObject : public CThingGameObject {
public:
    virtual void VtableSlot04();
    virtual void VtableSlot05();
    virtual void VtableSlot06();
    virtual void VtableSlot07();
    virtual void VtableSlot08();
    virtual void VtableSlot09();
    virtual void VtableSlot10();
    virtual void VtableSlot11();
    virtual void VtableSlot12();
    virtual void VtableSlot13();
    virtual void VtableSlot14();
    virtual void VtableSlot15();
    virtual void VtableSlot16();
    virtual void VtableSlot17();
    virtual void VtableSlot18();
    virtual void VtableSlot19();
    virtual void VtableSlot20();
    virtual void VtableSlot21();
    virtual void VtableSlot22();
    virtual void VtableSlot23();
    virtual void VtableSlot24();
    virtual void VtableSlot25();
    virtual void VtableSlot26();
    virtual void VtableSlot27();
    virtual void VtableSlot28();
    virtual void VtableSlot29();
    virtual void VtableSlot30();
    virtual void VtableSlot31();
    virtual void VtableSlot32();
    virtual void VtableSlot33();
    virtual void VtableSlot34();
    virtual void VtableSlot35();
    virtual void VtableSlot36();
    virtual void VtableSlot37();
    virtual void VtableSlot38();
    virtual void VtableSlot39();
    virtual void VtableSlot40();
    virtual void VtableSlot41();
    virtual void VtableSlot42();
    virtual void VtableSlot43();
    virtual void VtableSlot44();
    virtual void VtableSlot45();
    virtual void VtableSlot46();
    virtual void VtableSlot47();
    virtual unsigned long GetProperties() const;          // slot 48
};
class CThingTrackNode : public CThingGameObject {        // Ego_r.pdb CThingTrackNode (retail CThingGameObject 0xD0 -> members from +0xD0)
public:
    unsigned char    NodeLinks[0x8];                     // +0xd0 Ego_r unions CThingTrackNode_u_232 / _u_236 (unmodelled)
    unsigned __int64 NextNodeUID;                        // +0xd8
    unsigned __int64 PreviousNodeUID;                    // +0xe0
    CCharString      TrackName;                          // +0xe8
    bool             IsStart;                            // +0xec
    bool             IsEnd;                              // +0xed
};
#include "engine/CVillageTaskPatrol.h"
#include "engine/CVillageTaskPickCrops.h"
// The TU's own out-of-line template bodies (retail COMDATs): IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340,
// std::find<const EVillageTaskType*, EVillageTaskType> 0x0065E5A4 -> std::__find 0x0065D9EE (STLport 4.6 _algobase.c).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing);
template <> __declspec(noinline) bool __fastcall IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& thing)
{
    return thing.Get() && !thing->IsDead();
}
namespace std {
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                                         const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}
template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}
}
#pragma auto_inline(on)
