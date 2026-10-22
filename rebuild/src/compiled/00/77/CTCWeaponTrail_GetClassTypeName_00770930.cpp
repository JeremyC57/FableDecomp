struct CCharString { char* p; CCharString(const char* s, int n); };
struct CTCWeaponTrail { CCharString GetClassTypeName(); };

CCharString CTCWeaponTrail::GetClassTypeName()
{
    return CCharString((const char*)0x123b46c, -1);
}