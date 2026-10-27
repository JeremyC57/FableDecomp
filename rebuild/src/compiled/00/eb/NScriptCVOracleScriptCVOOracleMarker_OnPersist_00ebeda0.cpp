// NScript::CV_OracleScript::CVO_OracleMarker::OnPersist(CPersistContext& context) -- saves/loads TimesPlayedACat, TimesPlayedCCat, TimesPlayedMCat, TimesPlayedYCat, HaveBeenToldHowToInteract by name (default false / 0)
// Leaf: ?OnPersist@CVO_OracleMarker@CV_OracleScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ebeda0, 144 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the vtable 0x012F0CD8 that the landed NScript::CV_OracleScript::CVO_OracleMarker::Alloc 0x00EBCE80 stores; the five literals name Ego_r.pdb CV_OracleScript::CVO_OracleMarker members (TimesPlayedACat, TimesPlayedCCat, TimesPlayedMCat, TimesPlayedYCat, HaveBeenToldHowToInteract); the PDB name sets hold its OnPersist.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0, CPersistContext::Transfer<long>(const char*, long&, const long&) 0x00410BE0 (FableWin.pdb template names, bound).
// Shape (family L144 = the landed entity OnPersist 0x00E641B0's model): each default is a temporary in a dead stack slot;
//   the member offsets are retail's.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
namespace NScript {
class CEntityScriptBase {                           // retail 0x14 bytes (vptr, Game, CScriptThing Me)
public:
    virtual ~CEntityScriptBase();                    // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();
private:
    virtual void OnPersist(CPersistContext& context);   // slot 4
protected:
    unsigned char _base[0x14 - 4];
};
class CV_OracleScript {
public:
    class CVO_OracleMarker;
};
class CV_OracleScript::CVO_OracleMarker : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    CV_OracleScript* ParentClass;                      // +0x14
    void* PMasterData;                               // +0x18
    long           TimesPlayedYCat;                         // +0x1c
    long           TimesPlayedMCat;                         // +0x20
    long           TimesPlayedCCat;                         // +0x24
    long           TimesPlayedACat;                         // +0x28
    bool           HaveBeenToldHowToInteract;               // +0x2c
};
void CV_OracleScript::CVO_OracleMarker::OnPersist(CPersistContext& context)
{
    context.Transfer("TimesPlayedACat", TimesPlayedACat, 0L);
    context.Transfer("TimesPlayedCCat", TimesPlayedCCat, 0L);
    context.Transfer("TimesPlayedMCat", TimesPlayedMCat, 0L);
    context.Transfer("TimesPlayedYCat", TimesPlayedYCat, 0L);
    context.Transfer("HaveBeenToldHowToInteract", HaveBeenToldHowToInteract, false);
}
}