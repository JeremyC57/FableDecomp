// CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const -- CIDrawWorldMap override forwarding to CWorldMap::FindLastUnblockedThemePositionAndReportCollision (tail jump with `this` moved back from the +4 interface)
// Retail: Fable.exe @ 0x004FB2A0, 8 bytes: `add ecx,-4 ; jmp 0x0063DB30`.
// RTTI (COL at vtable-4): CWorldMap : CBaseClassNonCopyable (: CBaseClass) at +0, CIDrawWorldMap at +4;
// this body is slot 9 of CIDrawWorldMap's vtable in CWorldMap (0x01244AB4). VC7.1 compiles an override of an
// interface introduced only in the second base with `this` = the interface subobject, so the forwarding call to
// the non-virtual CWorldMap member (manifest 0x0063DB30: FindLastUnblockedThemePositionAndReportCollision, thiscall,
// three C3DVector arguments) moves `this` by -4 and becomes a tail jump.
// Layout: local model (engine/CWorldMap.h is a flattened view without the two bases; its +0x04 "f4" is this vptr).
class C3DVector { public: float X, Y, Z; };

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
};

class CIDrawWorldMap { public:                              // vtable 0x01244AB4 inside CWorldMap
    virtual ~CIDrawWorldMap() {}
    virtual void Draw1() const = 0;
    virtual void Draw2() const = 0;
    virtual void Draw3() const = 0;
    virtual void Draw4() const = 0;
    virtual void Draw5() const = 0;
    virtual void Draw6() const = 0;
    virtual void Draw7() const = 0;
    virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const = 0;
    virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const = 0;
};

class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap { public:
    bool FindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const;   // 0x0063D960
    bool FindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const;    // 0x0063DB30
    virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const;
};

bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const
{
    return FindLastUnblockedThemePositionAndReportCollision(from, to, lastUnblocked);
}