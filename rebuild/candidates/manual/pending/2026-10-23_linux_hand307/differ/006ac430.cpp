// Grade: DIFFER(603v606) -- register allocation: item in edi/map in ebx/inventory in ebp (retail ebp/edi/ebx) and ebp pushed late (shrink-wrapped into the inventory block)
// CThingPlayerCreature::LearnExpression(const CCharString& expressionName, long quickAccessSlot, bool silent) -- unless the expression is already available: look the CExpressionDef up, spawn its dummy inventory object at the hero, put it in the hero's inventory, record the learned expression, assign a quick-access slot and (unless silent) give the category tutorial
// Leaf: ?LearnExpression@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC430, length 604 bytes (`ret 0xc`).
// Callees: CThingPlayerCreature::IsExpressionAvailable @ 0x006AC390, CThing::GetDefinitionManager @ 0x004C7990,
//   CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410 / GetDefClassTypeNameFromGlobalIndex @ 0x009ACCE0 (CDefString
//   by value) / GetDef<CExpressionDef>(long, CDefPointer&) @ 0x005F81BE, CDefString::operator==(const char*) @ 0x005B3440
//   on "EXPRESSION" @ 0x01235D94, CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0 ("" @ 0x0122D70E),
//   CThing::GetPosition @ 0x004C73D0, the __fastcall thing factory @ 0x00703210, CThing::Kill @ 0x004C9B80, LowerBound
//   @ 0x0040F020, CTCInventoryItem::AddToInventory @ 0x005D8D50 / GetCategoryIdentifier @ 0x005D92B0,
//   CTCInventoryAbilities::AddLearnedExpression @ 0x005EEDCF, CTCInventory::GetQuickAccessNextFreeSlot @ 0x00582027 /
//   SetThingAsQuickAccessItem @ 0x00584157, CTCHero::GiveTutorialForCategory @ 0x006C9FA0.
// Layout (retail): CThing presence bitset +0x20, component map +0x44, def index +0x8c, player number (signed byte) +0x90;
//   CExpressionDef DummyObject +0x70; interfaces 0x11 inventory, 0x1a inventory item, 0x29 hero, 0x6f abilities, 0x8f.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString {
    long TablePos;
    bool operator==(const char* text) const;            // 0x005B3440
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
struct CExpressionDef : CDefPointeeBase {
    unsigned char _pad08[0x70 - 0x08];
    long DummyObject;                                   // +0x70
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);            // 0x009AD410
    CDefString GetDefClassTypeNameFromGlobalIndex(long globalIndex);    // 0x009ACCE0
    bool GetDef(long globalIndex, CDefPointer<CExpressionDef>& out);    // 0x005F81BE
};
void* FindDef_CDefinitionManager_GetDef_c430();
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CExpressionDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_c430();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing;
struct CTCInventory {
    long GetQuickAccessNextFreeSlot(long item);                         // 0x00582027
    void SetThingAsQuickAccessItem(long item, long slot, bool a);       // 0x00584157
};
struct CTCInventoryItem {
    bool AddToInventory(CTCInventory* inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
    long GetCategoryIdentifier();                                       // 0x005D92B0
};
struct CTCInventoryAbilities {
    void AddLearnedExpression(long expression);                         // 0x005EEDCF
};
struct CTCHero {
    bool GiveTutorialForCategory(long category, bool force);            // 0x006C9FA0
};
struct CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210

class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned char _pad8e[0x90 - 0x8e];
    signed char PlayerNumber;                           // +0x90
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    void Kill(bool immediately);                        // 0x004C9B80
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
class CThingPlayerCreature : public CThing {
public:
    bool IsExpressionAvailable(const CCharString& expressionName);      // 0x006AC390
    bool LearnExpression(const CCharString& expressionName, long quickAccessSlot, bool silent);
};

__forceinline CThing* CreateThingObject(long defIndex, const CThing* creature, const CCharString& name)
{
    return CreateThingObject(defIndex, &creature->GetPosition(), creature->PlayerNumber, 0, 0, name);
}

bool CThingPlayerCreature::LearnExpression(const CCharString& expressionName, long quickAccessSlot, bool silent)
{
    if (!IsExpressionAvailable(expressionName)) {
        long expression = GetDefinitionManager()->GetDefGlobalIndexFromName(expressionName);
        if (expression > 0) {
            if (GetDefinitionManager()->GetDefClassTypeNameFromGlobalIndex(expression) == "EXPRESSION") {
                CDefPointer<CExpressionDef> def;
                def.Object = 0;
                GetDefinitionManager()->GetDef(expression, def);
                CThing* item;
                {
                    CCharString name("");
                    item = CreateThingObject(def->DummyObject, this, name);
                }
                if (item) {
                    if (item->HasTC(0x8f)) {
                        if (HasTC(0x11)) {
                            CTCInventory* inventory = (CTCInventory*)GetTC(0x11);
                            CTCInventoryItem* itemTC = 0;
                            if (item->HasTC(0x1a))
                                itemTC = (CTCInventoryItem*)item->GetTC(0x1a);
                            itemTC->AddToInventory(inventory, false, true, 0, silent, false);
                            if (HasTC(0x6f))
                                ((CTCInventoryAbilities*)GetTC(0x6f))->AddLearnedExpression(expression);
                            if (quickAccessSlot != -1 || (quickAccessSlot = inventory->GetQuickAccessNextFreeSlot(item->m_defGlobalIndex)) != -1)
                                inventory->SetThingAsQuickAccessItem(item->m_defGlobalIndex, quickAccessSlot, false);
                            if (!silent && HasTC(0x29)) {
                                CTCHero* hero = (CTCHero*)GetTC(0x29);
                                if (hero && itemTC->GetCategoryIdentifier())
                                    hero->GiveTutorialForCategory(itemTC->GetCategoryIdentifier(), false);
                            }
                            if (--def.Object->m_refCount == 0)
                                def.Object->Destroy();
                            return true;
                        }
                    } else
                        item->Kill(false);
                }
                if (--def.Object->m_refCount == 0)
                    def.Object->Destroy();
            }
        }
    }
    return false;
}
