// CAIStateGroup_AvoidRegionExit::Update() -- walk back to the stored position, then terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092BC60, length 69 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (12 arguments: repeat
// mode 1, the C3DVector Position (+0x14, engine header) by value, radius 2.0f, &reached,
// movement type 5, true, false, true, no leader, true, 0.0f), CAIStateGroupBase::Terminate
// @ 0x008FCF40 (sets Terminated at +0x10).
// The by-value C3DVector is built in its argument slot (`sub esp,0Ch ; mov eax,esp ; mov [eax]..`)
// because it has a user copy constructor; the `reached` bool is an uninitialised local that VC
// homes in the high byte of the `push ecx` frame slot (`lea eax,[esp+23h]`).
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    void Terminate();                       // 0x008FCF40
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct CAIStateGroup_AvoidRegionExit : CAIStateGroupBase {
    C3DVector Position;                     // +0x14
    virtual void Update();
};

void CAIStateGroup_AvoidRegionExit::Update()
{
    bool reached;
    AIAction_MoveToPosProximity(1, Position, 2.0f, &reached, 5, true, false, true, 0, true, 0.0f);
    Terminate();
}