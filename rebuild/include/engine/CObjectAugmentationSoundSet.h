#ifndef FABLE_ENGINE_COBJECTAUGMENTATIONSOUNDSET_H
#define FABLE_ENGINE_COBJECTAUGMENTATIONSOUNDSET_H
#include "fable_string.h"
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
class CObjectAugmentationSoundSet{
public:
 CObjectAugmentationSoundSet(const CObjectAugmentationSoundSet & p0);
 CObjectAugmentationSoundSet();
 bool Played;
 CCharString Sound;
 unsigned long Handle;
 ~CObjectAugmentationSoundSet();
 CObjectAugmentationSoundSet & operator=(const CObjectAugmentationSoundSet & p0);
};
FABLE_STATIC_ASSERT(sizeof(CObjectAugmentationSoundSet)==12);
#endif
