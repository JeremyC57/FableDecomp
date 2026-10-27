// CGameScriptInterface::MsgIsRegionLoaded(const CCharString& name) const -- is there a message of type 0x26 in this frame window whose extra data is the number of the named region
// Leaf: ?MsgIsRegionLoaded@CGameScriptInterface@@UBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008932d0, 135 bytes, thiscall, `ret 4`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 20; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order; neighbours 18/19 MsgOnLevelLoaded/Unloaded and 22/23 MsgOnRegionLoaded/Unloaded landed) names it ?MsgIsRegionLoaded@CGameScriptInterface@@UBE_NABVCCharString@@@Z; message type 0x26 (the landed MsgOnRegionLoaded 0x008933F0 uses 0x26).
// Callees: CWorld::PeekWorldMap() const (virtual slot 13, +0x34), CWorldMap::GetRegionNumberFromName(const CCharString&) const 0x004FC210,
//   the frame getters 0x006E7510 / 0x006E7530 (GetMaxWorldFrameForMessages, Ego_r.pdb protected const),
//   CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<IsOfType, OccuredBetween, HasExtraData> >(const&) const 0x008ABE70.
// Body: the filter chain `IsOfType(0x26) && OccuredBetween(frame, maxFrame) && HasExtraData(number)` -- each operator&& returns a
//   bind object holding POINTERS to its operands; the IsOfType temporary is homed in the dead name-argument slot; the result is
//   `message != 0` (neg / sbb / neg). The operand order follows the frame (Bind3 first = IsOfType, second = the frame pair,
//   third = the number) -- the catalog label's IsOfType/HasExtraData/OccuredBetween order is the other PDB instantiation.
// Header debt: engine/CWorld.h / CGameScriptInterface.h / CWorldMap.h are flat layouts without vtables or methods; modelled locally.
class CCharString { public: void* m_pData; };
class CWorldMap {
public:
    long GetRegionNumberFromName(const CCharString& name) const;   // 0x004FC210
};
enum EMessageEventType { EMessageEventType_FIRST };
class CMessageEvent;
class CMessageEventFilter_IsOfType {
public:
    EMessageEventType Type;
    CMessageEventFilter_IsOfType(EMessageEventType type) : Type(type) { }
};
class CMessageEventFilter_OccuredBetween {
public:
    long StartFrame, EndFrame;
    CMessageEventFilter_OccuredBetween(long start, long end) : StartFrame(start), EndFrame(end) { }
};
class CMessageEventFilter_HasExtraData {
public:
    long ExtraData;
    CMessageEventFilter_HasExtraData(long data) : ExtraData(data) { }
};
template <class A, class B> class CFilter_Bind2 {
public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B, class C> class CFilter_Bind3 {
public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template <class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
class CMessageEventManager {
public:
    template <class F> const CMessageEvent* FindMostRecentMessage(const F& filter) const;   // 0x008ABE70 for this filter
};
class CWorld {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual CWorldMap& GetWorldMap();                      // slot 12 (+0x30)
    virtual const CWorldMap& PeekWorldMap() const;         // slot 13 (+0x34)
    unsigned char         _pad04[0x5C];
    CMessageEventManager* PMessageEventManager_Data;       // +0x60 (CCountedPointer<CMessageEventManager>::Data)
};
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    CWorld* World;                                         // +0x04
    long GetCurrentWorldFrameForMessages() const;          // 0x006E7510 (descriptive spelling, as bound)
protected:
    long GetMaxWorldFrameForMessages() const;              // 0x006E7530
public:
    virtual bool MsgIsRegionLoaded(const CCharString& name) const;
};
bool CGameScriptInterface::MsgIsRegionLoaded(const CCharString& name) const
{
    long number = World->PeekWorldMap().GetRegionNumberFromName(name);
    if (number == 0)
        return false;
    long frame = GetCurrentWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    const CMessageEventManager* messages = World->PMessageEventManager_Data;
    return messages->FindMostRecentMessage(
        CMessageEventFilter_IsOfType((EMessageEventType)0x26) && CMessageEventFilter_OccuredBetween(frame, maxFrame)
        && CMessageEventFilter_HasExtraData(number)) != 0;
}