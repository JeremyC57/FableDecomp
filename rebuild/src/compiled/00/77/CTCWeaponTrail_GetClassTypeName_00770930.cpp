class CCharString { public: char* p; CCharString(const char* s, long n); };
class CTCWeaponTrail { public: CCharString GetClassTypeName(); };

CCharString CTCWeaponTrail::GetClassTypeName()
{
    return CCharString((const char*)0x123b46c, -1);
}