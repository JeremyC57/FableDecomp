#include <stdio.h>
#include <string.h>
// CInputEvent::SetAsRightMouseButtonPressed(const C2DVector& position,double time) -- Set the mouse button state, position and event times.
// Leaf: ?SetAsRightMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@N@Z
// Identity: Original PDB SetAsRightMouseButtonPressed references and double parameters; mouse device3, own kind10, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
class C2DVector{public:float X,Y;};class C3DVector{public:float X,Y,Z;};
// Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.
struct UInputEventData{unsigned char UnprovenPayload0[12];C3DVector MouseMovement;C2DVector MousePos;};
enum EInputDeviceType{InputDeviceMouseDescriptive=3};enum EInputEventType{InputEventZeroDescriptive=0};
class CInputEvent{public:UInputEventData Data;EInputDeviceType DeviceType;long DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;void SetAsRightMouseButtonPressed(const C2DVector& position,double time);};
void CInputEvent::SetAsRightMouseButtonPressed(const C2DVector& position,double time){DeviceType=InputDeviceMouseDescriptive;Type=(EInputEventType)10;Data.MousePos=position;StartTime=(float)time;CurrentTime=(float)time;}

static bool run(float x,float y,double start,double current){CInputEvent e;memset(&e,0x5a,sizeof(e));e.DeviceNumber=19;e.Data.MouseMovement.X=8.0f;e.Data.MouseMovement.Y=-4.0f;e.Data.MouseMovement.Z=2.0f;C2DVector p;p.X=x;p.Y=y;C3DVector movement;movement.X=0.5f;movement.Y=-1.5f;movement.Z=4.0f;e.SetAsRightMouseButtonPressed(p,start);bool ok=e.DeviceType==(EInputDeviceType)3&&e.Type==(EInputEventType)10&&e.DeviceNumber==19&&e.Data.MousePos.X==x&&e.Data.MousePos.Y==y&&e.StartTime==(float)start&&e.CurrentTime==(float)start&&e.Data.MouseMovement.X==8.0f&&e.Data.MouseMovement.Y==-4.0f&&e.Data.MouseMovement.Z==2.0f;for(unsigned int i=0;i<12;++i)ok=ok&&e.Data.UnprovenPayload0[i]==0x5a; return ok;}
int main(){bool ok=run(3.0f,-2.0f,1.5,4.0)&&run(-0.5f,0.0f,-2.0,0.5);puts(ok?"00a03e40_TEST PASS":"00a03e40_TEST FAIL");return ok?0:1;}