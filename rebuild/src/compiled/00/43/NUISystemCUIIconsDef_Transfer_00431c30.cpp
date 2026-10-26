// NUISystem::CUIIconsDef::Transfer(CPersistContext& context) -- persist the thirty-two icon graphic indices (+0x3c..+0xb8) of the UI icons def
// Leaf: ?Transfer@CUIIconsDef@NUISystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00431C30, 410 bytes (416 with padding), virtual, `ret 4`.
// Identity: vtable slot 18 of NUISystem::CUIIconsDef (vtable 0x01230F54, RTTI `.?AVCUIIconsDef@NUISystem@@`), the
// def-class Transfer slot; the catalog label "InitialiseDefaultCameraTags" is a propagated name.
// Callee: CPersistContext::Transfer<unsigned long> 0x00431020 (CheckCRC + binary load/save by mode), 32 times.
// Layout: Ego_r.pdb CUIIconsDef (0xC0) re-based on the retail bytes: the def base is 0x3c bytes in retail (PDB 0x40),
// so IconFriendRequestReceived sits at +0x3c and IconListHighlight at +0xb8. No generated engine header.
// Shape: speed TU; the context (the argument) is held in edi and reloaded into ecx before every call, the member
// addresses rotate through eax/ecx/edx, and the last member uses `this` itself (`add esi,0B8h`) since it is dead after.
class CPersistContext { public:
    template <class T> void Transfer(T& value);                  // 0x00431020
};

class CDefClassBase { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x3c - 4];
};

namespace NUISystem {

class CUIIconsDef : public CDefClassBase { public:
    unsigned long IconFriendRequestReceived;     // +0x3c
    unsigned long IconFriendRequestReceivedOn;   // +0x40
    unsigned long IconFriendRequestSent;         // +0x44
    unsigned long IconFriendRequestSentOn;       // +0x48
    unsigned long IconGameInviteReceived;        // +0x4c
    unsigned long IconGameInviteReceivedOn;      // +0x50
    unsigned long IconGameInviteSent;            // +0x54
    unsigned long IconGameInviteSentOn;          // +0x58
    unsigned long IconMute;                      // +0x5c
    unsigned long IconMuteOn;                    // +0x60
    unsigned long IconOnline;                    // +0x64
    unsigned long IconOnlineOn;                  // +0x68
    unsigned long IconPasscodeBlank;             // +0x6c
    unsigned long IconPasscodeFilled;            // +0x70
    unsigned long IconTV;                        // +0x74
    unsigned long IconTVOn;                      // +0x78
    unsigned long IconVoice;                     // +0x7c
    unsigned long IconVoiceOn;                   // +0x80
    unsigned long IconWait1;                     // +0x84
    unsigned long IconWait2;                     // +0x88
    unsigned long IconWait3;                     // +0x8c
    unsigned long IconWait4;                     // +0x90
    unsigned long IconProgress;                  // +0x94
    unsigned long IconProgressOn;                // +0x98
    unsigned long IconA;                         // +0x9c
    unsigned long IconB;                         // +0xa0
    unsigned long IconX;                         // +0xa4
    unsigned long IconY;                         // +0xa8
    unsigned long IconBlank;                     // +0xac
    unsigned long IconUpArrow;                   // +0xb0
    unsigned long IconDownArrow;                 // +0xb4
    unsigned long IconListHighlight;             // +0xb8
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

}