// NScript::CQ_HeroSoulsScript::OnPersist(CPersistContext& context) -- saves/loads HeardFirstBriarRoseMessage, HeardSecondBriarRoseMessage, HeardGMMessage, HeardJackMessage, PlayedOpeningCS by name (default false / 0)
// Leaf: ?OnPersist@CQ_HeroSoulsScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d785e0, 144 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the vtable 0x012D2F48 that the landed constructor 0x00D784D0 stores; the five literals name Ego_r.pdb CQ_HeroSoulsScript members (HeardFirstBriarRoseMessage, HeardSecondBriarRoseMessage, HeardGMMessage, HeardJackMessage, PlayedOpeningCS); the PDB name sets hold its OnPersist.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FableWin.pdb template names, bound).
// Shape (family L144 = the landed entity OnPersist 0x00E641B0's model): each default is a temporary in a dead stack slot;
//   the member offsets are retail's.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
namespace NScript {
class CQ_HeroSoulsScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    void* m_pInterface;                                 // +0x40
    void* m_pOwner;                                     // +0x44
    bool           HeardFirstBriarRoseMessage;              // +0x48
    bool           HeardSecondBriarRoseMessage;             // +0x49
    bool           HeardGMMessage;                          // +0x4a
    bool           HeardJackMessage;                        // +0x4b
    bool           PlayedOpeningCS;                         // +0x4c
};
void CQ_HeroSoulsScript::OnPersist(CPersistContext& context)
{
    context.Transfer("HeardFirstBriarRoseMessage", HeardFirstBriarRoseMessage, false);
    context.Transfer("HeardSecondBriarRoseMessage", HeardSecondBriarRoseMessage, false);
    context.Transfer("HeardGMMessage", HeardGMMessage, false);
    context.Transfer("HeardJackMessage", HeardJackMessage, false);
    context.Transfer("PlayedOpeningCS", PlayedOpeningCS, false);
}
}