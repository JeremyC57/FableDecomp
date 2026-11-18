#ifndef FABLE_ENGINE_CTCSKELETALMORPH_H
#define FABLE_ENGINE_CTCSKELETALMORPH_H
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"

class C3DSkeletalMorph;

// Ego_d class record 0x2C6DD and Ego_r class layout. The STLport array
// places PSkeletalMorph at +0x1C, as native 0x00786870 confirms.
class CTCSkeletalMorph : public CTCBase
{
public:
    virtual ~CTCSkeletalMorph();
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType()
    {
        return TCI_SKELETAL_MORPH;
    }
    const CCountedPointer<C3DSkeletalMorph> GetPSkeletalMorph() const;
private:
    class CSkeletalMorph;
    bool Changed;
    CArray<CSkeletalMorph> Morphs;
    CCountedPointer<C3DSkeletalMorph> PSkeletalMorph;
};
FABLE_STATIC_ASSERT(sizeof(CTCSkeletalMorph) == 0x24);
#endif
