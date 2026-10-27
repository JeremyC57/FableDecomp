// CThing::IsLineOfSightTo_00838950(const CThing&) const -- world-map line of sight from this thing to another (descriptive name)
// Leaf: ?IsLineOfSightTo_00838950@CThing@@QBE_NABV1@@Z
// Retail: Fable.exe @ 0x00838950, 28 bytes, thiscall `ret 4`, bool in al.
// Identity (descriptive): a gap-scan row with no caller and no vtable reference in Fable.exe (xref.py: only a false abs hit
// inside an instruction), between CThingNoise::WorldUpdate (0x00838570) and the next thing classes. It calls the
// PROTECTED CThing::PeekWorldMap (Ego_r.pdb ?PeekWorldMap@CThing@@IBEABVCWorldMap@@XZ) on `this`, so it is a CThing (or
// derived) member; no PDB name in the CThing / CThingNoise / CThingSoundEmitter / CThingTrackNode name sets fits
// "(const CThing&) -> bool via IsLineOfSight", so the name is descriptive.
// Callees: 0x004C79E0 ?PeekWorldMap@CThing@@IBEABVCWorldMap@@XZ; 0x0063E6F0 ?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z.
// Shape: `if (!f()) return false; return true;` over the bool callee gives retail's `test al,al ; setne al` (a bare `return f();`
// passes al through). Arguments pushed right to left (true, other, *this) before PeekWorldMap, whose result becomes `this` of the test.
class CThing;

class CWorldMap {
public:
    bool IsLineOfSight(const CThing& from, const CThing& to, bool flag) const;   // 0x0063E6F0
};

class CThing {
public:
    virtual ~CThing();
    bool IsLineOfSightTo_00838950(const CThing& other) const;
protected:
    const CWorldMap& PeekWorldMap() const;                                     // 0x004C79E0
};

bool CThing::IsLineOfSightTo_00838950(const CThing& other) const
{
    if (!PeekWorldMap().IsLineOfSight(*this, other, true))
        return false;
    return true;
}