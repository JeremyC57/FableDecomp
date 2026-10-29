// CCombatActionBase::SetupCombatStrikeEvents() -- build one CStrikeEvent per strike-start event of the action's animation (weapon doing the damage, its weapon-trail strike index, the first helper point name, the start frame), then give each strike event the nearest matching end frame (dropping the events without one), shift both frames by the anim delay and record the last end frame
// Leaf: SetupCombatStrikeEvents
// Retail: Fable.exe (TLC, VC7.1) @ 0x00859A70, 775 bytes (784 with padding), 8-byte aligned ebp frame, plain `ret`.
// Callees: the anim-manager getter 0x006F90B0 (`__fastcall`) and CManager::GetEvents (virtual slot 0), the same-TU
//   strike-event name parser 0x00856780 (`__fastcall`; manifest "CComboStrike_CheckPattern" is propagated),
//   GetThingDoingDamageFromCreatureAndEventName 0x00859040 and GetHelperPointNamesFromStrikeEventName 0x00858E00 (both
//   `__fastcall`), CBaseIntelligentPointer::GetPItem 0x00A01B50, CInterfaceMap::LowerBound 0x0040F020 (the inlined
//   GetInterface for the weapon trail, interface 0x4c), CTCWeaponTrail::GetStrikeEventIndexFromEventName 0x0076DF90,
//   operator new 0x00BFEA1A, CStrikeEvent::CStrikeEvent 0x0085ADB0, CCountedPointer<CStrikeEvent>(T*) 0x0085B250 and its
//   out-of-line destructor 0x0062D9A0 (manifest "_Dest_val<...COptimisedPrimitive...>" is a folded body), the strike list's
//   push_back 0x0085AE20 (manifest "_Buynode" is propagated), ~vector<CCharString> 0x00414EA0 (manifest
//   "CCombatWheel::ResetRings" is propagated), the end-event finder's operator() 0x0085B720 (manifest
//   "CDefendingCombatantInfo_SetupHelperRings" is propagated), virtual GetAnimDelayTime (slot 6, +0x18), _ftol 0x00BFEA70,
//   ~CCharString 0x0099EAE0, operator delete 0x00BFE9BC, free 0x00BFEA14 (the event vector's storage and the list nodes).
//   Data: g_FramesPerSecond 0x01375550 (long).
// Layout: engine/CCombatActionBase.h is quarantined (PDB build differs); retail offsets follow the landed
//   InitialiseActionTimes 0x00856A20 / SetupCombatInterrupts 0x00858A20 models: PCreature +8, Initialised +0x63 (PDB
//   CCreatureActionBase, unchanged below +0x84), PAnimEntry +0x74, the STLport strike list (one node pointer) +0xbc,
//   LastStrikeEventEndTime +0xc0. CStrikeEvent (retail 0x28): StartFrame +0xc, EndFrame +0x10 (Ego_r.pdb).
//   CThing: interface bitset +0x20, interface map +0x44, Dead bit +0x91.
// Shape: the frame rate is a float local and the 1.0f time scale folds into the fild/_ftol round trips of the two frames.
//   The parser is a same-TU stand-in (it only writes through its four bool references, so two of them stay cached in
//   ebx/ebp across the later calls). The creature is a named local fetched before the damage lookup (otherwise the bools
//   are pushed before GetPItem). GetInterface spells `m_interfaces.last` (no map pointer local). The end-event search is
//   for_each(events.begin(), events.end(), CFindStrikeEventEnd(event)) through inline begin()/end() accessors (field reads
//   schedule the `last` register move one store later); the finder's NearestFrame starts at 10000000000 (0x540BE400 once
//   truncated to long). The list erase is inline (unlink, counted-pointer release, zeroed words, free).
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
extern long g_FramesPerSecond;                         // 0x01375550

class CCharString { public:
    void* m_pData;
    ~CCharString();                                    // 0x0099EAE0
};

