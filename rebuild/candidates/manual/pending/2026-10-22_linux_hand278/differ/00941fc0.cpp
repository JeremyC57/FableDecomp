// Grade: DIFFER(578v578) -- one register choice left: retail adds the radius into y's register (add edi,eax) for the bottom-right corner, ours into the radius register (add eax,edi); advisor AdvB3 best (noinline same-TU search stand-in that lets the box escape, operator= conversion for the kept creature), see probe/advisor/00941fc0/NOTES.md
// CAIStateGroup_KidReactToBlockage::IsRunnable() -- runnable once the activation interval has passed, the player is in none of the blocking modes, and the kid has stopped moving; then find the nearest other very-close motionless creature in a rough area of ten radii, keep it and its radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941FC0, length 578 bytes.
// Callees: CPlayer::GetCurrentMode @ 0x008FCFA0, CWorld::GetWorldFrame @ 0x0049D870 (static `__fastcall`), CVectorMap::LowerBound
// @ 0x0040F020 / const overload @ 0x004365B0, CTCPlayerModes::HasMode @ 0x0073A8A0, CThingCreatureBase::GetPosition
// @ 0x006A4D00, _ftol @ 0x00BFEA70, CIsVeryCloseMotionlessCreature ctor @ 0x00942230, PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThingInRoughArea<...> @ 0x00942390, CIntelligentPointer<const CThing> ctor @ 0x00668B60,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10.
// Layout: Ego_r.pdb CAIStateGroup_KidReactToBlockage LastEndState +0x14, CurrentActivationMinInterval +0x18, our_radius
// +0x24, their_radius +0x28, PThingCreature +0x2c.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return Z * Z + Y * Y + X * X; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
extern const float EPSILON;                     // 0x0129BA3C (0.0001f)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPhysicsBody {
    unsigned char _pad0[0x34];
    float Radius;                               // +0x34
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                  // +0x60
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CTCPlayerModes {                         // interface 0x31
    bool HasMode(long mode);                    // 0x0073A8A0
};
struct CTCMovement {                            // interface 2
    unsigned char _pad0[0x0c];
    C3DVector Position;                         // +0x0c
    C3DVector LastPosition;                     // +0x18
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    CIntelligentPointerConstThing(const CThing* item);   // 0x00668B60
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>
    CIntelligentPointerCreature& operator=(const CIntelligentPointerConstThing& o) { CThing* item = o.GetPItem(); SetPItem(item); return *this; }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C2DBoxI {
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    C2DBoxI(const C2DCoordI& topLeft, const C2DCoordI& bottomRight) : TopLeft(topLeft), BottomRight(bottomRight) { }
};
struct CThingFilter_IsNot {
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) { }
};
struct CIsVeryCloseMotionlessCreature {
    unsigned char _data[0x10];
    CIsVeryCloseMotionlessCreature(CThingCreatureBase* creature);   // 0x00942230
};
struct CFilter_Bind2 {
    const CThingFilter_IsNot* A;
    const CIsVeryCloseMotionlessCreature* B;
    CFilter_Bind2(const CThingFilter_IsNot& a, const CIsVeryCloseMotionlessCreature& b) : A(&a), B(&b) { }
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
struct CThingSearchTools {
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CFilter_Bind2& filter, const CThingCompare_Nearest& compare,
                                  CThingCreatureBase** out);   // 0x00942390
};
long NearSink_1fc0(const void* self, const C2DBoxI* area, long seen);   // stand-in: the work GetPBestThingInRoughArea does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestThingInRoughArea @ 0x00942390 (compiled in retail's TU): reads its const
// reference arguments, writes the out-parameter, lets no argument escape.
__declspec(noinline) bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area, const CFilter_Bind2& filter,
                                                                      const CThingCompare_Nearest& compare, CThingCreatureBase** out)
{
    *out = 0;
    long best = NearSink_1fc0(this, &area, *(const long*)&filter + *(const long*)&compare);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)
struct CWorld {
    static long __fastcall GetWorldFrame();     // 0x0049D870
};
struct CPlayer;
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetCurrentMode();                   // 0x008FCFA0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_KidReactToBlockage : CAIStateGroupBase {
    long LastEndState;                          // +0x14
    long CurrentActivationMinInterval;          // +0x18
    long LastActivation;                        // +0x1c
    long CurrentPersistMaxInterval;             // +0x20
    float our_radius;                           // +0x24
    float their_radius;                         // +0x28
    CIntelligentPointerCreature PThingCreature;   // +0x2c
    bool IsRunnable();
};

bool CAIStateGroup_KidReactToBlockage::IsRunnable()
{
    if (GetCurrentMode()->GetWorldFrame() - LastEndState < CurrentActivationMinInterval)
        return false;
    if (GetCreature()->HasTC(0x31)) {
        CTCPlayerModes* modes = GetCreature()->GetTC<CTCPlayerModes>(0x31);
        if (modes->HasMode(0x28) || modes->HasMode(8) || modes->HasMode(9) || modes->HasMode(10) || modes->HasMode(11)
            || modes->HasMode(0x2d) || modes->HasMode(0x27))
            return false;
    }
    CAIBrain* brain = PBrain;
    CTCMovement* movement;
    brain->PCreature->GetInterface(&movement, 2);
    if (!((movement->Position - movement->LastPosition).GetLengthSquared() > EPSILON * EPSILON)) {
        const C3DVector& pos = brain->PCreature->GetPosition();
        our_radius = GetCreature()->PhysicsBody->Radius;
        long x = (long)pos.X;
        long y = (long)pos.Y;
        long r = (long)(our_radius * 10.0f);
        C2DCoordI topLeft(x, y);
        x += r;
        y += r;
        topLeft.Y -= r;
        topLeft.X -= r;
        C2DBoxI area(topLeft, C2DCoordI(x, y));
        CThingCreatureBase* found;
        if (PeekThingSearchTools()->GetPBestThingInRoughArea(area,
                CFilter_Bind2(CThingFilter_IsNot(GetCreature()), CIsVeryCloseMotionlessCreature(GetCreature())),
                CThingCompare_Nearest(GetCreature()->GetPosition()), &found)) {
            PThingCreature = found;
            their_radius = PThingCreature.GetPItem()->PhysicsBody->Radius;
            return true;
        }
    }
    return false;
}
