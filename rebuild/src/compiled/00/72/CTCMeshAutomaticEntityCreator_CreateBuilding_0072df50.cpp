// CTCMeshAutomaticEntityCreator::CreateBuilding(const CCharString& dummyName, const CCharString& buildingName, CTCGraphicAppearance* appearance) -- create the named building at every dummy of that name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072DF50, length 471 bytes (+ int3 padding), esp frame, `ret 0Ch`.
// Callees: as CreateObject 0x0072DDB0, plus the `__fastcall` building factory Create @ 0x0082D920, the CTCBase accessor
// GetThingManager @ 0x00686AE0 (`mov eax,[0x013B8A1C] ; ret`; label "GFGetPreMainMemoryUsed" is propagated) and the
// CTCPhysicsBase virtuals slot 32 (+0x80, set orientation), slot 93 (+0x174, GetPhysicsMeshInfo) and slot 85 (+0x154, takes
// the mesh info's first word zero-extended; names descriptive).
// Strings: "" @ 0x0122D70E, "CTCCreatedEntity" @ 0x0123AA60.
// Shape: the sibling of CreateObject. A failed lookup / factory call / dead building falls through to the thing manager's
// failure flag (+0xC8); success ends with `continue` (the `jmp` to the loop increment).
// Layout (local model): as CreateObject, plus CThing PTCPhysics +0x60.
class CTCBase;
class CTCPhysicsBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector First; C3DVector Second; };
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CTCParamsBase;
struct CTCGraphicAppearance {
    unsigned char _pad00[0x34];
    float         m_scale;                            // +0x34
    float GetScale() const { return m_scale; }
    long GetNoDummyObjects(const CCharString& name, float scale);   // 0x004BE000
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
    void SetScale(float scale);                       // 0x004BFA50
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PTCPhysics;                       // +0x60

    CTCGraphicAppearance* PGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x91 - 0x68];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead, bit 1: created by owner
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateBuildingThing(long defIndex, const C3DVector* position, long player, const CCharString& scriptName);   // 0x0082D920
class CThingPhysicsMeshInfo { public:
    unsigned short m_type;                            // +0x00
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void SetRHSet(const CRightHandedSet& orientation);   // slot 32 (+0x80)
    virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void SetPhysicsMeshType(unsigned short type);        // slot 85 (+0x154)
    virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
};
class CThingManager { public:
    unsigned char _pad00[0xc8];
    bool          m_creationFailed;                   // +0xC8
};
class CTCCreatedEntity { public:                             // interface 0x32
    void SetPCreator(CThing* creator);                // 0x005ED440
};
class CWorldMap { public:
    bool IsPosInMap(const C3DVector& pos) const;            // 0x004FC600
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};
class CTCBase { public:
    void*   __vftable;                                // +0x00
    CThing* ParentThing;                              // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    CThingManager& GetThingManager();   // 0x00686AE0
    const CWorldMap& PeekWorldMap() const;   // 0x00686D20
};
struct CTCMeshAutomaticEntityCreator : CTCBase {
    void AddOwnedEntityTC(CThing* created);           // 0x0072DD60
    void CreateBuilding(const CCharString& dummyName, const CCharString& buildingName, CTCGraphicAppearance* appearance);
};

void CTCMeshAutomaticEntityCreator::CreateBuilding(const CCharString& dummyName, const CCharString& buildingName,
                                                 CTCGraphicAppearance* appearance)
{
    for (long i = 0; i < appearance->GetNoDummyObjects(dummyName, 1.0f); ++i) {
        C3DVector position;
        CRightHandedSet orientation;
        appearance->GetDummyObject(dummyName, i, 0, &position, &orientation, 1.0f);
        if (PeekWorldMap().IsPosInMap(position)) {
            long defIndex = GetDefinitionManager().GetDefGlobalIndexFromName(buildingName);
            if (defIndex) {
                CThing* building;
                {
                    CCharString scriptName("");
                    building = CreateBuildingThing(defIndex, &position, -1, scriptName);
                }
                if (building && !(building->m_flags91 & 1)) {
                    float scale = ParentThing->PGraphicAppearance->GetScale();
                    if (scale != 1.0f)
                        building->PGraphicAppearance->SetScale(scale);
                    building->PTCPhysics->SetRHSet(orientation);
                    const CThingPhysicsMeshInfo* info = building->PTCPhysics->GetPhysicsMeshInfo();
                    if (info)
                        building->PTCPhysics->SetPhysicsMeshType(info->m_type);
                    building->m_flags91 |= 2;
                    building->AddTC("CTCCreatedEntity", false, 0);
                    CTCCreatedEntity* created;
                    building->GetInterface(&created, 0x32);
                    created->SetPCreator(ParentThing);
                    AddOwnedEntityTC(building);
                    continue;
                }
            }
            GetThingManager().m_creationFailed = true;
        }
    }
}