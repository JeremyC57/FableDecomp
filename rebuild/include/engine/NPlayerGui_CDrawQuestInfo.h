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
enum EMiniGameType {MINIGAME_NONE=0, MINIGAME_FISHING=1, MINIGAME_DIGGING=2, MINIGAME_PICKPOCKET=3, MINIGAME_PICKLOCK=4, MINIGAME_STEAL=5, MINIGAME_TROPHY=6, MINIGAME_ORACLE=7};
namespace NPlayerGui {
// Layout: donor CDrawQuestInfo116 shrinks to92 with retail vector/list sizes.
// Native nested element constructor00656caa and DrawAt00651273 prove40-byte tail.
class CDrawQuestInfo : public CDrawBase {
public:
    enum EElementType {QUEST_INFO_ELEMENT_NONE=0, QUEST_INFO_ELEMENT_BAR=1, QUEST_INFO_ELEMENT_TIMER=2, QUEST_INFO_ELEMENT_COUNTER=3, QUEST_INFO_ELEMENT_COUNTER_LIST=4, QUEST_INFO_ELEMENT_COUNTER_TICK=5, QUEST_INFO_ELEMENT_MINIGAME_BAR=6};
    class CQuestInfoElement {
    public:
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
private:
    unsigned char GuiMode[4];
    float FadeInTimer,MiniModeTimer;
    bool MiniMode;
    long FramesUntilMiniMode,MiniGameElementIndex;
    std::list<long> OracleSymbols;
    std::vector<CEnginePrimitiveHandle> OraclePrimitiveHandles;
    CWideString Name,Text;
    std::list<CCountedPointer<CQuestInfoElement> > Elements;
};
FABLE_STATIC_ASSERT(sizeof(CDrawQuestInfo::CQuestInfoElementMiniGameBar)==0x40);
FABLE_STATIC_ASSERT(sizeof(CDrawQuestInfo::CQuestInfoElementBar)==0x58);
FABLE_STATIC_ASSERT(sizeof(CDrawQuestInfo)==0x5c);
}
#endif
