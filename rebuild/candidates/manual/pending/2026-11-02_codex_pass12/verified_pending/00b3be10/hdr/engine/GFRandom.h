#ifndef FABLE_ENGINE_GFRANDOM_H
#define FABLE_ENGINE_GFRANDOM_H
// Original FableWin ?GFROR13@@YIJK@Z and ?GFRandom@@YIKKPAK@Z;
// native00b3be92 recurrence and unsigned zero-range-checked remainder.
long __fastcall GFROR13(unsigned long);
inline unsigned long __fastcall GFRandom(unsigned long range,unsigned long*seed){*seed=(unsigned long)GFROR13(*seed*0x24a1+0x24df);return range?*seed%range:0;}
#endif
