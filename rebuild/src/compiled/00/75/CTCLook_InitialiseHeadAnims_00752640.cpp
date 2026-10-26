// CTCLook::InitialiseHeadAnims() -- build the vertical and horizontal head-turn animation entries: from the live parent creature's animations when it has them, otherwise from the appearance def's animation set (no-group filter)
// Leaf: InitialiseHeadAnims
// Retail: Fable.exe (TLC, VC7.1) @ 0x00752640, 607 bytes, plain `ret`.
// Callees: CThingCreatureBase::HasAnimation (virtual slot 73, +0x124), CThingCreatureBase::GetAnimFromName @ 0x00662A00,
//   operator new @ 0x00BFEA1A / operator delete @ 0x00BFE9BC, NAnimationEntryBodge::CAnimEntryLite(const CAnimationEntry*)
//   @ 0x0073A9B0, CCountedPointer<CAnimEntryLite>(CAnimEntryLite*) @ 0x0073D180 and operator= @ 0x00754F10, the animation
//   table's Find(name, filter*) @ 0x005DC2E0 and GetAnimation(name, index, filter&) @ 0x005DC340 (CAnimFilterNoGroup
//   vtable 0x0122FFDC).
// Data: the global head-turn animation names 0x013BACE8 (vertical) and 0x013BACE4 (horizontal) (CCharString globals).
// Layout: retail CTCLook (PDB minus 4 after the 12-byte TurningAnims vector): PParentCreature +0x10, PAppearanceDef +0x18,
//   PHeadTurnAnimH +0xBC, PHeadTurnAnimV +0xC4 (PDB +0xC0/+0xC8). CAppearanceDef Animation +0x34. No engine header.
class CCharString { public: void* PStringData; };
class CAnimationEntry;
class CAnimFilterBase { public:                                   // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
struct CAnimFilterNoGroup : CAnimFilterBase {              // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
};
class CAnimationSet { public:
    bool Find(const CCharString& name, const CAnimFilterBase* filter) const;                               // 0x005DC2E0
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
class CAppearanceDef { public:
    unsigned char _pad00[0x34];
    CAnimationSet Animation;                               // +0x34
};
class CThingCreatureBase { public:
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
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72();
    virtual bool HasAnimation(const CCharString& name, bool currentModeOnly) const;   // slot 73 (+0x124)
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Flags91;                                 // +0x91 (bit 0: dead)
    const CAnimationEntry* GetAnimFromName(const CCharString& name, long index) const;   // 0x00662A00
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->Flags91 & 1))
            return true;
    }
    return false;
}


struct CAnimEntryLite {
    unsigned char _data[0x10];
    CAnimEntryLite(const CAnimationEntry* entry);          // 0x0073A9B0
};
struct SCountBlock {
    long  m_references;                                    // +0
    void (__fastcall* m_pfnDestroy)(void*);                // +4
    void* m_pObject;                                       // +8
};
template <class T> class CCountedPointer { public:
    T*           m_pObject;                                // +0
    SCountBlock* m_pCount;                                 // +4
    CCountedPointer(T* object);                            // 0x0073D180
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00754F10
};

template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

extern CCharString g_HeadTurnAnimVName;                    // 0x013BACE8
extern CCharString g_HeadTurnAnimHName;                    // 0x013BACE4

class CTCLook { public:
    void* __vftable;                                       // +0x00
    void* ParentThing;                                     // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;                   // +0x10
    unsigned char _pad14[0x18 - 0x14];
    CDefPointer<const CAppearanceDef> PAppearanceDef;      // +0x18
    unsigned char _pad1c[0xbc - 0x1c];
    CCountedPointer<CAnimEntryLite> PHeadTurnAnimH;        // +0xbc
    CCountedPointer<CAnimEntryLite> PHeadTurnAnimV;        // +0xc4
    void InitialiseHeadAnims();
};

void CTCLook::InitialiseHeadAnims()
{
    if (IS_THING_ALIVE(PParentCreature)) {
        if (PParentCreature->HasAnimation(g_HeadTurnAnimVName, false))
            PHeadTurnAnimV = CCountedPointer<CAnimEntryLite>(new CAnimEntryLite(PParentCreature->GetAnimFromName(g_HeadTurnAnimVName, 0)));
        if (PParentCreature->HasAnimation(g_HeadTurnAnimHName, false))
            PHeadTurnAnimH = CCountedPointer<CAnimEntryLite>(new CAnimEntryLite(PParentCreature->GetAnimFromName(g_HeadTurnAnimHName, 0)));
    } else {
        if (PAppearanceDef->Animation.Find(g_HeadTurnAnimVName, &CAnimFilterNoGroup()))
            PHeadTurnAnimV = CCountedPointer<CAnimEntryLite>(new CAnimEntryLite(PAppearanceDef->Animation.GetAnimation(g_HeadTurnAnimVName, 0, CAnimFilterNoGroup())));
        if (PAppearanceDef->Animation.Find(g_HeadTurnAnimHName, &CAnimFilterNoGroup()))
            PHeadTurnAnimH = CCountedPointer<CAnimEntryLite>(new CAnimEntryLite(PAppearanceDef->Animation.GetAnimation(g_HeadTurnAnimHName, 0, CAnimFilterNoGroup())));
    }
}