#ifndef FABLE_ENGINE_CANIMINIT_H
#define FABLE_ENGINE_CANIMINIT_H
#include "engine/EAnimUpdateMode.h"
#ifdef FABLE_RETAIL_LOOK_HEAD_ANIM
// Layout: original Ego1f467/FW21078,44 bytes; complete fields retained.
class CAnimFrameInfo {public:
 float Current,Last;
 CAnimFrameInfo():Current(0.0f),Last(0.0f){}
 CAnimFrameInfo(float value):Current(value),Last(value){}
 CAnimFrameInfo(float current,float last):Current(current),Last(last){}
};
class CAnimInit {public:
 CAnimInit(){}
#ifdef FABLE_RETAIL_CANIMINIT_STRAFE_CALLED_PASS24
 CAnimInit(unsigned char type,unsigned long type_flags,long anim,EAnimUpdateMode mode,bool looping,unsigned long flags,long priority);
#else
 CAnimInit(unsigned char type,unsigned long type_flags,long anim,EAnimUpdateMode mode,bool looping,unsigned long flags,long priority)
 {Init(type,type_flags,anim,mode,looping,flags,priority);}
#endif
 void Init(unsigned char,unsigned long,long,EAnimUpdateMode,bool,unsigned long,long);
 void Clear();
 EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;
};
#elif defined(FABLE_RETAIL_CANIMINIT_CTORS)
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
#ifdef FABLE_RETAIL_CANIMINIT_CTOR_OUTOFLINE
 // Nested opt-in (agentNF54, rows 0x005B33F4 / 0x00728FF0 / 0x00862780): the argument constructor (FableWin.pdb
 // ??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z) out of line, as retail's size-optimised TU emits it at 0x005B33F4 (Frame zeroed,
 // then Init with the seven arguments re-pushed) and its callers call it. Declaration only; the body is that row's.
 CAnimInit(unsigned char type,unsigned long typeFlags,long anim,EAnimUpdateMode updateMode,bool looping,unsigned long flags,long priority); // 0x005B33F4
#else
 CAnimInit(unsigned char type,unsigned long typeFlags,long anim,EAnimUpdateMode updateMode,bool looping,unsigned long flags,long priority)
 {Init(type,typeFlags,anim,updateMode,looping,flags,priority);}
#endif
 void Init(unsigned char type,unsigned long typeFlags,long anim,EAnimUpdateMode updateMode,bool looping,unsigned long flags,long priority); // 0x0070C050
 EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;};
#else
class CAnimFrameInfo{public:float Current,Last;};
class CAnimInit{public:EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;};
#endif
#endif
