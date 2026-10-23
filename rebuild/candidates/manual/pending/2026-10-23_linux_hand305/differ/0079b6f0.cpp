// CTCSummonSpell::ResetMinionPos() -- play the summon sound, move the live minion to the owner's summon point (then to the nearest valid summon position), turn it to face away from the owner, and spawn the def's CREATURE_APPEARING effect there
// Leaf: ResetMinionPos
// Grade: DIFFER(732v732) -- with a same-TU stand-in GetSubDef the size now matches (was 739v732); residue: retail pushes ebp in the prologue (ours just before the facing block, so every frame offset in between is 4 lower), the negated-facing fchs pair is scheduled before the pop of the last normalise temp, and the effect-lookup argument registers rotate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079B6F0, 732 bytes, plain `ret`.
// Callees: LowerBound 0x0040F020, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CTCSoundPlayer::PlayAttachedCriteria
//   0x0066BD00 (catalog "CSoundInfo::PlaySound"), the owner frame's virtual slots 70 (+0x118 summon offset), 30 (+0x78
//   SetPosition) and 74 (+0x128 the orientation set), CThing::GetPosition 0x004C73D0, GetPItem 0x00A01B50,
//   CTCSummonSpell::GetValidSummonPos 0x0079AA90 (hidden-pointer C3DVector), CDefClassBase::GetSubDef 0x00669580 (catalog
//   "GetSubDef<CAICreatureWillPowerIndicatorDef>", propagated), the def's effect lookup 0x00669D30 (catalog
//   "CCRC::CalcAndLookup"), CreateParticleEffect 0x0072AB70 (fastcall).
// Tables: the inverse-square-root mantissa table 0x013CE558; floats 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CTCSummonSpell (PDef +0x0c, Minion +0x11 packed), as the landed FrameUpdate 0x0079B9D0.
extern unsigned long g_InvSqrtMantissaTable[128];           // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline void operator*=(const float& s) { X *= s; Y *= s; Z *= s; }
    __forceinline void Normalise();
};
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator-(const C3DVector& a) { return C3DVector(-a.X, -a.Y, -a.Z); }
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
__forceinline void C3DVector::Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); }
struct CRightHandedSet {
    C3DVector Forward, Up;
    CRightHandedSet(const C3DVector& forward, const C3DVector& up) : Forward(forward), Up(up) { }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct CThingFrame {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69();
    virtual void GetSummonOffset(C3DVector* out);           // slot 70 (+0x118)
    virtual void v71(); virtual void v72(); virtual void v73();
    virtual void SetRHSet(const CRightHandedSet& set);      // slot 74 (+0x128)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingFrame*  Frame;                                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead : 1;                                 // +0x91 bit0
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline T* GetTC(long type, T*)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCSoundPlayer {
    void PlayAttachedCriteria(const CCharString& name, bool flag, long a, long b, long c);   // 0x0066BD00
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};
struct CSummonSpellDef {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    bool LookupEffect(const CCharString& name, long* effect) const;   // 0x00669D30
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CSummonSpellDef>* out);     // 0x00669580
};
CSummonSpellDef* FindRef_GetSubDef_0();
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CSummonSpellDef>* out)
{
    CSummonSpellDef* def = FindRef_GetSubDef_0();
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& pos);   // 0x0072AB70

#pragma pack(push, 1)
struct CTCSummonSpell {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CDefClassBase* PDef;                                    // +0x0c
    bool JustReleased;                                      // +0x10
    CBaseIntelligentPointer Minion;                         // +0x11
    C3DVector GetValidSummonPos(CThing* minion);            // 0x0079AA90
    void ResetMinionPos();
};
#pragma pack(pop)

void CTCSummonSpell::ResetMinionPos()
{
    if (ParentThing->HasTC(0x10)) {
        CTCSoundPlayer* sound = ParentThing->GetTC(0x10, (CTCSoundPlayer*)0);
        sound->PlayAttachedCriteria("HERO_ABILITY_SUMMON_CREATURE_APPEAR", true, 0, 0, -1);
    }
    C3DVector offset;
    ParentThing->Frame->GetSummonOffset(&offset);
    C3DVector target = offset + ParentThing->GetPosition();
    C3DVector pos = target;
    if (Minion.GetPItem() && !Minion.GetPItem()->Dead) {
        CThingFrame* frame = Minion.GetPItem()->Frame;
        if (frame) {
            frame->SetPosition(target);
            pos = GetValidSummonPos(Minion.GetPItem());
            frame->SetPosition(pos);
            offset = pos - ParentThing->GetPosition();
            offset.Normalise();
            frame->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), -offset));
        }
    }
    CDefPointer<CSummonSpellDef> def;
    long effect = 0;
    if (PDef->GetSubDef(&def) && def->LookupEffect("CREATURE_APPEARING", &effect))
        CreateParticleEffect(effect, pos);
}
