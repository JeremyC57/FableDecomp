#ifndef FABLE_ENGINE_CSHADERBANKSTATEBLOCK_RENDER2D_H
#define FABLE_ENGINE_CSHADERBANKSTATEBLOCK_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CBankStateBlock.h"
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CShaderBankStateBlock:public CBankStateBlock {public: enum EShaderTarget {SHADER_TARGET_PC=0,SHADER_TARGET_XBOX=1};
struct CStateBlock {
public: CShaderBankStateBlock::EShaderTarget ShaderTarget;
public: bool EnableShaderOptimisation;
};
public: CShaderBankStateBlock::CStateBlock State;
public: virtual void GetDefault(CBankStateBlock &);
public: virtual unsigned long CreateCRC();
protected: virtual void Set(const CBankStateBlock *);
public: virtual ~CShaderBankStateBlock();
};
#endif
