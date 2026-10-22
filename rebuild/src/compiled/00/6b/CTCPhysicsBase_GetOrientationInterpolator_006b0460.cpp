// CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator& out) const -- the orientation interpolator from the last frame's set to the current one; under bullet time for a thing it affects, the pair is instead two sets interpolated between them at the current and next sub-step of the bullet-time cycle
// Leaf: ?GetOrientationInterpolator@CTCPhysicsBase@@QBEXAAVC3DOrientationInterpolator@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0460, 310 bytes, `ret 4`.
// Callees: CTCBase::GetWorld() const 0x00686A80 (twice), CThing::IsAffectedByBulletTime 0x004C9920, CBulletTime::GetTimeScale
//   0x004C5FA0, CWorld::GetWorldFrame 0x0049D870 (static), _ftol 0x00BFEA70, CRightHandedSet::Interpolate(const
//   CRightHandedSet&, float) const 0x00A23190 (hidden result), C3DOrientationInterpolator(const CRightHandedSet&, const
//   CRightHandedSet&) 0x005508D0, C3DOrientationInterpolator::operator= 0x006B0E00 (landed), ~CBase 0x0099A2E0, the
//   physics virtual slots 72 (+0x120, the current right-handed set, as the landed 0x006B0850) and 73 (+0x124, the
//   previous frame's set; name descriptive). Global 1.0f 0x0122DED8.
// Layout: CTCBase ParentThing +0x04; CWorld bullet-time controller +0x68 (active flag +0x0c, controller +0x04, as the
//   landed CTCAnimatingObject::FrameUpdate 0x007F8E20); C3DOrientationInterpolator 0x34 bytes (CBase vptr, start +0x04,
//   end +0x1c).
// Shape: plain source order: the step is (frame - 1) % (long)scale, the step size 1 / scale, the two sets interpolated
//   from the old set towards the current one (the current set's getter runs first as the right-most argument), and the
//   result assigned from an interpolator temporary in each branch.
class CThing;

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Up, Forward;
    CRightHandedSet Interpolate(const CRightHandedSet& other, float t) const;   // 0x00A23190
};
struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A2E0
};
struct C3DOrientationInterpolator : CBase {         // 0x34 bytes
    CRightHandedSet StartOrientation;               // +0x04
    CRightHandedSet EndOrientation;                 // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
    C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& other);         // 0x006B0E00
};

struct CBulletTime {
    unsigned char _pad0[4];
    void*         m_pController;                    // +0x04
    unsigned char _pad1[4];
    bool          m_active;                         // +0x0c
    float GetTimeScale();                           // 0x004C5FA0
    bool IsActive() const { return m_active || m_pController != 0; }
};
struct CWorld {
    unsigned char _pad[0x68];
    CBulletTime*  m_pBulletTime;                    // +0x68
    static long GetWorldFrame();                    // 0x0049D870
};
class CThing {
public:
    bool IsAffectedByBulletTime() const;            // 0x004C9920
};

class CTCPhysicsBase {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual const CRightHandedSet& GetRightHandedSet() const;      // slot 72 (+0x120)
    virtual const CRightHandedSet& GetOldRightHandedSet() const;   // slot 73 (+0x124)
    CThing* ParentThing;                            // +0x04 (CTCBase)
    const CWorld* GetWorld() const;                 // 0x00686A80 (CTCBase)
    void GetOrientationInterpolator(C3DOrientationInterpolator& out) const;
};

void CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator& out) const
{
    if (GetWorld()->m_pBulletTime->IsActive() && ParentThing->IsAffectedByBulletTime()) {
        float timeScale = GetWorld()->m_pBulletTime->GetTimeScale();
        long step = (CWorld::GetWorldFrame() - 1) % (long)timeScale;
        float stepSize = 1.0f / timeScale;
        CRightHandedSet start = GetOldRightHandedSet().Interpolate(GetRightHandedSet(), step * stepSize);
        ++step;
        CRightHandedSet end = GetOldRightHandedSet().Interpolate(GetRightHandedSet(), step * stepSize);
        out = C3DOrientationInterpolator(start, end);
    } else {
        out = C3DOrientationInterpolator(GetOldRightHandedSet(), GetRightHandedSet());
    }
}