// Behaviour test for 0x00431C30 NUISystem::CUIIconsDef::Transfer: every icon index is transferred once, in retail order.
#include <cstdio>

struct CPersistContext {
    unsigned long* seen[64];
    int count;
    unsigned long next;
    CPersistContext() : count(0), next(100) {}
    void Transfer(unsigned long& value) { seen[count++] = &value; value = next++; }   // binary load: fill in sequence
};

struct CDefClassBase {
    virtual void Transfer(CPersistContext& context) {}
    virtual ~CDefClassBase() {}
};

struct CUIIconsDef : CDefClassBase {
    unsigned long IconFriendRequestReceived;
    unsigned long IconFriendRequestReceivedOn;
    unsigned long IconFriendRequestSent;
    unsigned long IconFriendRequestSentOn;
    unsigned long IconGameInviteReceived;
    unsigned long IconGameInviteReceivedOn;
    unsigned long IconGameInviteSent;
    unsigned long IconGameInviteSentOn;
    unsigned long IconMute;
    unsigned long IconMuteOn;
    unsigned long IconOnline;
    unsigned long IconOnlineOn;
    unsigned long IconPasscodeBlank;
    unsigned long IconPasscodeFilled;
    unsigned long IconTV;
    unsigned long IconTVOn;
    unsigned long IconVoice;
    unsigned long IconVoiceOn;
    unsigned long IconWait1;
    unsigned long IconWait2;
    unsigned long IconWait3;
    unsigned long IconWait4;
    unsigned long IconProgress;
    unsigned long IconProgressOn;
    unsigned long IconA;
    unsigned long IconB;
    unsigned long IconX;
    unsigned long IconY;
    unsigned long IconBlank;
    unsigned long IconUpArrow;
    unsigned long IconDownArrow;
    unsigned long IconListHighlight;
    virtual void Transfer(CPersistContext& context);
};

void CUIIconsDef::Transfer(CPersistContext& context)
{
    context.Transfer(IconFriendRequestReceived);
    context.Transfer(IconFriendRequestReceivedOn);
    context.Transfer(IconFriendRequestSent);
    context.Transfer(IconFriendRequestSentOn);
    context.Transfer(IconGameInviteReceived);
    context.Transfer(IconGameInviteReceivedOn);
    context.Transfer(IconGameInviteSent);
    context.Transfer(IconGameInviteSentOn);
    context.Transfer(IconMute);
    context.Transfer(IconMuteOn);
    context.Transfer(IconOnline);
    context.Transfer(IconOnlineOn);
    context.Transfer(IconPasscodeBlank);
    context.Transfer(IconPasscodeFilled);
    context.Transfer(IconTV);
    context.Transfer(IconTVOn);
    context.Transfer(IconVoice);
    context.Transfer(IconVoiceOn);
    context.Transfer(IconWait1);
    context.Transfer(IconWait2);
    context.Transfer(IconWait3);
    context.Transfer(IconWait4);
    context.Transfer(IconProgress);
    context.Transfer(IconProgressOn);
    context.Transfer(IconA);
    context.Transfer(IconB);
    context.Transfer(IconX);
    context.Transfer(IconY);
    context.Transfer(IconBlank);
    context.Transfer(IconUpArrow);
    context.Transfer(IconDownArrow);
    context.Transfer(IconListHighlight);
}

int main()
{
    CUIIconsDef def;
    CDefClassBase* base = &def;
    CPersistContext context;
    base->Transfer(context);
    unsigned long* first = &def.IconFriendRequestReceived;
    if (context.count != 32) { std::printf("00431c30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 32; ++i) {
        if (context.seen[i] != first + i || first[i] != (unsigned long)(100 + i)) { std::printf("00431c30_TEST FAIL\n"); return 1; }
    }
    std::printf("00431c30_TEST PASS\n");
    return 0;
}