// CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- per-frame draw of a tavern game: clears last frame's primitives, stamps the render time and, while the game is being played, renders the score / target / best (and, unless the quit prompt is up, additional-info) texts, the bet-and-money boxes and the score box, plus a full-screen fade sprite in the leaving state
// Grade: DIFFER(1185v1176) -- constant/register choice: retail loads `info` into ebp before the state tests and pushes / compares the state 5 as immediates; ours holds 5 in ebp during the tests (push ebp / cmp [esi+0A8h],ebp) and loads info into ebp afterwards, which rotates the eax/ecx/edx argument loads of every RenderText call (also tried: ETavernGameStates enum typing, inert; the vptr base split and the named win-phrase local fixed the layout and frame)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ECA50, 1174 bytes, `ret 4`. CTCTavernGame vtable slot 11; FableWin.pdb
//   ?DrawPreparePrimitivesForRendering@CTCTavernGame@@UAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0, vector<CEnginePrimitiveHandle>::erase 0x00440D70,
//   GFGetTime 0x009E1BC0, the game's text virtuals (slots 42 GetWinPhrase, 43 GetBestScore, 44 GetCurrentScore,
//   45 GetRequiredScore, 46 GetAdditionalInfo, 47 GetBet, 48 GetPlayersMoney, 49 GetTotalWinnings) and slot 50
//   DrawBetAndMoney, IsCurrentOrUpcomingState 0x005E9BF0, CWideString(const CWideString&) 0x0099B720 / ~ 0x0099B510 /
//   operator!= 0x0099B150, RenderText 0x005EC7C0, ClearBetAndMoney 0x005E9F30, DrawBox 0x005EC990, ClearBox 0x005EC5C0,
//   CDisplayManager::GetRenderTargetDimensions 0x009BEDC0, CEnginePrimitive2DSprite ctor 0x0041BEB0, the handle vector's
//   grow-by-one 0x005ED940, the renderer's slot 23 (+0x5C), CFontBank::GetFont 0x009E2C80 (bank 0x013B838C) and the font's
//   slot 2, CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator delete 0x00BFE9BC.
// Shape: IsCurrentOrUpcomingState is defined above in the TU: VC7.1 expands its first three uses and calls the last three
//   without reloading ecx (it knows the callee keeps ecx); the eight text strings are function-block locals destroyed in
//   reverse; white is CRGBColour(255, 255, 255, 255) built in its argument slot; the fade sprite block is the landed
//   0x007E7C90 form; the font temporary's release is the landed CCountedPointer Release + clears.
// Layout: retail CTCTavernGame as the landed 0x005EC8A0 (LastRenderTime +0x18, IsBeingPlayed +0x20, Handles +0x44, fonts
//   +0x80/+0x88/+0x90, YesState +0xA8, AltState +0xB0, TavernGameState +0xC0, PreviousTavernGameState +0xC4);
//   CTavernGameDef (PDB = retail up to here) text positions +0x128..+0x16C, DisplayErrata +0x19C.
struct CThing;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool operator!=(const unsigned short* text) const;  // 0x0099B150
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    C2DVector TopLeft, BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
struct C2DExtentsI { long X, Y; };
struct CDisplayManager {
    C2DExtentsI GetRenderTargetDimensions() const;      // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;              // 0x013B8390

void __cdecl operator delete(void* block);              // 0x00BFE9BC
struct CCPPointerInfo {                                 // CCountedPointer count block
    long References;                                    // +0
    void (__fastcall* Destroy)(void* object);           // +4
    void* Object;                                       // +8
};
template <class T> struct CCountedPointer {
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    T* operator->() const { return Data; }
};
struct CFontBank {
    virtual void v00(); virtual void v01();
    virtual float GetLineHeight() const;                // slot 2, name descriptive
    CCountedPointer<const CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
extern CFontBank* g_pFontBank;                         // 0x013B838C

struct CEnginePrimitiveHandle { long m_header; void* Data; };
struct CHandleVector {                                  // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    void erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    void clear() { erase(_M_start, _M_finish); }
    void push_back_default();                           // 0x005ED940
};
struct CEnginePrimitive2DSprite {
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long a, unsigned long b,
                             long blend, long interpolation, float c, float d, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned char alpha, unsigned long e, const C2DVector& uv0,
                             const C2DVector& uv1);     // 0x0041BEB0
};
struct CEnginePrimitiveRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;                 // +0x34
};

double __fastcall GFGetTime();                          // 0x009E1BC0

