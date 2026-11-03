#ifndef FABLE_ENGINE_CBANKSTATEBLOCK_RENDER2D_H
#define FABLE_ENGINE_CBANKSTATEBLOCK_RENDER2D_H
#include "rebuild_abi.h"

// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CBankStateBlock {
public: CBankStateBlock(const CBankStateBlock *);
public: CBankStateBlock();
public: virtual ~CBankStateBlock();
public: virtual void GetDefault(CBankStateBlock &)=0;
public: void Reset(const CBankStateBlock *);
public: virtual unsigned long CreateCRC()=0;
protected: unsigned long CalcCRC(void *,unsigned long);
protected: virtual void Set(const CBankStateBlock *)=0;
};
#endif
