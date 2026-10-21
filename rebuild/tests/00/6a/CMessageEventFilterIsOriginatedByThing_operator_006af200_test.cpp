// behaviour test for 0x006AF200
#include <stdio.h>
#include <string.h>
class CMessageEvent {
public:
    unsigned char    _pad00[0x20];
    unsigned __int64 OriginatorUID;                      // +0x20
    unsigned __int64 TargetUID;                          // +0x28
};
class CMessageEventFilter_IsOriginatedByThing {
public:
    unsigned __int64 UID;                                // +0x00
    bool operator()(const CMessageEvent& event) const;
};
bool CMessageEventFilter_IsOriginatedByThing::operator()(const CMessageEvent& event) const
{
    return event.OriginatorUID == UID;
}

int main()
{
    CMessageEvent e; memset(&e, 0, sizeof(e)); CMessageEventFilter_IsOriginatedByThing f;
    f.UID = 0x0000000500000007ULL; e.OriginatorUID = 0x0000000500000007ULL; int ok = f(e);
    e.OriginatorUID = 0x0000000600000007ULL; ok &= !f(e);
    e.OriginatorUID = 0x0000000500000008ULL; ok &= !f(e);
    printf(ok ? "006af200_TEST PASS\n" : "006af200_TEST FAIL\n");
    return ok ? 0 : 1;
}