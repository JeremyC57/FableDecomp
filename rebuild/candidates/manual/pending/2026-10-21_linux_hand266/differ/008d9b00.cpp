// Grade: DIFFER(406v408) -- retail has a 0xc frame (one dead slot) and converts the fps global with its own fild + fdivp; the lane folds it into fidiv with an 8-byte frame (inline FramesToSeconds, float local and Clamp-by-reference all tried)
// NCreatureMode::CModeScorpionKingVulnerable::AnimUpdate(CAnimateThing& anim) -- drive the tail-curl combination anim weight from the frame count over the battle def's curl time (clamped 0..1); past the threshold reopen the body hit location and kill the eye emitter, at full weight flag the curl finished
// Leaf: AnimUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D9B00, 0x198 bytes (padded to 416), `ret 4`.
// Callees: CModeStandBase::AnimUpdate 0x00862EC0 (qualified), CAnimateThing::FindCombinationAnim 0x0070C4E0 (label
//   "CTCAnimation::FindCombinationAnim", chunk 209), CDefClassBase::GetSubDef(CDefPointer<const CScorpionKingBattleDef>*)
//   0x008C5A50, CVectorMap::LowerBound 0x0040F020 (inline interface lookup 0x7F), CTCHitLocations::GetPHitLocationFromName
//   0x00815820 ("HIT_LOCATION_SCORPION_KING_BELLY" 0x01285A28, "HIT_LOCATION_SCORPION_KING" 0x01285A0C),
//   CHitLocation::SetEnabled 0x00816390, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill 0x004C9B80.
//   Globals: frames per second 0x01375550; floats 0.0f 0x0122DEDC, 1.0f 0x0122DED8, the threshold 0x01254DA8.
// Layout: Ego_r.pdb CModeScorpionKingVulnerable (188): PCreature +0x78, bRecovering +0x7c, PTarget +0x7d (packed), then the
//   12-byte retail vector moves PEyeEmitter to +0x94, PStingEmitter +0x9c, FrameCount +0xa4 ... TailCurlFinished +0xb4
//   (PDB - 4, like the landed Recover 0x008DF3A0 / Exhausted::OnActivate 0x008DA2E0 models).
namespace NCreatureMode {
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
struct CScorpionKingBattleDef : CDefPointeeBase {
    unsigned char _pad08[0x50 - 0x08];
    float         TailCurlTime;                         // +0x50
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (--Object->m_refCount == 0)
            Object->Delete();
    }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CScorpionKingBattleDef>* out);   // 0x008C5A50
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CScorpionKingBattleDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CScorpionKingBattleDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CHitLocation {
    void SetEnabled(bool enabled);                      // 0x00816390
};
struct CTCHitLocations {                                // interface type 0x7F
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                            // +0x91 (bit 0: killed)
    void Kill(bool immediately);                        // 0x004C9B80
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +4
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CCombinationAnim {
    unsigned char _pad0[0x1c];
    float         Weight;                               // +0x1c
    float         LastWeight;                           // +0x20
};
struct CAnimateThing {
    CCombinationAnim* FindCombinationAnim(long id);     // 0x0070C4E0
};
extern long g_framesPerSecond;                          // 0x01375550
extern const float g_tailCurlThreshold;                 // 0x01254DA8
inline float FramesToSeconds(long frames) { return (float)frames / (float)g_framesPerSecond; }

struct CModeBase {
    virtual ~CModeBase();
    void* ParentThing;                                  // +0x04
};
struct CModeStandBase : CModeBase {
    unsigned char _stand[0x68 - 0x08];
    void AnimUpdate(CAnimateThing& anim);               // 0x00862EC0
};
#pragma pack(push, 1)
struct CModeScorpionKingVulnerable : CModeStandBase {
    long                    RecoveryCount;              // +0x68
    unsigned char           Filter[0xc];                // +0x6c CAnimFilterOfGroup
    CThingCreatureBase*     PCreature;                  // +0x78
    bool                    bRecovering;                // +0x7c
    CBaseIntelligentPointer PTarget;                    // +0x7d
    unsigned char           Obstructions[0xc];          // +0x85 vector<CIntelligentPointer<CThing> >
    unsigned char           _pad91[3];
    CBaseIntelligentPointer PEyeEmitter;                // +0x94
    CBaseIntelligentPointer PStingEmitter;              // +0x9c
    long                    FrameCount;                 // +0xa4
    bool                    EffectsDone;                // +0xa8
    unsigned char           _pada9[3];
    long                    TailEffectIndex;            // +0xac
    float                   CollisionRadius;            // +0xb0
    bool                    TailCurlFinished;           // +0xb4
    void AnimUpdate(CAnimateThing& anim);
};
#pragma pack(pop)

void CModeScorpionKingVulnerable::AnimUpdate(CAnimateThing& anim)
{
    CModeStandBase::AnimUpdate(anim);
    CCombinationAnim* curl = anim.FindCombinationAnim(0x21);
    if (!curl)
        return;
    CDefPointer<CScorpionKingBattleDef> def;
    PCreature->PDef->GetSubDef(&def);
    float weight = FramesToSeconds(FrameCount) / def.Object->TailCurlTime;
    float lastWeight = curl->Weight;
    if (weight < 0.0f)
        weight = 0.0f;
    else if (weight > 1.0f)
        weight = 1.0f;
    curl->Weight = weight;
    curl->LastWeight = lastWeight;
    if (lastWeight >= g_tailCurlThreshold) {
        CTCHitLocations* hitLocations = PCreature->GetTCIfPresent<CTCHitLocations>(0x7f);
        CHitLocation* belly = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING_BELLY");
        CHitLocation* body = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING");
        belly->SetEnabled(false);
        body->SetEnabled(true);
        if (PEyeEmitter.GetPItem() && !(PEyeEmitter.GetPItem()->m_flags91 & 1))
            PEyeEmitter.GetPItem()->Kill(true);
    }
    if (lastWeight >= 1.0f)
        TailCurlFinished = true;
}
}
