// NAnimationEvents::CManager::GetEmbeddedEventsBetweenTimes(long, CSmallVector<CEvent,32>&, float, float) -- for every timing event embedded in the animation's stats, append a CEvent (symbol index, externally-imported flag, normalised time) when its time / Duration lies in [start, end]; true when any was added
// Leaf: GetEmbeddedEventsBetweenTimes
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F9820, 311 bytes (`ret 0x10`).
// Callees: the bank (+4) GetAnimationInfo 0x00A242C0 (labelled CTCInventoryAbilities::GetAbilityLevel), C3DAnimationInfo::
//   GetAnimationStats 0x00A269C0 (CCountedPointer by hidden pointer), the stats' timing-event name list 0x00A4EF90 (labelled
//   CGraphicDataBank::ChangeScene) and per-name time 0x00A4EAE0 (labelled CRegion::SetMiniMapRegionExitTextOffsetY), the symbol
//   map (+8) Lookup 0x009E5120, CSmallVector<CEvent,32>::resize 0x006FADE0, ~CCharString 0x0099EAE0, free 0x00BFEA14,
//   operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CEvent (Index:30, ExternallyImported:1, StopEvent:1, Time), CSmallVector {Start, Size}, C3DAnimationStats
//   Duration +0x0c.
// Shape: Duration through an inline getter (its value is fetched before the time call and parked in the dead `events` argument
//   slot: right operand of `/` first); the new event is a NAMED local whose bitfields are all assigned (one and/or word in esi,
//   the time spilled once on the true path, the x87 value kept on the stack through both fcom tests); the name vector's
//   destructor is STLport's `_Destroy(first, last)` + free.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
void operator delete(void* p);                     // 0x00BFE9BC
class CCharString { public:
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
class C3DAnimationStats { public:
    unsigned char _pad00[0x0c];
    float         Duration;                        // +0x0c
    float GetDuration() const { return Duration; }
    void GetTimingEventNames(vector<CCharString>& names) const;   // 0x00A4EF90
    float GetTimingEventTime(const CCharString& name) const;      // 0x00A4EAE0
};
class C3DAnimationInfo { public:
    CCountedPointer<const C3DAnimationStats> GetAnimationStats();  // 0x00A269C0
};
struct CAnimationBank {
    C3DAnimationInfo* GetAnimationInfo(long animation);            // 0x00A242C0
};
class CSymbolMap { public:
    unsigned long Lookup(const CCharString& name);                 // 0x009E5120
};
namespace NAnimationEvents {
class CEvent { public:                                    // 8 bytes
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float         Time;
    CEvent() { }
};
template <class T, int N> struct CSmallVector {
    T*   Start;                                    // +0x00
    long Size;                                     // +0x04
    void resize(long size, const T& fill);         // 0x006FADE0
};
class CManager { public:
    void*           __vftable;                     // +0x00
    CAnimationBank* Bank;                          // +0x04
    CSymbolMap      Symbols;                       // +0x08
    bool GetEmbeddedEventsBetweenTimes(long animation, CSmallVector<CEvent, 32>& events, float start, float end);
};

bool CManager::GetEmbeddedEventsBetweenTimes(long animation, CSmallVector<CEvent, 32>& events, float start, float end)
{
    CCountedPointer<const C3DAnimationStats> stats = Bank->GetAnimationInfo(animation)->GetAnimationStats();
    vector<CCharString> names;
    stats->GetTimingEventNames(names);
    bool found = false;
    for (CCharString* name = names._M_start; name != names._M_finish; ++name) {
        unsigned long index = Symbols.Lookup(*name);
        float time = stats->GetTimingEventTime(*name) / stats->GetDuration();
        if (time >= start && time <= end) {
            CEvent event;
            event.Index = index;
            event.ExternallyImported = 1;
            event.StopEvent = 0;
            event.Time = time;
            found = true;
            events.resize(events.Size + 1, CEvent());
            events.Start[events.Size - 1] = event;
        }
    }
    return found;
}
}