struct CTavernGameDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x128 - 0x08];
    float ScoreX, ScoreY;                               // +0x128
    float TargetX, TargetY;                             // +0x130
    float BestX, BestY;                                 // +0x138
    float AdditionalX, AdditionalY;                     // +0x140
    float BetX, BetY;                                   // +0x148
    float MoneyX, MoneyY;                               // +0x150
    float WinningsX, WinningsY;                         // +0x158
    float MainBetX, MainBetY;                           // +0x160
    float MainMoneyX, MainMoneyY;                       // +0x168
    unsigned char _pad170[0x19c - 0x170];
    bool  DisplayErrata;                                // +0x19C
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameDef>* out);   // 0x005ED4A0
};
// Same-TU stand-in for GetSubDef<CTavernGameDef> 0x005ED4A0 (as the landed 0x005EA400): it only writes the out slot, so
// VC7.1 keeps the def in a register from the call on.
CTavernGameDef* FindTavernGameDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CTavernGameDef>* out)
{
    CTavernGameDef* def = FindTavernGameDef(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
struct CThingWithDef {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                            // +0x70 (CDefPointer<const CThingBaseDef>)
};

struct CTCBase {                                        // polymorphic root (a double in the derived class would pad its vptr)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual CWideString GetWinPhrase();                 // slot 42
    virtual CWideString GetBestScore();                 // slot 43
    virtual CWideString GetCurrentScore();              // slot 44
    virtual CWideString GetRequiredScore();             // slot 45
    virtual CWideString GetAdditionalInfo();            // slot 46
    virtual CWideString GetBet();                       // slot 47
    virtual CWideString GetPlayersMoney();              // slot 48
    virtual CWideString GetTotalWinnings();             // slot 49
    virtual void DrawBetAndMoney(const CEnginePrimitivePrepareInfo& info, float betX, float betY, float moneyX,
                                 float moneyY, float winningsX, float winningsY);   // slot 50
    CThingWithDef*  ParentThing;                        // +0x04
    bool            ToBeRemoved;                        // +0x08
    bool            IsInGlobalUpdateTCs;                // +0x09
};
struct CTCTavernGame : CTCBase {
    unsigned char   UIComponents[0x0c];                 // +0x0C vector<NUISystem::CComponent*>
    double          LastRenderTime;                     // +0x18
    bool            IsBeingPlayed;                      // +0x20
    unsigned char   _pad21[0x44 - 0x21];
    CHandleVector   Handles;                            // +0x44
    unsigned char   _pad50[0x80 - 0x50];
    CCountedPointer<CFontBank> PScoreFont;              // +0x80
    CCountedPointer<CFontBank> PTargetFont;             // +0x88
    CCountedPointer<CFontBank> PStatsFont;              // +0x90
    unsigned char   _pad98[0xa8 - 0x98];
    long            YesState;                           // +0xA8 ETavernGameStates
    long            NoState;                            // +0xAC
    long            AltState;                           // +0xB0
    unsigned char   _padb4[0xc0 - 0xb4];
    long            TavernGameState;                    // +0xC0
    long            PreviousTavernGameState;            // +0xC4
    bool IsCurrentOrUpcomingState(long state);          // 0x005E9BF0
    void RenderText(const CEnginePrimitivePrepareInfo& info, float x, float y, CWideString text, const CFontBank& font,
                    CRGBColour colour);                 // 0x005EC7C0
    void ClearBetAndMoney();                            // 0x005E9F30
    void DrawBox(const CEnginePrimitivePrepareInfo& info);   // 0x005EC990
    void ClearBox();                                    // 0x005EC5C0
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CTCTavernGame::IsCurrentOrUpcomingState(long state)
{
    return TavernGameState == state || YesState == state || AltState == state;
}

void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer<CTavernGameDef> def;
    ParentThing->PBaseDef->GetSubDef(&def);
    Handles.clear();
    GFGetTime();
    LastRenderTime = GFGetTime();
    if (IsBeingPlayed) {
        CWideString currentScore = GetCurrentScore();
        CWideString bestScore = GetBestScore();
        CWideString requiredScore = GetRequiredScore();
        CWideString additionalInfo = GetAdditionalInfo();
        CWideString bet = GetBet();
        CWideString money = GetPlayersMoney();
        CWideString winnings = GetTotalWinnings();
        if ((!IsCurrentOrUpcomingState(1) && !IsCurrentOrUpcomingState(2) && !IsCurrentOrUpcomingState(3)
             && !IsCurrentOrUpcomingState(0xa) && !IsCurrentOrUpcomingState(0x2f) && !IsCurrentOrUpcomingState(5))
            || PreviousTavernGameState == 9) {
            bool quitPrompt = TavernGameState == 0x2d && YesState == 5;
            RenderText(info, def->ScoreX, def->ScoreY, currentScore, *PScoreFont.Data, CRGBColour(0xff, 0xff, 0xff, 0xff));
            RenderText(info, def->TargetX, def->TargetY, requiredScore, *PTargetFont.Data, CRGBColour(0xff, 0xff, 0xff, 0xff));
            RenderText(info, def->BestX, def->BestY, bestScore, *PStatsFont.Data, CRGBColour(0xff, 0xff, 0xff, 0xff));
            if (!quitPrompt)
                RenderText(info, def->AdditionalX, def->AdditionalY, additionalInfo, *PStatsFont.Data,
                           CRGBColour(0xff, 0xff, 0xff, 0xff));
            if (!def->DisplayErrata && TavernGameState == 0x17) {
                ClearBetAndMoney();
                DrawBox(info);
            } else {
                DrawBetAndMoney(info, def->BetX, def->BetY, def->MoneyX, def->MoneyY, def->WinningsX, def->WinningsY);
                DrawBox(info);
            }
        } else {
            if (YesState == 5)
                DrawBetAndMoney(info, def->MainBetX, def->MainBetY, def->MainMoneyX, def->MainMoneyY, 0.0f, 0.0f);
            else
                ClearBetAndMoney();
            ClearBox();
        }
        if (TavernGameState == 0x2d && YesState == 0xb) {
            C2DExtentsI size = g_pDisplayDevice->GetRenderTargetDimensions();
            C2DBoxF box(0.0f, 0.0f, (float)size.X, (float)size.Y);
            CEnginePrimitive2DSprite sprite(0, box, 0x4a, 0, 0, 2, 0, 0.0f, 0.0f, 0xff, 0, 0, 0, 0, C2DVector(0.0f, 0.0f),
                                            C2DVector(0.0f, 0.0f));
            Handles.push_back_default();
            info.Renderer->AddPrimitive(Handles._M_finish - 1, sprite, 0x40);
        }
        g_pFontBank->GetFont("ENG_ARIAL_16")->GetLineHeight();
        CWideString winPhrase = GetWinPhrase();
        if (winPhrase != (const unsigned short*)L"") {
        }
    }
}
