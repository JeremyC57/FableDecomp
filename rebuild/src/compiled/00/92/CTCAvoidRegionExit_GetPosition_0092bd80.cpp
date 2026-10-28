// CTCAvoidRegionExit::GetPosition() -- the entrance marker's position while the marker is set and alive, else the zero vector
// Leaf: ?GetPosition@CTCAvoidRegionExit@@QAE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092BD80, length 107 bytes. Returns C3DVector through the hidden pointer; `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3: the inline HasPosition() test, then the position fetch),
// CThing::GetPosition @ 0x004C73D0 (const C3DVector&; catalog "CVertexBufferWin32::DoSizeof").
// Shape: one conditional expression `HasPosition() ? marker->GetPosition() : C3DVector(0,0,0)`: both arms yield an address in eax
// (the reference, or a frame temporary at esp+4) and one shared three-dword copy into the result through edx (the memberwise
// user copy ctor; the implicit copy goes through esi).
// Layout: PDB CTCAvoidRegionExit (0x20): EntranceMarker +0xC, ExitMarker +0x14 (CIntelligentPointer<CThing>), NextFrameCheck +0x1C;
// retail CThing Dead = bit 0 of +0x91.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPos() const;    // 0x004C73D0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCAvoidRegionExit : public CTCBase {
public:
    CIntelligentPointer<CThing> EntranceMarker;     // +0x0c CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> ExitMarker;         // +0x14
    unsigned long NextFrameCheck;               // +0x1c
    bool HasPosition() { return EntranceMarker.GetPItem() && !EntranceMarker.GetPItem()->IsDead(); }
    C3DVector GetPosition();
};

C3DVector CTCAvoidRegionExit::GetPosition()
{
    return HasPosition() ? EntranceMarker.GetPItem()->GetPos() : C3DVector(0.0f, 0.0f, 0.0f);
}