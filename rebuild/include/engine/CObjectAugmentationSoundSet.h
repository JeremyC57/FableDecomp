#ifndef FABLE_ENGINE_COBJECTAUGMENTATIONSOUNDSET_H
#define FABLE_ENGINE_COBJECTAUGMENTATIONSOUNDSET_H
#include "fable_string.h"
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
class CObjectAugmentationSoundSet{
public:
#ifndef FABLE_RETAIL_COBJECTAUGMENTATIONSOUNDSET_IMPLICIT_COPY
 CObjectAugmentationSoundSet(const CObjectAugmentationSoundSet & p0);
#else
 // NF57 opt-in (q10438, rows 0x00769980 / 0x0076A110): retail copies the sound set inline, memberwise (Played byte, the
 // CCharString copy ctor 0x0099EC30, Handle dword) inside STLport's _M_create_node; the out-of-line declaration above would
 // make it a call. The copy constructor is the implicit one in those TUs.
#endif
 CObjectAugmentationSoundSet();
 bool Played;
 CCharString Sound;
 unsigned long Handle;
 ~CObjectAugmentationSoundSet();
 CObjectAugmentationSoundSet & operator=(const CObjectAugmentationSoundSet & p0);
};
FABLE_STATIC_ASSERT(sizeof(CObjectAugmentationSoundSet)==12);
#endif
