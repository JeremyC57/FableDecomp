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