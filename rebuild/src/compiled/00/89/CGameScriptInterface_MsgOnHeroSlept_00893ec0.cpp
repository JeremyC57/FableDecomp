// CGameScriptInterface::MsgOnHeroSlept(bool&) const @ 0x00893EC0 (retail): read the two message frame bounds
// (0x006E7510, then GetMaxWorldFrameForMessages @ 0x006E7530), find the most recent message of type 0x42
// in that window (0x008ABED0) and, when there is one, report the time manager's IsDay (0x006BB2C0).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Message893ec0 {};
struct TypeFilter893ec0 { unsigned long messageType; };
struct VisibleFilter893ec0 { long maxWorldFrame0; long maxWorldFrame1; };
struct BoundFilter893ec0 { TypeFilter893ec0* filter0; VisibleFilter893ec0* filter1; };
struct MessageManager893ec0 { Message893ec0* FindMostRecentMessage(BoundFilter893ec0* filter); };
struct TimeManager893ec0 { bool IsDay(); };
struct CWorld { unsigned char unused00[0x1c]; TimeManager893ec0* time1c; unsigned char unused20[0x40]; MessageManager893ec0* messages60; };
struct CGameScriptInterface_Methods : CGameScriptInterface {
    long GetFrameForMessages_006e7510() const;   // 0x006E7510: same accessor path as GetMaxWorldFrameForMessages but
                                                  //   reads the +0x18 frame word (catalog label duplicated; not attributed)
    long GetMaxWorldFrameForMessages() const;    // 0x006E7530 (reads the +0x1c frame word)
    bool MsgOnHeroSlept(bool& outIsDay) const;
};
bool CGameScriptInterface_Methods::MsgOnHeroSlept(bool& outIsDay) const{long frame0=GetFrameForMessages_006e7510();long frame1=GetMaxWorldFrameForMessages();MessageManager893ec0* manager=World->messages60;VisibleFilter893ec0 visible;visible.maxWorldFrame0=frame0;visible.maxWorldFrame1=frame1;TypeFilter893ec0 type;type.messageType=0x42;BoundFilter893ec0 filter;filter.filter0=&(type);filter.filter1=&(visible);Message893ec0* message=manager->FindMostRecentMessage(&(filter));if(message!=0){outIsDay=World->time1c->IsDay();return true;}return false;}