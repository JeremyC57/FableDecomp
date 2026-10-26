// behaviour test for 0x00461ACD CHitLocationDef::operator=
#include <cstdio>
class CParentDefClassBase { public:
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
class CCPPointerInfo { public: long count; };
struct CPhysicalPrimitiveBase { int id; };
struct CPhysicalPrimitivePointer {
    CPhysicalPrimitiveBase* Data;
    CCPPointerInfo* Info;
    void ShareData(CPhysicalPrimitiveBase* data, CCPPointerInfo* info)
    {
        if (Info == info) return;
        if (Info) --Info->count;
        Data = data; Info = info;
        if (info) ++info->count;
    }
    CPhysicalPrimitivePointer& operator=(const CPhysicalPrimitivePointer& other) { ShareData(other.Data, other.Info); return *this; }
};
class CPhysicalPrimitiveInit { public:
    CPhysicalPrimitivePointer PPrimitive;
    CPhysicalPrimitiveInit& operator=(const CPhysicalPrimitiveInit& other) { PPrimitive = other.PPrimitive; return *this; }
};
class CHitLocationDef : public CParentDefClassBase { public:
    CPhysicalPrimitiveInit PhysicalPrimitive;
    long Armour, HitResponseAction, HitResponseToEpicSpellsAction, HitKnockdownAction, HitKnockdownDeathAction,
         BlockResponseAction, HitDamageEffect, HitBlockedEffect, VulnerableEffect, Flags;
    bool PlayGetHitResponse, DecapitateOnHit, DefendableAgainstShots, DefendableAgainstMelee;
    long Priority; bool Default, EnableOnCreate; long HitLocationCodelet;
    CHitLocationDef& operator=(const CHitLocationDef& other)
    {
        CParentDefClassBase::operator=(other);
        PhysicalPrimitive = other.PhysicalPrimitive;
        Armour = other.Armour;
        HitResponseAction = other.HitResponseAction;
        HitResponseToEpicSpellsAction = other.HitResponseToEpicSpellsAction;
        HitKnockdownAction = other.HitKnockdownAction;
        HitKnockdownDeathAction = other.HitKnockdownDeathAction;
        BlockResponseAction = other.BlockResponseAction;
        HitDamageEffect = other.HitDamageEffect;
        HitBlockedEffect = other.HitBlockedEffect;
        VulnerableEffect = other.VulnerableEffect;
        Flags = other.Flags;
        PlayGetHitResponse = other.PlayGetHitResponse;
        DecapitateOnHit = other.DecapitateOnHit;
        DefendableAgainstShots = other.DefendableAgainstShots;
        DefendableAgainstMelee = other.DefendableAgainstMelee;
        Priority = other.Priority;
        Default = other.Default;
        EnableOnCreate = other.EnableOnCreate;
        HitLocationCodelet = other.HitLocationCodelet;
        return *this;
    }
};
int main()
{
    CPhysicalPrimitiveBase prim = { 5 };
    CCPPointerInfo info = { 1 };
    CHitLocationDef a, b;
    b.baseTag = 2; b.PhysicalPrimitive.PPrimitive.Data = &prim; b.PhysicalPrimitive.PPrimitive.Info = &info;
    b.Armour = 30; b.Flags = 0x11; b.DecapitateOnHit = true; b.Priority = 4; b.EnableOnCreate = true; b.HitLocationCodelet = 6;
    a.baseTag = 0; a.PhysicalPrimitive.PPrimitive.Data = 0; a.PhysicalPrimitive.PPrimitive.Info = 0;
    a.Armour = 0; a.Flags = 0; a.DecapitateOnHit = false; a.Priority = 0; a.EnableOnCreate = false; a.HitLocationCodelet = 0;
    CHitLocationDef* r = &(a = b);
    if (r != &a || a.baseTag != 2 || a.PhysicalPrimitive.PPrimitive.Data != &prim || info.count != 2 || a.Armour != 30
        || a.Flags != 0x11 || !a.DecapitateOnHit || a.Priority != 4 || !a.EnableOnCreate || a.HitLocationCodelet != 6)
    { std::printf("00461acd_TEST FAIL\n"); return 1; }
    std::printf("00461acd_TEST PASS\n");
    return 0;
}