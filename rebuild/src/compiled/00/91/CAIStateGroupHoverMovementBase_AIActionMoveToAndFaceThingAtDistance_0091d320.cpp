// CAIStateGroup_HoverMovementBase::AIAction_MoveToAndFaceThingAtDistance(EAIActionRepeat, CIntelligentPointer<CThingCreatureBase>&, float, bool&, EMovementType) -- move to the point `distance` out from the thing along the thing-to-creature direction (flattened), facing the thing; once (with movement 8) or repeated while the move reports 4; 3 for a dead/missing thing or another repeat mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091D320, length 491 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, C3DVector::AccurateNormalise @ 0x00A14440,
// own virtual slot 22 (+0x58) AIAction_MoveToPosAndFacePos(EAIActionRepeat, const C3DVector&, const C3DVector&, bool&,
// EMovementType) (FableWin.pdb mangled names for both prototypes).
// Shape: the alive test is an inline bool helper (a plain `GetPItem() == 0 || Dead` early return is laid out at the end
// with the default return; the helper keeps it inline after the test, which also puts the loop top on a 16-byte boundary
// behind `jmp`/`lea ecx,[ecx]`); the two repeat modes are an if chain (a switch dispatches with dec/dec); the loop is
// for(;;) returning on anything but 4.
// Layout: CAIStateGroupBase PBrain +0x04, CAIBrain PCreature +0x18 (Ego_r.pdb); CThing Dead = bit 0 of +0x91 (retail offset).
typedef long EAIActionRepeat;
typedef long EMovementType;
typedef long EAIActionReturn;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& AccurateNormalise();                           // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: Dead
    const C3DVector& GetPosition();             // 0x004C73D0
};
class CThingCreatureBase : public CThing { public: };
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: 
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IsThingAlive(const CIntelligentPointer<CThingCreatureBase>& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroup_HoverMovementBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual EAIActionReturn AIAction_MoveToPosAndFacePos(EAIActionRepeat repeat, const C3DVector& pos, const C3DVector& facePos,
                                                         bool& arrived, EMovementType movement);   // slot 22 (+0x58)
    CAIBrain* PBrain;                           // +0x04
    CThing* GetCreature() const { return PBrain->PCreature; }
    EAIActionReturn AIAction_MoveToAndFaceThingAtDistance(EAIActionRepeat repeat, CIntelligentPointer<CThingCreatureBase>& thing,
                                                          float distance, bool& arrived, EMovementType movement);
};

EAIActionReturn CAIStateGroup_HoverMovementBase::AIAction_MoveToAndFaceThingAtDistance(EAIActionRepeat repeat, CIntelligentPointer<CThingCreatureBase>& thing,
                                                                                       float distance, bool& arrived, EMovementType movement)
{
    if (!IsThingAlive(thing))
        return 3;
    if (repeat == 2) {
        CThing* me = GetCreature();
        const C3DVector& thingPos = thing.GetPItem()->GetPosition();
        const C3DVector& myPos = me->GetPosition();
        C3DVector dir(myPos.X - thingPos.X, myPos.Y - thingPos.Y, 0.0f);
        dir.AccurateNormalise();
        C3DVector offset = dir * distance;
        C3DVector target = thing.GetPItem()->GetPosition() + offset;
        return AIAction_MoveToPosAndFacePos(2, target, thing.GetPItem()->GetPosition(), arrived, 8);
    }
    if (repeat == 1) {
        for (;;) {
            CThing* me = GetCreature();
            const C3DVector& thingPos = thing.GetPItem()->GetPosition();
            const C3DVector& myPos = me->GetPosition();
            C3DVector dir(myPos.X - thingPos.X, myPos.Y - thingPos.Y, 0.0f);
            dir.AccurateNormalise();
            C3DVector offset = dir * distance;
            C3DVector target = thing.GetPItem()->GetPosition() + offset;
            EAIActionReturn result = AIAction_MoveToPosAndFacePos(1, target, thing.GetPItem()->GetPosition(), arrived, movement);
            if (result != 4)
                return result;
        }
    }
    return 3;
}