// CGamePlayerInterface::GetMouseMovement(CInputEvent*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004458F0, oracle length 40 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov ecx,[esp+8] ; sub esp,0Ch ; lea eax,[esp] ; push eax ; call <event movement> ; fld [esp] ; mov eax,[esp+10h] ; fstp [eax] ; fld [esp+4] ; fchs ; fstp [eax+4] ; add esp,0Ch ; ret 8
// `this` is unused. The input event's movement comes back as a 12-byte vector (0x00A043D0,
// __fastcall on the event, hidden result pointer) and is turned into screen-space mouse movement
// by negating Y. C2DVector is returned through the caller's hidden pointer (first stack slot; the
// event pointer is the second).

struct C3DVector {
    float X, Y, Z;
    C3DVector();
};

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

struct CInputEventCalls {
    C3DVector GetMovement();        // 0x00A043D0
};

struct CGamePlayerInterface_Methods {
    C2DVector GetMouseMovement(CInputEventCalls* event);
};

C2DVector CGamePlayerInterface_Methods::GetMouseMovement(CInputEventCalls* event)
{
    C3DVector movement = event->GetMovement();
    return C2DVector(movement.X, -movement.Y);
}