// CTCScriptedControl::IsActive() -- the scripted control is active while it is locked (+0x18) or still has queued script tasks (the task list at +0x14 is not empty)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C70F0, length 25 bytes.
// Callee: the task-list empty test @ 0x007122C0 (`head->next == head` on the list head pointer at +0x14, returned as a
// bool through `xor ecx,ecx ; sete cl ; mov al,cl`; manifest "CScriptTask_IsRunning"). Callers: 0x00659412, 0x00659478,
// 0x00667EB6, 0x0076CF0D. The +0x18 lock byte is the one SetLocked (0x00712C20) / ClearLocked (0x00712C80) write.
// Shape: a 4-byte 0/1 result (`xor eax,eax` / `mov eax,1`) written `if (!locked && empty) return 0; return 1;` (two
// separate ifs returning 1 then 0 fold the second test into `neg al ; sbb eax,eax ; inc eax`).
class CTCScriptedControl { public:
    unsigned char _pad[0x18];
    unsigned char m_locked;                  // +0x18
    bool AreTasksEmpty();                    // 0x007122C0
    long IsActive();
};

long CTCScriptedControl::IsActive()
{
    if (!m_locked && AreTasksEmpty())
        return 0;
    return 1;
}