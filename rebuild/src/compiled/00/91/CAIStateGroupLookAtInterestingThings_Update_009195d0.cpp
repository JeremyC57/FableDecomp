// CAIStateGroup_LookAtInterestingThings::Update() -- pick an interesting thing; while it exists walk to within 6 units of it (avoiding dynamic obstacles as the creature's navigation def says), then keep looking at it until the look ends; an unreachable thing loses its interest component (0xb3); the group terminates when done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009195D0, length 429 bytes (+ int3 padding), plain `ret` (virtual), `and esp,-8`
// frame for the 8-byte intelligent pointer.
// Callees: CAIStateGroup_LookAtInterestingThings::FindInterestingThing @ 0x00919240, CBaseIntelligentPointer ctor @ 0x00A01B10 /
// SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
// CDefClassBase::GetSubDef<CCreatureNavigationDef> @ 0x006CD2F0 (on the creature's +0x70), CThing::GetPosition @ 0x004C73D0,
// CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 / AIAction_LookAtPos @ 0x0097D580 / Terminate @ 0x008FCF40,
// CThing::RemoveTC @ 0x004C9840, the def's virtual slot 1 (release).
// Prototypes: FableWin.pdb ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@
// MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z and ?AIAction_LookAtPos@...@@W4EAIActionRepeat@@VC3DVector@@JM@Z.
// Shape: GetSubDef is a same-TU auto_inline(off) stand-in (accepted precedent 0x00961FB0 / 0x0092C070) so the def stays in
// edi and its count release is inline at each exit (null test only where the def was never dereferenced); the def class is
// held in a local before the call; the look loop uses `== 4 continue; break` so it is not rotated.
// Layout: CCreatureNavigationDef AvoidDynamicObstacles +0x34 (Ego_r.pdb), count at +4; CThing PhysicsTC +0x70 (retail).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CCreatureNavigationDef { public:
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x34 - 8];
    bool AvoidDynamicObstacles;                 // +0x34
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerNavigation {                  // CDefPointer<const CCreatureNavigationDef>
    CCreatureNavigationDef* m_p;
    CDefPointerNavigation() : m_p(0) { }
    ~CDefPointerNavigation() { if (m_p) m_p->Release(); }
    CCreatureNavigationDef* operator->() const { return m_p; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointerNavigation* out) const;   // 0x006CD2F0 (GetSubDef<CCreatureNavigationDef>)
};
#pragma auto_inline(off)
long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerNavigation* out) const
{
    CCreatureNavigationDef* old = out->m_p;
    out->m_p = (CCreatureNavigationDef*)TmplSink(this, (long)old);
    return out->m_p != 0;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x70];
    CDefClassBase* PhysicsTC;                   // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: Dead
    const C3DVector& GetPosition();             // 0x004C73D0
    void RemoveTC(long interfaceType);          // 0x004C9840
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // <CThing>: vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool& arrived, long movement,
                                     bool a, bool b, bool avoidDynamicObstacles, const CThing* ignore, bool c, float d);   // 0x0097E890
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
};
struct CAIStateGroup_LookAtInterestingThings : CAIStateGroupBase {
    long FrameToLook;                           // +0x14
    CThing* FindInterestingThing();             // 0x00919240
    virtual void Update();
};

void CAIStateGroup_LookAtInterestingThings::Update()
{
    CIntelligentPointer<CThing> thing(FindInterestingThing());
    if (!IsThingAlive(thing))
        return;
    CDefPointerNavigation navigationDef;
    CDefClassBase* defClass = GetCreature()->PhysicsTC;
    defClass->GetSubDef(&navigationDef);
    bool arrived;
    for (;;) {
        if (thing.GetPItem() == 0) {
            Terminate();
            return;
        }
        CThing* target = thing.GetPItem();
        long result = AIAction_MoveToPosProximity(2, target->GetPosition(), 6.0f, arrived, 3, true, false,
                                                  navigationDef->AvoidDynamicObstacles, 0, true, 0.0f);
        if (result == 4)
            continue;
        if (result == 2) {
            for (;;) {
                if (thing.GetPItem() == 0) {
                    Terminate();
                    return;
                }
                CThing* target = thing.GetPItem();
                if (AIAction_LookAtPos(2, target->GetPosition(), 0x11, 0.0027777f) == 4)
                    continue;
                break;
            }
        } else if (result == 3)
            thing.GetPItem()->RemoveTC(0xb3);
        Terminate();
        return;
    }
}