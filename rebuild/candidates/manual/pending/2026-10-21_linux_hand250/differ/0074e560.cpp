// Grade: DIFFER(334v330) -- argument-slot choice only: retail homes the 0x103 lookup key and the "CTCLifeTimer" temporary in the same dead `scriptName` slot (one `lea ecx,[esp+18h]` before the branch); ours puts the key in the dead `position` slot
// CTCContainerRewardHero::CreateNewObject(long def, const C3DVector& position, const C3DVector& acceleration, const CCharString& scriptName) -- spawn a reward object popping out of the container: push it, give it a random spin, make it not solid to the container, copy the container's lighting, and optionally give it a 90-second life timer
// Leaf: CreateNewObject
// Retail 0x0074E560, 336 bytes, `ret 10h`.
// Callees: the thing factory Create(long, const C3DVector*, long, CRightHandedSet*, ulong, const CCharString*) 0x00703210
// (`__fastcall`), CTCPhysicsBase::ApplyAcceleration(const C3DVector&) 0x006B0210, the TC's game accessor 0x00686A80 and its
// random-seed accessor 0x0049D810, GFFloatRandom(float, ulong*) 0x004978A0 (`__fastcall`), the physics' slot 66 (+0x108,
// float spin), CCharString(const char*, long) 0x0099EBF0 ("CTCSolidToThing" 0x0123B47C, "CTCLifeTimer" 0x0123E424) / ~
// 0x0099EAE0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60, CTCSolidToThing::AddThingNotSolidTo
// 0x00770FA0, CTCGraphicAppearance::SetLightingChannel 0x004BFB40 / SetAsChanged 0x004BF9E0, CVectorMap::LowerBound
// 0x0040F020 (interface 0x103), the life timer's setter 0x007E5950 (90.0f).
// Layout: Ego_r.pdb CTCContainerRewardHero - 4: CreateObjectsThatTimeOut +0x19, ParentThing +4; CThing physics +0x60,
// graphic appearance +0x64 (lighting channel +0x40), dead flag +0x91; the life timer's flags +0x10/+0x11.
// Shape: the random spin is a named float local passed on as a dword; the AddTC names are temporaries; the life timer is
// `if (HasTC) t = GetTC; else t = AddTC(...)` with the string slot address formed before the branch.
struct CTCBase;
struct CTCParamsBase;
struct CRightHandedSet;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CEnginePrimitiveThemeChannelDesc;
struct CTCGraphicAppearance {
    unsigned char _pad00[0x40];
    unsigned char LightingChannel[4];                   // +0x40
    void SetLightingChannel(CEnginePrimitiveThemeChannelDesc* channel, bool a, bool b);   // 0x004BFB40
    void SetAsChanged(unsigned long what);              // 0x004BF9E0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void SetSpin(float spin);                   // slot 66 (+0x108)
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CThing {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];             // +0x20
    CInterfaceMap         m_interfaces;                 // +0x44
    unsigned char         _pad4c[0x60 - 0x4c];
    CTCPhysicsBase*       Physics;                      // +0x60
    CTCGraphicAppearance* Graphic;                      // +0x64
    unsigned char         _pad68[0x91 - 0x68];
    unsigned char         m_flags;                      // +0x91 bit 0: dead
    CTCBase* AddTC(const CCharString& name, bool flag, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
CThing* __fastcall CreateThing(long def, const C3DVector* position, long kind, CRightHandedSet* orientation,
                               unsigned long flags, const CCharString* scriptName);   // 0x00703210
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CGameComponents {
    unsigned long* GetRandomSeed();                     // 0x0049D810
};
struct CTCSolidToThing {
    void AddThingNotSolidTo(CThing* thing);             // 0x00770FA0
};
struct CTCLifeTimer {
    unsigned char _pad00[0x10];
    bool          Active;                               // +0x10
    bool          Expired;                              // +0x11
    void SetLifeTime(float seconds);                    // 0x007E5950
};
class CTCContainerRewardHero {
public:
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x18 - 0x08];
    bool          Enabled;                              // +0x18
    bool          CreateObjectsThatTimeOut;             // +0x19
    CGameComponents* GetGame();                         // 0x00686A80
    CThing* CreateNewObject(long def, const C3DVector& position, const C3DVector& acceleration, const CCharString& scriptName);
};

CThing* CTCContainerRewardHero::CreateNewObject(long def, const C3DVector& position, const C3DVector& acceleration,
                                                const CCharString& scriptName)
{
    CThing* thing = CreateThing(def, &position, 5, 0, 0, &scriptName);
    if (thing && !(thing->m_flags & 1)) {
        thing->Physics->ApplyAcceleration(acceleration);
        float spin = GFFloatRandom(1.0f, GetGame()->GetRandomSeed());
        thing->Physics->SetSpin(spin);
        CTCSolidToThing* solid = (CTCSolidToThing*)thing->AddTC("CTCSolidToThing", false, 0);
        solid->AddThingNotSolidTo(ParentThing);
        CTCGraphicAppearance* containerGraphic = ParentThing->Graphic;
        if (containerGraphic && thing->Graphic) {
            thing->Graphic->SetLightingChannel((CEnginePrimitiveThemeChannelDesc*)containerGraphic->LightingChannel, true, true);
            thing->Graphic->SetAsChanged(4);
        }
        if (CreateObjectsThatTimeOut) {
            CTCLifeTimer* timer;
            if (thing->HasTC(0x103))
                timer = thing->GetTC<CTCLifeTimer>(0x103);
            else
                timer = (CTCLifeTimer*)thing->AddTC("CTCLifeTimer", false, 0);
            timer->SetLifeTime(90.0f);
            timer->Active = true;
            timer->Expired = false;
        }
    }
    return thing;
}
