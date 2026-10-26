// CExpressionDef::Transfer(CPersistContext&) -- transfers expression definition fields.
// Retail004569a7 length306. Quarantined donor CExpressionDef first member+0x40;
// native+0x3c, all subsequent members -4. Native layout below uses donor names.
#pragma optimize("s",on)
#include <stddef.h>
class CDefString;
enum EOpinionDeedType { };
class CPersistContext { public:
 template <class T> void Transfer(T& value); //0045228f
 void TransferDefIndex(int&); //004522f4
 void TransferGameEvent(int&); //00456ad9
 void TransferExpressionType(int&); //00456c21
};
class CExpressionDef { public:
 unsigned char DefinitionBase[0x3c];
 int Animation_TablePos, AnimationInto_TablePos, AnimationLoop_TablePos, AnimationOutOf_TablePos;
 int AnimationToOther_TablePos, AnimationToOtherInto_TablePos, AnimationToOtherLoop_TablePos, AnimationToOtherOutOf_TablePos;
 float SpriteDuration; int SpriteGraphic, SpriteOverrideText_TablePos, SoundCriteria_TablePos, LoopSoundCriteria_TablePos;
 int DummyObject_Val, GameEventType, ScriptName_TablePos;
 bool ScriptLoadResources, DeactivateScriptAtEnd, PerformAfterScript;
 int OpinionDeedType;
 bool Extendable, SheatheWeapons, ReplaceHeldItemWhenDone;
 unsigned int TextTag;
 int ExpressionType;
 void Transfer(CPersistContext& c);
};
typedef char NativeExpressionSize[(sizeof(CExpressionDef)==0x90)?1:-1];
typedef char NativeExpressionOffset[(offsetof(CExpressionDef,ExpressionType)==0x8c)?1:-1];
void CExpressionDef::Transfer(CPersistContext& c) {
 c.Transfer(*(CDefString*)&(Animation_TablePos));
 c.Transfer(*(CDefString*)&(AnimationInto_TablePos));
 c.Transfer(*(CDefString*)&(AnimationLoop_TablePos));
 c.Transfer(*(CDefString*)&(AnimationOutOf_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOther_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOtherInto_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOtherLoop_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOtherOutOf_TablePos));
 c.Transfer(SpriteDuration);
 c.Transfer(*(long*)&(SpriteGraphic));
 c.Transfer(*(CDefString*)&(SpriteOverrideText_TablePos));
 c.Transfer(*(CDefString*)&(SoundCriteria_TablePos));
 c.Transfer(*(CDefString*)&(LoopSoundCriteria_TablePos));
 c.TransferDefIndex(DummyObject_Val);
 c.TransferGameEvent(GameEventType);
 c.Transfer(*(CDefString*)&(ScriptName_TablePos));
 c.Transfer(ScriptLoadResources);
 c.Transfer(DeactivateScriptAtEnd);
 c.Transfer(PerformAfterScript);
 c.Transfer(*(EOpinionDeedType*)&(OpinionDeedType));
 c.Transfer(Extendable);
 c.Transfer(SheatheWeapons);
 c.Transfer(ReplaceHeldItemWhenDone);
 c.Transfer(*(unsigned long*)&(TextTag));
 c.TransferExpressionType(ExpressionType);
}