// CTCTeleporter::vfunc_1(CPersistContext* context, bool persist) -- persist hook: when the flag is set, transfers "TeleportActive" (+0x10) and "TeleportActivated" (+0x11) with false defaults
// Retail: Fable.exe @ 0x007DFF40, 73 bytes (vtable 0x0126FDA4 slot 1, `ret 8`).
// Callees: CPersistContext::Transfer<bool> 0x004045C0 (name, value, const& default; 1-byte instance).
// Shape: the second argument is a byte tested first (`mov al,[esp+8] ; test al,al` before the
//        pushes); each `const bool&` false default is a temporary homed in that (now dead) argument
//        slot (`mov byte ptr [esp+1Ch],0`); the first member address is `lea ecx,[esi+10h]`, the
//        second advances `this`. Members: TeleportActive / TeleportActivated (the persisted keys;
//        the constructor at 0x007DFEE0 clears both).
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCTeleporter {
    unsigned char _pad[0x10];
    bool m_teleportActive;              // +0x10
    bool m_teleportActivated;           // +0x11
    void vfunc_1(CPersistContext* context, bool persist);
};

void CTCTeleporter::vfunc_1(CPersistContext* context, bool persist)
{
    if (persist) {
        context->Transfer("TeleportActive", m_teleportActive, false);
        context->Transfer("TeleportActivated", m_teleportActivated, false);
    }
}