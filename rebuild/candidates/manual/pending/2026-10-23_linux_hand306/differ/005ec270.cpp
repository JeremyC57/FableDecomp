// Grade: DIFFER(883v843) -- def cached in regs (retail re-reads [esp+10] every use), zero reg ebx
// CTCTavernGame::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) -- the tavern game's state-driven input: start/quit prompts, the yes/no/alt answers, the bet stick and the round-over continue
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EC270, 843 bytes (+ jump tables), `ret 0xc`. Ego_r.pdb
//   `?ProcessInput@CTCTavernGame@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z`.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (inlined GetDef 0x005EA000);
//   CInputProcessGameBase::IsEventButtonPressed 0x00687A90 / IsEventGameAction 0x00687CF0 /
//   GetEventGameActionVector 0x00687D00; CInputEvent::GetType 0x00A03B40; CProcessedInput::SetHandled 0x00A0D390;
//   CTCTavernGame::GetText 0x005E9E70 / GetTextWithNarrator 0x005EB3A0 / SetStateAfterYesNo 0x005EAF30 /
//   ChangeBet 0x005EB810 / SetState 0x005E9B20; CSubtitleRenderer::Clear(bool) 0x00492BAB on g_pPlayerGui 0x013B8790
//   (+0x18). Float pool: -0.2f 0x012560B4, 0.2f 0x012305A0.
// Layout (retail CTCTavernGame, the Ego_r.pdb view less 8 up to +0x48 and less 0x1c after the five 12-byte vectors):
//   GameWon +0x21, GameFinished +0x28, BaseBet +0x34, LastBet +0x38, CurrentMoney +0x40, YesState/NoState/AltState
//   +0xa8/+0xac/+0xb0, TavernGameState +0xc0, NextTavernGameState +0xc8; CTavernGameDef text ids +0xfc/+0x100/+0x104,
//   MinBet/MaxBet +0x110/+0x114 (as the landed ChangeBet 0x005EB810).
struct C2DVector { float X, Y; };

struct CWideString {
    void* m_data;
    CWideString(const CWideString& other);              // 0x0099B5B0
    ~CWideString();                                     // 0x0099B510
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CTavernGameDef : CDefBase {
    unsigned char _pad08[0xfc - 0x08];
    unsigned long StartQuestionText;                    // +0xfc
    unsigned long StartYesText;                         // +0x100
    unsigned long StartNoText;                          // +0x104
    unsigned char _pad108[0x110 - 0x108];
    long MinBet;                                        // +0x110
    long MaxBet;                                        // +0x114
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);                   // 0x005ED4A0
};
struct CThing {
    unsigned char _pad0[0x70];
    CDefClassBase* PDef;                                // +0x70
};

