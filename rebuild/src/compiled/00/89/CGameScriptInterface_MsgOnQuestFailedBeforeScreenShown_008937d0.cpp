// CGameScriptInterface::MsgOnQuestFailedBeforeScreenShown(const CCharString& name) const -- did a message of type 0x36 occur since the previous frame for the named quest
// Leaf: ?MsgOnQuestFailedBeforeScreenShown@CGameScriptInterface@@UBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008937d0, 154 bytes, thiscall, `ret 4`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 29; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order; neighbours 26 MsgOnAnyQuestCompleted 0x00893610 (type 0x33) and 30 MsgOnQuestAccepted landed) names it ?MsgOnQuestFailedBeforeScreenShown@CGameScriptInterface@@UBE_NABVCCharString@@@Z; message type 0x36.
// Callees: the frame getters 0x006E7510 / 0x006E7530, CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType,
//   OccuredBetween> >(const&) const 0x008ABED0, CQuestManager::GetRegisteredQuestFromIndex(long) const 0x004AF3C0 on the quest
//   manager global 0x013B89FC (descriptive spelling g_pQuestManager), CBasicString<char>::Compare(const char*, const char*)
//   0x00411570 (the inlined CCharString / CBasicString equality).
// Body: as the landed MsgOnAnyQuestCompleted 0x00893610 (frame - 1 .. max frame, the manager in a named local), then
//   `quest == name`: the same string block -> true; either block null or the lengths differ -> false; else Compare == 0
//   (`neg ; sbb al,al ; inc al`).
// Header debt: engine/CWorld.h / CGameScriptInterface.h are flat layouts without vtables or methods; modelled locally.
template <class T> class CBasicString {                  // Ego_r.pdb, 13 bytes
public:
    T*            PData;                                   // +0x00
    unsigned long StringLength;                            // +0x04
    bool operator==(const CBasicString<T>& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) ? false : true;
    }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for char
};
class CCharStringData {
public:
    CBasicString<char> Data;                               // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;                          // +0x00
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return PStringData->Data == other.PStringData->Data;
    }
};
class CQuestManager {
public:
    const CCharString& GetRegisteredQuestFromIndex(long index) const;   // 0x004AF3C0
};
extern CQuestManager* g_pQuestManager;                     // 0x013B89FC
enum EMessageEventType { EMessageEventType_FIRST };
class CMessageEvent {
public:
    unsigned char _pad00[0x3C];
    long          ExtraData;                               // +0x3C
};
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
template <class A, class B> class CFilter_Bind2 {
public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CMessageEventManager {
public:
    template <class F> const CMessageEvent* FindMostRecentMessage(const F& filter) const;   // 0x008ABED0 for this filter
};
class CWorld {
public:
    virtual ~CWorld();
    unsigned char         _pad04[0x5C];
    CMessageEventManager* PMessageEventManager_Data;       // +0x60
};
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    CWorld* World;                                         // +0x04
protected:
    long GetMinWorldFrameForMessages() const;          // 0x006E7510
public:
protected:
    long GetMaxWorldFrameForMessages() const;              // 0x006E7530
public:
    virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString& name) const;
};
bool CGameScriptInterface::MsgOnQuestFailedBeforeScreenShown(const CCharString& name) const
{
    long frame = GetMinWorldFrameForMessages() - 1;
    long maxFrame = GetMaxWorldFrameForMessages();
    const CMessageEventManager* messages = World->PMessageEventManager_Data;
    const CMessageEvent* message = messages->FindMostRecentMessage(
        CMessageEventFilter_IsOfType((EMessageEventType)0x36) && CMessageEventFilter_OccuredBetween(frame, maxFrame));
    if (message && g_pQuestManager->GetRegisteredQuestFromIndex(message->ExtraData) == name)
        return true;
    return false;
}