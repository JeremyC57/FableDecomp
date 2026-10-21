// Grade: DIFFER(449v437) -- register colouring only: retail this=ebp, creature=ebx (pushed late) where ours swaps them, so ours duplicates the first early-out epilogue and the X sum operand order flips; shape, frame slots and the named ahead/approach order match
// CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable() -- the live, visible attack target is moving away along our facing faster than half its slot-81 measure and lies 3..5.5 units away (2D): remember it in PTarget
// Leaf: IsRunnable@CAIStateGroup_BalverineJumpAtFleeingEnemy
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLengthSquared2D() const { return X * X + Y * Y; }
};
struct CThingPlacement {
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
    virtual C3DVector GetFacing() const;        // slot 70 (+0x118)
    unsigned char _pad04[0x8];
    C3DVector Position;                         // +0x0c
    C3DVector LastPosition;                     // +0x18
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThingVirtuals {
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
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80();
    virtual float Slot81(const C3DVector& point) const;   // slot 81 (+0x144)
};
struct CThing : CThingVirtuals {
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPlacement* Placement;                 // +0x60 (PDB PhysicsTC +0x70; retail CThing is 0x10 smaller)
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
struct CTCAttackTarget {                        // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget();                // slot 29 (+0x74)
};
bool __fastcall CanCreatureSeeCreature(const CThing* viewer, const CThing* target, bool ignoreLight);   // 0x008645E0
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CThing* m_pItem;
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(CThing* item);    // 0x00668B60
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CIntelligentPointerCreature& operator=(const CIntelligentPointerConstThing& other);   // 0x00668B80
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* Creature; };
struct BalverineMethods : CAIStateGroup_BalverineJumpAtFleeingEnemy {
    bool IsRunnable();
    __forceinline CThing* GetCreature() { return (*(CAIBrain**)_pad_0x04)->Creature; }
    __forceinline CIntelligentPointerCreature& Target() { return *(CIntelligentPointerCreature*)PTarget; }
};
bool BalverineMethods::IsRunnable()
{
    CTCAttackTarget* targeting;
    if (!GetCreature()->GetInterface(&targeting, 8))
        return false;
    CThing* enemy = targeting->GetTarget();
    if (!enemy || (enemy->m_flags91 & 1))
        return false;
    if (!CanCreatureSeeCreature(GetCreature(), enemy, false))
        return false;
    CThingPlacement* enemyPlacement = enemy->Placement;
    C3DVector enemyMovement = enemyPlacement->Position - enemyPlacement->LastPosition;
    C3DVector enemyFacing = enemyPlacement->GetFacing();
    C3DVector facing = GetCreature()->Placement->GetFacing();
    C3DVector toEnemy = enemyPlacement->Position - GetCreature()->Placement->Position;
    C3DVector ahead = GetCreature()->GetPosition() + GetCreature()->Placement->GetFacing();
    float approach = facing * enemyMovement;
    if (!(approach > enemy->Slot81(ahead) * 0.5f))
        return false;
    float distanceSquared = toEnemy.GetLengthSquared2D();
    if (distanceSquared < 30.25f && distanceSquared > 9.0f) {
        Target() = enemy;
        return true;
    }
    return false;
}
