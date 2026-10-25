// CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation, const C3DVector& offset, float forwardAngle, float upAngle) -- place a tavern-game mesh: offset it in the orientation's right/forward/up frame from position, turn the frame about Up then Forward, and push both to the thing's physics component (immediate setters unless the thing is in limbo)
// Grade: DIFFER(495v511) -- x87 allocation of the right-vector temporary (retail spills Y and Z of Forward ^ Up to [esp+20]/[esp+24] and keeps X in st0; ours keeps X and Y on the stack), and the two virtual-setter branches are not head-merged (retail hoists `mov edx,[edi] ; lea eax,[esp+10] ; push eax` above `je`)
// Leaf: ?PositionMesh@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EABC0, 512 bytes (511 without padding), static `__fastcall` (ecx = thing,
//   edx = position), `ret 10h`. Ego_r.pdb name set: ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z.
// Callees: CThing vtable slot 27 (+0x6C, bool) and slot 28 (+0x70, push 1), CInterfaceMap::LowerBound 0x0040F020
//   (interface 2, the physics TC; out-parameter form: the absent path reads the orientation argument's slot),
//   CTCPhysicsBase::SetAsMoving(bool) 0x006B0130, physics vtable +0x78 / +0x7C (position) and +0x80 / +0x12C (right-
//   handed set), CRightHandedSet::RotateAboutAxis(const C3DVector&, float) 0x00A22F80 (twice: Up by upAngle, then
//   Forward by forwardAngle), CThing::SetAsFrameUpdated 0x004C7C40, CThing 0x004C7590(4).
// Tried (cmp.py, no grade cost): member vs free operator* / operator^, copy ctor on/off, statement-wise += sum (the
//   one-expression sum evaluates Up * offset.Z first), named-result GetRight, inline RotateAboutUp/Forward (fixes the
//   tail: `push edx ; mov ecx,edx`), ternary / inline helper for the setter pairs, same-TU LowerBound stand-in (fixes
//   the frame: the key shares the orientation slot). Best: 60 normalised diff lines.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& operator+=(const C3DVector& b) { X += b.X; Y += b.Y; Z += b.Z; return *this; }
};
inline C3DVector operator*(const C3DVector& v, float s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b) { return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
class CRightHandedSet {
public:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
    __forceinline void RotateAboutUp(float angle) { RotateAboutAxis(Up, angle); }
    __forceinline void RotateAboutForward(float angle) { RotateAboutAxis(Forward, angle); }
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
#pragma auto_inline(off)
inline __declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    int n = last - first;
    while (n > 0) {
        int half = n >> 1;
        if (lo[half].type < *type) { lo += half + 1; n -= half + 1; }
        else n = half;
    }
    return lo;
}
#pragma auto_inline(on)
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);              // slot 30 (+0x78)
    virtual void SetPositionNow(const C3DVector& position);           // slot 31 (+0x7c)
    virtual void SetRHSet(const CRightHandedSet& set);                // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void SetRHSetNow(const CRightHandedSet& set);             // slot 75 (+0x12c)
    void SetAsMoving(bool moving);                                    // 0x006B0130
};
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool IsInLimbo();                          // slot 27 (+0x6c)
    virtual void SetVisible(bool visible);             // slot 28 (+0x70)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    void SetAsFrameUpdated();                          // 0x004C7C40
    void SetUpdateFlags(long flags);                   // 0x004C7590
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CTCTavernGame {
public:
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float forwardAngle, float upAngle);
};

void __fastcall CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                           const C3DVector& offset, float forwardAngle, float upAngle)
{
    if (thing) {
        bool immediate = !thing->IsInLimbo();
        CRightHandedSet set = orientation;
        CTCPhysicsBase* physics;
        thing->GetInterface(&physics, 2);
        physics->SetAsMoving(true);
        C3DVector meshPosition = set.GetRight() * offset.X;
        meshPosition += set.Forward * offset.Y;
        meshPosition += set.Up * offset.Z;
        meshPosition += position;
        if (immediate)
            physics->SetPositionNow(meshPosition);
        else
            physics->SetPosition(meshPosition);
        set.RotateAboutUp(upAngle);
        set.RotateAboutForward(forwardAngle);
        if (immediate)
            physics->SetRHSet(set);
        else
            physics->SetRHSetNow(set);
        thing->SetAsFrameUpdated();
        thing->SetUpdateFlags(4);
        thing->SetVisible(true);
    }
}
