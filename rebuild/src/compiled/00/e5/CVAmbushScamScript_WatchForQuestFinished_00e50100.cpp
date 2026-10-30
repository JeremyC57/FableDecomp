// CV_AmbushScamScript::WatchForQuestFinished() -- wait (one script frame at a time) until any of three end conditions (+0x68, +0x69, +0x6a) is set; unless the thread is terminating, mark the quest finished (+0x48)
// Retail: Fable.exe @ 0x00E50100, 73 bytes. Referenced once as a thread entry (0x00E4FB7F).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame
// (slot 7, +0x1c) through Game at +0x40.
// Layout: QuestFinished +0x48 (the common quest-script flag); the three end-condition bytes at +0x68..+0x6a belong to
// an unidentified quest-script class (the catalog label is shared by several script bodies).
// Shape: the WAIT macro `while (!(a || b || c)) { NewScriptFrame(); if (terminating) return; }` (rotated: `a` tested
// at the entry and at the bottom, `b`/`c` at the aligned loop head), the trailing terminate test, the flag store.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04..+0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

struct CV_AmbushScamScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    unsigned char _flags49[0x68 - 0x49];
    bool m_endConditionA;                                // +0x68
    bool m_endConditionB;                                // +0x69
    bool m_endConditionC;                                // +0x6a
    void WatchForQuestFinished();
};

void CV_AmbushScamScript::WatchForQuestFinished()
{
    while (!(m_endConditionA || m_endConditionB || m_endConditionC)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    QuestFinished = true;
}