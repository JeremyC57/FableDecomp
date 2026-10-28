// CAIStateGroup_HoverAttackArea::IsRunnable() -- runnable when there is an enemy, the base test passes and the enemy is within 20 units (2D) of the start position
// Leaf: ?IsRunnable@CAIStateGroup_HoverAttackArea@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A2C0, length 159 bytes. Vtable slot 11 of
// CAIStateGroup_HoverAttackArea (0x0128C754).
// Callees: CAIStateGroup_EnemyCreatureBase::GetEnemy @ 0x0091DB50, CBaseIntelligentPointer ctor/SetPItem/
// GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (vtable 0x0123E7D8 =
// CIntelligentPointer<CThingCreatureBase>), virtual slot 15 (+0x3c, a bool test of the enemy base),
// CThing::GetPosition @ 0x004C73D0 (landed name; manifest "CVertexBufferWin32::DoSizeof" is propagated).
// Float constant 0x01294EBC = 20.0f.
//
// Shape: `PTargetThing = CIntelligentPointer<CThingCreatureBase>(enemy)` (stack temporary as in the
// InitStateGroup rows), then a 2D delta built in a NAMED C2DVector local (both differences stay on the FPU
// stack: `fstp st(2) ; fstp st(0)` after the fsqrt), `Length()` = sqrt(x*x + y*y) (the `a + b` form
// squares y first), tested `!(length > 20.0f)` (`test ah,41h ; je` to the shared `xor al,al` exit; a plain
// `<= 20.0f` gives `jp`). The member is assigned through the implicit conversion `PTargetThing = enemy`.
// Tried: explicit temporary, implicit conversion (this), `void operator=` -- all hoist the lea.
// PDB: CAIStateGroup_EnemyCreatureBase (0x1c) holds PTargetThing at +0x14; StartPos (C3DVector) at +0x1c.
// The engine header CAIStateGroup_HoverAttackArea.h is a flattened data-only layout: local model.
// Shape: the CIntelligentPointer operator= names the GetPItem() CALL result (`item = other.GetPItem(); SetPItem(item);`), which
//   keeps `lea edi,[esi+14h]` after the temporary's GetPItem call as retail (the inline expression form hoists it above the call).
extern "C" double sqrt(double);
#pragma intrinsic(sqrt)

class CThingCreatureBase;

class C3DVector { public: float X, Y, Z; };

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float Length() const { return (float)sqrt(X * X + Y * Y); }
};

class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();                         // one vtable per instantiation
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool IsEnemyValid();                // slot 15 (+0x3c)
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer<CThingCreatureBase> PTargetThing;   // +0x14
    CThingCreatureBase* GetEnemy();             // 0x0091DB50
};

struct CAIStateGroup_HoverAttackArea : CAIStateGroup_EnemyCreatureBase {
    C3DVector StartPos;                         // +0x1c
    virtual bool IsRunnable();
};

bool CAIStateGroup_HoverAttackArea::IsRunnable()
{
    CThingCreatureBase* enemy = GetEnemy();
    if (enemy) {
        PTargetThing = enemy;
        if (IsEnemyValid()) {
            const C3DVector* pos = &PTargetThing.GetPItem()->GetPos();
            C2DVector delta(pos->X - StartPos.X, pos->Y - StartPos.Y);
            if (!(delta.Length() > 20.0f))
                return true;
        }
    }
    return false;
}