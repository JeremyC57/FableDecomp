// Grade: DIFFER(469v469) -- frame slots + one scheduling pair: retail gives the two implicit CCharString temporaries distinct slots ([ebp-0xc], [ebp-0x10]) so the wide strings sit at [ebp-0x18]/[ebp-0x14]; ours shares one char temp slot (wide strings [ebp-0x14]/[ebp-0x10]); retail also loads the quest-info ecx before the colour's constant B/A stores.
// NPlayerGui::CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo& info) -- when the main player's hero is alive and has an active
// fishing component (TC 0xb1) in states 2..4: mark drawn, build the strain / distance texts, and push the line-distance progress (0 while
// states 2/3) and a strain colour (red = 255*strain, green = 255*(1-strain)) into the quest-info mini-game bar.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00649E8B, length 469 bytes, `ret 4`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (manifest "Catch@0040d959"), CPlayer::GetCharacterThing 0x00487DD0 (manifest
// "_Cons_val"), IS_THING_ALIVE 0x0043DBF0, bitset::test 0x00410DE0, LowerBound 0x004365B0, CTCFishing::GetPFishingRodDef 0x007B0880,
// CCharString 0x0099EBF0 / 0x0099EAE0, CDataBank::GetTextBySymbol 0x009C95E0, ConstructString 0x009C7B70, _ftol 0x00BFEA70,
// CDrawQuestInfo::UpdateMiniGame 0x00644EFD (float,long,long) / 0x00644F85 (CRGBColour), ~CWideString 0x0099B510.
// Strings "TEXT_GUI_FISHING_STRAIN" 0x0125A428, "TEXT_GUI_FISHING_DISTANCE" 0x0125A40C; 1.0f 0x0122DED8, 255.0f 0x01230014.
// Layout (descriptive): game UI +4 (quest info +0x1c8), player manager +8, text owner +0x10 (data bank +0x14), drawn +0x18; CTCFishing
// state +0x14, active +0x18, distance +0x2c, strain +0x34; CFishingRodDef max strain +0x34, max distance +0x38.
// Shape: the hero is passed to IS_THING_ALIVE through a derived->base conversion temporary (esi keeps the hero); the max values are
// `x = comp; if (rod) x = rod->...` (fld/fstp/fld); the final progress is an if/else of two calls (tail-merged); nested ifs, not returns.
// Tried: early returns, explicit vs implicit CCharString temporaries (implicit reached 469), temp vs named colour, reference-bound wide
// strings, an inline GetText helper, a nested block for the distance part.
// Leaf: Draw
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    char* m_pString;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    ~CWideString();                                    // 0x0099B510
};
bool __fastcall ConstructString(CWideString& text, long value);   // 0x009C7B70
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CDrawQuestInfo {
    void UpdateMiniGame(float progress, long a, long b);   // 0x00644EFD
    void UpdateMiniGame(const CRGBColour& colour);        // 0x00644F85
};
struct CGameUI { unsigned char _pad[0x1c8]; CDrawQuestInfo* PQuestInfo; };   // +0x1c8
struct CTCBase;
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0
};
struct CTCBitset {
    unsigned long _M_w[9];
    bool test(long bit) const;                           // 0x00410DE0
};
struct CThing {
    unsigned char _pad00[0x20];
    CTCBitset     UsingComponents;                       // +0x20
    CTCMap        Components;                            // +0x44
    bool HasTC(long type) const { return UsingComponents.test(type); }
    template<class T> __forceinline T* GetTC(long type) const
    {
        unsigned long key = type;
        const CTCMap* map = &Components;
        CTCEntry* it = map->LowerBound(key);
        if (it == map->_M_finish || type < it->Type)
            it = map->_M_finish;
        return (T*)it->TC;
    }
};
struct CThingCreatureBase : CThing {};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x0043DBF0
struct CFishingRodDef {
    unsigned char _pad00[0x34];
    float MaxStrain;                                     // +0x34
    float MaxDistance;                                   // +0x38
};
struct CTCFishing {                                      // interface type 0xb1
    unsigned char _pad00[0x14];
    long State;                                          // +0x14
    bool Active;                                         // +0x18
    unsigned char _pad19[0x2c - 0x19];
    float Distance;                                      // +0x2c
    unsigned char _pad30[4];
    float Strain;                                        // +0x34
    CFishingRodDef* GetPFishingRodDef();                 // 0x007B0880
};
struct CPlayer { CThingCreatureBase* GetCharacterThing(); };         // 0x00487DD0
struct CPlayerManager { CPlayer* GetMainPlayer(); };     // 0x00449970
struct CEnginePrimitivePrepareInfo;
struct CDataOwner { unsigned char _pad[0x14]; CDataBank* PBank; };   // +0x14

namespace NPlayerGui {
struct CDrawFishingGame {
    virtual void v00();
    CGameUI* PGameUI;                                    // +0x04
    CPlayerManager* PPlayerManager;                      // +0x08
    unsigned char _pad0c[4];
    CDataOwner* POwner;                                  // +0x10
    unsigned char _pad14[4];
    bool Drawn;                                          // +0x18
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo& info)
{
    CThingCreatureBase* hero = PPlayerManager->GetMainPlayer()->GetCharacterThing();
    if (IS_THING_ALIVE(hero) && hero->HasTC(0xb1)) {
        CTCFishing* fishing = hero->GetTC<CTCFishing>(0xb1);
        if (fishing->Active) {
            Drawn = true;
            long state = fishing->State;
            if (state != 0 && state >= 2 && state <= 4) {
                CFishingRodDef* rod = fishing->GetPFishingRodDef();
                CWideString strainText = POwner->PBank->GetTextBySymbol("TEXT_GUI_FISHING_STRAIN");
                ConstructString(strainText, (long)fishing->Strain);
                float maxStrain = fishing->Strain;
                if (rod)
                    maxStrain = rod->MaxStrain;
                float strain = fishing->Strain / maxStrain;
                if (strain > 1.0f)
                    strain = 1.0f;
                unsigned char red = (unsigned char)(strain * 255.0f);
                unsigned char green = (unsigned char)((1.0f - strain) * 255.0f);
                CWideString distanceText = POwner->PBank->GetTextBySymbol("TEXT_GUI_FISHING_DISTANCE");
                ConstructString(distanceText, (long)fishing->Distance);
                float maxDistance = fishing->Distance;
                if (rod)
                    maxDistance = rod->MaxDistance;
                if (fishing->State == 3 || fishing->State == 2)
                    PGameUI->PQuestInfo->UpdateMiniGame(0.0f, 0, 0);
                else
                    PGameUI->PQuestInfo->UpdateMiniGame(fishing->Distance / maxDistance, 0, 0);
                CRGBColour colour(red, green, 0, 0xff);
                PGameUI->PQuestInfo->UpdateMiniGame(colour);
            }
        }
    }
}
}
