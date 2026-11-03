float CTCInventoryWeapons::GetActiveProjectileWeaponDamage() const
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        CDefPointer<const CWeaponDef> weaponDef;
        PActiveRangedWeapon->GetPBaseDef()->GetSubDef(&weaponDef);
        CDefPointer<const CThingShotDef> shotDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot);
        damage = shotDef->HitThingDamage;
    }
    return damage;
}
