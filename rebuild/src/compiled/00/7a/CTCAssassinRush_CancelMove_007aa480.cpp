// CTCAssassinRush::CancelMove() -- while the main player holds its cancel lock (+0x215) only schedule the cancel; otherwise clean up the effects, remove this TC (type 0xA9) from the parent thing and clear ScheduleForCancel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AA480, length 68 bytes (+ int3 padding).
// Callees: CTCBase::GetPlayerManager @ 0x00686D40 (it ignores `this`: the game singleton
// @ 0x013B8A1C, +0x30; manifest label "CViewBase::GetDisplayEngine" is propagated),
// CPlayerManager::GetMainPlayer @ 0x00449970 (landed name; manifest "Catch@0040d959" is wrong),
// CTCAssassinRush::CleanUpEffects @ 0x007AA370, CThing::RemoveTC(long) @ 0x004C9840 (landed name).
// Layout: CTCBase ParentThing +4; Ego_r.pdb CTCAssassinRush ScheduleForCancel PDB +0xa2 = retail +0x9e
// (ItemAlphaList is a 12-byte vector in retail). The player's byte at +0x215 is unnamed (descriptive).
// Shape: the parent pointer is read into a local BEFORE the accessor call (edi), the two flag stores
// are tail-duplicated into both exits; the unlocked path is the `if` arm (the early-return form
// places the locked exit first).
enum ETCInterfaceType { };
class CThing { public:
    void RemoveTC(ETCInterfaceType interfaceType);                      // 0x004C9840
};

class CPlayer { public:
    unsigned char _pad[0x215];
    bool          m_bCancelLocked;                          // +0x215
};

class CPlayerManager { public:
    CPlayer& GetMainPlayer();                               // 0x00449970
};

class CTCBase { protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40 (Ego_r.pdb CTCBase, protected)
};

#pragma pack(push, 1)
class CTCAssassinRush : public CTCBase { public:
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x9e - 0x08];
    bool          ScheduleForCancel;                        // +0x9e
    void CleanUpEffects();                                  // 0x007AA370
    void CancelMove();
};
#pragma pack(pop)

void CTCAssassinRush::CancelMove()
{
    CThing* parent = ParentThing;
    if (!GetPlayerManager().GetMainPlayer().m_bCancelLocked) {
        CleanUpEffects();
        parent->RemoveTC((ETCInterfaceType)(0xa9));
        ScheduleForCancel = false;
    } else {
        ScheduleForCancel = true;
    }
}