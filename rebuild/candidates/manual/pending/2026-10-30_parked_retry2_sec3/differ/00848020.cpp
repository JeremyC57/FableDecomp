// Grade: DIFFER(1151v1139) 106 difflines (wave 2 sec3: first reconstruction from the disassembly; was the empty skeleton, 352 lines)
// Tried: g1 straight draft (1195v1139/209); g2 __forceinline HasTC + one GetPItem per interface test (1151v1139/126);
//   g3 STLport bitset test with an unsigned shift and noreturn abort (`jmp abort`), named CCharString + font locals so
//   the text is built before the display-engine call (this file, 1151v1139/106).
// Next: retail's frame is 0x18 and ebx is pushed late (after the first returns): the entry zero (`mov [esp+1Ch],0`) is
//   one slot shared by the lookup keys and `inventory`, ours keeps an extra slot and a zero register; the
//   CDefPointer<CContextSensitiveItemDef> dtor should test the `dec [p+4]` flags directly (retail keeps the def in ebp,
//   ours reloads it twice): move onto engine/CDefPointer.h once it has the null and raw-pointer ctors.
// CCreatureAction_UseWallMount::PerformAction() -- use a wall mount: when the mount already holds an object, take it
//   back into the creature's inventory (or the "no room" message); else, when the object to use is a trophy
//   (CContextSensitiveItemDef type 9), take it out of the inventory and attach it at its bought price; otherwise the
//   trophy-mounting help text
// Leaf: PerformAction
// Name: Ego_r.pdb ?PerformAction@CCreatureAction_UseWallMount@@UAEXXZ.
// Layout: engine/CCreatureAction_UseWallMount.h PObjectToUse +0xA8; CCreatureActionBase PCreature +0x08 (Ego_r.pdb).
// Callees (reloc_bindings + PDB names): CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap::LowerBound 0x004365B0
//   (const) / 0x0040F020; IS_THING_ALIVE 0x004CC340; CTCWallMount::HasObjectAttached 0x007DE500, GetObjectAttached
//   0x007DE4E0, UnattachObject(long*) 0x007DF130, AttachObject(CThing&, long) 0x007DE710; CTCInventoryItem::
//   GetInventoryType 0x005D9030, AddToInventory 0x005D8D50, GetBoughtPrice 0x005D8E70, RemoveFromInventory 0x005D8E20;
//   CTCInventoryBase::CanAddItem 0x005BF2ED; CThing::GetDisplayEngine 0x004C7A70 (catalog: PeekComponent);
//   CSubtitleRenderer::SetText 0x00492ED1 on g_pPlayerGui 0x013B8790 (+0x18); operator!=<CThing> 0x005CBDD2 (__fastcall);
//   CDefClassBase::GetSubDef<CContextSensitiveItemDef> 0x005933C8; CThing::SetInLimbo 0x004C8CF0; CCharString ctor
//   0x0099EBF0 / dtor 0x0099EAE0; puts / abort (STLport bitset range check, "bitset" 0x0122E068).
#include "engine/CCreatureAction_UseWallMount.h"
extern "C" int __cdecl puts(const char*);
extern "C" __declspec(noreturn) void __cdecl abort(void);

class CTCBase;
class CBaseObject;
class CBaseObjectPointer;
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> bool __fastcall operator!=(const CBaseObject* object, const CIntelligentPointer<T>& pointer); // 0x005CBDD2

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
    SEntry* LowerBoundConst(const long* type) const;     // 0x004365B0
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long RefCount;                                       // +0x04
    template <class T> bool GetSubDef(T* out) const;     // 0x005933C8 (CDefPointer<const CContextSensitiveItemDef>*)
};
class CContextSensitiveItemDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long Type;                                           // +0x28 (9: trophy)
};
template <class T> class CDefPointer {                   // probe model: engine/CDefPointer.h lacks the null/raw ctors
public:
    T* PDef;
    __forceinline CDefPointer() : PDef(0) { }
    __forceinline CDefPointer(T* p) : PDef(p) { if (PDef) ++PDef->RefCount; }
    __forceinline ~CDefPointer() { if (PDef && --PDef->RefCount == 0) PDef->Destroy(); }
    __forceinline T* operator->() const { return PDef; }
    __forceinline operator bool() const { return PDef != 0; }
};

