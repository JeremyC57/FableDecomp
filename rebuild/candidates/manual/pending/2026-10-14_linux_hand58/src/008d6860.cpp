// NCreatureMode::CModeJackOfBladesDeath::OnActivate() -- remember the thing's position (CThing::GetPosition @ 0x004C73D0, a C3DVector reference copied memberwise into +0x74) and clear the counter at +0x80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6860, length 42 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0 (labelled CVertexBufferWin32::DoSizeof, propagated; thiscall returning a const C3DVector&). Vtable slot 5 of CModeJackOfBladesDeath (rtti-vtables.tsv). Shape: no base call, `this` in esi, the vector assigned one dword per register (a user memberwise assignment) with the zero store scheduled between the second and third, `ret`.
namespace NCreatureMode {
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // a user copy constructor; the assignment stays implicit (memberwise)
};

struct CThing {
    const C3DVector& GetPosition();                // 0x004C73D0
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
};

struct CModeJackOfBladesDeath : CModeBase {
    unsigned char _pad08[0x6c];                    // +0x08 .. +0x74
    C3DVector     m_startPosition;                 // +0x74
    long          m_counter;                       // +0x80
    virtual void OnActivate();
};

void CModeJackOfBladesDeath::OnActivate()
{
    m_startPosition = m_pThing->GetPosition();
    m_counter = 0;
}
}
