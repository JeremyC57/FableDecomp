// CTCGuard::OnDie() -- a guard holding bribe money drops it: create a small gold bag at his position, put the bribe pool in its money component and throw the bag off in a random direction
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: OnDie
//
// Retail 0x007B2AB0, 320 bytes incl. padding (plain `ret`). Ego_r.pdb `?OnDie@CTCGuard@@UAEXXZ`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("OBJECT_GOLDBAG_SMALL_WITH_COINS" @ 0x0125D760, "" @ 0x0122D70E),
// ~CCharString @ 0x0099EAE0, CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName
// @ 0x009AD410, CThing::GetPos @ 0x004C73D0, CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*,
// unsigned long, const CCharString&) @ 0x00703210 (static __fastcall), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface for CTCMoney, type 0xCC = bit 12 of the bitset word +0x38, `test ah,10h`),
// CTCBase::GetPWorldSeed @ 0x00686DA0, GFFloatRandom(float, unsigned long*) @ 0x004978A0 (__fastcall), C3DVector::RotateXY
// @ 0x00A14040, CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130.
// Layout: Ego_r.pdb CTCGuard: PDef +0x0c, BribePool +0x10; CTCMoney gold +0x0c (landed model); retail CThing bitset +0x20,
// interface map +0x44, PhysicsTC +0x60, dead bit +0x91; the physics component's velocity at +0x28.
// Shape: the def name is a named local in its own block (the definition-manager getter is called after the name is
// pushed), the empty script name an implicit conversion temporary (built first, re-addressed with `lea`); the money component is null-tested after the lookup; the throw velocity is a C3DVector(0.3, 0, 0.4) local rotated
// by a random angle and copied into the physics component (a named local) with the implicit assignment (`lea edx,[ecx+28h]` then three dword stores).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void RotateXY(float angle);                             // 0x00A14040
};
class CRightHandedSet;

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CTCPhysicsBase { public:
    unsigned char _pad00[0x28];
    C3DVector     Velocity;                                 // +0x28
    void SetAsMoving(bool moving);                          // 0x006B0130
};

struct CTCMoney {
    unsigned char _pad00[0x0c];
    long          Gold;                                     // +0x0c
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                                  // +0x91 bit 0 Dead
    const C3DVector& GetPos() const;                   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
class CThingObject : public CThing {
public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CThing* GetParentThing() const { return ParentThing; }
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    unsigned long* GetPWorldSeed() const;   // 0x00686DA0
};

class CGuardDef;
class CTCGuard : public CTCBase {
public:
    const CGuardDef* PDef;                                  // +0x0c CDefPointer<CGuardDef const>
    long BribePool;                                         // +0x10
    void OnDie();
};

void CTCGuard::OnDie()
{
    if (BribePool > 0) {
        long index;
        {
            CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS", -1);
            index = GetDefinitionManager().GetDefGlobalIndexFromName(name);
        }
        if (index > 0) {
            CThingObject* bag = CThingObject::Create(index, GetParentThing()->GetPos(), 5, 0, 0, "");
            if (bag && !(bag->Flags91 & 1)) {
                CTCMoney* money;
                if (bag->GetInterface(&money, 0xcc) && money)
                    money->Gold = BribePool;
                C3DVector velocity(0.3f, 0.0f, 0.4f);
                velocity.RotateXY(GFFloatRandom(1.0f, GetPWorldSeed()));
                CTCPhysicsBase* physics = bag->PhysicsTC;
                physics->Velocity = velocity;
                physics->SetAsMoving(true);
            }
        }
    }
}