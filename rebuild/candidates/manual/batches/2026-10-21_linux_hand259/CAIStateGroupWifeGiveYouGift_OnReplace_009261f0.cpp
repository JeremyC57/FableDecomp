// CAIStateGroup_WifeGiveYouGift::OnReplace() -- once the give speech was delivered and the hero is alive: create the gift object at the hero, put it into the hero's inventory of its category, show the "wife gave you a gift" subtitle with the item description, and clear the wife component's gift state
// Leaf: OnReplace
// Retail: Fable.exe (TLC, VC7.1) @ 0x009261F0, 512 bytes.
// Callees: CAIStateGroupBase::GetPlayerManager @ 0x008FCF90 (manifest "CPixelShader::CountReferences", propagated),
//   CPlayerManager::GetMainPlayer @ 0x00449970 (manifest "Catch@0040d959"), CPlayer::GetHeroCreature @ 0x00487DC0 (manifest
//   "_Cons_val<...EHeroMorphType...>"), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::
//   GetPosition @ 0x004C73D0, CreateThingObject @ 0x00703210 (__fastcall), CVectorMap::LowerBound @ 0x0040F020, the inventory
//   item's category getter @ 0x005D9030, puts @ 0x00BFEAF2 + abort @ 0x00BFEAEC (STLport bitset<274>::test range check),
//   CTCInventoryItem::AddToInventory @ 0x005D8D50, CDataBank::GetTextBySymbol @ 0x009C95E0 (CWideString by hidden pointer),
//   CTCInventoryItem::GetDescription @ 0x005D9070, ConstructString @ 0x009C7980 (__fastcall), ~CWideString @ 0x0099B510,
//   CSubtitleRenderer::SetText @ 0x00492E4B; globals 0x013B86A0 (game: text bank +0x14, subtitle font +0x1614C) and
//   0x013B8790 (display engine, subtitle renderer +0x18).
// Layout: engine/CAIStateGroup_WifeGiveYouGift.h (GiftToGiveDef +0x18, DeliveredGiveSpeech +0x1c) over the landed local
//   CAIStateGroupBase model; CThing UsingComponents bitset<274> +0x20, Components +0x44, player number (signed char) +0x90.
//   The wife component (0xd7) field +0x74 is named descriptively.
// Shape: the landed CTCContainer::AddItemToInventory (0x008ED090) model -- the gift's item component through HasTC + GetTC with
//   an unset result local (the absent arm reads the lookup key slot), the hero's inventory through the range-checked
//   bitset::test; the "" script name and the text symbol as implicit const char* -> CCharString conversions; the item
//   description as an unnamed CWideString temporary handed to ConstructString.
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CTCBase;
struct C3DVector { float x, y, z; };
struct CRightHandedSet;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x90 - 0x4c];
    signed char PlayerNumber;                           // +0x90
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    bool TestComponent(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (UsingComponents[position >> 5] & (1UL << (position & 31))) != 0;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;
        if (TestComponent(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
    __forceinline CTCBase* GetTCIfHas(long interfaceType)
    {
        CTCBase* tc;
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    ~CWideString();                                     // 0x0099B510
};
bool __fastcall ConstructString(CWideString* text, const CWideString* argument);   // 0x009C7980
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharString& scriptName);   // 0x00703210
struct CTCInventoryBase;
struct CTCInventoryItem {
    long GetInventoryType();                            // 0x005D9030
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
    CWideString GetDescription();                       // 0x005D9070
};
struct CPlayer { CThing* GetHeroCreature(); };          // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449970
struct CDataBank { CWideString GetTextBySymbol(const CCharString& symbol); };   // 0x009C95E0
struct CFontBank;
struct CGame {
    unsigned char _pad[0x14];
    CDataBank* PTextBank;                               // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank* PSubtitleFont;                           // +0x1614c
};
extern CGame* g_pGame;                                  // 0x013B86A0
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CWideString* text, unsigned long colour, float time, bool a, bool b);   // 0x00492E4B
};
struct CDisplayEngine {
    unsigned char _pad[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
};
extern CDisplayEngine* g_pDisplayEngine;                // 0x013B8790
struct CTCWife {                                        // interface 0xd7
    unsigned char _pad[0x74];
    long GiftState;                                     // +0x74
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                   // +0x04
    long ScriptAIPriority;                              // +0x08
    void* Name_PStringData;                             // +0x0c
    unsigned char Terminated;                           // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CPlayerManager* GetPlayerManager();                 // 0x008FCF90
};
struct CAIStateGroup_WifeGiveYouGift : CAIStateGroupBase {
    long FrameToSpeakNext;                              // +0x14
    long GiftToGiveDef;                                 // +0x18
    bool DeliveredGiveSpeech;                           // +0x1c
    virtual void OnReplace();
};

void CAIStateGroup_WifeGiveYouGift::OnReplace()
{
    if (!DeliveredGiveSpeech)
        return;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    if (hero == 0 || (hero->m_flags91 & 1))
        return;
    CThing* gift = CreateThingObject(GiftToGiveDef, &hero->GetPosition(), hero->PlayerNumber, 0, 0, "");
    CTCInventoryItem* item = (CTCInventoryItem*)gift->GetTCIfHas(0x1a);
    long type = item->GetInventoryType();
    item->AddToInventory((CTCInventoryBase*)hero->GetTCIfPresent(type), false, true, 0, false, false);
    CWideString text = g_pGame->PTextBank->GetTextBySymbol("TEXT_GUI_WIFE_GIVEN_YOU_GIFT");
    ConstructString(&text, &item->GetDescription());
    g_pDisplayEngine->Subtitles.SetText(g_pGame->PSubtitleFont, &text, 0x122, 0.0f, false, true);
    if (PBrain->PCreature->HasTC(0xd7))
        ((CTCWife*)PBrain->PCreature->GetTC(0xd7))->GiftState = 0;
}
