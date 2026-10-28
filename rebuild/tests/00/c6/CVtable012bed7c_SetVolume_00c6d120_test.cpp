#include <stdio.h>
// CVtable_012bed7c::SetVolume(float value,unsigned long unused,bool alternate) -- Rebuild mixer gain only when the volume changes.
// Leaf: ?SetVolume@CVtable_012bed7c@@UAEXMK_N@Z
// Identity: own table012bed7c slot3 and inherited float32; actual helper00c6cf40 takes bool, stores flag36, rebuilds gain records under lock. Class and method names descriptive; unused parameter type unresolved.
// Layout: original descriptive libpk CPKObject root (vtable012b5868) and CVtable_012beda4 base from constructor00c6e6a0/destructor00c6e630. Critical-section holder is a member with own vptr+4 and24 bytes at+8, volume32 and flag36; inherited base40. No shared headers. Slot3 has one float, an unused four-byte parameter of unknown original type, and bool consumed by actual rebuild helper. Unused parameter is represented as a word, no original signature claim.
struct CPKObject{virtual ~CPKObject(){};};
struct CCriticalSectionOwner_012B8FC8{virtual void UnusedSlot0(){}unsigned char CriticalSection[24];};
struct CVtable_012beda4:CPKObject{CCriticalSectionOwner_012B8FC8 Lock;float Volume;unsigned char Alternate;virtual void UnusedSlot1(){}virtual void UnusedSlot2(){}virtual void SetVolume(float,unsigned long,bool)=0;};
struct CVtable_012bed7c:CVtable_012beda4{void RebuildGain(bool);virtual void SetVolume(float,unsigned long,bool);};
void CVtable_012bed7c::SetVolume(float value,unsigned long,bool alternate){if(Volume!=value){Volume=value;RebuildGain(alternate);}}
static int calls,errors;static CVtable_012bed7c* expected;static float expectedVolume;static bool observed;void CVtable_012bed7c::RebuildGain(bool alternate){++calls;if(this!=expected||Volume!=expectedVolume)++errors;observed=alternate;}
int main(){CVtable_012bed7c object;expected=&object;object.Volume=1.5f;object.Alternate=7;object.SetVolume(1.5f,0,true);bool ok=!calls;expectedVolume=-2;object.SetVolume(-2,123,true);ok=ok&&calls==1&&observed&&object.Volume==-2&&!errors;expectedVolume=0;object.SetVolume(0,456,false);ok=ok&&calls==2&&!observed&&object.Volume==0&&!errors;object.SetVolume(-0.0f,789,true);ok=ok&&calls==2&&object.Alternate==7;puts(ok?"00c6d120_TEST PASS":"00c6d120_TEST FAIL");return ok?0:1;}