struct CAnimEvent {                                    // std::pair<CCharString, float>
    CCharString Name;
    float       Time;
};
struct CAnimEventVector {                              // std::vector<std::pair<CCharString, float> >
    CAnimEvent* First;
    CAnimEvent* Last;
    CAnimEvent* End;
    CAnimEventVector() : First(0), Last(0), End(0) { }
    CAnimEvent* begin() const { return First; }
    CAnimEvent* end() const { return Last; }
    static void Destroy(CAnimEvent* first, CAnimEvent* last)
    {
        for (; first != last; ++first)
            first->~CAnimEvent();
    }
    ~CAnimEventVector()
    {
        Destroy(First, Last);
        if (First)
            free(First);
    }
};
struct CCharStringVector {                             // std::vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CCharStringVector();                              // 0x00414EA0
    CCharString& front() { return *_M_start; }
};

namespace NAnimationEvents {
class CManager { public:
    virtual void GetEvents(unsigned long animID, CAnimEventVector& events) const;          // slot 0
};
}
NAnimationEvents::CManager* __fastcall GetAnimManager();   // 0x006F90B0

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);              // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                            // +0x91 bit 0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CTCWeaponTrail { public:                                // interface 0x4c
    long GetStrikeEventIndexFromEventName(const CCharString& eventName);   // 0x0076DF90
};

bool StrikeEventNameHasFlag(const CCharString& eventName, long flag);   // stand-in callee of the body below
#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line parser 0x00856780 (the retail body sits in this TU and only writes through its
// reference arguments, so VC7.1 knows they do not escape).
__declspec(noinline) bool __fastcall ParseStrikeEventName(const CCharString& eventName, bool& isStart, bool& isWeapon,
                                                          bool& legacy, bool& primaryLegacy)   // 0x00856780
{
    isWeapon = StrikeEventNameHasFlag(eventName, 0);
    legacy = StrikeEventNameHasFlag(eventName, 1);
    primaryLegacy = StrikeEventNameHasFlag(eventName, 2);
    isStart = StrikeEventNameHasFlag(eventName, 3);
    return StrikeEventNameHasFlag(eventName, 4);
}
#pragma auto_inline(on)
CThing* __fastcall GetThingDoingDamageFromCreatureAndEventName(CThing* creature, const CCharString& eventName,
                                                               bool isWeapon, bool legacy, bool primaryLegacy);   // 0x00859040
void __fastcall GetHelperPointNamesFromStrikeEventName(const CCharString& eventName, CCharStringVector& names,
                                                       bool isWeapon, bool legacy, bool primaryLegacy);   // 0x00858E00

class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data);                          // 0x0085B250
    ~CCountedPointer();                                // 0x0062D9A0
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void operator delete(void* p);                         // 0x00BFE9BC

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pData;
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
class CAnimationEntry { public:
    unsigned long AnimID;                              // +0x00
};

class CCombatActionBase { public:
    class CStrikeEvent { public:                              // retail 0x28 bytes
        bool          Legacy;                          // +0x00
        bool          PrimaryLegacy;                   // +0x01
        bool          Weapon;                          // +0x02
        CCharString   HelperPointName;                 // +0x04
        long          StrikeEventIndex;                // +0x08
        long          StartFrame;                      // +0x0c
        long          EndFrame;                        // +0x10
        unsigned char _pad14[0x28 - 0x14];             // PWeapon, StruckThingList
        CStrikeEvent(CThing* weapon, long strikeEventIndex, const CCharString& helperPointName, long startFrame,
                     bool isWeapon, bool legacy, bool primaryLegacy);   // 0x0085ADB0
    };
    struct CFindStrikeEventEnd {                       // the nearest end event of one strike event
        CStrikeEvent* Event;
        long          NearestFrame;
        bool          Found;
        CFindStrikeEventEnd(CStrikeEvent* strikeEvent) : Event(strikeEvent), NearestFrame(10000000000), Found(false) { }
        void operator()(const CAnimEvent& animEvent);  // 0x0085B720
    };
    struct CStrikeEventNode {                          // STLport _List_node
        CStrikeEventNode*             _M_next;
        CStrikeEventNode*             _M_prev;
        CCountedPointer<CStrikeEvent> _M_data;
    };
    struct CStrikeEventList {                          // std::list<CCountedPointer<CStrikeEvent> > (STLport, 4 bytes)
        CStrikeEventNode* _M_node;
        CStrikeEventNode* begin() const { return _M_node->_M_next; }
        CStrikeEventNode* end() const { return _M_node; }
        void push_back(const CCountedPointer<CStrikeEvent>& value);   // 0x0085AE20
        CStrikeEventNode* erase(CStrikeEventNode* position)
        {
            CStrikeEventNode* next = position->_M_next;
            CStrikeEventNode* prev = position->_M_prev;
            prev->_M_next = next;
            next->_M_prev = prev;
            CCPPointerInfo* info = position->_M_data.Info;
            if (info) {
                --info->RefCount;
                if (position->_M_data.Info->RefCount == 0) {
                    position->_M_data.Info->DeleteFunc(position->_M_data.Info->Data);
                    delete position->_M_data.Info;
                }
            }
            position->_M_data.Data = 0;
            position->_M_data.Info = 0;
            free(position);
            return next;
        }
    };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetAnimDelayTime() const;             // slot 6 (+0x18)
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0x63 - 0x10];
    bool                    Initialised;               // +0x63
    unsigned char           _pad64[0x74 - 0x64];
    CAnimationEntry*        PAnimEntry;                // +0x74 (CCountedPointer<CAnimationEntry>)
    void*                   PAnimEntryInfo;            // +0x78
    unsigned char           _pad7c[0xbc - 0x7c];
    CStrikeEventList        StrikeEvents;              // +0xbc
    long                    LastStrikeEventEndTime;    // +0xc0
    unsigned char           _padc4[0x114 - 0xc4];
    void SetupCombatStrikeEvents();
};

