#include <cstdio>
// Behaviour test for CMessageEventFilter_IsOfType::operator() (0x006AF1C0), linked against the row's object
// (??RCMessageEventFilter_IsOfType@@QBE_NABVCMessageEvent@@@Z): event Type at +0x08, filter Type at +0x00.
enum EMessageEventType { MET_A = 7, MET_B = 8 };
class CMessageEvent { public: char pad[8]; EMessageEventType Type; };
class CMessageEventFilter_IsOfType { public:
    bool operator()(const CMessageEvent& event) const;
    EMessageEventType Type;
};
int main(){ CMessageEvent e; e.Type = MET_A; CMessageEventFilter_IsOfType o; o.Type = MET_A;
 if(!o(e)){ std::printf("006af1c0_TEST FAIL\n"); return 1;}
 o.Type = MET_B; if(o(e)){ std::printf("006af1c0_TEST FAIL\n"); return 1;}
 std::printf("006af1c0_TEST PASS\n"); return 0;}
