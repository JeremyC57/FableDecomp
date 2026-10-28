#include <stdio.h>
#include <string.h>
// CInputEvent::SetAsRightMouseButtonHeld(const C2DVector& position,const C3DVector& movement,double start,double current) -- Set the mouse button state, position and event times.
// Leaf: ?SetAsRightMouseButtonHeld@CInputEvent@@QAEXABVC2DVector@@ABVC3DVector@@NN@Z
// Identity: Original PDB SetAsRightMouseButtonHeld references and double parameters; mouse device3, own kind11, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
class C2DVector{public:float X,Y;};class C3DVector{public:float X,Y,Z;};
// Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.
struct UInputEventData{unsigned char UnprovenPayload0[12];C3DVector MouseMovement;C2DVector MousePos;};
enum EInputDeviceType{InputDeviceMouseDescriptive=3};enum EInputEventType{InputEventZeroDescriptive=0};
class CInputEvent{public:UInputEventData Data;EInputDeviceType DeviceType;long DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;void SetAsRightMouseButtonHeld(const C2DVector& position,const C3DVector& movement,double start,double current);};
void CInputEvent::SetAsRightMouseButtonHeld(const C2DVector& position,const C3DVector& movement,double start,double current){DeviceType=InputDeviceMouseDescriptive;Type=(EInputEventType)11;Data.MousePos=position;StartTime=(float)start;CurrentTime=(float)current;*reinterpret_cast<C3DVector*>(Data.UnprovenPayload0)=movement;}

static bool run(float x,float y,double start,double current){CInputEvent e;memset(&e,0x5a,sizeof(e));e.DeviceNumber=19;e.Data.MouseMovement.X=8.0f;e.Data.MouseMovement.Y=-4.0f;e.Data.MouseMovement.Z=2.0f;C2DVector p;p.X=x;p.Y=y;C3DVector movement;movement.X=0.5f;movement.Y=-1.5f;movement.Z=4.0f;e.SetAsRightMouseButtonHeld(p,movement,start,current);bool ok=e.DeviceType==(EInputDeviceType)3&&e.Type==(EInputEventType)11&&e.DeviceNumber==19&&e.Data.MousePos.X==x&&e.Data.MousePos.Y==y&&e.StartTime==(float)start&&e.CurrentTime==(float)current&&e.Data.MouseMovement.X==8.0f&&e.Data.MouseMovement.Y==-4.0f&&e.Data.MouseMovement.Z==2.0f;C3DVector* moved=reinterpret_cast<C3DVector*>(e.Data.UnprovenPayload0);ok=ok&&moved->X==movement.X&&moved->Y==movement.Y&&moved->Z==movement.Z; return ok;}
int main(){bool ok=run(3.0f,-2.0f,1.5,4.0)&&run(-0.5f,0.0f,-2.0,0.5);puts(ok?"00a03e70_TEST PASS":"00a03e70_TEST FAIL");return ok?0:1;}