template <class It, class F> inline F for_each(It first, It last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

void CCombatActionBase::SetupCombatStrikeEvents()
{
    if (!Initialised)
        return;
    float framesPerSecond = (float)g_FramesPerSecond;
    float timeScale = 1.0f;
    unsigned long animID = PAnimEntry->AnimID;
    NAnimationEvents::CManager* manager = GetAnimManager();
    CAnimEventVector events;
    manager->GetEvents(animID, events);
    for (CAnimEvent* it = events.First; it != events.Last; ++it) {
        long startFrame = (long)(it->Time * framesPerSecond);
        bool isStart, isWeapon, legacy, primaryLegacy;
        if (ParseStrikeEventName(it->Name, isStart, isWeapon, legacy, primaryLegacy) && isStart) {
            if (startFrame == 0)
                startFrame = 1;
            CThing* creature = ((CThing*)PCreature.GetPItem());
            CThing* weapon = GetThingDoingDamageFromCreatureAndEventName(creature, it->Name, isWeapon, legacy, primaryLegacy);
            if (weapon && !weapon->Dead) {
                CTCWeaponTrail* trail;
                if (weapon->GetInterface(&trail, 0x4c)) {
                    long index = trail->GetStrikeEventIndexFromEventName(it->Name);
                    CCharStringVector helperNames;
                    GetHelperPointNamesFromStrikeEventName(it->Name, helperNames, isWeapon, legacy, primaryLegacy);
                    StrikeEvents.push_back(CCountedPointer<CStrikeEvent>(new CStrikeEvent(weapon, index, helperNames.front(),
                                                                                          startFrame, isWeapon, legacy, primaryLegacy)));
                }
            }
        }
    }
    long lastEndTime = 0;
    for (CStrikeEventNode* node = StrikeEvents.begin(); node != StrikeEvents.end(); ) {
        CStrikeEvent* strikeEvent = node->_M_data.Data;
        CFindStrikeEventEnd finder = for_each(events.begin(), events.end(), CFindStrikeEventEnd(strikeEvent));
        if (finder.Found) {
            strikeEvent->EndFrame = finder.NearestFrame;
            if (strikeEvent->EndFrame <= strikeEvent->StartFrame)
                strikeEvent->EndFrame = strikeEvent->StartFrame + 1;
            strikeEvent->StartFrame = (long)(strikeEvent->StartFrame * timeScale);
            strikeEvent->EndFrame = (long)(strikeEvent->EndFrame * timeScale);
            long delay = GetAnimDelayTime();
            strikeEvent->StartFrame += delay;
            strikeEvent->EndFrame += delay + 1;
            if (strikeEvent->EndFrame > lastEndTime)
                lastEndTime = strikeEvent->EndFrame;
            node = node->_M_next;
        } else {
            node = StrikeEvents.erase(node);
        }
    }
    LastStrikeEventEndTime = lastEndTime;
}