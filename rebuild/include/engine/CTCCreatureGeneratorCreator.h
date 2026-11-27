#ifndef FABLE_ENGINE_CTCCREATUREGENERATORCREATOR_H
#define FABLE_ENGINE_CTCCREATUREGENERATORCREATOR_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#pragma pack(push,1)
class CTCCreatureGeneratorCreator:public CTCBase {
public:
    virtual void FrameUpdate();
protected:
    bool IsGeneratorAlive() const;
private:
    long FrameToRegenerate;
    bool Generated;
    CIntelligentPointer<CThing> PGenerator;
    unsigned char _pad19[7];
    unsigned __int64 GeneratorUID;
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCCreatureGeneratorCreator)==0x28);
#endif
