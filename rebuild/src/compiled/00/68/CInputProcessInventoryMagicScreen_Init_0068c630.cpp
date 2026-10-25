// CInputProcessInventoryMagicScreen::Init() -- look up the player character's CTCInventoryMagic component (interface type 0x15: bit 21 of the creature's interface bitset, then the sorted {type, component} map) and keep it in PInventory
// Leaf: ?Init@CInputProcessInventoryMagicScreen@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068C630, 69 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: CInputProcessInventoryMagicScreen::Init -- proven; evidence: vtable 0x01237858 slot 2 (RTTI .?AVCInputProcessInventoryMagicScreen@@ : CInputProcessInventoryBase :
//   CInputProcessGameBase : CAInputProcess : CBaseClassNonCopyable : CBaseClass); FableWin.pdb names ?Init@CInputProcessInventoryMagicScreen@@UAEXXZ and
//   ?GetPInventory@CInputProcessInventoryMagicScreen@@MBE...; Ego_r.pdb CInputProcessInventoryMagicScreen::PInventory is a CTCInventoryMagic* at +0x34. Family G037 (8 Init bodies that
//   differ only in the interface type), the sibling of the landed CInputProcessInventoryQuestsScreen::Init 0x0068BC20.
// Callees: CPlayer::GetPPlayerCharacter() const 0x00487DC0 (MyPlayer +0x18; `add ecx,2Ch ; jmp GetPItem`, byte twin of
//   GetPControlledCreature 0x00487DD0 -- the catalog and the landed bindings split the two FableWin.pdb getters this way),
//   CInterfaceMap::LowerBound 0x0040F020 (the creature's {type, component} vector map at +0x44).
// Shape: the GetInterface template is `__forceinline` (as in the landed GetInterface rows): one `push ecx` frame slot for the
//   key, the map pointer advanced into esi once, `it = last` when the key is past it, PInventory the out pointer.
// Model: local -- the generated engine headers flatten the vptr into a word, which cannot carry a virtual Init.
class CTCBase;
struct CInterfaceMap {                                       // a thing's sorted {type, component} vector map
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                              // +0x44
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
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;       // 0x00487DC0
};
class CBaseClass {                                           // RTTI root, vtable 0x01231710
public:
    virtual ~CBaseClass();                                   // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                        // 0x0099A300
};
class CInputProcessManager;
class CAInputProcess : public CBaseClassNonCopyable {        // 0x14 bytes
public:
    virtual void v01();
    virtual void Init();                                     // slot 2
    CInputProcessManager* PPlayerInterface;                  // +0x04
    unsigned char InputProcessListPos[0x0c];                 // +0x08 CLinkedListPosition
};
class CWorld;
class CGameDefinitionManager;
class CMainGameComponent;
class CGamePlayerInterface;
class CInputProcessGameBase : public CAInputProcess {
public:
    CWorld*                 World;                           // +0x14
    CPlayer*                MyPlayer;                        // +0x18
    CGameDefinitionManager* DefinitionManager;               // +0x1c
    CMainGameComponent*     Component;                       // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;            // +0x24
};
enum ECurrentMovement { CURRENT_MOVEMENT_NONE = 0 };
class CInputProcessInventoryBase : public CInputProcessGameBase {
public:
    ECurrentMovement CurrentMovement;                        // +0x28
    unsigned long    FrameSinceLastMovement;                 // +0x2c
    bool             StartedRepeatingMovement;               // +0x30
};
class CTCInventoryMagic;
class CInputProcessInventoryMagicScreen : public CInputProcessInventoryBase {
public:
    virtual void Init();
    CTCInventoryMagic* PInventory;                                     // +0x34
};

void CInputProcessInventoryMagicScreen::Init()
{
    MyPlayer->GetPPlayerCharacter()->GetInterface(&PInventory, 0x15);
}