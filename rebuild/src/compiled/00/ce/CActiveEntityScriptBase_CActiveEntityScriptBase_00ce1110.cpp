// CActiveEntityScriptBase::CActiveEntityScriptBase(const CScriptThing&, unsigned __int64, unsigned long) -- spawned entity-script function bound to an entity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE1110, length 153 bytes (`ret 10h`).
// Callees: the entity's virtual slot 1 (+0x04, its name as a const CCharString&), the process root constructor
// (long, long, float) @ 0x00A44740 (called with 1, 0x40, 0.1f; the landed CLevelLoader 0x006C26B0 calls it
// too, its catalog label "CCreatureAction_CastBattleCharge" is propagated), CCharString::CCharString(const
// CCharString&) @ 0x0099EC30, CCharString::CCharString() @ 0x0099E4B0. Vtables: 0x012C2F9C (CSpawnedFuncBase)
// and 0x012C3594 (this class), no RTTI; 0x01238C8C = CScriptThing (rtti-vtables.tsv).
//
// Shape: the entity's name is fetched first (the argument of the inline CSpawnedFuncBase constructor, kept in
// ebx across the root constructor). CSpawnedFuncBase (PDB layout, 0x34 bytes): its vtable, ID = 1,
// LastScriptFrame = CurrentScriptFrame = -1 (`or eax,-1`), Name copied, AssociatedRegionName default-built,
// then FuncPaused/Interrupted and the PPredicate counted pointer zeroed from one `xor eax,eax` before the
// derived vtable store. This class (PDB layout, 0x58 bytes): PEntityScript zeroed, Flags, EntityUID (the
// 64-bit argument's two words) and Entity copy-constructed inline (only the CScriptThing vtable store
// survives; the counted pointer's info block is add-ref'd behind a null test).
// The CSpawnedFuncBase constructor must be `__forceinline` (a plain inline one is emitted out of line).
// Leaf: ??0CActiveEntityScriptBase

struct CCharString {
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    CCharString(const CCharString& other);      // 0x0099EC30
};

struct SCountBlock {
    long m_count;                               // +0x00
};

struct CScriptThingImpPointer {                 // CCountedPointer<CScriptThing>: data + shared count block
    void* PImp_Data;
    SCountBlock* PImp_Info;
    CScriptThingImpPointer(const CScriptThingImpPointer& other) : PImp_Data(other.PImp_Data), PImp_Info(other.PImp_Info)
    {
        if (PImp_Info != 0)
            ++PImp_Info->m_count;
    }
};

struct CBaseObject {                            // root of CScriptThing (vtable 0x01231710)
    virtual void v00();
};

struct CScriptThing : CBaseObject {             // vtable 0x01238C8C, 12 bytes
    CScriptThingImpPointer PImp;                // +0x04
    virtual void v00();
    virtual const CCharString& GetName() const; // slot 1 (+0x04)
};

struct CPredicatePointer {                      // CCountedPointer<NScript::CPredicateBase const>
    void* Data;
    void* Info;
    CPredicatePointer() : Data(0), Info(0) { }
};

struct CEntityScriptPointer {                   // CCountedPointer<CEntityScriptBase>
    void* Data;
    void* Info;
    CEntityScriptPointer() : Data(0), Info(0) { }
};

struct CScriptProcessBase {                     // the process root (0x14 bytes)
    virtual void v00();
    unsigned char _body[0x10];                  // +0x04
    CScriptProcessBase(long a, long size, float fraction);   // 0x00A44740
};

struct CSpawnedFuncBase : CScriptProcessBase {  // vtable 0x012C2F9C, 0x34 bytes
    unsigned long ID;                           // +0x14
    long LastScriptFrame;                       // +0x18
    long CurrentScriptFrame;                    // +0x1c
    CCharString Name;                           // +0x20
    CCharString AssociatedRegionName;           // +0x24
    bool FuncPaused;                            // +0x28
    bool Interrupted;                           // +0x29
    CPredicatePointer PPredicate;               // +0x2c
    virtual void v00();
    __forceinline CSpawnedFuncBase(const CCharString& name)
        : CScriptProcessBase(1, 0x40, 0.1f), ID(1), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name),
          AssociatedRegionName(), FuncPaused(false), Interrupted(false), PPredicate() { }
};

struct CActiveEntityScriptBase : CSpawnedFuncBase {   // vtable 0x012C3594, 0x58 bytes
    CEntityScriptPointer PEntityScript;         // +0x34
    unsigned long Flags;                        // +0x3c
    unsigned __int64 EntityUID;                 // +0x40
    CScriptThing Entity;                        // +0x48
    virtual void v00();
    CActiveEntityScriptBase(const CScriptThing& entity, unsigned __int64 entityUID, unsigned long flags);
};

CActiveEntityScriptBase::CActiveEntityScriptBase(const CScriptThing& entity, unsigned __int64 entityUID,
                                                 unsigned long flags)
    : CSpawnedFuncBase(entity.GetName())
    , PEntityScript()
    , Flags(flags)
    , EntityUID(entityUID)
    , Entity(entity)
{
}