// CTCSmashableBase::Smash() -- smash the parent thing: when its smashable def names a replacement object, create that object at the thing's position and orientation (same map level), hand a live replacement's smashable component the last hit frame, then kill the thing
// Leaf: Smash
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EED20, 275 bytes (plain `ret`).
// Callees: the physics body's GetRHSet (slot 72, +0x120, const CRightHandedSet&), CCharString(const char*, long) @ 0x0099EBF0
//   ("" @ 0x0122D70E), CThing::GetPosition() const @ 0x004C73D0, Create(long, const C3DVector*, long, const CRightHandedSet*,
//   unsigned long, const CCharString&) @ 0x00703210 (`__fastcall`), ~CCharString @ 0x0099EAE0, CVectorMap::LowerBound
//   @ 0x0040F020 (interface 0x4E), CThing::Kill(bool) @ 0x004C9B80; the def's virtual slot 1 on the last release.
// Layout: Ego_r.pdb CTCSmashableBase (the landed OnInitialActivate 0x007EECF0 model): LastWorldFrameHit +0x0C, Definition
//   +0x14; CSmashableDef ReplacementObject +0x28; retail CThing interface bitset +0x20 (0x4E = word +0x28 bit 14), map +0x44,
//   map level byte +0x90, dead bit +0x91, PhysicsTC +0x60.
// Shape: `GetDef()->ReplacementObject` through the by-value def temporary (null-tested add-ref, release from the def's own
//   Release) read twice; the orientation is a local copy, the empty name a named local; the created object's component is
//   the HasTC + end/hit ternary lookup.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCPhysicsBase { public:
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;    // slot 72 (+0x120)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x90 - 0x64];
    signed char     MapLevel;                           // +0x90
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (HasTC(type)) {
            *out = GetTC<T>(type);
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CThingObject : public CThing {};
CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long mapLevel, const CRightHandedSet* orientation,
                                unsigned long flags, const CCharString& scriptName);   // 0x00703210
class CSmashableDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          m_refCount;                           // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
    unsigned char _pad08[0x25 - 0x08];
    bool          Smashable;                            // +0x25
    long          ReplacementObject;                    // +0x28 CDefIndex
};
struct CDefPointer {
    CSmashableDef* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { m_pDef->Release(); }
    CSmashableDef* operator->() const { return m_pDef; }
};
struct CTCSmashableComponent {                          // interface 0x4E (named descriptively)
    unsigned char _pad00[0x0c];
    long          LastWorldFrameHit;                    // +0x0c
};
class CTCSmashableBase { public:
    void*       __vftable;                              // +0x00 (CTCBase)
    CThing*     ParentThing;                            // +0x04
    bool        ToBeRemoved;                            // +0x08
    bool        IsInGlobalUpdateTCs;                    // +0x09
    long        LastWorldFrameHit;                      // +0x0c
    bool        Smashed;                                // +0x10
    CDefPointer Definition;                             // +0x14
    CDefPointer GetDef() const { return Definition; }
    void Smash();
};

void CTCSmashableBase::Smash()
{
    long replacement = GetDef()->ReplacementObject;
    if (replacement > 0) {
        CRightHandedSet orientation = ParentThing->PhysicsTC->GetRHSet();
        CThingObject* object;
        {
            CCharString scriptName("", -1);
            CThing* thing = ParentThing;
            long objectDef = GetDef()->ReplacementObject;
            object = Create(objectDef, &thing->GetPosition(), thing->MapLevel, &orientation, 0, scriptName);
        }
        CTCSmashableComponent* smashable;
        if (object && !(object->m_flags91 & 1) && object->GetInterface(&smashable, 0x4e))
            smashable->LastWorldFrameHit = LastWorldFrameHit;
    }
    ParentThing->Kill(true);
}