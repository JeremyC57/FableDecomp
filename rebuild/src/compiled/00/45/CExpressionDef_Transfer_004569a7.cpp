// CExpressionDef::Transfer(CPersistContext&) -- transfers expression definition fields.
// Retail004569a7 length306. Quarantined donor CExpressionDef first member+0x40;
// native+0x3c, all subsequent members -4. Native layout below uses donor names.
#pragma optimize("s",on)
#include <stddef.h>
class CPersistContext { public:
 void TransferDefString(int&); //0045228f
 void TransferFloat(float&); //00431061
 void TransferInt(int&); //00431102
 void TransferDefIndex(int&); //004522f4
 void TransferGameEvent(int&); //00456ad9
 void TransferBool(bool&); //0043314a
 void TransferOpinionDeed(int&); //00456b7d
 void TransferUnsigned(unsigned int&); //00431020
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
 c.TransferDefString(Animation_TablePos);
 c.TransferDefString(AnimationInto_TablePos);
 c.TransferDefString(AnimationLoop_TablePos);
 c.TransferDefString(AnimationOutOf_TablePos);
 c.TransferDefString(AnimationToOther_TablePos);
 c.TransferDefString(AnimationToOtherInto_TablePos);
 c.TransferDefString(AnimationToOtherLoop_TablePos);
 c.TransferDefString(AnimationToOtherOutOf_TablePos);
 c.TransferFloat(SpriteDuration);
 c.TransferInt(SpriteGraphic);
 c.TransferDefString(SpriteOverrideText_TablePos);
 c.TransferDefString(SoundCriteria_TablePos);
 c.TransferDefString(LoopSoundCriteria_TablePos);
 c.TransferDefIndex(DummyObject_Val);
 c.TransferGameEvent(GameEventType);
 c.TransferDefString(ScriptName_TablePos);
 c.TransferBool(ScriptLoadResources);
 c.TransferBool(DeactivateScriptAtEnd);
 c.TransferBool(PerformAfterScript);
 c.TransferOpinionDeed(OpinionDeedType);
 c.TransferBool(Extendable);
 c.TransferBool(SheatheWeapons);
 c.TransferBool(ReplaceHeldItemWhenDone);
 c.TransferUnsigned(TextTag);
 c.TransferExpressionType(ExpressionType);
}