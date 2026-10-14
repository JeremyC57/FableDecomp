// CV_StatueMasterScript::GetTimeOfDayFraction() -- converts the game interface's HHMM clock (virtual slot 647 on the interface at +0x40) to a fraction of a day: (minutes / 60 + hours) / 24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED43D0, length 80 bytes.
// Callees: virtual slot 647 (+0xa1c) of the interface pointer at +0x40 (returns a long HHMM time). Float pool: 1/60 @ 0x0125BB08,
// 1/24 @ 0x0122DC88.
// Class attribution: manifest label "CGameCameraManager::HasCameraMode" is propagated; the body sits in the quest-script module
// (next function 0x00ED4420 is CV_StatueMasterScript::GetStatuePointingPosition and +0x40 is the script's interface pointer),
// so the class name is provisional.
// Shape: signed /100 by the 0x51EB851F reciprocal (`imul ; sar 5 ; add sign`), the quotient and remainder spilled to two frame
// words for `fild` / `fiadd` (written `time - hours * 100`, since `%` would make VC emit one idiv for both); the remainder is scaled first (`fmul 1/60`), the quotient added, then the whole scaled by 1/24.
struct IGameInterfaceVtbl {
    void* m_slots[647];
    long (__fastcall* GetTimeOfDay)(void* self);    // slot 647 (+0xa1c)
};

struct IGameInterface {
    IGameInterfaceVtbl* m_pVtable;
};

struct CV_StatueMasterScript {
    unsigned char _pad[0x40];
    IGameInterface* m_pGameInterface;               // +0x40
    float GetTimeOfDayFraction();
};

float CV_StatueMasterScript::GetTimeOfDayFraction()
{
    IGameInterface* game = m_pGameInterface;
    long time = game->m_pVtable->GetTimeOfDay(game);
    long hours = time / 100;
    long minutes = time - hours * 100;           // retail derives the remainder from the quotient (no idiv)
    return ((1.0f / 60.0f) * (float)minutes + (float)hours) * (1.0f / 24.0f);
}
