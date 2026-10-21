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
struct CCharString { void* PStringData; };

struct CExpressionDef {
    virtual void VSlot0();
    virtual void Destroy() const;                     // slot 1
    mutable long RefCount;                            // +0x04
    unsigned char _pad_0x08[0x70 - 0x08];
    long DummyObject;                                 // +0x70 [PDB +0x74] CDefIndex of the required item
};

template <class T> struct CDefPointer {             // the GetDef out slot (released through CDefHolder)
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

struct CDefinitionManager {
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out);   // 0x00593666
};

struct CTCBase;
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


struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    CDefinitionManager* GetDefinitionManager() const; // 0x004C7990
    __forceinline unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetInterface(long type) { return m_interfaces.Find(type); }
};

struct CThingPlayerCreature : CThing {
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