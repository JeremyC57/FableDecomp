// Grade: DIFFER(393v395) -- colour-delta channel scheduling only: retail subtracts t.R/t.G with a memory operand (mov al,t ; sub al,c) and keeps four byte registers (push ebx), stores R,G,A,B; ours loads both operands of each channel into registers (no ebx), stores R,G,B,A. The linear-flag push 1/push 0 branches now match (explicit if/else)
// NUISystem::CChangingStateComponent::ProcessChangeState() -- when a state change is pending, start the moves towards the target state's record: a position delta and a zoom delta when they are longer than the epsilon, and a colour delta when any channel differs, each with the update time and the record's linear flag
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052C920, 395 bytes (400 with padding). NUISystem::CSprite vtable 0x0122F5D4 slot 58.
// Callees: virtual slots 32 (+0x80, ChangePositionDelta), 36 (+0x90, ChangeZoomDelta), 39 (+0x9c,
//          ChangeColourDelta) and 135 (+0x21c, GetState(index) -> CUIState*). Float pool 0x0129BA3C (1e-4f).
//
// Shape: GetState is called for the target and then for the current state (the second result is unused,
// as in retail). Each vector delta is computed twice: once in registers for the squared-length test
// (`d.Y*d.Y + d.X*d.X` against eps*eps, both multiplied at run time) and once into the frame temporary
// passed by reference. The colour delta is a per-channel byte difference tested as one dword. The linear flag is
// an explicit if/else around two identical calls (VC cross-jumps them into retail's push 1 / push 0 branches).
// Layout: local model -- retail CComponent / CChangingStateComponent offsets of the landed rows (Position
// +0x34, Zoom +0x5c, Colour +0x84, CurrentState +0x144, TargetState +0x148, UpdateTime +0x150); CUIState
// is the PDB layout (0x24 bytes).
namespace NUISystem {

extern float g_Epsilon;                     // 0x0129BA3C (1e-4f)

struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float GetLengthSquared() const { return Y * Y + X * X; }
};
struct CRGBColour {                         // D3DCOLOR byte order
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour operator-(const CRGBColour& o) const { return CRGBColour(R - o.R, G - o.G, B - o.B, A - o.A); }
};
struct CUIState {                           // PDB 0x24 bytes
    unsigned long StateChangeFlag;          // +0x00
    long          StateChangeType;          // +0x04 EStateChangeType
    C2DVector     Position;                 // +0x08
    C2DVector     Zoom;                     // +0x10
    CRGBColour    Colour;                   // +0x18
    float         UpdateTime;               // +0x1c
    bool          LinearChange;             // +0x20
};
class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool linear);   // slot 32 (+0x80)
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void ChangeZoomDelta(const C2DVector& delta, float time, bool linear);       // slot 36 (+0x90)
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void ChangeColourDelta(const CRGBColour& delta, float time, bool linear);     // slot 39 (+0x9c)
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void ProcessChangeState();                              // slot 58 (+0xe8)
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual void VSlot102();
    virtual void VSlot103();
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void VSlot114();
    virtual void VSlot115();
    virtual void VSlot116();
    virtual void VSlot117();
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void VSlot125();
    virtual void VSlot126();
    virtual void VSlot127();
    virtual void VSlot128();
    virtual void VSlot129();
    virtual void VSlot130();
    virtual void VSlot131();
    virtual void VSlot132();
    virtual void VSlot133();
    virtual void VSlot134();
    virtual CUIState* GetState(unsigned long state);                // slot 135 (+0x21c)
    unsigned char _pad04[0x30];             // +0x04
    C2DVector     Position;                 // +0x34
    unsigned char _pad3c[0x20];             // +0x3c
    C2DVector     Zoom;                     // +0x5c
    unsigned char _pad64[0x20];             // +0x64
    CRGBColour    Colour;                   // +0x84
    unsigned char _pad88[0xac];             // +0x88 .. +0x134
};
class CChangingStateComponent : public CComponent {
public:
    long          StatesBeingDone;          // +0x134
    long          StatesDone;               // +0x138
    void*         StatesToDo;               // +0x13c
    float         ParentUpdateTime;         // +0x140
    unsigned long CurrentState;             // +0x144
    unsigned long TargetState;              // +0x148
    unsigned long RequestedState;           // +0x14c
    float         UpdateTime;               // +0x150
    bool          PreviousUpdateChanged;    // +0x154
    unsigned long PreviousState;            // +0x158
    virtual void ProcessChangeState();
};

void CChangingStateComponent::ProcessChangeState()
{
    if (TargetState == CurrentState)
        return;
    CUIState* target = GetState(TargetState);
    CUIState* current = GetState(CurrentState);
    if (target->StateChangeFlag & 1) {
        if ((target->Position - Position).GetLengthSquared() > g_Epsilon * g_Epsilon)
            if (target->LinearChange)
                ChangePositionDelta(target->Position - Position, UpdateTime, true);
            else
                ChangePositionDelta(target->Position - Position, UpdateTime, false);
    }
    if (target->StateChangeFlag & 4) {
        if ((target->Zoom - Zoom).GetLengthSquared() > g_Epsilon * g_Epsilon)
            if (target->LinearChange)
                ChangeZoomDelta(target->Zoom - Zoom, UpdateTime, true);
            else
                ChangeZoomDelta(target->Zoom - Zoom, UpdateTime, false);
    }
    if (target->StateChangeFlag & 2) {
        CRGBColour delta = target->Colour - Colour;
        if (delta.Colour != 0)
            if (target->LinearChange)
                ChangeColourDelta(delta, UpdateTime, true);
            else
                ChangeColourDelta(delta, UpdateTime, false);
    }
}

}
