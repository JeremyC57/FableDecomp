// Grade: DIFFER(764v768): one 4-byte gap -- retail re-tests the male def (cmp eax,ebp; jne) before the female=true arm on the both-defs path; VC7.1 threads that test away in every if/ternary/&& form tried
// CTCBoastingCrowdControl::SpawnCrowdMember(CThing* lookAt, CThing* spawnPoint, CThing* village) -- create one crowd villager (male or female def from the crowd def, chosen at random when both exist) near the spawn point, face it towards the look-at thing, add it to the village and set it up as a non-persistent crowd member; false when the village or defs are missing or creation fails
// Leaf: SpawnCrowdMember
// Retail 0x00783380, 768 bytes, `ret 0xc`. Callees: LowerBound 0x0040F020 (interfaces 0x22 village, 2 placement),
// GetSubDef<crowd def> 0x00512FE0, CTCBase::GetRandomSeed 0x00686DA0, GFROR13 0x00497890, GFFloatRandom 0x004978A0,
// CCharString 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0 / operator= 0x0099EFB0, CThingAICreature::Create 0x00833800
// (__fastcall), CThing::GetPosition 0x004C73D0, CThingCreatureBase::AutoPlaceNear 0x00665DF0, CTCVillage::AddThingToVillage
// 0x0065B854, CThingAICreature::AutoSetupSexuality 0x008330D0, CThing::SetAsGameStatePersisted 0x004C7880, _CIfmod
// 0x00BFEB28, the placement TC's slot 66 (+0x108) facing setter. Globals: crowd script name 0x013BADE8, "" 0x0122D70E,
// 0.5f 0x0122F59C, 5.0f 0x0126B388, 1/(2pi) 0x01237F08.
// Layout: CThingAICreatureInit (Ego_r.pdb, 28 bytes) as the landed 0x008A9100; crowd def MaleChance +0x34, MaleDef +0x80,
// FemaleDef +0x84; creature flag bytes +0x92 (bit 4), +0x15c (bit 6), +0x1c9 (bit 0).
#include <math.h>
#pragma intrinsic(atan2, fmod)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern CCharString g_CrowdMemberScriptName;             // 0x013BADE8
struct C3DVector { float X, Y, Z; };
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
__forceinline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
struct CThingPhysical;
struct CThingBuilding;
struct CThingAICreatureInit {                           // Ego_r.pdb, 28 bytes
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString ScriptName;                             // +0x08
    CCharString OverridingBrainName;                    // +0x0C
    bool PlayerCharacter;                               // +0x10
    bool Drawable;                                      // +0x11
    bool PlaceAtEndOfList;                              // +0x12
    bool EnableCreatureAutoPlacing;                     // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    __forceinline CThingAICreatureInit(const CCharString& scriptName = "", const CCharString& brainName = "")
        : OriginalThing(0), PreviousThing(0), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(false), Drawable(true), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};
struct CTCPlacement {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65();
    virtual void SetFacingAngleXY(float angle);         // slot 66 (+0x108)
};
struct CThing;
struct CTCVillage {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    void AddThingToVillage(CThing* thing);              // 0x0065B854
};
struct STCEntry { long type; void* component; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);    // 0x0040F020
    __forceinline void* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->type) ? m_pLast : it)->component;
    }
};
struct CCrowdDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x34 - 0x08];
    float         MaleChance;                           // +0x34
    unsigned char _pad38[0x80 - 0x38];
    long          MaleDef;                              // +0x80
    long          FemaleDef;                            // +0x84
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CCrowdDef>* out);        // 0x00512FE0
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCrowdDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[9];                         // +0x20
    CTCMap         m_tcs;                               // +0x44
    unsigned char  _pad50[0x70 - 0x50];
    CDefClassBase* m_pDef;                              // +0x70
    unsigned char  _pad74[0x92 - 0x74];
    unsigned char  _flag92_0 : 4;
    unsigned char  InCrowd : 1;                         // +0x92 bit 4
    unsigned char  _flag92_5 : 3;
    unsigned char  _pad93[0x15c - 0x93];
    unsigned char  _flag15c_0 : 6;
    unsigned char  IsCrowdMember : 1;                   // +0x15c bit 6
    unsigned char  _flag15c_7 : 1;
    unsigned char  _pad15d[0x1c9 - 0x15d];
    unsigned char  NoLOD : 1;                           // +0x1c9 bit 0
    unsigned char  _flag1c9_1 : 7;
    CDefClassBase* GetDef() const { return m_pDef; }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline void* GetTC(long kind) { return m_tcs.Find(kind); }
};
struct CThingAICreature : CThing {
    void AutoPlaceNear(const C3DVector& pos, float radius);   // 0x00665DF0
    void AutoSetupSexuality(float maleChance);          // 0x008330D0
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                               CThingAICreatureInit* init);   // 0x00833800
};
struct CTCBoastingCrowdControl {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned long* GetRandomSeed();                     // 0x00686DA0
    bool SpawnCrowdMember(CThing* lookAt, CThing* spawnPoint, CThing* village);
};

bool CTCBoastingCrowdControl::SpawnCrowdMember(CThing* lookAt, CThing* spawnPoint, CThing* village)
{
    if (village->HasTC(0x22)) {
        CTCVillage* tcVillage = (CTCVillage*)village->GetTC(0x22);
        CDefPointer<CCrowdDef> def;
        if (tcVillage->ParentThing->GetDef()->GetSubDef(&def)) {
            long maleDef = def->MaleDef;
            long femaleDef = def->FemaleDef;
            if (maleDef == 0 && femaleDef == 0)
                return false;
            bool female;
            if (femaleDef == 0)
                female = false;
            else if (maleDef == 0)
                female = true;
            else
                female = (GFRandom(GetRandomSeed()) & 1) != 0;
            CThingAICreatureInit init;
            init.ScriptName = g_CrowdMemberScriptName;
            init.EnableCreatureAutoPlacing = false;
            CThingAICreature* creature = CThingAICreature::Create(female ? def->FemaleDef : def->MaleDef,
                                                                  spawnPoint->GetPosition(), 4, &init);
            if (creature) {
                creature->AutoPlaceNear(spawnPoint->GetPosition(), (GFFloatRandom(0.5f, GetRandomSeed()) + 0.5f) * 5.0f);
                CTCPlacement* placement;
                if (creature->HasTC(2))
                    placement = (CTCPlacement*)creature->GetTC(2);
                const C3DVector& from = creature->GetPosition();
                const C3DVector& to = lookAt->GetPosition();
                placement->SetFacingAngleXY(WrapAngle((float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f));
                tcVillage->AddThingToVillage(creature);
                creature->AutoSetupSexuality(def->MaleChance);
                creature->SetAsGameStatePersisted(false);
                creature->InCrowd = false;
                creature->NoLOD = true;
                creature->IsCrowdMember = true;
                return true;
            }
        }
    }
    return false;
}
