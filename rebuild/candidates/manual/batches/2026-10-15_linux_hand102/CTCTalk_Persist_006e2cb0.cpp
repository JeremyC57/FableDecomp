// CTCTalk::Persist(CPersistContext* context, bool flag) -- when the flag is set, persists IsPureAISpeaker (default true) and DialogueVoice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2CB0, length 63 bytes (ret 8). Vtable 0x0125FFEC (CTCTalk) slot 1,
// the TC persist hook (catalogued as "CTCTalk::vfunc_1"; named after the CTCChest/CTCHeroRememberCrimes slot-1 hooks).
// Callees: CPersistContext::Transfer<bool>(name, value, default) @ 0x004045C0 (folded with Transfer<signed char>),
// CPersistContext::Transfer<CCharString>(name, CCharString*) @ 0x00411010.
// Ego_r.pdb CTCTalk: IsPureAISpeaker +0x34, DialogueVoice (CCharString) +0x38.
// Shape: no base hook call; the whole body sits under `if (flag)`. The `true` default is a one-byte temporary bound to
// the const reference and homed in the (already tested, now dead) flag slot (`mov byte ptr [esp+1Ch],1`); the voice's
// address is formed by advancing `this` (`add esi,38h`) because it is the last use.
struct CCharString { void* m_pData; };

struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    void Transfer(const char* name, CCharString* value);                     // 0x00411010
};

struct CTCTalk {
    virtual void v00();
    virtual void Persist(CPersistContext* context, bool flag);               // slot 1
    unsigned char _body[0x30];               // CTCBase rest + SpeechSample .. LastNarrator
    bool          IsPureAISpeaker;           // +0x34
    bool          LipSynching;               // +0x35
    bool          UseLipSynching;            // +0x36
    unsigned char _pad_0x37[1];
    CCharString   DialogueVoice;             // +0x38
};

void CTCTalk::Persist(CPersistContext* context, bool flag)
{
    if (flag) {
        context->Transfer("IsPureAISpeaker", IsPureAISpeaker, true);
        context->Transfer("DialogueVoice", &DialogueVoice);
    }
}
