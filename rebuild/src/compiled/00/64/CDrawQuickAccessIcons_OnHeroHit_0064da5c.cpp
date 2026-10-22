#pragma optimize("s",on)
struct OH { char pad[0x2c]; int f2c; char pad2[0x34-0x30]; char f34; };
void __fastcall h_0064da5c(OH* icons);
void __fastcall fn_0064da5c(OH* icons){ h_0064da5c(icons); icons->f2c = -1; icons->f34 = 0; }