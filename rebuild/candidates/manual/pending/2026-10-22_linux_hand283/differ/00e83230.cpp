// HandleVillageSelection() -- a quest entity script picks its village from its data string (BowerstoneSlums,
// Grade: DIFFER(818v820) -- branch 1 keeps Game in esi and reloads its vtable for GetHero in retail; ours shares one vtable load (or reloads Game without the named local); register picks around the inline counted-pointer assign
//   KnotholeGlade, Oakvale, BanditCamp: selection 1/4/2/3, anything else 4) and stores the village thing nearest the
//   hero with that village def name in the parent quest (the catalog class CCreatureAction_TrollWhackGroundBase is a
//   propagated label)
// Leaf: HandleVillageSelection
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E83230, length 832 bytes, plain `ret`.
// Callees: IsInterrupted 0x00F35B30 (CEntityScriptBase), CScriptThing slot 3 GetDataString / ~CScriptThing 0x004AA840 /
//   operator= 0x008AB980, the game script interface (+4) slots 70 GetHero and 81 GetNearestWithDefName
//   (ghidra_out/cgsi_slots_aligned.tsv), CCharString 0x0099EBF0 / ~ 0x0099EAE0, Compare 0x00411570 (__fastcall),
//   operator delete 0x00BFE9BC.
// Layout (retail bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; parent +0x48
//   selection, +0x9c CScriptThing village (PImp {Data +0xa0, Info +0xa4}); CCPPointerInfo {RefCount, DeleteFunc, Data}.
#include <string.h>
struct CCharStringData {
    char*         PData;
    unsigned long Length;
};
long __fastcall Compare(const char* a, const char* b);       // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return Compare(PStringData->PData, text) == 0;
    }
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CCountedPointerWords {
    void*           Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointerWords& operator=(const CCountedPointerWords& other)
    {
        CCPPointerInfo* info = other.Info;
        void* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
struct CScriptThing {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CCharString GetDataString() const;               // slot 3 (+0xc)
    CCountedPointerWords PImp;                               // +0x04
    ~CScriptThing();                                         // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);      // 0x008AB980
};
struct CGameScriptInterface_Calls {               // slots: ghidra_out/cgsi_slots_aligned.tsv
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080();
    virtual CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& defName) const;   // slot 81 (+0x144)
};
struct CVillageQuest {
    unsigned char _pad00[0x48];
    long          Selection;                                 // +0x48
    unsigned char _pad4c[0x9c - 0x4c];
    CScriptThing  Village;                                   // +0x9c
};
struct CVillageSelectionScript {
    void*                       __vftable;                   // +0x00
    CGameScriptInterface_Calls* Game;                        // +0x04
    CScriptThing                Me;                          // +0x08
    CVillageQuest*              ParentClass;                 // +0x14
    bool IsInterrupted();                                    // 0x00F35B30
    void HandleVillageSelection();
};

void CVillageSelectionScript::HandleVillageSelection()
{
    if (Me.GetDataString() == "BowerstoneSlums") {
        if (IsInterrupted())
            return;
        ParentClass->Selection = 1;
        CCharString name("VILLAGE_BOWERSTONE_SLUMS");
        CGameScriptInterface_Calls* game = Game;
        ParentClass->Village.PImp = game->GetNearestWithDefName(game->GetHero(), name).PImp;
    } else if (Me.GetDataString() == "KnotholeGlade") {
        if (IsInterrupted())
            return;
        ParentClass->Selection = 4;
        ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_KNOTHOLE_GLADE");
    } else if (Me.GetDataString() == "Oakvale") {
        if (IsInterrupted())
            return;
        ParentClass->Selection = 2;
        ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_OAKVALE");
    } else if (Me.GetDataString() == "BanditCamp") {
        if (IsInterrupted())
            return;
        ParentClass->Selection = 3;
        ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BANDIT_CAMP_RESIDENTIAL");
    } else {
        if (IsInterrupted())
            return;
        ParentClass->Selection = 4;
    }
}
