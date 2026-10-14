// CDisplayEngine::ConfirmFeedbackGameEventPackage(CGameEventPackage&) -- dispatch every event of the package to the handler registered for its type
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434A60, length 74 bytes.
// Callees: CGameEventPackage::GetNoEvents() @ 0x009F16E0 (called for every iteration), CGameEventPackage::GetEvent(unsigned long)
// @ 0x009F16D0; the handler is a multiple-inheritance pointer to member {function, this-adjust} at +0x30 of the 64-byte
// entries of the handler table @ 0x013B9288, indexed by the event's type (`shl ecx,6`), called through `add ecx,ebx ; call edx`
// when the function pointer is set. The manifest labels of the callees (CWorld::DrawGetEnvironment / GetBlankTexture) are
// propagated across identical bodies. Shape: an unsigned counted loop (`jbe` / `jb`) with the count re-read each iteration.
struct CGameEvent {
    long m_type;                                   // +0 the handler-table index
};

struct CGameEventPackage {
    unsigned long GetNoEvents();                   // 0x009F16E0
    CGameEvent* GetEvent(unsigned long index);     // 0x009F16D0
};

struct __multiple_inheritance CDisplayEngine;    // 8-byte {function, this-adjust} pointers to member

struct SGameEventHandler {                         // 64-byte handler-table entry
    unsigned char _pad[0x30];                      // +0x00
    void (CDisplayEngine::*m_pHandler)(CGameEvent* event);   // +0x30 {function, this-adjust}
    unsigned char _pad2[8];                        // +0x38
};

extern SGameEventHandler g_GameEventHandlers[];    // 0x013B9288

struct CDisplayEngineBaseA { unsigned char _body[4]; };
struct CDisplayEngineBaseB { unsigned char _body[4]; };

struct CDisplayEngine : CDisplayEngineBaseA, CDisplayEngineBaseB {   // two bases: the handler is an 8-byte {function, this-adjust} pointer to member
    void ConfirmFeedbackGameEventPackage(CGameEventPackage& package);
};

void CDisplayEngine::ConfirmFeedbackGameEventPackage(CGameEventPackage& package)
{
    for (unsigned long i = 0; i < package.GetNoEvents(); ++i) {
        CGameEvent* event = package.GetEvent(i);
        SGameEventHandler& handler = g_GameEventHandlers[event->m_type];
        if (handler.m_pHandler)
            (this->*handler.m_pHandler)(event);
    }
}