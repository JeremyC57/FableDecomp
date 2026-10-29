// CTCLook::FrameUpdate() -- per-frame look update: refresh the head position, run the stop-looking countdown, react to the creature's interface-8 / interface-0xAA look overrides, drive the body turning, then the head state machine (centre, interested, look at a position), and finally apply the head settings
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00753C70, 544 bytes, plain `ret`. Vtable 0x01269744 (CTCLook) slot 9 (catalog "vfunc_9").
// Callees: CTCLook::UpdateHeadPos @ 0x00752000, LowerBound @ 0x0040F020 (interfaces 8 and 0xAA, inlined GetTC), the CTCBase
//   frame-source accessor @ 0x00686D60 and its GetFrameNumber @ 0x004167C8 (the inlined UpdateLookTimer),
//   CThingCreatureBase::IsLookTurningOverridenByParent @ 0x0085F200, CTCLook::FrameUpdateOverriddenByParent @ 0x00753080,
//   ImplementHeadSettings @ 0x00753BA0, IsAbleToTurnDaBody @ 0x00751DA0, IsCurrentlyTurningViaAnimation @ 0x00753B40,
//   SetBodyMovingViaAnimation @ 0x00753930, UpdateLookTimer @ 0x00751AD0, SetBodyMoving @ 0x00752E00, CentreHead @ 0x007528A0,
//   IsLookTimeOver @ 0x00753AD0, SetHeadTurnState @ 0x007517B0, FrameUpdateHeadInterested @ 0x00753380,
//   FrameUpdateHeadLookAtPos @ 0x007530D0.
// Layout (retail, PDB minus 4 after the 12-byte TurningAnims vector): LookTimer +0x3C, HeadTurnState +0x48, BodyTurnState
//   +0x4C, CanTurnBody +0x50, PosToLookAt +0x68, TimeToStopLooking +0xB4, OneFrameDelay +0xB8. No engine header for CTCLook.
// Shape: the stop-looking reset and the interface-8 head state are written out inline (retail inlines those two state
//   setters and the first look-timer update; the later ones are calls).
class C3DVector { public: float X, Y, Z; };
struct CTCBaseComponent;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                  // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                        // +0x20
    CInterfaceMap m_interfaces;                            // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || wanted < it->first)
            it = map->last;
        return (T*)((CTCBaseComponent*)it->second);
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((CTCBaseComponent*)it->second);
            return true;
        }
        return false;
    }
    bool IsLookTurningOverridenByParent();                 // 0x0085F200 (CThingCreatureBase)
};
struct CTCInterface8 {                                     // descriptive: the owner's interface-8 component
    unsigned char _pad00[0x34];
    bool ForceLookInterested;                              // +0x34
};
struct CTCInterfaceAA {                                    // descriptive: the owner's interface-0xAA component
    unsigned char _pad00[0x78];
    bool ForceCentreHead;                                  // +0x78
};
struct CFrameSource {
    long GetFrameNumber();                                 // 0x004167C8
};

class CTCLook { public:
    void*         __vftable;                               // +0x00
    CThing*       ParentThing;                             // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long          LookTimer;                               // +0x3c
    unsigned char _pad40[0x48 - 0x40];
    long          HeadTurnState;                           // +0x48
    long          BodyTurnState;                           // +0x4c
    bool          CanTurnBody;                             // +0x50
    unsigned char _pad51[0x68 - 0x51];
    C3DVector     PosToLookAt;                             // +0x68
    unsigned char _pad74[0xb4 - 0x74];
    long          TimeToStopLooking;                       // +0xb4
    bool          OneFrameDelay;                           // +0xb8
    CFrameSource* GetFrameSource();                        // 0x00686D60 (CTCBase accessor)
    void UpdateHeadPos();                                  // 0x00752000
    void FrameUpdateOverriddenByParent();                  // 0x00753080
    void ImplementHeadSettings();                          // 0x00753BA0
    bool IsAbleToTurnDaBody();                             // 0x00751DA0
    bool IsCurrentlyTurningViaAnimation();                 // 0x00753B40
    void SetBodyMovingViaAnimation();                      // 0x00753930
    void UpdateLookTimer();                                // 0x00751AD0
    void SetBodyMoving();                                  // 0x00752E00
    void CentreHead();                                     // 0x007528A0
    bool IsLookTimeOver();                                 // 0x00753AD0
    void SetHeadTurnState(long state);                     // 0x007517B0
    void FrameUpdateHeadInterested();                      // 0x00753380
    void FrameUpdateHeadLookAtPos(const C3DVector& pos, bool turnBody);   // 0x007530D0
    void FrameUpdate();
};

#pragma auto_inline(off)
// Same-TU body of CTCLook::SetHeadTurnState (the landed 0x007517B0): with it visible VC7.1 knows the call leaves ecx
// intact, so `this` is not reloaded before the following ImplementHeadSettings call, as in retail.
__declspec(noinline) void CTCLook::SetHeadTurnState(long state)
{
    TimeToStopLooking = -1;
    if (HeadTurnState != state) {
        HeadTurnState = state;
        OneFrameDelay = true;
    }
}
#pragma auto_inline(on)

void CTCLook::FrameUpdate()
{
    UpdateHeadPos();
    if (TimeToStopLooking == 0) {
        if (BodyTurnState != 1)
            BodyTurnState = 1;
        if (HeadTurnState != 1) {
            HeadTurnState = 1;
            OneFrameDelay = true;
        }
        TimeToStopLooking = -2;
    } else if (TimeToStopLooking > 0)
        --TimeToStopLooking;
    CTCInterface8* interested;
    if (ParentThing->GetInterface(&interested, 8) && interested->ForceLookInterested) {
        if (HeadTurnState != 4) {
            HeadTurnState = 4;
            OneFrameDelay = true;
        }
        TimeToStopLooking = -1;
        LookTimer = GetFrameSource()->GetFrameNumber() + 90;
    }
    CThing* parent = ParentThing;
    CTCInterfaceAA* centre;
    if (parent->GetInterface(&centre, 0xaa) && centre->ForceCentreHead) {
        CentreHead();
        ImplementHeadSettings();
        return;
    }
    if (parent->IsLookTurningOverridenByParent()) {
        FrameUpdateOverriddenByParent();
        ImplementHeadSettings();
        return;
    }
    if (BodyTurnState & 2) {
        if (IsAbleToTurnDaBody()) {
            if (!(BodyTurnState & 8)) {
                if (IsCurrentlyTurningViaAnimation()) {
                    SetBodyMovingViaAnimation();
                    UpdateLookTimer();
                } else {
                    SetBodyMoving();
                    UpdateLookTimer();
                }
            } else if (BodyTurnState != 1)
                BodyTurnState = 1;
        }
    }
    if (OneFrameDelay) {
        OneFrameDelay = false;
        ImplementHeadSettings();
        return;
    }
    if (HeadTurnState & 1) {
        if (!(HeadTurnState & 8)) {
            CentreHead();
            ImplementHeadSettings();
            return;
        }
    } else if (HeadTurnState & 4) {
        if (IsLookTimeOver()) {
            SetHeadTurnState(1);
            ImplementHeadSettings();
            return;
        }
        FrameUpdateHeadInterested();
        ImplementHeadSettings();
        return;
    } else if (HeadTurnState & 2) {
        if (IsLookTimeOver()) {
            SetHeadTurnState(1);
            ImplementHeadSettings();
            return;
        }
        if (!(HeadTurnState & 8)) {
            FrameUpdateHeadLookAtPos(PosToLookAt, CanTurnBody);
            UpdateLookTimer();
        }
    }
    ImplementHeadSettings();
}