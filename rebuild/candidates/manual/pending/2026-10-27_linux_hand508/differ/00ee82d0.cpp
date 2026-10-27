// NScript::CCS_OakValeRevisitedScript::Main() -- Oakvale revisited: post-add the scripted entities, wait for OakBay to load, add logbook story entry 100 and run the Oakvale sequence
// Grade: DIFFER(172v181) -- residue: retail opens with `mov dword [esp+8],0` (an 8-byte dead zero store into the single local slot the "OakBay" temporary later uses; probably the entry-time init of a conditional-temporary flag VC7.1 leaves behind) plus a 1-byte nop aligning the loop head; the body is otherwise identical. Tried: plain while (172v181), `true &&` guard (live flag code, 171v181), dead `|| false &&` temp (C1001 ICE).
// Leaf: ?Main@CCS_OakValeRevisitedScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE82D0, 181 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the RTTI-less level-script vtable 0x012F53C0, which the landed constructor NScript::CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript 0x00EE8100 stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Main@CCS_OakValeRevisitedScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CCS_OakValeRevisitedScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptBase::PostAddScriptedEntities 0x00CB8930, IsActiveThreadTerminating 0x00CB7940, AddLogbookStoryEntry
//   0x00CBE87F (__fastcall), OakVale 0x00EE8390; interface slots 64 PostAddScriptedEntities, 12 IsRegionLoaded ("OakBay"
//   0x012D31E4 temporary), 7 NewScriptFrame; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void __fastcall AddLogbookStoryEntry(long entry);       // 0x00CBE87F (bound ?AddLogbookStoryEntry@@YIXJ@Z)
class CGameScriptInterface {                     // the script interface vtable (slot names: ghidra_out/cgsi_slots_aligned.tsv)
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                            // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& a0) const;   // slot 12 (+0x30)
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100)
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void PostAddScriptedEntities();                     // 0x00CB8930
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CCS_OakValeRevisitedScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void OakVale();                                     // 0x00EE8390
    virtual void Main();
};

void CCS_OakValeRevisitedScript::Main()
{
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("OakBay")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddLogbookStoryEntry(100);
    OakVale();
}
}
