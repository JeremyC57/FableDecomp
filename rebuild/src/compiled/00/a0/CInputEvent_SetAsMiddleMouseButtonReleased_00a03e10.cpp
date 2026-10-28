// CInputEvent::SetAsMiddleMouseButtonReleased(const C2DVector& position,double start,double current) -- Set the mouse button state, position and event times.
// Leaf: ?SetAsMiddleMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NN@Z
// Identity: Original PDB SetAsMiddleMouseButtonReleased references and double parameters; mouse device3, own kind9, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
class C2DVector{public:float X,Y;};class C3DVector{public:float X,Y,Z;};
// Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.
struct UInputEventData{unsigned char UnprovenPayload0[12];C3DVector MouseMovement;C2DVector MousePos;};
enum EInputDeviceType{InputDeviceMouseDescriptive=3};enum EInputEventType{InputEventZeroDescriptive=0};
class CInputEvent{public:UInputEventData Data;EInputDeviceType DeviceType;long DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;void SetAsMiddleMouseButtonReleased(const C2DVector& position,double start,double current);};
void CInputEvent::SetAsMiddleMouseButtonReleased(const C2DVector& position,double start,double current){DeviceType=InputDeviceMouseDescriptive;Type=(EInputEventType)9;Data.MousePos=position;StartTime=(float)start;CurrentTime=(float)current;}