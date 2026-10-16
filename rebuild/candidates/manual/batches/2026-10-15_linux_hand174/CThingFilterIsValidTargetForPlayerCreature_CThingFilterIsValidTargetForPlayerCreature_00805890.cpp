// CThingFilter_IsValidTargetForPlayerCreature::CThingFilter_IsValidTargetForPlayerCreature(CThingPlayerCreature* player, CThing* targetOnly) -- caches whether the player may steal, is a child and is carrying a weapon
// Leaf: ??0CThingFilter_IsValidTargetForPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00805890, length 110 bytes (`ret 8`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("EXPRESSION_STEAL" @ 0x0124F534),
// CThingPlayerCreature::IsExpressionAvailable(const CCharString&) @ 0x006AC390, CCharString::~CCharString @ 0x0099EAE0,
// CThingCreatureBase::IsChild @ 0x00661F70, IsThingCarryingWeapon(const CThing*) @ 0x006A1540 (__fastcall).
// Layout: PDB CThingFilter_IsValidTargetForPlayerCreature (12 bytes): AcceptStealableStockItems +0, PlayerIsChild +1,
// PlayerCarryingWeapon +2, PlayerCreature +4, PTargetOnlyThing +8.
// Shape: init list in member order (three byte zeros, then the two pointers); the CCharString is a named local in its own
// block (it takes the dead player argument slot; `lea ecx ; push ecx`) whose result goes to a bool local tested after
// the block (kept in bl across the dtor); IsChild assigned straight into
// the bool; the weapon test reads the PlayerCreature member back as the fastcall argument.
class CThing;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CThingPlayerCreature {
    bool IsExpressionAvailable(const CCharString& name);   // 0x006AC390
    bool IsChild() const;                                   // 0x00661F70 (CThingCreatureBase)
};
bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540
struct CThingFilter_IsValidTargetForPlayerCreature {
    bool                  AcceptStealableStockItems;   // +0x00
    bool                  PlayerIsChild;               // +0x01
    bool                  PlayerCarryingWeapon;        // +0x02
    CThingPlayerCreature* PlayerCreature;              // +0x04
    CThing*               PTargetOnlyThing;            // +0x08
    CThingFilter_IsValidTargetForPlayerCreature(CThingPlayerCreature* player, CThing* targetOnly);
};

CThingFilter_IsValidTargetForPlayerCreature::CThingFilter_IsValidTargetForPlayerCreature(CThingPlayerCreature* player, CThing* targetOnly)
    : AcceptStealableStockItems(false), PlayerIsChild(false), PlayerCarryingWeapon(false),
      PlayerCreature(player), PTargetOnlyThing(targetOnly)
{
    bool canSteal;
    {
        CCharString name("EXPRESSION_STEAL");
        canSteal = player->IsExpressionAvailable(name);
    }
    if (canSteal)
        AcceptStealableStockItems = true;
    PlayerIsChild = player->IsChild();
    if (IsThingCarryingWeapon((CThing*)PlayerCreature))
        PlayerCarryingWeapon = true;
}
