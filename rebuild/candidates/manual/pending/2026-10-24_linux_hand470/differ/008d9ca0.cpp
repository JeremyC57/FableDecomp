// NCreatureMode::CModeScorpionKingVulnerable::OnDeactivate() -- the scorpion king leaves its vulnerable state: close the belly hit location and reopen the body one, restore the physics radius, kill every live obstruction and the eye / sting emitters, and mark the mode inactive
// Grade: DIFFER(393v394) -- the kill re-index: retail `mov eax,[esi+0x88] ; push 1 ; lea ecx,[ebx+eax]`, ours `mov ecx,[esi+0x88] ; add ecx,ebx ; push 1` (one byte); everything else matches
// Leaf: ?OnDeactivate@CModeScorpionKingVulnerable
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D9CA0, 394 bytes (padded to 400), `ret`; vtable slot 6 of
//   NCreatureMode::CModeScorpionKingVulnerable (rtti-vtables.tsv; abs ref 0x01285844).
// Name: FableWin.pdb `?OnDeactivate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXXZ` (slot 6 = OnDeactivate, as in
//   CModeIntangible 0x008E74F0).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined accessor, type 0x7f CTCHitLocations), CCharString(const char*, long)
//   @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("HIT_LOCATION_SCORPION_KING_BELLY" 0x01285A28, "HIT_LOCATION_SCORPION_KING"
//   0x01285A0C; implicit temporaries), CTCHitLocations::GetPHitLocationFromName @ 0x00815820, CHitLocation::SetEnabled
//   @ 0x00816390, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CModeScorpionKingVulnerable (the landed Recover 0x008DF3A0 model): PCreature +0x78, bRecovering +0x7c,
//   PTarget +0x7d (packed), Obstructions (12-byte STLport vector of CIntelligentPointer<CThing>) +0x88, PEyeEmitter +0x94,
//   PStingEmitter +0x9c, FrameCount +0xa4, EffectsDone +0xa8, TailEffectIndex +0xac, CollisionRadius +0xb0; CModeBase Active
//   +0x18. CThing: PhysicsTC +0x60 (CTCPhysicsBase::Radius +0x34), Dead bit 0 of +0x91.
// Shape: the hit-location component comes from the landed `T* c = 0; if (HasTC) c = GetTC(ternary)` accessor (zero in edi);
//   the obstruction count is a named local (edi across the loop); the liveness test takes the pointer by reference (one
//   element address for both GetPItem calls), the kill re-indexes the vector.
namespace NCreatureMode {
struct CTCBase;
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float         Radius;                       // +0x34
};
struct CThing {
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];           // +0x20
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad4c[0x14];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad64[0x2d];
    unsigned char   Dead : 1;                   // +0x91 bit 0
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
    void Kill(bool immediately);                // 0x004C9B80
};
struct CThingCreatureBase : CThing {};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;                                // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};
__forceinline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() && !p.GetPItem()->Dead;
}
template <class T> struct CVector {             // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return (long)(_M_finish - _M_start); }
    T& operator[](long n) { return _M_start[n]; }
};
struct CHitLocation {
    void SetEnabled(bool enabled);              // 0x00816390
};
struct CTCHitLocations {                        // interface type 0x7F
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};

struct CModeBase {                              // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual ~CModeBase();
    CThing*       ParentThing;                  // +0x04
    unsigned char WeaponBasedFilter[0x0c];      // +0x08
    long          FramesBeforeRemovingSelf;     // +0x14
    bool          Active;                       // +0x18
    void OnDeactivate() { Active = false; }
};
struct CModeScorpionKingVulnerable : CModeBase {
    unsigned char _pad1c[0x78 - 0x1c];
    CThingCreatureBase* PCreature;              // +0x78
    bool          bRecovering;                  // +0x7c
    unsigned char PTarget[8];                   // +0x7d CIntelligentPointer<CThingCreatureBase> (packed in the PDB)
    unsigned char _pad85[3];
    CVector<CIntelligentPointer<CThing> > Obstructions;   // +0x88
    CIntelligentPointer<CThing> PEyeEmitter;    // +0x94
    CIntelligentPointer<CThing> PStingEmitter;  // +0x9c
    long          FrameCount;                   // +0xa4
    bool          EffectsDone;                  // +0xa8
    long          TailEffectIndex;              // +0xac
    float         CollisionRadius;              // +0xb0
    bool          TailCurlFinished;             // +0xb4
    void OnDeactivate();
};

void CModeScorpionKingVulnerable::OnDeactivate()
{
    CTCHitLocations* hitLocations = PCreature->GetTCIfPresent<CTCHitLocations>(0x7f);
    CHitLocation* belly = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING_BELLY");
    CHitLocation* body = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING");
    belly->SetEnabled(false);
    body->SetEnabled(true);
    PCreature->PhysicsTC->Radius = CollisionRadius;
    long count = Obstructions.size();
    for (long i = 0; i < count; ++i) {
        if (IsThingAlive(Obstructions[i]))
            Obstructions[i]->Kill(true);
    }
    if (IsThingAlive(PEyeEmitter))
        PEyeEmitter->Kill(true);
    if (IsThingAlive(PStingEmitter))
        PStingEmitter->Kill(true);
    CModeBase::OnDeactivate();
}
}
