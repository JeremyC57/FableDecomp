class CCharString { public: char* p; CCharString(const char* s, long n); };
class CTCWeaponTrail { public: CCharString GetClassTypeName(); };

CCharString CTCWeaponTrail::GetClassTypeName()
{
    return CCharString("CTCWeaponTrail", -1);
}