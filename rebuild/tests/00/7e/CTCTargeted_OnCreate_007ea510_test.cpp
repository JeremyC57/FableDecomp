// Behaviour test for CTCTargeted::OnCreate @ 0x007EA510: glow from the sub-def (default on), flags from the def (default 0).
#include <cstdio>
class CTargetingDef { public: unsigned long TargetingFlags; bool UseTargetGlow; };
struct CDefPointer { CTargetingDef* Object; };
class CDefClassBase { public: CTargetingDef* sub; bool GetSubDef(CDefPointer* out) { out->Object = sub; return sub != 0; } };
class CThing { public: CDefClassBase* PDef; };
class CTCTargeted { public:
    CThing* ParentThing; unsigned long TargetingFlags; bool UseTargetGlow; CDefPointer PTargetingDef;
    void OnCreate()
    {
        if (ParentThing->PDef->GetSubDef(&PTargetingDef)) UseTargetGlow = PTargetingDef.Object->UseTargetGlow; else UseTargetGlow = true;
        if (PTargetingDef.Object) TargetingFlags = PTargetingDef.Object->TargetingFlags; else TargetingFlags = 0;
    }
};
int main()
{
    CTargetingDef d; d.TargetingFlags = 0x55; d.UseTargetGlow = false; CDefClassBase base; base.sub = &d; CThing th; th.PDef = &base;
    CTCTargeted t; t.ParentThing = &th; t.TargetingFlags = 9; t.UseTargetGlow = true; t.PTargetingDef.Object = 0;
    t.OnCreate();
    if (t.UseTargetGlow || t.TargetingFlags != 0x55) { std::printf("007ea510_TEST FAIL\n"); return 1; }
    base.sub = 0; t.OnCreate();
    if (!t.UseTargetGlow || t.TargetingFlags != 0) { std::printf("007ea510_TEST FAIL\n"); return 1; }
    std::printf("007ea510_TEST PASS\n");
    return 0;
}