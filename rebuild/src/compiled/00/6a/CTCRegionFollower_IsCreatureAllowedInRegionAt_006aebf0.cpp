// CTCRegionFollower::IsCreatureAllowedInRegionAt(const CThing* creature, const C3DVector& position) -- a creature with a def may follow into the region at a position unless its def says it is not allowed in protected towns and the region's def is a protected town
// Leaf: IsCreatureAllowedInRegionAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AEBF0, 248 bytes, `ret 8`.
// Callees (CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, GetSubDefIndex slot 14 and
//   CDefinitionManagerBase::GetPDefFromGlobalIndex 0x009ADA10 only inside the stand-in): CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) @ 0x00485815, CTCBase::PeekWorldMap() const
//   @ 0x00686D20 (three times), CWorldMap::GetMapNumberAt(const C3DVector&) const (virtual slot 16, +0x40),
//   CWorldMap::GetRegionNumberMapIsIn(long) const @ 0x004FC190, CWorldMap::GetRegion(long) const @ 0x004FC180; the defs'
//   virtual slot 1 on the last release.
// Layout: the creature's def pointer at +0x70; retail CCreatureDef AllowedInProtectedTowns +0xCC (PDB 0xDC); CRegion PDef +0x24
//   (PDB 0x2C, 8 lower behind the two 12-byte vectors); CRegionDef protected-town flag +0x70 (named descriptively).
// Shape: the creature def is read twice through an inline by-value getter (copy +1, temporary release -1): once for the
//   null test (bool kept in bl), once as the GetSubDef `this`; the CCreatureDef and CRegionDef pointers are CDefPointer
//   locals (copy-construct +1, release on every exit; the dereferenced region def's null test folds). GetSubDef is the
//   same-TU stand-in under auto_inline(off) (the landed 0x00485815 body), so VC7.1 keeps the filled def in esi as retail's
//   LTCG did. The copy constructor bumps the count through a `long&` local (`count = count + 1`): a plain `++Object->m_refCount`
//   lets VC7.1 hoist the region def's flag load above the count store, retail loads it after.
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class C3DVector { public: float X, Y, Z; };
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) { Object = other.Object; if (Object) { long& count = Object->m_refCount; count = count + 1; } }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_refCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefinitionManagerBase {
    CDefPointer<CDefPointee> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CCreatureDef : public CDefPointee { public:
    unsigned char _pad08[0xcc - 0x08];
    bool AllowedInProtectedTowns;                       // +0xcc (PDB 0xdc)
};
class CDefClassBase : public CDefPointee { public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManagerBase* m_pManager;                 // +0x1c
    bool GetSubDef(CDefPointer<CCreatureDef>* out);     // 0x00485815 (GetSubDef<CCreatureDef>)
};
#pragma auto_inline(off)
// Same-TU stand-in: the landed body of GetSubDef<CCreatureDef> 0x00485815 (retail's LTCG knowledge that the out pointer
// does not escape; precedent NEntityEvents::CEventKnockedOut::OnAdd 0x00811470).
bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCreatureDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CDefPointee> base = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        CDefPointer<CCreatureDef> result;
        result.Object = (CCreatureDef*)base.Object;
        if (result.Object)
            ++result.Object->m_refCount;
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CRegionDef : public CDefPointee { public:
    unsigned char _pad08[0x70 - 0x08];
    bool ProtectedTown;                                 // +0x70
};
class CRegion { public:
    unsigned char _pad00[0x24];
    CDefPointer<CRegionDef> PDef;                       // +0x24
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& position) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long mapNumber) const;  // 0x004FC190
    const CRegion& GetRegion(long region) const;        // 0x004FC180
};
class CThing {
public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                    // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
};
class CTCBase
{
protected:
    const CWorldMap& PeekWorldMap() const;   // 0x00686D20
};

class CTCRegionFollower : public CTCBase { public:
    void*   __vftable;                                  // +0x00 (CTCBase)
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x4];
    bool IsCreatureAllowedInRegionAt(const CThing* creature, const C3DVector& position) const;
};

bool CTCRegionFollower::IsCreatureAllowedInRegionAt(const CThing* creature, const C3DVector& position) const
{
    bool hasDef = creature->GetPDef().IsValid();
    if (hasDef) {
        CDefPointer<CCreatureDef> creatureDef;
        bool ok = creature->GetPDef()->GetSubDef(&creatureDef);
        if (ok && !creatureDef->AllowedInProtectedTowns) {
            const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(position)));
            CDefPointer<CRegionDef> regionDef = region.PDef;
            if (regionDef.IsValid() && regionDef->ProtectedTown)
                return false;
        }
    }
    return true;
}