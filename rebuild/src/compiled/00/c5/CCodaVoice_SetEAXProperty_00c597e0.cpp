// CCodaVoice::SetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size) -- write an EAX property of this voice through the global EAX manager (0x0143BB70) when the voice supports EAX (virtual slot 4); else -9
// Leaf: ?SetEAXProperty@CCodaVoice@@UAEJPBXKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C597E0, 55 bytes, thiscall, `ret 10h`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 440 "0xC597E0/0xC59820 "CThing::AddToRenderList" = a voice's EAX Set/Get"):
//   - this is slot 15 (+0x3c) of the GameCODA voice base vtable 0x012B5C10 (agent441's CCodaVoice, ctor 0x00C59BA0) and of
//     the derived voice vtables 0x012B5C60 (CSoftwareVoice), 0x012B5CD0, 0x012B5D70 (CSoftwareStreamVoice) and 0x012B8AE0;
//   - the callee 0x00C51F00 is agent440's landed CEAXManager::SetVoiceProperty(id, data, size, voice), thiscall on the manager
//     pointer 0x0143BB70; the voice's EAX handle at +0x50 is its `voice` argument;
//   - slot 4 is the EAX capability test: the base's 0x00C3F430 is `return false;` (-9, the library's "unsupported"
//     result), CSoftwareVoice's 0x00C3F500 `return true;`;
//   - the first stack argument (unused) is the property-set GUID, as in CEAXManager::SetListenerProperty 0x00C52980
//     (EAXSet/EAXGet signature: set, property, source, value, size).
//   The catalog "CThing::AddToRenderList" is a propagated label; descriptive names (no PDB/RTTI covers GameCODA).
// Shape: `ret 10h` (four stack arguments, the first unused); the handle is loaded first, then (id, data, size) pushed
//   right to left.
struct CEAXVoice;

class CEAXManager {
public:
    long SetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice);   // 0x00C51F00
};
extern CEAXManager* g_pEAXManager;                  // 0x0143BB70

class CCodaVoice {                                  // vtable 0x012B5C10 (19 slots)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool SupportsEAX();                     // slot 4 (+0x10): 0x00C3F430 in the base (false)
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual long SetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size);   // slot 15: 0x00C597E0
    virtual long GetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size);   // slot 16: 0x00C59820
    unsigned char _pad04[0x50 - 4];
    CEAXVoice*    m_pEAXVoice;                      // +0x50 (the EAX manager's per-voice record)
};

long CCodaVoice::SetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size)
{
    if (SupportsEAX())
        return g_pEAXManager->SetVoiceProperty(id, data, size, m_pEAXVoice);
    return -9;
}
