float CTCInventoryWeapons::GetActiveProjectileWeaponDamage() const
{
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        CDefPointer<const CWeaponDef> weaponDef;
        PActiveRangedWeapon->GetPBaseDef()->GetSubDef(&weaponDef);
        CDefPointer<const CThingShotDef> shotDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot);
        return shotDef->HitThingDamage;
    }
    return 0.0f;
}
