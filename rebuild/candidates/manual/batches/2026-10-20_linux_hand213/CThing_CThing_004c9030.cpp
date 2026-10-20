// CThing::CThing(unsigned long classType) -- registers the thing with the thing manager's class factory, then starts it blank: no unique id, no components in the presence bits, map or update list, empty section/script names, no def, no driver/physics/appearance, off both linked lists, and the default flag set (in thing scans, drawable, deleted with its owner level, killed on level unload, bullet-time affected, adding components permitted); bumps the global thing count
// Leaf: ??0CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9030, 237 bytes (+3 int3). `ret 4`, returns this. CThing vtable 0x01239ABC.
// Callees: CClass::CClass(CClassFactory&, unsigned long, bool) 0x00A372D0 (the factory is g_pThingManager 0x013B8A1C),
//          malloc 0x00BFEA0E (the update list's 12-byte header node), CCharString::CCharString() 0x0099E4B0 (x3).
// Shape: members in declaration order through one zero register (ebx) except the bitset words, which the inlined
//        bitset constructor (a memset of the nine words) clears through a fresh zero (edx) off the member's address; the list header node is
//        self-linked before it is stored; the four flag bytes end the init list (+0x91 fully assigned so a single
//        byte store, +0x92/+0x93/+0x94 read-modify-write around the bits left alone); the body bumps the counter.
// Layout: Ego_r.pdb CThing with retail containers (bitset<274> 9 words +0x20, CVectorMap 13 bytes +0x44, list 4 bytes
//         +0x54): UniqueID +0x18, Components +0x44 (sorted +0x51), UpdateComponentsList +0x54, SerialisationSectionName
//         +0x58, PTCDriver +0x5C, PhysicsTC +0x60, PTCGraphicAppearance +0x64, WorldFrameCreatedAt +0x68 (not set),
//         ThingTypeFlags +0x6C (not set), PBaseDef +0x70, ScriptName +0x74, ScriptData +0x78, RenderListPrev/Next
//         +0x7C/+0x80, UpdateListPrev/Next +0x84/+0x88, DefGlobalIndex +0x8C (not set), CurrentMapNumber +0x8E,
//         PlayerNumber +0x90 (not set), flag bytes +0x91..+0x94. Global thing counter 0x013B8A20.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void* __cdecl memset(void* block, int value, unsigned int size);
#pragma intrinsic(memset)
struct CClassFactory;
struct CClass {
    virtual void v00();
    CClass(CClassFactory& factory, unsigned long type, bool registerNow);   // 0x00A372D0
};
struct CThingManager;
extern CThingManager* g_pThingManager;                // 0x013B8A1C
extern long g_thingCount;                             // 0x013B8A20
class CCharString {
public:
    void* PStringData;
    CCharString();                                    // 0x0099E4B0
};
struct CPresenceBits {                                // std::bitset<274>
    unsigned long words[9];
    CPresenceBits() { memset(words, 0, sizeof(words)); }   // STLport _Base_bitset: _M_do_reset
};
struct CTCBase;
struct SComponentPair { long first; CTCBase* second; };
struct CKeyPairCompareLess { };
struct CComponentMap {                                // CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess>: vector + compare + sorted
    SComponentPair*     m_first;
    SComponentPair*     m_last;
    SComponentPair*     m_end;
    CKeyPairCompareLess m_compare;
    bool                m_sorted;
    CComponentMap() : m_first(0), m_last(0), m_end(0), m_sorted(false) { }
};
struct SListNode { SListNode* _M_next; SListNode* _M_prev; CTCBase* _M_data; };
struct CTCList {                                      // std::list<CTCBase*>: the header-node pointer
    SListNode* _M_node;
    CTCList() : _M_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct CThingBaseDef;
struct CDefPointer {                                  // CDefPointer<const CThingBaseDef>
    const CThingBaseDef* Object;
    CDefPointer() : Object(0) { }
};
struct CTCDriverBase; struct CTCPhysicsBase; struct CTCGraphicAppearance;
class CThing : public CClass {                        // vtable 0x01239ABC
public:
    unsigned char          _pad04[0x18 - 0x04];
    unsigned __int64       UniqueID;                  // +0x18
    CPresenceBits          UsingComponents;           // +0x20
    CComponentMap          Components;                // +0x44
    CTCList                UpdateComponentsList;      // +0x54
    CCharString            SerialisationSectionName;  // +0x58
    CTCDriverBase*         PTCDriver;                 // +0x5c
    CTCPhysicsBase*        PhysicsTC;                 // +0x60
    CTCGraphicAppearance*  PTCGraphicAppearance;      // +0x64
    long                   WorldFrameCreatedAt;       // +0x68
    unsigned long          ThingTypeFlags;            // +0x6c
    CDefPointer            PBaseDef;                  // +0x70
    CCharString            ScriptName;                // +0x74
    CCharString            ScriptData;                // +0x78
    CThing*                RenderListPrev;            // +0x7c
    CThing*                RenderListNext;            // +0x80
    CThing*                UpdateListPrev;            // +0x84
    CThing*                UpdateListNext;            // +0x88
    unsigned short         DefGlobalIndex;            // +0x8c
    short                  CurrentMapNumber;          // +0x8e
    signed char            PlayerNumber;              // +0x90
    unsigned char          Dead : 1;                  // +0x91
    unsigned char          CreatedByOwnerThingCreation : 1;
    unsigned char          IncludeInThingScans : 1;
    unsigned char          Activated : 1;
    unsigned char          GloballyLoaded : 1;
    unsigned char          InLimbo : 1;
    unsigned char          KillLocked : 1;
    unsigned char          CreatedBySerialisation : 1;
    unsigned char          DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char          Drawable : 1;
    unsigned char          Dying : 1;
    unsigned char          ForceFrameUpdate : 1;
    unsigned char          LevelPersistent : 1;
    unsigned char          GameStatePersisted : 1;
    unsigned char          PersistOnUnloadWhenInLimbo : 1;
    unsigned char          PausedDuringCutscenes : 1;
    unsigned char          NotPausedDuringCutscenes : 1;        // +0x93
    unsigned char          DeathPersistent : 1;
    unsigned char          KillOnLevelUnload : 1;
    unsigned char          ActivatedScript : 1;
    unsigned char          LevelIsUnloading : 1;
    unsigned char          IsAffectedByBulletTimeOverride : 1;
    unsigned char          AddingTCsPermitted : 1;
    unsigned char          InitialisationStage : 3;             // +0x94
    CThing(unsigned long classType);
};
CThing::CThing(unsigned long classType)
    : CClass(*(CClassFactory*)g_pThingManager, classType, false),
      UniqueID(0), UsingComponents(), Components(), UpdateComponentsList(), SerialisationSectionName(),
      PTCDriver(0), PhysicsTC(0), PTCGraphicAppearance(0), PBaseDef(), ScriptName(), ScriptData(),
      RenderListPrev(0), RenderListNext(0), UpdateListPrev(0), UpdateListNext(0), CurrentMapNumber(0),
      Dead(false), CreatedByOwnerThingCreation(false), IncludeInThingScans(true), Activated(false), GloballyLoaded(false),
      InLimbo(false), KillLocked(false), CreatedBySerialisation(false),
      DeletedOnOwnerLevelOutOfScope(true), Drawable(true), Dying(false), ForceFrameUpdate(false), GameStatePersisted(false),
      PersistOnUnloadWhenInLimbo(false), PausedDuringCutscenes(false),
      NotPausedDuringCutscenes(false), DeathPersistent(false), KillOnLevelUnload(true), ActivatedScript(false),
      LevelIsUnloading(false), IsAffectedByBulletTimeOverride(true), AddingTCsPermitted(true), InitialisationStage(0)
{
    ++g_thingCount;
}
