// Behaviour test for 0x0075C4F0 CTCCameraPointScriptedSpline::IsCombatSpline
#include <cstdio>
#include <cstring>
struct CDefString {
    const char* m_name;
    CDefString() : m_name("") {}
    bool operator==(const char* text) const { return std::strcmp(m_name, text) == 0; }
};
struct CThing {
    const char* m_defName;
    CDefString GetDefName() const { CDefString s; s.m_name = m_defName; return s; }
};
struct CTCCameraPointScriptedSpline {
    long m_type;
    CThing* ParentThing;
    virtual long GetCameraPointType() { return m_type; }
    bool IsCombatSpline()
    {
        if (GetCameraPointType() == 7) {
            CThing* parent = ParentThing;
            if (parent->GetDefName() == "CAMERA_POINT_COMBAT_SPLINE")
                return true;
        }
        return false;
    }
};
int main()
{
    CThing t; t.m_defName = "CAMERA_POINT_COMBAT_SPLINE";
    CTCCameraPointScriptedSpline s; s.m_type = 7; s.ParentThing = &t;
    bool ok = s.IsCombatSpline();
    s.m_type = 6; if (s.IsCombatSpline()) ok = false;
    s.m_type = 7; t.m_defName = "CAMERA_POINT_OTHER"; if (s.IsCombatSpline()) ok = false;
    if (!ok) { std::printf("0075c4f0_TEST FAIL\n"); return 1; }
    std::printf("0075c4f0_TEST PASS\n");
    return 0;
}