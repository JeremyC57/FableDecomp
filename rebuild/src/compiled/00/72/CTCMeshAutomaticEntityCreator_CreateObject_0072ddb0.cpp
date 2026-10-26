// CTCMeshAutomaticEntityCreator::CreateObject(const CCharString& dummyName, const CCharString& objectName, CTCGraphicAppearance* appearance) -- create the named object at every dummy of that name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072DDB0, length 403 bytes (+ int3 padding), esp frame, `ret 0Ch`.
// Callees: CTCGraphicAppearance::GetNoDummyObjects @ 0x004BE000 (re-evaluated per pass) / GetDummyObject @ 0x004C08C0 /
// SetScale @ 0x004BFA50, the CTCBase accessors GetWorldMap @ 0x00686D20 and GetDefinitionManager @ 0x006869D0 (manifest
// labels "CViewBase::GetDisplayEngine" / "_Cons_val" are propagated), CWorldMap::IsPosInMap @ 0x004FC600,
// CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, the `__fastcall` thing factory Create @ 0x00703210,
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::AddTC @ 0x004C9D60,
// CVectorMap::LowerBound @ 0x0040F020 (inline GetInterface 0x32 = CTCCreatedEntity, bitset word +0x24 bit 18),
// CTCCreatedEntity::SetPCreator @ 0x005ED440 (label "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated), and the
// same-TU AddOwnedEntityTC @ 0x0072DD60 (landed; called here, inlined in CreateParticle 0x0072E130).
// Strings: "" @ 0x0122D70E, "CTCCreatedEntity" @ 0x0123AA60.
// Shape: the empty script name is a NAMED CCharString in its own block around the factory call (an unnamed temporary
// argument pushes the ctor's eax and lands in another frame slot); the creator's scale is copied through an inline
// by-value getter (integer move into a frame float, then `fld`); the created-entity lookup is the out-pointer GetInterface
// whose key slot doubles as the result (no zero store when the interface is absent).
// Layout (local model): CThing bitset +0x20, interface map +0x44, PGraphicAppearance +0x64, flags +0x91 (bit 1 = created
// by owner); CTCGraphicAppearance scale +0x34; the component's owning thing +0x04.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector First; C3DVector Second; };
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
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
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x91 - 0x68];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead, bit 1: created by owner
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
                               unsigned long flags, const CCharString& scriptName);   // 0x00703210
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
    const CWorldMap& PeekWorldMap() const;   // 0x00686D20
};
struct CTCMeshAutomaticEntityCreator : CTCBase {
    void AddOwnedEntityTC(CThing* created);           // 0x0072DD60
    void CreateObject(const CCharString& dummyName, const CCharString& objectName, CTCGraphicAppearance* appearance);
};

void CTCMeshAutomaticEntityCreator::CreateObject(const CCharString& dummyName, const CCharString& objectName,
                                                 CTCGraphicAppearance* appearance)
{
    for (long i = 0; i < appearance->GetNoDummyObjects(dummyName, 1.0f); ++i) {
        C3DVector position;
        CRightHandedSet orientation;
        appearance->GetDummyObject(dummyName, i, 0, &position, &orientation, 1.0f);
        if (PeekWorldMap().IsPosInMap(position)) {
            long defIndex = GetDefinitionManager().GetDefGlobalIndexFromName(objectName);
            if (defIndex) {
                CThing* object;
                {
                    CCharString scriptName("");
                    object = CreateThing(defIndex, &position, -1, &orientation, 0, scriptName);
                }
                if (object) {
                    float scale = ParentThing->PGraphicAppearance->GetScale();
                    if (scale != 1.0f)
                        object->PGraphicAppearance->SetScale(scale);
                    object->m_flags91 |= 2;
                    object->AddTC("CTCCreatedEntity", false, 0);
                    CTCCreatedEntity* created;
                    object->GetInterface(&created, 0x32);
                    created->SetPCreator(ParentThing);
                    AddOwnedEntityTC(object);
                }
            }
        }
    }
}