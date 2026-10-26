// CAIStateGroup_ServeFood::MakeFood(position, owner) -- create an OBJECT_VILLAGE_TAVERN_APPLE thing object at the
// position for the main player, flag it (+0x91 |= 2), add CTCCreatedEntity (creator = owner) and CTCOwnedEntity
// (owner = owner) and return it
// Leaf: MakeFood
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923510, length 421 bytes (`ret 8`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CAIStateGroupBase::GetPWorld
//          @ 0x008FCFC0 (manifest "CPlayer::GetCurrentMode" is wrong), CDefinitionManager::GetDefGlobalIndexFromName
//          @ 0x009AD410, CAIStateGroupBase player-manager getter @ 0x008FCF80 (manifest "CPixelShader::CountReferences"
//          is wrong), CPlayerManager::GetMainPlayer() const @ 0x00449960 (manifest "Catch@0040d959" is wrong),
//          CreateThingObject @ 0x00703210 (__fastcall), CThing::AddTC @ 0x004C9D60, CVectorMap::LowerBound @ 0x0040F020
//          (inlined GetInterface: 0x32 = bit 18, 0x33 = bit 19 of +0x24; misses read the key slot),
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (temporary
//          CIntelligentPointer<const CThing>, vtable 0x01237F1C), CTCOwnedEntity::SetPOwner @ 0x007E8590.
// Layout: local stand-ins; CTCCreatedEntity +0x0c creator pointer (retail `lea ecx,[edi+0xc]`), CPlayer +0x28 number
//         (retail and the landed 0x00449910 scan), CWorld +0x10 definition manager (retail).
// Shape: the def-name string is a named local in a block with the manager pointer read into a local first (an inline
//         `GetPWorld()->PDefManager->Get(...)` pushes the string before the GetPWorld call); the player number is a
//         local read before the implicit "" conversion.
class CTCBase;
class CThingCreatureBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    class CTCBase* AddTC(const class CCharString& name, bool a, void* params);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing> (vtable 0x01237F1C)
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
class CRightHandedSet;
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};
class CWorld { public:
    unsigned char _pad[0x10];
    CDefinitionManager* PDefManager;            // +0x10
};
class CPlayer { public:
    unsigned char _pad[0x28];
    long Number;                                // +0x28
};
class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;       // 0x00449960
};
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharString& scriptName);   // 0x00703210
class CTCCreatedEntity { public:                       // interface 0x32
    unsigned char _pad[0x0c];
    CIntelligentPointerConstThing PCreator;     // +0x0c
};
class CTCOwnedEntity { public:                         // interface 0x33
    void SetPOwner(CThing* owner);              // 0x007E8590
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    const CPlayerManager* GetPPlayerManager() const;    // 0x008FCF80
    CWorld* GetPWorld() const;                  // 0x008FCFC0
};
struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    CThing* MakeFood(const C3DVector* position, CThing* owner);
};

CThing* CAIStateGroup_ServeFood::MakeFood(const C3DVector* position, CThing* owner)
{
    long def;
    {
        CCharString name("OBJECT_VILLAGE_TAVERN_APPLE");
        CDefinitionManager* defs = GetPWorld()->PDefManager;
        def = defs->GetDefGlobalIndexFromName(name);
    }
    long player = GetPPlayerManager()->GetMainPlayer().Number;
    CThing* food = CreateThingObject(def, position, player, 0, 0, "");
    food->m_flags91 |= 2;
    food->AddTC("CTCCreatedEntity", false, 0);
    CTCCreatedEntity* created;
    food->GetInterface(&created, 0x32);
    created->PCreator = CIntelligentPointerConstThing(owner);
    food->AddTC("CTCOwnedEntity", false, 0);
    CTCOwnedEntity* owned;
    food->GetInterface(&owned, 0x33);
    owned->SetPOwner(owner);
    return food;
}