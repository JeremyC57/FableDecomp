#ifndef FABLE_ENGINE_CANIMINIT_H
#define FABLE_ENGINE_CANIMINIT_H
#include "engine/EAnimUpdateMode.h"
#ifdef FABLE_RETAIL_CANIMINIT_CTORS
// Retail opt-in (chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): the inline constructors of both classes (FableWin.pdb
// ??0CAnimFrameInfo@@QAE@XZ / ??0CAnimFrameInfo@@QAE@MM@Z, ??0CAnimInit@@QAE@XZ, ??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z: the
// constructors only build Frame (retail `fldz ; fstp [init+18h] ; fldz ; fstp [init+1Ch]` at 0x005B3FE5), the argument one then calls Init) and the out-of-line ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z
// (Ego_r.pdb / FableWin.pdb, retail 0x0070C050: type, type flags, anim, update mode, looping, flags, priority). Same layout.
class CAnimFrameInfo{public:float Current,Last;
 CAnimFrameInfo():Current(0.0f),Last(0.0f){}
 CAnimFrameInfo(float current,float last):Current(current),Last(last){}
};
class CAnimInit{public:
 CAnimInit(){}
 CAnimInit(unsigned char type,unsigned long typeFlags,long anim,EAnimUpdateMode updateMode,bool looping,unsigned long flags,long priority)
 {Init(type,typeFlags,anim,updateMode,looping,flags,priority);}
 void Init(unsigned char type,unsigned long typeFlags,long anim,EAnimUpdateMode updateMode,bool looping,unsigned long flags,long priority); // 0x0070C050
 EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;};
#else
class CAnimFrameInfo{public:float Current,Last;};
class CAnimInit{public:EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;};
#endif
#endif
