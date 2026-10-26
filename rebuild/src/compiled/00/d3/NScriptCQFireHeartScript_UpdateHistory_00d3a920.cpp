// CQ_FireHeartScript::UpdateHistory(bool good) -- fold the hero's latest choice into the guest history: a good choice moves an unset or already-good history to 3, a bad one moves an unset or already-bad history to 1, any other mix becomes 2; each store is skipped when the script thread is terminating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D3A920, length 132 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed).
// Layout: Guest +0x184 (Ego_r.pdb +0x1a0: the script base is 0x14 smaller and each of the two vectors before it 4).
// Shape: two arms, each a terminating test, the two-value test and a terminating test before its store; VC
// tail-merges the shared "mixed" arm (`state = 2`) into the good arm.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

namespace NScript {
class CQ_FireHeartScript : public CScriptBase {
public:
    unsigned char _pad40[0x184 - 0x40];                     // +0x40 .. +0x183
    long Guest;                                             // +0x184 0 none, 1 bad, 2 mixed, 3 good
    void UpdateHistory(bool good);
};
} // namespace NScript
using NScript::CQ_FireHeartScript;

void NScript::CQ_FireHeartScript::UpdateHistory(bool good)
{
    if (good) {
        if (IsActiveThreadTerminating())
            return;
        if (Guest == 0 || Guest == 3) {
            if (IsActiveThreadTerminating())
                return;
            Guest = 3;
        } else {
            if (IsActiveThreadTerminating())
                return;
            Guest = 2;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        if (Guest == 0 || Guest == 1) {
            if (IsActiveThreadTerminating())
                return;
            Guest = 1;
        } else {
            if (IsActiveThreadTerminating())
                return;
            Guest = 2;
        }
    }
}