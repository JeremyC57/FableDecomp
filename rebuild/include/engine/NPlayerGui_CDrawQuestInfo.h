#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWQUESTINFO_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWQUESTINFO_H
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
class CThingPhysical;
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
class CCounter;
#endif
class C2DVector; class CEnginePrimitivePrepareInfo;
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_DRAWMINIGAMEINFO
class C2DBoxF;
#endif
enum EMiniGameType {MINIGAME_NONE=0, MINIGAME_FISHING=1, MINIGAME_DIGGING=2, MINIGAME_PICKPOCKET=3, MINIGAME_PICKLOCK=4, MINIGAME_STEAL=5, MINIGAME_TROPHY=6, MINIGAME_ORACLE=7};
namespace NPlayerGui {
// Layout: donor CDrawQuestInfo116 shrinks to92 with retail vector/list sizes.
// Native nested element constructor00656caa and DrawAt00651273 prove40-byte tail.
class CDrawQuestInfo : public CDrawBase {
public:
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_CTOR_PASS23
// Both original PDBs, public original constructor; fixture normal lifetimes.
 CDrawQuestInfo(CPlayerGui&,std::vector<CDrawBase*>&);
#endif
    enum EElementType {QUEST_INFO_ELEMENT_NONE=0, QUEST_INFO_ELEMENT_BAR=1, QUEST_INFO_ELEMENT_TIMER=2, QUEST_INFO_ELEMENT_COUNTER=3, QUEST_INFO_ELEMENT_COUNTER_LIST=4, QUEST_INFO_ELEMENT_COUNTER_TICK=5, QUEST_INFO_ELEMENT_MINIGAME_BAR=6};
    class CQuestInfoElement {
    public:
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_CTOR_PASS23
        // Both original PDBs: user constructor takes Scale.
        CQuestInfoElement(float);
#endif
        virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&)=0;
        virtual bool Update();
        virtual void Clear();
        virtual EElementType GetType() const=0;
        long ID,ScriptID;
        float Scale;
        std::vector<CEnginePrimitiveHandle> PrimitiveHandles;
    };
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
    // Retail opt-in (Codex2026-11-10): both PDB nested class types, native fields1c..28.
    class CQuestInfoElementCounter : public CQuestInfoElement {
    public:
        virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
        virtual EElementType GetType()const;
        const class ::CCounter* PCounter;
        long Graphic,Value,MaxValue;
    };
#endif
    // Ego_r donor Value32..PThing61, native0064fdb4 Value1c..PThing39.
    class CQuestInfoElementBar : public CQuestInfoElement {
    public:
        virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
        virtual bool Update();
        virtual EElementType GetType() const;
        float Value,MaxValue,MinValue;
        CRGBColour Colour,FadeColour;
        long Graphic1,Graphic2;
        bool ThingValid;
        CIntelligentPointer<const CThingPhysical> PThing;
        // Native constructor00656b91/94/97 initializes UID48/4c and map50.
#ifdef _MSC_VER
        unsigned __int64 ThingUID;
#else
        unsigned long long ThingUID;
#endif
        long ThingMapNumber;
    };
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
    // Per-TU opt-in (agentFAM01 2026-11-11, rows 0x00654B4F..0x00654C8F): the other PDB nested element classes (FableWin.pdb
    // CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer / CounterList / Tick> members; the converting copy ctors
    // ??$?0VCQuestInfoElementTimer@CDrawQuestInfo@NPlayerGui@@@?$CCountedPointer@VCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@@@...).
    // Members are Ego_r.pdb's (Timer: TimerIndex, Graphic; CounterList: Graphic, Value, MaxValue; Tick: Graphic, Text, Ticked); single
    // inheritance from CQuestInfoElement at +0 (the retail converting ctors copy the pointer unadjusted). Default path unchanged.
    class CQuestInfoElementTimer : public CQuestInfoElement {
    public:
        virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
        virtual EElementType GetType() const;
        long TimerIndex,Graphic;
    };
    class CQuestInfoElementCounterList : public CQuestInfoElement {
    public:
        virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
        virtual EElementType GetType() const;
        long Graphic,Value,MaxValue;
    };
    class CQuestInfoElementTick : public CQuestInfoElement {
    public:
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_CTOR_PASS23
        // Both original public overloads; graphic constructor native00656c3c.
        CQuestInfoElementTick(long,bool,float);
        CQuestInfoElementTick(const CWideString&,bool,float);
#endif
        virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
        virtual EElementType GetType() const;
        long Graphic;
        CWideString Text;
        bool Ticked;
    };
#endif
    class CQuestInfoElementMiniGameBar : public CQuestInfoElement {
    public:
        virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
        virtual bool Update();
        virtual EElementType GetType() const;
        EMiniGameType Type;
        float Percentage;
        CRGBColour Colour,FadeColour;
        bool Updatable;
        long Graphic;
        std::vector<long> ExtraElements;
    };
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLS
    // Retail opt-in (chunk Q2, row 0x007E6750 CTCOracleMinigame::FrameUpdate): Ego_r.pdb ?OracleSymbolSize@CDrawQuestInfo@NPlayerGui@@QAEJXZ
    // (retail 0x00647308: counts the OracleSymbols list +0x40), ?OracleSymbolPopBack@..QAEXXZ (0x006463E1) / ?OracleSymbolPopFront@..QAEXXZ
    // (0x006463F0: `if (!OracleSymbols.empty())` jmp list::pop_front 0x00655A2E; Ego_r defines PopBack then PopFront, as retail) and
    // ?DisplayMiniGameInfo@..QAEX_NW4EMiniGameType@@J@Z (0x00654593). Declarations only: no layout change.
public:
    long OracleSymbolSize();                                             // 0x00647308
    void OracleSymbolPopBack();                                          // 0x006463E1
    void OracleSymbolPopFront();                                         // 0x006463F0
    void DisplayMiniGameInfo(bool, EMiniGameType, long);                 // 0x00654593
#endif
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_UPDATEMINIGAME
    // Retail opt-in (chunk Q22, row 0x00649E8B CDrawFishingGame::Draw): Ego_r.pdb / FableWin.pdb
    // ?UpdateMiniGame@CDrawQuestInfo@NPlayerGui@@QAEXMJJ@Z (retail 0x00644EFD, landed) and ?UpdateMiniGame@..QAEXABVCRGBColour@@@Z
    // (retail 0x00644F85, landed); parameter names as the landed bodies. Declarations only: no layout change.
public:
    void UpdateMiniGame(float percentage, long value, long maxValue);   // 0x00644EFD
    void UpdateMiniGame(const CRGBColour& colour);                       // 0x00644F85
#endif
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_MINIGAMETYPE
    // Opt-in (chunk 2056 scout, row 0x0048E78E CSubtitleRenderer::DrawButtonGraphics): Ego_r.pdb / FableWin.pdb
    // ?GetMiniGameType@CDrawQuestInfo@NPlayerGui@@QBE?AW4EMiniGameType@@XZ (public const), out of line in retail at 0x00644D54 (the row:
    // `mov ecx,[PlayerGui+1C8h] ; call 0x00644D54 ; cmp eax,1`, 0x0048F081..0x0048F09A; also DrawButtonText 0x0048FE2D). Declaration only.
public:
    EMiniGameType GetMiniGameType() const;                               // 0x00644D54
#endif
private:
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_DRAW
    // Retail opt-in (wfA2, row 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine): Ego_r.pdb / FableWin.pdb
    // ?Draw@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z (retail 0x00654438, landed) and
    // ?DrawMiniGameInfo@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (retail 0x0064D404), both called out of line on
    // PDrawQuestInfo +0x1C8. Declarations only: no layout change.
public:
    void Draw(const CEnginePrimitivePrepareInfo& info, bool);            // 0x00654438
    void DrawMiniGameInfo(const CEnginePrimitivePrepareInfo& info);      // 0x0064D404
private:
#endif
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_DRAWMINIGAMEINFO
    // Opt-in (hand3c, row 0x0064D404 DrawMiniGameInfo): Ego_r.pdb / FableWin.pdb ?DrawBox@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitive
    // PrepareInfo@@ABVC2DBoxF@@@Z (0x00643EB2, landed) and ?GetElementByIndex@CDrawQuestInfo@NPlayerGui@@QBEPAVCQuestInfoElement@12@J@Z
    // (0x00644FC5, landed), both public and called out of line. Declarations only: no layout change.
public:
    void DrawBox(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box);   // 0x00643EB2
    CQuestInfoElement* GetElementByIndex(long index) const;              // 0x00644FC5
private:
#endif
#ifdef FABLE_RETAIL_CDRAWQUESTINFO_ADDELEMENT
    void AddElement(CCountedPointer<CQuestInfoElement> pelement);
#endif
    unsigned char GuiMode[4];
    float FadeInTimer,MiniModeTimer;
    bool MiniMode;
    long FramesUntilMiniMode,MiniGameElementIndex;
    std::list<long> OracleSymbols;
    std::vector<CEnginePrimitiveHandle> OraclePrimitiveHandles;
    CWideString Name,Text;
    std::list<CCountedPointer<CQuestInfoElement> > Elements;
};
FABLE_STATIC_ASSERT_AT(131, sizeof(CDrawQuestInfo::CQuestInfoElementMiniGameBar)==0x40);
FABLE_STATIC_ASSERT_AT(132, sizeof(CDrawQuestInfo::CQuestInfoElementBar)==0x58);
FABLE_STATIC_ASSERT_AT(133, sizeof(CDrawQuestInfo)==0x5c);
}
#endif
