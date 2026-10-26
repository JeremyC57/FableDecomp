// CCreatureAction_BlockBase::GetNextPos() -- the owner's position pushed along the block's movement direction by a decaying amount: pos + dir * 0.2 * exp(-100 * t^2), t = 1 - remaining/total frames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008552F0, length 194 bytes (+ int3 padding), `ret 4` (hidden return-object pointer).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (the owner pointer at +8), CThing::GetPosition 0x004C73D0
// (manifest label "CVertexBufferWin32::DoSizeof" is propagated). Constants 1.0 @ 0x0122DED8, 0.2 @ 0x012305A0, -100.0 @ 0x01275E8C.
// Shape: the frame counts are CCreatureActionBase +0x10 (total) / +0x1c (current); t = 1 - (float)(total - current) / total
// (fild/fidiv); exp is the inline x87 intrinsic (fldl2e/f2xm1/fscale); dir * 0.2 and then * the factor are inline C3DVector
// operators; the sum adds the position on the right.
// Layout: MovementDirection at retail +0x118 (PDB +0x130; the PDB CCombatActionBase is 0xc larger), BlowStrength +0x114.
#include <math.h>
#pragma intrinsic(exp)

class CBaseObject;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

class CThing { public:
    const C3DVector& GetPosition() const;   // 0x004C73D0
};

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

class CCreatureAction_BlockBase { public:
    virtual void v00();
    long _pad04;
    CIntelligentPointer<CThing> Owner;          // +0x08 (the owning creature)
    long TotalFrames;                       // +0x10
    long _pad14[2];
    long CurrentFrame;                      // +0x1c
    unsigned char _pad20[0x114 - 0x20];
    long BlowStrength;                      // +0x114 (PDB +0x12c)
    C3DVector MovementDirection;            // +0x118 (PDB +0x130)
    C3DVector GetNextPos();
};

C3DVector CCreatureAction_BlockBase::GetNextPos()
{
    float t = 1.0f - (float)(TotalFrames - CurrentFrame) / TotalFrames;
    C3DVector dir = MovementDirection * 0.2f;
    return Owner.GetPItem()->GetPosition() + dir * (float)exp(t * t * -100.0f);
}