class CFontBank;
class CSubtitleRenderer {
public:
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b); // 0x00492ED1
};
class CPlayerGui {
public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                         // +0x18
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790
class CDisplayEngine {
public:
    unsigned char _pad00[0x1614c];
    CFontBank* PFontBank;                                // +0x1614C
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                      // +0x20 bitset<274>
    CInterfaceMap m_interfaces;                          // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                 // +0x70
    unsigned char _pad74[0x8c - 0x74];
    unsigned short m_w8c;                                // +0x8C
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char m_flags91;                             // +0x91 bit 0: dead
    CDisplayEngine* GetDisplayEngine();                  // 0x004C7A70
    void SetInLimbo(bool limbo);                         // 0x004C8CF0
    __forceinline CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(PDef); }
    __forceinline unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    __forceinline bool TestTC(long t) const
    {
        if ((unsigned long)t >= 274) {
            puts("bitset");
            abort();
        }
        return (m_presentBits[(unsigned long)t >> 5] & (1UL << (t & 31))) != 0;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBoundConst(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (TestTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>& p);   // 0x004CC340

struct CTCOwned {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32();
    virtual CThing* GetOwner();                          // slot 33 (+0x84)
};
struct CTCUsable { unsigned char _pad[0x48]; bool Usable; };   // interface 0x39, +0x48
class CTCInventoryBase {
public:
    bool CanAddItem(long type) const;                    // 0x005BF2ED
};
class CTCInventoryItem {
public:
    unsigned char _pad00[0x10];
    void* PInventory;                                    // +0x10
    long GetInventoryType() const;                       // 0x005D9030
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long price, bool c, bool d); // 0x005D8D50
    long GetBoughtPrice();                               // 0x005D8E70
    void RemoveFromInventory();                          // 0x005D8E20
};
class CTCWallMount {
public:
    bool HasObjectAttached() const;                      // 0x007DE500
    CThing* GetObjectAttached();                         // 0x007DE4E0
    CThing* UnattachObject(long* price);                 // 0x007DF130
    bool AttachObject(CThing& thing, long price);        // 0x007DE710
};

struct CCreatureAction_UseWallMount_Methods : CCreatureAction_UseWallMount {
    void PerformAction();
};

void CCreatureAction_UseWallMount_Methods::PerformAction()
{
    CTCInventoryBase* inventory = 0;
    CIntelligentPointer<CThing>& use = *(CIntelligentPointer<CThing>*)PObjectToUse;
    CIntelligentPointer<CThing>& creature = *(CIntelligentPointer<CThing>*)_pad_0x08;
    if (use.GetPItem() == 0 || (use.GetPItem()->m_flags91 & 1))
        return;
    CThing* actor = creature.GetPItem();
    if (!actor->HasTC(8))
        return;
    CThing* owner = actor->GetTCConst<CTCOwned>(8)->GetOwner();
    if (owner == 0 || !owner->HasTC(0xda))
        return;
    CTCWallMount* mount = owner->GetTC<CTCWallMount>(0xda);
    CThing* user = creature.GetPItem();
    if (!user->HasTC(0x46) || user->GetTC<CTCBase>(0x46) == 0)
        return;
    if (IS_THING_ALIVE(use)) {
        CThing* used = use.GetPItem();
        if (used->HasTC(0x39) && !used->GetTCConst<CTCUsable>(0x39)->Usable)
            return;
    }

    bool handled = false;
    if (mount->HasObjectAttached()) {
        handled = true;
        CThing* object = mount->GetObjectAttached();
        if (object->HasTC(0x1a)) {
            CTCInventoryItem* item = object->GetTC<CTCInventoryItem>(0x1a);
            CThing* thing = creature.GetPItem();
            thing->GetInterface(&inventory, item->GetInventoryType());
            if (!inventory->CanAddItem(object->m_w8c)) {
                CCharString text("You don't have room in your inventory for this item!");
                CFontBank* font = creature.GetPItem()->GetDisplayEngine()->PFontBank;
                g_pPlayerGui->Subtitles.SetText(font, text, 0, 3.0f, false, true);
            } else {
                long price = 0;
                mount->UnattachObject(&price);
                item->AddToInventory(inventory, false, true, price, false, false);
            }
        } else {
            mount->UnattachObject(0);
        }
    }
    if (!mount->HasObjectAttached() && operator!=((const CBaseObject*)(void*)owner, use)) {
        CDefPointer<CContextSensitiveItemDef> sensitive;
        bool trophy = use.GetPItem()->GetDef() && use.GetPItem()->GetDef()->GetSubDef(&sensitive) && sensitive->Type == 9;
        if (trophy) {
            long price = 0;
            CThing* thing = use.GetPItem();
            if (thing->HasTC(0x1a)) {
                CTCInventoryItem* item = thing->GetTC<CTCInventoryItem>(0x1a);
                if (item->PInventory) {
                    price = item->GetBoughtPrice();
                    item->RemoveFromInventory();
                }
            }
            mount->AttachObject(*use.GetPItem(), price);
            use.GetPItem()->SetInLimbo(false);
        }
    } else if (!handled) {
        CCharString text("TEXT_GUI_TROPHY_MOUNTING_HELP");
        CFontBank* font = creature.GetPItem()->GetDisplayEngine()->PFontBank;
        g_pPlayerGui->Subtitles.SetText(font, text, 10, 0.0f, false, true);
    }
}
