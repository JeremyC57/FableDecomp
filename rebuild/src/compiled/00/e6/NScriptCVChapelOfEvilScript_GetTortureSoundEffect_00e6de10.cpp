// CV_ChapelOfEvilScript::GetTortureSoundEffect(ESex sex) -- pick the torture sound for the current torture type and the victim's sex
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6DE10, 0x23b bytes, `ret 4`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (the terminate check before every statement),
//   CCharString::operator=(const char*) 0x0099EFE0.
// Strings: CHAPEL_OF_EVIL_<TORTURE>_{FEMALE,MALE} 0x012E8AC4 .. 0x012E8954.
// Signature: FableWin.pdb ?GetTortureSoundEffect@CV_ChapelOfEvilScript@NScript@@QAEXW4ESex@@@Z.
// Shape: an if/else-if chain over TortureType 1..6 (no jump table); each arm is the script translator's
//   `if (IsActiveThreadTerminating()) return;` before the sex test and again before each assignment. VC7.1 keeps
//   the constant 2 in edi after the first arm and merges all terminate exits into the shared epilogues.
// Layout: Ego_r.pdb CV_ChapelOfEvilScript - 0x14 (retail CScriptBase is 0x40): TortureSound +0x88, TortureType +0x8c.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };

class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const char* text);            // 0x0099EFE0
};

class CGameScriptInterface;

class CScriptBase { public:                                     // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

namespace NScript {
class CV_ChapelOfEvilScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x88 - 0x48];
    CCharString TortureSound;                            // +0x88
    long TortureType;                                    // +0x8c (ChapelOfEvilTortureType)
    void GetTortureSoundEffect(ESex sex);
};
} // namespace NScript
using NScript::CV_ChapelOfEvilScript;

void NScript::CV_ChapelOfEvilScript::GetTortureSoundEffect(ESex sex)
{
    if (TortureType == 1) {
        if (IsActiveThreadTerminating())
            return;
        if (sex == SEX_FEMALE) {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_TUG_OF_WAR_FEMALE";
        } else {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_TUG_OF_WAR_MALE";
        }
    } else if (TortureType == 2) {
        if (IsActiveThreadTerminating())
            return;
        if (sex == SEX_FEMALE) {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_PESTILENCE_FEMALE";
        } else {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_PESTILENCE_MALE";
        }
    } else if (TortureType == 3) {
        if (IsActiveThreadTerminating())
            return;
        if (sex == SEX_FEMALE) {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_INFERNO_FEMALE";
        } else {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_INFERNO_MALE";
        }
    } else if (TortureType == 4) {
        if (IsActiveThreadTerminating())
            return;
        if (sex == SEX_FEMALE) {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_BONING_MACHINE_FEMALE";
        } else {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_BONING_MACHINE_MALE";
        }
    } else if (TortureType == 5) {
        if (IsActiveThreadTerminating())
            return;
        if (sex == SEX_FEMALE) {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_ACID_BATH_FEMALE";
        } else {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_ACID_BATH_MALE";
        }
    } else if (TortureType == 6) {
        if (IsActiveThreadTerminating())
            return;
        if (sex == SEX_FEMALE) {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_EVISCERATER_FEMALE";
        } else {
            if (IsActiveThreadTerminating())
                return;
            TortureSound = "CHAPEL_OF_EVIL_EVISCERATER_MALE";
        }
    }
}