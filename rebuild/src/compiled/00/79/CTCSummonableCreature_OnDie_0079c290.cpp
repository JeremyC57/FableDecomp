// CTCSummonableCreature::OnDie() -- when the dying creature was killed by a summoned creature of no higher summon level and of a different def, hand the dead creature's def to the killer's summoner's summon spell, at the dead creature's position pushed along its velocity and facing back against it
// Leaf: OnDie
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079C290, 336 bytes (`ret`); CTCSummonableCreature vtable 0x0126C41C slot 13.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2); CVectorMap::LowerBound 0x0040F020 (non-const copy:
//          CTCSummonableCreature 0xa0, CTCSummonSpell 0x91); the owner's physics virtual slot 70 (+0x118, a C3DVector by value;
//          descriptive name); CThing::GetPosition 0x004C73D0; CTCSummonSpell::SetSummonCreatureGlobalDefNum 0x0079B650.
// Layout: CTCSummonableCreature PDef +0x0c, HasBeenSummoned +0x10, Summoner +0x11 (PDB, packed); the def's summon level +0x28
//          (descriptive); retail CThing interface bitset +0x20, map +0x44, physics +0x60, flags +0x6c (bit 3: dead/killed),
//          def index word +0x8c, killer intelligent pointer +0xf4 (descriptive).
// Shape: the spawn position is a named sum computed before the call, so the negated velocity (third argument) is formed
//          after it; the sum is velocity + position (Z first through the inline ctor).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
class CBaseObject;
struct CBaseIntelligentPointer {
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
struct CPhysicsComponent {
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetVelocity() const;          // slot 70 (+0x118)
};
class CThing {
public:
    unsigned char           _pad00[0x20];
    unsigned long           m_presentBits[9];       // +0x20
    CInterfaceMap           m_interfaces;           // +0x44
    unsigned char           _pad4c[0x60 - 0x4c];
    CPhysicsComponent*      Physics;                // +0x60
    unsigned char           _pad64[0x6c - 0x64];
    unsigned char           m_flags6c;              // +0x6c
    unsigned char           _pad6d[0x8c - 0x6d];
    unsigned short          DefIndex;               // +0x8c
    unsigned char           _pad8e[0xf4 - 0x8e];
    CBaseIntelligentPointer Killer;                 // +0xf4
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
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
class CSummonableCreatureDef { public:
    unsigned char _pad00[0x28];
    float         SummonLevel;                      // +0x28 (descriptive)
};
class CTCSummonSpell { public:
    void SetSummonCreatureGlobalDefNum(long defIndex, const C3DVector& pos, const C3DVector& facing);   // 0x0079B650
};
#pragma pack(push, 1)
class CTCSummonableCreature {
public:
    virtual void v00();
    CThing*                       ParentThing;      // +0x04
    long                          _base08;          // +0x08
    const CSummonableCreatureDef* PDef;             // +0x0c
    bool                          HasBeenSummoned;  // +0x10
    CBaseIntelligentPointer       Summoner;         // +0x11
    void OnDie();
};
#pragma pack(pop)

void CTCSummonableCreature::OnDie()
{
    CThing* owner = ParentThing;
    if (owner->m_flags6c & 8) {
        CThing* killer = (CThing*)owner->Killer.GetPItem();
        CTCSummonableCreature* killerSummonable;
        if (killer && killer->GetInterface(&killerSummonable, 0xa0) && killerSummonable->HasBeenSummoned
            && PDef->SummonLevel >= killerSummonable->PDef->SummonLevel) {
            long defIndex = owner->DefIndex;
            if (defIndex != killer->DefIndex) {
                CThing* summoner = (CThing*)killerSummonable->Summoner.GetPItem();
                CTCSummonSpell* spell;
                if (summoner->GetInterface(&spell, 0x91)) {
                    C3DVector velocity = owner->Physics->GetVelocity();
                    C3DVector spawnPos = velocity + owner->GetPosition();
                    spell->SetSummonCreatureGlobalDefNum(defIndex, spawnPos, -velocity);
                }
            }
        }
    }
}