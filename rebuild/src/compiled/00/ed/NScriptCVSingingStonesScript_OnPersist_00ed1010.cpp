// NScript::CV_SingingStonesScript::OnPersist(CPersistContext& context) -- saves/loads the script members DoorManIntroComplete, DoorManAttackedByHero, DoorManHasBribe, DoorManComplete by name (default false / 0)
// Leaf: ?OnPersist@CV_SingingStonesScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ed1010, 131 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the script vtable 0x012F2B0C (the landed constructor 0x00ED0E60 (catalogued under the stand-in CVtable_012f2b0c)); the four literals name Ego_r.pdb CV_SingingStonesScript members (DoorManIntroComplete, DoorManAttackedByHero, DoorManHasBribe, DoorManComplete); the PDB name sets hold ?OnPersist@CV_SingingStonesScript@NScript@@UAEXAAVCPersistContext@@@Z.
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
class CV_SingingStonesScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    void* m_pInterface;                                 // +0x40
    unsigned char _pad44[0x98 - 0x44];
    bool           DoorManIntroComplete;                    // +0x98
    bool           DoorManAttackedByHero;                   // +0x99
    bool           DoorManHasBribe;                         // +0x9a
    bool           DoorManComplete;                         // +0x9b
};
void CV_SingingStonesScript::OnPersist(CPersistContext& context)
{
    context.Transfer("DoorManIntroComplete", DoorManIntroComplete, false);
    context.Transfer("DoorManAttackedByHero", DoorManAttackedByHero, false);
    context.Transfer("DoorManHasBribe", DoorManHasBribe, false);
    context.Transfer("DoorManComplete", DoorManComplete, false);
}
}