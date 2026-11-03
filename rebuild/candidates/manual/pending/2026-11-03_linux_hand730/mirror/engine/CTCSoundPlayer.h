#ifndef FABLE_ENGINE_CTCSOUNDPLAYER_H
#define FABLE_ENGINE_CTCSOUNDPLAYER_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThingSoundEmitter;
// Original complete36-byte class and native component16 receiver.
class CTCSoundPlayer : public CTCBase {
public:
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
protected:
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;
    CCharString VoiceName,ExtraTag;
    short AnimCriteriaClipDistance;
    long LastMaterialIndex;
};
FABLE_STATIC_ASSERT(sizeof(CTCSoundPlayer)==36);
#endif
