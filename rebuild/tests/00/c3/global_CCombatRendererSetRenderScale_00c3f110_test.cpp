// 00c3f110 CCombatRenderer_SetRenderScale
// Flags: /O2 /Oy /W3
// Leaf: SetRenderScale
namespace X_00c3f110{
// Layout: interface methods identified by virtual slots only. Object +20 is current setting; original names remain unknown.
struct ScaleController{virtual void Slot0()=0;virtual void Recreate()=0;unsigned char Reserved[28];unsigned Setting;};
struct ScaleResource{virtual void Slot0()=0;virtual void Slot1()=0;virtual void Slot2()=0;virtual bool Create(unsigned,void*)=0;virtual bool CreateSecondary(void*)=0;virtual void Release()=0;};
extern unsigned CurrentSetting;extern void*Device;
// Layout: retail receiver +58 controller, +180/+184 resources, +1b8 requested mode, +1bc actual setting. Manifest class/method are descriptive placeholders; no active header exists.
struct CCombatRenderer{unsigned char Prefix[0x58];ScaleController*Controller;unsigned char Middle[0x124];ScaleResource*Primary;ScaleResource*Secondary;unsigned char Tail[0x30];unsigned Mode,Setting;bool SetRenderScale(unsigned);};
bool CCombatRenderer::SetRenderScale(unsigned mode){
 unsigned setting;
 switch(mode){
 case 1:Mode=1;setting=2;break;
 case 4:Mode=4;setting=2;break;
 case 8:Mode=8;setting=4;break;
 case 64:case 128:Mode=64;setting=6;break;
 default:Mode=64;setting=6;break;
 }
 if(Setting!=setting){Setting=setting;CurrentSetting=setting;ScaleController*controller=Controller;controller->Setting=setting;controller->Recreate();Primary->Release();
 if(Secondary){Secondary->Release();if(!Secondary->CreateSecondary(Device))return false;}
 return Primary->Create(setting,Device);
 }
 return true;
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
namespace X_00c3f110{unsigned CurrentSetting;void*Device;
struct ControllerFixture:ScaleController{unsigned calls;ControllerFixture():calls(0){}void Slot0(){}void Recreate(){++calls;}};
struct ResourceFixture:ScaleResource{unsigned released,created,secondary,lastSetting;void*lastDevice;bool result;ResourceFixture():released(0),created(0),secondary(0),lastSetting(0),lastDevice(0),result(true){}void Slot0(){}void Slot1(){}void Slot2(){}bool Create(unsigned n,void*d){++created;lastSetting=n;lastDevice=d;return result;}bool CreateSecondary(void*d){++secondary;lastDevice=d;return result;}void Release(){++released;}};
}
int main(){using namespace X_00c3f110;assert(offsetof(CCombatRenderer,Mode)==0x1b8&&offsetof(ScaleController,Setting)==0x20);unsigned modes[]={0,1,2,4,8,64,128,129,0xffffffff};for(unsigned i=0;i<sizeof(modes)/sizeof(modes[0]);++i){CCombatRenderer r;memset(&r,0,sizeof(r));ControllerFixture c;ResourceFixture primary,secondary;r.Controller=&c;r.Primary=&primary;r.Secondary=&secondary;int device;Device=&device;assert(r.SetRenderScale(modes[i]));unsigned mode=modes[i]==1?1:modes[i]==4?4:modes[i]==8?8:64;unsigned setting=mode<=4?2:mode==8?4:6;assert(r.Mode==mode&&r.Setting==setting&&CurrentSetting==setting&&c.Setting==setting&&c.calls==1);assert(primary.created==1&&primary.released==1&&secondary.secondary==1&&secondary.released==1&&primary.lastDevice==Device&&primary.lastSetting==setting);assert(r.SetRenderScale(modes[i])&&primary.created==1);r.Setting=0;secondary.result=false;assert(!r.SetRenderScale(modes[i])&&primary.created==1);r.Setting=0;r.Secondary=0;primary.result=false;assert(!r.SetRenderScale(modes[i])&&primary.created==2);}puts("00c3f110_TEST PASS");return 0;}