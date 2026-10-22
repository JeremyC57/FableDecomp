// NCreatureMode::CModeStrafe::SetUpCombinationAnims() -- for each of the four combination animations (PROJECTILE_WEAPON_AIM,
//   _BUILDUP, _PULLBACK, STRAFE_UPPER_BODY): drop the cached counted pointer, then, if the creature has the animation (in any mode),
//   cache a CAnimEntryLite built from its animation entry
// Leaf: SetUpCombinationAnims
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9190, 705 bytes (`ret`).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CThingCreatureBase virtual slot 73 (+0x124, HasAnimation(name, bool) -- the landed
//   CThingAICreature 0x00830F20), CThingCreatureBase::GetAnimFromName(const CCharString&, long) 0x00662A00, operator new 0x00BFEA1A,
//   CAnimEntryLite::CAnimEntryLite(const CAnimationEntry&) 0x0073A9B0, CCountedPointer<CAnimEntryLite>::Reset(T*) 0x00863090,
//   operator delete 0x00BFE9BC (the inline count-block release).
// Data: "PROJECTILE_WEAPON_AIM" 0x01265684, "PROJECTILE_WEAPON_BUILDUP" 0x01287CA0, "PROJECTILE_WEAPON_PULLBACK" 0x01287C84,
//   "STRAFE_UPPER_BODY" 0x01287C70.
// Layout: Ego_r.pdb CModeStrafe: PAnimProjectileAim +0x50, PAnimProjectileBuildUp +0x58, PAnimProjectilePullBack +0x60,
//   PAnimStrafeUpperBody +0x68 (CCountedPointer {object, count block}; count block {refs, destroy fn, object} as the landed Reset).
// Shape: the parent creature is a named local read first; the clear is an inline member (release re-reads the block after the
//   decrement, then zeroes block and object); the names are passed as `const char*` so each CCharString is built before the pushes;
//   the animation entry is its own statement so the name temporary dies before operator new.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CAnimationEntry {
    long BankIndex;
    unsigned char _rest[0xc];
};
struct CAnimEntryLite {
    long BankIndex, DelayTime, TransitionTime, AnimFlags;
    CAnimEntryLite(const CAnimationEntry& entry);       // 0x0073A9B0
};
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};
template <class T> struct CCountedPointer {
    T* m_pObject;                          // +0
    SCountBlock* m_pCount;                 // +4
    void Reset(T* object);                 // 0x00863090
    void Clear()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};
struct CThingCreatureBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72();
    virtual bool HasAnimation(const CCharString& name, bool currentModeOnly);   // slot 73 (+0x124)
    CAnimationEntry* GetAnimFromName(const CCharString& name, long flags);      // 0x00662A00
};
struct CModeBase {
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char _body[0x14];
};
namespace NCreatureMode {
struct CModeStrafe : CModeBase {
    unsigned char _pad1c[0x50 - 0x1c];
    CCountedPointer<CAnimEntryLite> PAnimProjectileAim;        // +0x50
    CCountedPointer<CAnimEntryLite> PAnimProjectileBuildUp;    // +0x58
    CCountedPointer<CAnimEntryLite> PAnimProjectilePullBack;   // +0x60
    CCountedPointer<CAnimEntryLite> PAnimStrafeUpperBody;      // +0x68
    void SetUpCombinationAnims();
};

void CModeStrafe::SetUpCombinationAnims()
{
    CThingCreatureBase* creature = ParentThing;
    PAnimProjectileAim.Clear();
    if (creature->HasAnimation("PROJECTILE_WEAPON_AIM", true)) {
        CAnimationEntry* entry = creature->GetAnimFromName("PROJECTILE_WEAPON_AIM", 0);
        PAnimProjectileAim.Reset(new CAnimEntryLite(*entry));
    }
    PAnimProjectileBuildUp.Clear();
    if (creature->HasAnimation("PROJECTILE_WEAPON_BUILDUP", true)) {
        CAnimationEntry* entry = creature->GetAnimFromName("PROJECTILE_WEAPON_BUILDUP", 0);
        PAnimProjectileBuildUp.Reset(new CAnimEntryLite(*entry));
    }
    PAnimProjectilePullBack.Clear();
    if (creature->HasAnimation("PROJECTILE_WEAPON_PULLBACK", true)) {
        CAnimationEntry* entry = creature->GetAnimFromName("PROJECTILE_WEAPON_PULLBACK", 0);
        PAnimProjectilePullBack.Reset(new CAnimEntryLite(*entry));
    }
    PAnimStrafeUpperBody.Clear();
    if (creature->HasAnimation("STRAFE_UPPER_BODY", true)) {
        CAnimationEntry* entry = creature->GetAnimFromName("STRAFE_UPPER_BODY", 0);
        PAnimStrafeUpperBody.Reset(new CAnimEntryLite(*entry));
    }
}
}