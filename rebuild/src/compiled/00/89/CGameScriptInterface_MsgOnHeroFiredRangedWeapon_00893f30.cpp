// CGameScriptInterface::MsgOnHeroFiredRangedWeapon() const -- true when a message of type 0x43 was sent between the script's current and maximum message frames
// Leaf: ?MsgOnHeroFiredRangedWeapon@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00893F30, 80 bytes, thiscall, plain `ret`.
// Identity (proven): CGameScriptInterface vtable 0x01260F0C slot 51; Ego_r.pdb ?MsgOnHeroFiredRangedWeapon@CGameScriptInterface@@UBE_NXZ (donor
//   0x008D0EB0) and the manifest label agree.
// Callees: CGameScriptInterface::GetMinWorldFrameForMessages() const 0x006E7510, GetMaxWorldFrameForMessages() const
//   0x006E7530, CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,
//   CMessageEventFilter_OccuredBetween> >(const ...&) const 0x008ABED0 (Ego_r.pdb template name).
// Shape (family K031, representative 0x00893AB0, grade 12): the two frame bounds are read first (current, then maximum;
//   the first held in edi), the manager is loaded before the filters are built in the frame (type, between, binding
//   pair), and the found message is tested with `if (...) return true; return false;` (`test ; setne`; a `!= 0`
//   return compiles to `neg ; sbb ; neg`).
class CMessageEvent;
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };   // enumerator names not recovered
class CMessageEventFilter_IsOfType {                     // Ego_r.pdb, 4 bytes
public:
    CMessageEventFilter_IsOfType(EMessageEventType type) : Type(type) { }   // ??0CMessageEventFilter_IsOfType@@QAE@W4EMessageEventType@@@Z
    EMessageEventType Type;                              // +0x00
};
class CMessageEventFilter_OccuredBetween {               // Ego_r.pdb ??0CMessageEventFilter_OccuredBetween@@QAE@JJ@Z, 8 bytes
public:
    CMessageEventFilter_OccuredBetween(long minFrame, long maxFrame) : MinFrame(minFrame), MaxFrame(maxFrame) { }
    long MinFrame;                                       // +0x00
    long MaxFrame;                                       // +0x04
};
template <class TF1, class TF2> class CFilter_Bind2 {    // Ego_r.pdb: F1, F2 (pointers to the bound filters), 8 bytes
public:
    CFilter_Bind2(const TF1& f1, const TF2& f2) : F1(const_cast<TF1*>(&f1)), F2(const_cast<TF2*>(&f2)) { }
    TF1* F1;                                             // +0x00
    TF2* F2;                                             // +0x04
};
class CMessageEventManager {
public:
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;
        // <CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween> > 0x008ABED0
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T* Data;                                             // +0x00
    CCPPointerInfo* Info;                                // +0x04
    T* operator->() const { return Data; }
};
class CWorld {
public:
    unsigned char _pad_0x00[0x60];                       // vptr .. PScriptInfoManager (not touched here)
    CCountedPointer<CMessageEventManager> PMessageEventManager;   // +0x60 (PDB +96)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::MsgOnHeroFiredRangedWeapon() const
{
    long first = GetMinWorldFrameForMessages();
    long last = GetMaxWorldFrameForMessages();
    CMessageEventManager* manager = World->PMessageEventManager.Data;
    if (manager->FindMostRecentMessage(
            CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>(
                CMessageEventFilter_IsOfType((EMessageEventType)0x43), CMessageEventFilter_OccuredBetween(first, last))))
        return true;
    return false;
}