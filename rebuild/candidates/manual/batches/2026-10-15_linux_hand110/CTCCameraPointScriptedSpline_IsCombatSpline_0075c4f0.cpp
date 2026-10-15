// CTCCameraPointScriptedSpline::IsCombatSpline() -- true when the camera point's virtual type (slot 28) is 7 and the parent thing's def name is "CAMERA_POINT_COMBAT_SPLINE"
// Leaf: IsCombatSpline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C4F0, length 53 bytes (+ int3 padding).
// Callees: this's virtual slot 28 (`call [eax+70h]`, the camera-point type), CThing::GetDefName @ 0x004C7CC0 (CDefString
// through the hidden pointer), CDefString::operator==(const char*) const @ 0x005B3440; literal 0x01269B94.
// Label correction: the manifest's "CThingFilter_IsSuspect::operator()" is propagated -- the body takes no argument
// (`ret`) and its only caller is CTCCameraPointScriptedSpline::IsCombatSplineValidForAnimAndThing @ 0x0075D8B0 (landed,
// which already names this callee IsCombatSpline).
// Layout: engine/CTCBase.h-style ParentThing at +4; the 4-byte CDefString temporary lives in the `push ecx` slot (landed
// model of 0x0085EE10).
// Shape: the parent thing is read into a named local before the literal push (a direct `ParentThing->` loads it
// after the hidden-pointer `lea ecx`; retail uses `mov ecx,[esi+4]` first and `lea edx`); one shared false tail.
struct CDefString {                                 // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_tablePos;
    CDefString();
    bool operator==(const char* text) const;        // 0x005B3440
};

struct CThing {
    CDefString GetDefName() const;                  // 0x004C7CC0
};

struct CTCCameraPointScriptedSpline {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType();              // slot 28 (+0x70)
    CThing* ParentThing;                            // +0x04 (CTCBase)
    bool IsCombatSpline();
};

bool CTCCameraPointScriptedSpline::IsCombatSpline()
{
    if (GetCameraPointType() == 7) {
        CThing* parent = ParentThing;
        if (parent->GetDefName() == "CAMERA_POINT_COMBAT_SPLINE")
            return true;
    }
    return false;
}
