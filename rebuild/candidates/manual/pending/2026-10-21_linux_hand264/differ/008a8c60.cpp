// CGameScriptInterface::EntitySetAsSittingOnFloor(const CScriptThing& thing, bool sit) -- for a living creature, either sit it on the floor (finish its action, pick the ST_SIT_ON_FLOOR animation and run a sit-down action chained to a looping play-animation action) or, when it is sitting, stand it up with the ST_STAND_UP_FROM_FLOOR get-up action
// Grade: DIFFER(472v472) -- temp slot packing only: retail puts the ST_SIT_ON_FLOOR name at +0x14 (shared with the counted pointer) and the filter at +0x1c; ours packs the name into the key slot +0x10
// Leaf: EntitySetAsSittingOnFloor
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A8C60, 470 bytes, `ret 8`.
// Callees: the const component-map LowerBound 0x004365B0 (TC 0x31 action user), the action user's current-action accessor
//   0x0073A6E0, CThing FinishCurrentAction 0x00662650 / GetRandomAnimFromNameWithFilter 0x00662EF0 / SetCurrentAction
//   0x006644F0, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0, the animation name's CCharString conversion
//   0x00415DD0, the 0x38-byte follow-on action ctor 0x008AC3C0 (operator new 0x00BFEA1A), CCountedPointer<CCreatureActionBase>
//   ctor 0x008AB380 / dtor 0x008AB3D0, CCreatureAction_SitDown ctor 0x008AB9D0 / dtor 0x008ABAA0,
//   CCreatureAction_GetUpFromSitting ctor 0x00860150 / dtor 0x0062E900; the script thing's virtual slots 11 and 75.
// Layout: local projections; the animation filter is an inline vtable-only object (vtables 0x0127261C / 0x0122FFCC).
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const long& key) const;   // 0x004365B0
};
struct CAnimName { operator CCharString() const; };     // 0x00415DD0
struct CAnim { unsigned char Data[8]; CAnimName Name; };
struct CAnimFilterBase {
    virtual bool Accept(const CAnim& anim) const;
    ~CAnimFilterBase() { }
};
struct CAnimFilterAny : CAnimFilterBase {
    virtual bool Accept(const CAnim& anim) const;
};
struct CCreatureActionBase {
    virtual void v0();
    virtual long GetType() const;
};
struct CCPPointerInfo { long Count; void (__fastcall* Delete)(void*); void* Object; };
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* p);                               // 0x008AB380
    CCountedPointer(const CCountedPointer& o)
    {
        Data = o.Data;
        CCPPointerInfo* info = o.Info;
        Info = info;
        if (info)
            ++info->Count;
    }
    ~CCountedPointer();                                  // 0x008AB3D0
};
struct CThing;
struct CCreatureActionPlayAnimLoop : CCreatureActionBase {
    unsigned char Data[0x34];
    CCreatureActionPlayAnimLoop(CThing* creature, const CCharString& anim);   // 0x008AC3C0
};
struct CCreatureAction_SitDown {
    unsigned char Data[0x11c];
    CCreatureAction_SitDown(CThing* creature, const CAnim* anim, CCountedPointer<CCreatureActionBase> next);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                          // 0x008ABAA0
};
struct CCreatureAction_GetUpFromSitting {
    unsigned char Data[0x11c];
    CCreatureAction_GetUpFromSitting(CThing* creature, const CCharString& anim, long type, long flags);   // 0x00860150
    ~CCreatureAction_GetUpFromSitting();                 // 0x0062E900
};
struct CTCActionUser { CCreatureActionBase* GetCurrentAction() const; };   // 0x0073A6E0
struct CThing {
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    unsigned char  State[0x20];
    unsigned long  ThingTypeFlags;                       // +0x6c
    void FinishCurrentAction();                          // 0x00662650
    const CAnim* GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter);   // 0x00662EF0
    void SetCurrentAction(const CCreatureAction_SitDown& action);            // 0x006644F0
    void SetCurrentAction(const CCreatureAction_GetUpFromSitting& action);   // 0x006644F0
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
template <class T, long TYPE> __forceinline T* GetComponent(const CThing* thing)
{
    T* result;
    if (thing->UsingComponents[TYPE >> 5] & (1u << (TYPE & 31))) {
        const ComponentMap* map = &thing->Components;
        const ComponentEntry* it = map->LowerBound(TYPE);
        const ComponentEntry* end = map->Finish;
        result = (T*)((it == end || it->Type > TYPE) ? end : it)->Component;
    }
    return result;
}
struct CGameScriptInterface {
    virtual void v000();
    void EntitySetAsSittingOnFloor(const CScriptThing& thing, bool sit);
};

void CGameScriptInterface::EntitySetAsSittingOnFloor(const CScriptThing& thing, bool sit)
{
    if (!thing.IsAlive() || !(thing.GetThing()->ThingTypeFlags & 8))
        return;
    CThing* creature = thing.GetThing();
    CTCActionUser* actionUser = GetComponent<CTCActionUser, 0x31>(creature);
    if (sit) {
        if (actionUser->GetCurrentAction()->GetType() == 10)
            return;
        creature->FinishCurrentAction();
        const CAnim* anim = creature->GetRandomAnimFromNameWithFilter("ST_SIT_ON_FLOOR", CAnimFilterAny());
        CCountedPointer<CCreatureActionBase> next(new CCreatureActionPlayAnimLoop(creature, anim->Name));
        creature->SetCurrentAction(CCreatureAction_SitDown(creature, anim, next));
    } else {
        if (actionUser->GetCurrentAction()->GetType() != 10)
            return;
        creature->FinishCurrentAction();
        creature->SetCurrentAction(CCreatureAction_GetUpFromSitting(creature, "ST_STAND_UP_FROM_FLOOR", 10, 0));
    }
}
