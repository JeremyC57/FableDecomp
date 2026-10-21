// Grade: DIFFER(316v335) -- block layout + one frame byte: retail branches straight out of the unassigned-string strcmp arm, keeps the block-compare arm out of line after the epilogue with its bool stored to a frame byte (+0xf, frame 0x18 vs 0x14), and lays the matched-chest body inline; ours merges the two arms (sete) and moves the matched body after the loop
// NScript::CQ_HobbeCaveScript::CCheckTheSilverKeyChest::Main() -- waits until the hero is rewarded from the "FiveKeyChest"; then, if the sacrificial bandit is still alive, he comments on the opened silver-key chest
// Leaf: ?Main@CCheckTheSilverKeyChest@CQ_HobbeCaveScript@NScript@@EAEXXZ
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (the landed body, stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) ? false : true; }   // 0x004115A0 (inlined here)
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
struct CScriptThing {                            // vtable 0x01238C8C, 12 bytes
    struct CImp {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
        virtual bool IsAlive() const;           // slot 75 (+0x12c)
    };
    virtual ~CScriptThing();                    // 0x004AA840
    CImp* PImp_Data;                            // +0x04
    void* PImp_Info;                            // +0x08
    bool IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }
};
struct CGameScriptInterface {                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void NewScriptFrame() const;        // slot 7 (+0x1c)
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString& chest) const;   // slot 57 (+0xe4)
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
};
namespace NScript {
struct CQ_SunnyvaleMasterData;
struct CQ_HobbeCaveScript {
    bool MakeBanditComment(const CCharString& text, const CScriptThing& bandit);   // 0x00D905C0
    class CCheckTheSilverKeyChest {
    public:
        CGameScriptInterface*   Game;           // +0x04
        CScriptThing            Me;             // +0x08
        CQ_HobbeCaveScript*     ParentClass;    // +0x14
        CQ_SunnyvaleMasterData* PMasterData;    // +0x18
        bool IsActiveThreadTerminating();       // 0x00F35B30
    private:
        virtual void Main();
    };
};

void CQ_HobbeCaveScript::CCheckTheSilverKeyChest::Main()
{
    CCharString chest;
    while (!IsActiveThreadTerminating()) {
        if (Game->MsgOnHeroRewardedWithItemsFrom(chest) && chest == "FiveKeyChest") {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            CScriptThing bandit = Game->GetThingWithScriptName("BanditSacrifice");
            if (bandit.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->MakeBanditComment("TEXT_QST_036_BANDIT_OPENED_SILVER_KEY_CHEST", bandit);
            }
            return;
        }
        Game->NewScriptFrame();
    }
}
}