struct CInputEvent {
    long GetType() const;                               // 0x00A03B40
};
struct CProcessedInput {
    void SetHandled();                                  // 0x00A0D390
};
struct CInputProcessTavernGame {
    bool IsEventButtonPressed(const CInputEvent& event);                                 // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);                       // 0x00687CF0
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector& v);  // 0x00687D00
};
struct CSubtitleRenderer {
    void Clear(bool immediately);                       // 0x00492BAB
};
struct CPlayerGui {
    unsigned char _pad0[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

enum ETavernGameStates { TAVERN_GAME_STATE_NONE = 0, TAVERN_GAME_STATE_5 = 5, TAVERN_GAME_STATE_6 = 6 };

struct CTCTavernGame {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x21 - 0x08];
    bool GameWon;                                       // +0x21
    unsigned char _pad22[0x28 - 0x22];
    bool GameFinished;                                  // +0x28
    unsigned char _pad29[0x34 - 0x29];
    long BaseBet;                                       // +0x34
    long LastBet;                                       // +0x38
    long TotalWinnings;                                 // +0x3c
    long CurrentMoney;                                  // +0x40
    unsigned char _pad44[0xa8 - 0x44];
    ETavernGameStates YesState;                         // +0xa8
    ETavernGameStates NoState;                          // +0xac
    ETavernGameStates AltState;                         // +0xb0
    unsigned char _padb4[0xc0 - 0xb4];
    ETavernGameStates TavernGameState;                  // +0xc0
    ETavernGameStates PreviousTavernGameState;          // +0xc4
    ETavernGameStates NextTavernGameState;              // +0xc8

    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef(out); }   // 0x005EA000
    CWideString GetText(unsigned long id);              // 0x005E9E70
    CWideString GetTextWithNarrator(unsigned long id);  // 0x005EB3A0
    void SetStateAfterYesNo(CWideString question, CWideString yes, ETavernGameStates yesState, CWideString no,
                            ETavernGameStates noState);   // 0x005EAF30
    void ChangeBet(bool increase);                      // 0x005EB810
    void SetState(ETavernGameStates state);             // 0x005E9B20
    __forceinline void AnswerQuestion(ETavernGameStates state, CProcessedInput& processed)
    {
        SetState(state);
        YesState = TAVERN_GAME_STATE_NONE;
        NoState = TAVERN_GAME_STATE_NONE;
        AltState = TAVERN_GAME_STATE_NONE;
        g_pPlayerGui->Subtitles.Clear(false);
        processed.SetHandled();
    }
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessTavernGame* input);
};

void CTCTavernGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessTavernGame* input)
{
    CDefPointer def;
    GetDef(&def);
    switch (TavernGameState) {
    case 5:
        if (input->IsEventButtonPressed(event) &&
            (input->IsEventGameAction(0x18, event) || input->IsEventGameAction(0x53, event))) {
            GameFinished = true;
            GameWon = false;
            SetStateAfterYesNo(GetTextWithNarrator(def->StartQuestionText), GetText(def->StartYesText), TAVERN_GAME_STATE_6,
                               GetText(def->StartNoText), TavernGameState);
            processed.SetHandled();
        }
        break;
    case 0x2c:
        if (input->IsEventButtonPressed(event) &&
            (input->IsEventGameAction(0x14, event) || input->IsEventGameAction(6, event)))
            AnswerQuestion(YesState, processed);
        break;
    case 0x2d:
        if (YesState == 5 && input->IsEventGameAction(0x72, event) && event.GetType() == 2) {
            C2DVector stick;
            input->GetEventGameActionVector(0x72, event, stick);
            if (stick.Y < -0.2f)
                ChangeBet(false);
            else if (stick.Y > 0.2f)
                ChangeBet(true);
            if (stick.X < -0.2f)
                BaseBet = def->MinBet;
            else if (stick.X > 0.2f)
                BaseBet = LastBet;
            if (BaseBet > CurrentMoney)
                BaseBet = CurrentMoney ? CurrentMoney : def->MinBet;
            if (BaseBet > def->MaxBet)
                BaseBet = def->MaxBet;
            processed.SetHandled();
        }
        if (input->IsEventButtonPressed(event)) {
            if ((BaseBet != 0 || YesState != 5) && input->IsEventGameAction(0x16, event))
                AnswerQuestion(YesState, processed);
            else if (input->IsEventGameAction(0x17, event))
                AnswerQuestion(NoState, processed);
        }
        break;
    case 0x2e:
        if (input->IsEventButtonPressed(event)) {
            if (input->IsEventGameAction(0x16, event))
                AnswerQuestion(YesState, processed);
            else if (input->IsEventGameAction(0x17, event))
                AnswerQuestion(NoState, processed);
            else if (input->IsEventGameAction(0x18, event))
                AnswerQuestion(AltState, processed);
        }
        break;
    case 0x2f:
        processed.SetHandled();
        break;
    }
    if (TavernGameState == 0xd && input->IsEventButtonPressed(event) && input->IsEventGameAction(0x16, event)) {
        SetState(NextTavernGameState);
        processed.SetHandled();
    }
}
