#ifndef FABLE_ENGINE_CTCEMOTEICON_H
#define FABLE_ENGINE_CTCEMOTEICON_H
#include "engine/CTCBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "fable_string.h"
namespace NTCMessages {class CMessageClearPrimitives;}
// Layout: original Ego2d5a2/FW complete48-byte owner; native fields12..40.
class CTCEmoteIcon:public CTCBase {
public:
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
 enum EEmotePriority { EMOTE_PRIORITY_DEBUG_HIGH=0,EMOTE_PRIORITY_FOLLOWING=1,EMOTE_PRIORITY_TROPHY_MINIGAME=2,EMOTE_PRIORITY_WIFE=3,EMOTE_PRIORITY_ATTRACTION=4,EMOTE_PRIORITY_EXPRESSION=5,EMOTE_PRIORITY_BRIBE=6,EMOTE_PRIORITY_DEBUG_LOW=7,EMOTE_PRIORITY_NONE=8 };
 enum EEmoteType;
#elif defined(_MSC_VER)
 enum EEmotePriority;
 enum EEmoteType;
#else
 enum EEmotePriority{};
 enum EEmoteType{};
#endif
public: void SetEmoteText(CTCEmoteIcon::EEmotePriority p0,const CWideString & p1,float p2,float p3);
public: void SetEmoteIcon(CTCEmoteIcon::EEmotePriority p0,long p1,float p2,float p3,unsigned char p4);
public: virtual void OnCreate();
public: virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0);
public: virtual bool HasFrameUpdate() const;
public: virtual void FrameUpdate();
public: virtual void HandleMessage(const NTCMessages::CMessageBase & p0);
public: void CatchMessage(const NTCMessages::CMessageClearPrimitives & p0);
public: bool IsActive() const;
public: bool IsText() const;
public: bool IsIcon() const;
public: const CWideString & GetText() const;
public: long GetIcon() const;
public: void SetReplaceable(bool p0);
public: bool IsReplaceable() const;
public: void SetActiveTimer(float p0);
public: void SetAsEnabled(bool p0);
public: void SetAsDrawable(bool p0);
public: void ClearPrimitives();
private: long ActiveTimer;
private: CWideString EmoteText;
private: long EmoteIcon;
private: float EmoteScale;
private: CTCEmoteIcon::EEmoteType EmoteType;
private: CTCEmoteIcon::EEmotePriority EmotePriority;
private: unsigned char EmoteAlpha;
private: bool Replaceable;
private: bool Enabled;
private: CEnginePrimitiveHandle PrimitiveHandle;
private: void DrawEmoteTypeText(const CEnginePrimitivePrepareInfo & p0) const;
private: void DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo & p0);
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public: static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_EMOTEICON;}
#else
public: static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
#endif
public: virtual ETCInterfaceType GetInterfaceType() const;
public: static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
public: static CCharString FABLE_FASTCALL StatGetClassTypeName();
public: virtual CCharString GetClassTypeName() const;
public: CTCEmoteIcon(const CTCEmoteIcon & p0);
private: CTCEmoteIcon(CThing & p0);
public: virtual ~CTCEmoteIcon();
public: CTCEmoteIcon & operator=(const CTCEmoteIcon & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCEmoteIcon)==48);
#endif
