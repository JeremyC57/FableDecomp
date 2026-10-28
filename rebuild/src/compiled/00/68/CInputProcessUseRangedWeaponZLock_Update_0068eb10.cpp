// CInputProcessUseRangedWeaponZLock::Update(long) -- calls CPlayer::GetPControlledCreature @ 0x00487DC0 on MyPlayer (+0x18) and drops the result; the 1 argument(s) are not forwarded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068EB10, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 0x18] ; call 0x487dc0 ; ret 4
// Tiny-body shape `ForwardThroughMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CThingPlayerCreature;
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};

class CInputProcessUseRangedWeaponZLock { public:
    unsigned char _pad[0x18];
    CPlayer* MyPlayer;                     // +0x18 (CInputProcessGameBase::MyPlayer)
    void Update(long);
};

void CInputProcessUseRangedWeaponZLock::Update(long)
{
    MyPlayer->GetPControlledCreature();
}