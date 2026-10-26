#pragma optimize("s", on)
// CTCTrapBase::FrameUpdate() -- advance the trap opener by the owner's update time, age the hit-things list (drop dead or expired entries), update the trigger things and the trigger, then the trap's own update
// Leaf: ?FrameUpdate@CTCTrapBase
// Retail: Fable.exe @ 0x007888B4, 105 bytes; size TU (unaligned entry, as the landed CTCTrapBase rows): `optimize("s",on)`.
// Callees: CTCBase::GetWorld() 0x00686A70; GetUpdateTimeForThing(CThing*, CWorld*) 0x00787C62 (__fastcall, float);
//          CFadeInFadeOutBase::Update(float) 0x00434870 (TrapOpener); IS_THING_ALIVE(const CIntelligentPointer<CThing>&)
//          0x004CC340 (__fastcall); vector<pair<CIntelligentPointer<CThing>,long>>::erase(iterator) 0x00789085 (manifest
//          label "Vector_PopBack_CBodyPartMesh" is wrong); CTCTrapBase::UpdateTriggerThings 0x007881AD (walks the +0x2c
//          vector; manifest "CTCCombatBase_UpdateEngageable" is wrong, real name unknown), CTCTrapBase::UpdateTrigger()
//          0x0078888A; the trap's virtual slot 29 (+0x74, UpdateTrap; tail jump).
// Layout: retail CTCTrapBase (12-byte vectors, below engine/CTCTrapBase.h): HitThings +0x0c (12-byte {pointer, frames}
//         pairs), TriggerThings +0x2c, TrapOpener (CFadeInFadeOutBase) +0x38.
// Shape: the owner is a named local read before the GetWorld call (kept in edi); the erase loop advances only on the kept branch; the trailing virtual call is a tail jump.
class CThing;
class CWorld;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                        // 8 bytes: vptr + item
    void* _vfptr;
    CThing* m_pItem;
};
#pragma pack(pop)

bool __fastcall IS_THING_ALIVE_004cc340(const CBaseIntelligentPointer& thing);   // 0x004CC340
float __fastcall GetUpdateTimeForThing(CThing* thing, CWorld* world);   // 0x00787C62

struct SHitThing {                                      // pair<CIntelligentPointer<CThing>, long>
    CBaseIntelligentPointer first;
    long second;
};
struct CHitThingVector {
    SHitThing* _M_start;
    SHitThing* _M_finish;
    SHitThing* _M_end_of_storage;
    SHitThing* begin() { return _M_start; }
    SHitThing* end() { return _M_finish; }
    SHitThing* erase(SHitThing* position);              // 0x00789085
};

class CFadeInFadeOutBase { public:
    void Update(float dt);                              // 0x00434870
};

class CTCBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void UpdateTrap();                          // slot 29 (+0x74)
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCTrapBase : public CTCBase { public:
    CThing*            ParentThing;                     // +0x04
    unsigned char      _pad_0x08[0x4];
    CHitThingVector    HitThings;                       // +0x0c
    unsigned char      _pad_0x18[0x20];
    CFadeInFadeOutBase TrapOpener;                      // +0x38
    void UpdateTriggerThings();                         // 0x007881AD
    void UpdateTrigger();                               // 0x0078888A
    void FrameUpdate();
};

void CTCTrapBase::FrameUpdate()
{
    CThing* thing = ParentThing;
    TrapOpener.Update(GetUpdateTimeForThing(thing, (&GetWorld())));
    for (SHitThing* it = HitThings.begin(); it != HitThings.end(); ) {
        if (it->second != 0 && IS_THING_ALIVE_004cc340(it->first)) {
            --it->second;
            ++it;
        } else {
            it = HitThings.erase(it);
        }
    }
    UpdateTriggerThings();
    UpdateTrigger();
    UpdateTrap();
}