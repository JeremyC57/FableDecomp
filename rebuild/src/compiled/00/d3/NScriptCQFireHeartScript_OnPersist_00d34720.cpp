// NScript::CQ_FireHeartScript::OnPersist(CPersistContext& context) -- saves/loads the script members HasHeroHitVillagersGroupOne, HasHeroHitVillagersGroupTwo, HasDemonDoorBeenEnabled, HasScytheCutscenePlayed by name (default false / 0)
// Leaf: ?OnPersist@CQ_FireHeartScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d34720, 131 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the script vtable 0x012CCB90 (the landed constructor 0x00D34470); the four literals name Ego_r.pdb CQ_FireHeartScript members (HasHeroHitVillagersGroupOne, HasHeroHitVillagersGroupTwo, HasDemonDoorBeenEnabled, HasScytheCutscenePlayed); the PDB name sets hold ?OnPersist@CQ_FireHeartScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FableWin.pdb template names, bound).
// Shape (family L141 = the landed entity OnPersist 0x00E641B0 with four members): each default is a temporary in a dead
//   stack slot; the member offsets are retail's (the script base is retail 0x40 bytes).
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
class CQ_FireHeartScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    void* m_pInterface;                                 // +0x40
    unsigned char _pad44[0x97 - 0x44];
    bool           HasHeroHitVillagersGroupOne;             // +0x97
    bool           HasHeroHitVillagersGroupTwo;             // +0x98
    unsigned char _pad99[0x1a0 - 0x99];
    bool           HasDemonDoorBeenEnabled;                 // +0x1a0
    bool           HasScytheCutscenePlayed;                 // +0x1a1
};
void CQ_FireHeartScript::OnPersist(CPersistContext& context)
{
    context.Transfer("HasHeroHitVillagersGroupOne", HasHeroHitVillagersGroupOne, false);
    context.Transfer("HasHeroHitVillagersGroupTwo", HasHeroHitVillagersGroupTwo, false);
    context.Transfer("HasDemonDoorBeenEnabled", HasDemonDoorBeenEnabled, false);
    context.Transfer("HasScytheCutscenePlayed", HasScytheCutscenePlayed, false);
}
}