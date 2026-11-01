class CThing;

class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct CThingRef {
};

class CGameScriptThing { public:
    char pad[0xc];
    CThingRef ref;
};

CThing* __fastcall CGameScriptThing_GetPThing(const CGameScriptThing* self)
{
    CThingRef* r = (CThingRef*)&self->ref;
    if (((CThing*)((CBaseIntelligentPointer*)r)->GetPItem()) != 0) {
        if ((*((unsigned char*)((CThing*)((CBaseIntelligentPointer*)r)->GetPItem()) + 0x91) & 1) == 0)
            return ((CThing*)((CBaseIntelligentPointer*)r)->GetPItem());
    }
    return 0;
}