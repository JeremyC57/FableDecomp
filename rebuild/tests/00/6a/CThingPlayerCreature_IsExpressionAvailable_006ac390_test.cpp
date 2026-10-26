// behaviour test for 0x006AC390 CThingPlayerCreature::IsExpressionAvailable (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CThingPlayerCreature::IsExpressionAvailable(const CCharString& expressionName) -- true when the named CExpressionDef exists, the creature has an inventory (interface 0x11) and the inventory holds at least one of the expression's DummyObject item
// Grade: RELOCATION_MATCH -- same-TU noinline body of the by-reference LowerBound 0x004365B0, the map Find as ONE ternary pick, and the count call made through a free __forceinline wrapper CountOfType(inventory, defIndex) fed by that pick: the component lands in ecx before the def argument is loaded (the 0072af30 lever)
// Leaf: IsExpressionAvailable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC390, length 160 bytes (`ret 4`).
// Callees: CThing::GetDefinitionManager @ 0x004C7990 (`[0x013B8A1C]+0x20`, `this` passed through), CDefinitionManager::
// GetDef<CExpressionDef>(const CCharString&, CDefPointer<const CExpressionDef>*) @ 0x00593666 (manifest label
// GetDef<CHeroPostcardGeneratorDef> is a byte-twin propagation), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0, CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08.
// Layout (retail): CThing presence bitset +0x20 (interface 0x11 = bit 17 of word 0), component map +0x44
// (as the landed 0x0074E460); CExpressionDef::DummyObject +0x70 [PDB +0x74]; the def's ref count +0x04 and
// its deleting release at virtual slot 1.
// Shape: GetDef fills a plain out slot (zero store between the pushes); retail loads the def from it once
// (edi) for every read and the release, so `out.Object` is copied into a small holder whose dtor releases it;
// the found flag, the presence test and the inventory count form one `&&` chain; on the success path the def
// has been dereferenced, so VC drops the dtor's null test there (`dec [edi+4] ; jne`), the fail path keeps it.
class CCharString { public: void* PStringData; };

class CExpressionDef { public:
    virtual void VSlot0();
    virtual void Destroy() const;                     // slot 1
    mutable long RefCount;                            // +0x04
    unsigned char _pad_0x08[0x70 - 0x08];
    long DummyObject;                                 // +0x70 [PDB +0x74] CDefIndex of the required item
};

template <class T> class CDefPointer { public:             // the GetDef out slot (released through CDefHolder)
    T* Object;
};

template <class T> struct CDefHolder {              // owns the def taken from the out slot
    T* Object;
    CDefHolder(T* object) : Object(object) {}
    ~CDefHolder()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CDefinitionManager { public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out);   // 0x00593666
};

class CTCBase;
struct CTCInventoryBase {
    long GetNumberOfItemsOfTypeInInventory(long defIndex);  // 0x005BDF08
};

struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const unsigned long& type);    // 0x004365B0
    __forceinline CTCBase* Find(long type)            // inline map find: end() when the key is absent
    {
        SEntry* it = LowerBound((unsigned long)type);
        return ((it == last || type < it->type) ? last : it)->component;
    }
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation 0x004365B0 (key by const
// reference): binary search for the first entry whose type is not below the key.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const unsigned long& type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < (long)type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    CDefinitionManager* GetDefinitionManager() const; // 0x004C7990
    __forceinline unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetInterface(long type) { return m_interfaces.Find(type); }
};

class CThingPlayerCreature : public CThing { public:
    bool IsExpressionAvailable(const CCharString& expressionName);
};

__forceinline long CountOfType(CTCInventoryBase* inventory, long defIndex) { return inventory->GetNumberOfItemsOfTypeInInventory(defIndex); }

bool CThingPlayerCreature::IsExpressionAvailable(const CCharString& expressionName)
{
    CDefPointer<const CExpressionDef> out;
    out.Object = 0;
    bool found = GetDefinitionManager()->GetDef(expressionName, &out);
    CDefHolder<const CExpressionDef> def(out.Object);
    if (found && HasInterface(0x11))
    {
        if (CountOfType((CTCInventoryBase*)GetInterface(0x11), def->DummyObject) > 0)
            return true;
    }
    return false;
}


static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)

static bool defFound; static const CExpressionDef* defResult; static const CCharString* defName;
static int defCalls, countCalls, destroyCalls; static CTCInventoryBase* countThis; static long countArg, countResult;
static CDefinitionManager manager;
struct CTestExpressionDef : CExpressionDef { };
void CExpressionDef::VSlot0() { }
void CExpressionDef::Destroy() const { ++destroyCalls; }
CDefinitionManager* CThing::GetDefinitionManager() const { return &manager; }
template <class T> bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out)
{ ++defCalls; defName = &name; if (defFound) out->Object = (const T*)defResult; return defFound; }
long CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long defIndex) { ++countCalls; countThis = this; countArg = defIndex; return countResult; }

static CTestExpressionDef expr;
static CThingPlayerCreature creature;
static CInterfaceMap::SEntry entries[3];
static CTCInventoryBase inventoryObj[4], otherObj[4];

static void reset(bool found, bool hasBit, long count, long refs)
{
    defFound = found; defResult = &expr; defName = 0; defCalls = countCalls = destroyCalls = 0; countThis = 0; countArg = 0; countResult = count;
    expr.RefCount = refs; expr.DummyObject = 77;
    memset(creature.m_presentBits, 0, sizeof(creature.m_presentBits));
    entries[0].type = 0x05; entries[0].component = (CTCBase*)otherObj;
    entries[1].type = 0x11; entries[1].component = (CTCBase*)inventoryObj;
    entries[2].type = 0x7fff; entries[2].component = (CTCBase*)otherObj;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    if (hasBit) creature.m_presentBits[0x11 >> 5] |= 1UL << (0x11 & 31);
}

int main()
{
    CCharString name; name.PStringData = 0;
    // 1: no such def -> false, no inventory query, nothing released
    reset(false, true, 3, 1); CHECK(!creature.IsExpressionAvailable(name) && defCalls == 1 && defName == &name && countCalls == 0 && destroyCalls == 0);
    // 2: def found, no inventory -> false, the def reference released (destroyed at the last reference)
    reset(true, false, 3, 1); CHECK(!creature.IsExpressionAvailable(name) && countCalls == 0 && destroyCalls == 1 && expr.RefCount == 0);
    // 3: inventory holds the dummy object -> true; the query runs on the inventory with the def's DummyObject
    reset(true, true, 2, 2);
    CHECK(creature.IsExpressionAvailable(name) && countCalls == 1 && countThis == inventoryObj && countArg == 77 && destroyCalls == 0 && expr.RefCount == 1);
    // 4: none held -> false, released
    reset(true, true, 0, 1); CHECK(!creature.IsExpressionAvailable(name) && countCalls == 1 && destroyCalls == 1);

    if (fails == 0) printf("006ac390_TEST PASS\n");
    return fails;
}