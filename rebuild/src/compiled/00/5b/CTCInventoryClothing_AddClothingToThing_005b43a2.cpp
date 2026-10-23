// CTCInventoryClothing::AddClothingToThing(long, CThing*) -- dress a thing in a clothing item or a whole hero suit, replacing the worn pieces it displaces
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                         // 0x00BFEA14

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                            // 0x0099EAE0
};
struct SAllocator { SAllocator() { } };
struct SLongListNode { SLongListNode* _M_next; SLongListNode* _M_prev; long _M_data; };
struct CLongListIterator {
    SLongListNode* _M_node;
    CLongListIterator(SLongListNode* n) : _M_node(n) { }
    CLongListIterator(const CLongListIterator& o) : _M_node(o._M_node) { }
};
struct CLongList {                                             // STLport list<long>: the sentinel pointer
    SLongListNode* _M_node;
    explicit CLongList(const SAllocator& allocator = SAllocator());   // 0x0042AB58
    ~CLongList();                                              // 0x0042AB73
    CLongListIterator erase(CLongListIterator position);       // 0x00429B02
};
struct CLongVector {                                           // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    void push_back(const long& value);                         // 0x005BBA94
};
long* __fastcall FindLong(long* first, long* last, const long& value);   // 0x005BA7F9 (std::find)

struct SNode { long _M_color; SNode* _M_parent; SNode* _M_left; SNode* _M_right; long first; long second; };
SNode* __cdecl Successor(SNode* node);                         // 0x004292C0
struct CSuitPartMap {                                          // map<EClothingSuitPart, long> (12 bytes)
    SNode* _M_header; unsigned long _M_node_count; long _M_key_compare;
    SNode* begin() const { return _M_header->_M_left; }
    SNode* end() const { return _M_header; }
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy() const;                              // slot 1
    mutable long RefCount;                                     // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
// Layout: retail CAppearanceModifierDef (Ego_r.pdb - 8, the two 16-byte containers are 12 in retail).
struct CAppearanceModifierDef : CDefBase {
    unsigned char _pad08[0x44 - 0x08];
    bool IsRemoveable;                                         // +0x44
    unsigned char _pad45[0x4c - 0x45];
    long SuitPart;                                             // +0x4c
};
struct CHeroSuitDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    CSuitPartMap Parts;                                        // +0x28
};
struct CThingDef : CDefBase {
    bool GetSubDef(CDefPointer<const CHeroSuitDef>* out) const;            // 0x005935C4
    bool GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const;  // 0x0057FF92
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<const CThingDef>* out) const;      // 0x0042AF3C
};
void* FindDef_43a2_0();
void* FindDef_43a2_1();
void* FindDef_43a2_2();
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<const CHeroSuitDef>* out) const
{
    void* def = FindDef_43a2_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const
{
    void* def = FindDef_43a2_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CThingDef>* out) const
{
    void* def = FindDef_43a2_2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CComponentList { void* m_pNode; ~CComponentList(); };   // 0x0042ABCA
struct SActionParamPair {
    unsigned long  first;
    CComponentList second;
    SActionParamPair();                                        // 0x0042BED4
    SActionParamPair(const SActionParamPair& other);           // 0x0042CD84
};
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);           // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void SetActionParameter(SActionParamPair parameter);   // slot 62 (+0xF8)
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86();
    virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92();
    virtual void Slot93();                                     // slot 93 (+0x174)
};
}
struct SClothingEntry { long SuitPart; NUISystem::CComponent* Component; };
struct CClothingEntryVector {
    SClothingEntry* _M_start;
    SClothingEntry* _M_finish;
    SClothingEntry* _M_end_of_storage;
    SClothingEntry* erase(SClothingEntry* position);           // 0x005BAAE3
};

struct CTCHeroAttachableAppearanceModifiers {                  // interface 0x5E
    void GetAppearanceModifierList(CLongList& out, long type) const;   // 0x00706800
    void RemoveAppearanceModifier(long def);                   // 0x00706370
    void RemoveAllAppearanceModifiers(long type);              // 0x00706640
    void AddAppearanceModifierWithReplace(long def);           // 0x00706950
};
struct CPresenceBits { bool test(long bit); };                 // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                                  // +0x20
    unsigned char  _pad21[0x23];
    CInterfaceMap  m_interfaces;                               // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

// Layout: retail CTCInventoryClothing (12-byte containers): ParentThing +0x04, the clothing menu entries vector +0x134.
struct CTCInventoryClothing {
    void*   __vftable;
    CThing* ParentThing;                                       // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CClothingEntryVector ClothingEntries;                      // +0x134
    const CDefinitionManager* GetDefinitions() const;          // 0x006869D0 (CTCBase)
    long GetNumberOfItemsOfTypeInInventory(long type);         // 0x005BDF08 (CTCInventoryBase)
    void AddClothingToThing(long item, CThing* thing);
};
// R&D I-3: the list ctor's same-TU out-of-line body (it builds only the sentinel) lets the default allocator
// temporary be homed in the dead byte of the `thing` argument slot, as in retail.
#pragma auto_inline(off)
CLongList::CLongList(const SAllocator&)
{
    _M_node = (SLongListNode*)operator new(sizeof(SLongListNode));
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
}
#pragma auto_inline(on)
void CTCInventoryClothing::AddClothingToThing(long item, CThing* thing)
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (thing->GetInterface(&modifiers, 0x5e)) {
        CDefPointer<const CThingDef> itemDef;
        GetDefinitions()->GetDef(item, &itemDef);
        CDefPointer<const CHeroSuitDef> suitDef;
        CLongList worn;
        modifiers->GetAppearanceModifierList(worn, 2);
        if (itemDef->GetSubDef(&suitDef)) {
            SLongListNode* node = worn._M_node->_M_next;
            CLongVector keptParts;
            for (; node != worn._M_node; node = node->_M_next) {
                CDefPointer<const CThingDef> wornDef;
                CDefPointer<const CAppearanceModifierDef> wornModifier;
                if (GetDefinitions()->GetDef(node->_M_data, &wornDef)) {
                    if (wornDef->GetSubDef(&wornModifier)) {
                        if (wornModifier->IsRemoveable)
                            modifiers->RemoveAppearanceModifier(node->_M_data);
                        else
                            keptParts.push_back(wornModifier->SuitPart);
                    }
                }
            }
            for (SNode* part = suitDef->Parts.begin(); part != suitDef->Parts.end(); part = Successor(part)) {
                if (GetNumberOfItemsOfTypeInInventory(part->second) > 0) {
                    if (FindLong(keptParts._M_start, keptParts._M_finish, part->first) == keptParts._M_finish)
                        modifiers->AddAppearanceModifierWithReplace(part->second);
                }
            }
        } else {
            CDefPointer<const CAppearanceModifierDef> clothing;
            long suitPart = 0;
            if (itemDef->GetSubDef(&clothing))
                suitPart = clothing->SuitPart;
            CLongListIterator it(worn._M_node->_M_next);
            while (it._M_node != worn._M_node) {
                CDefPointer<const CThingDef> wornDef;
                CDefPointer<const CAppearanceModifierDef> wornModifier;
                if (GetDefinitions()->GetDef(it._M_node->_M_data, &wornDef)) {
                    if (wornDef->GetSubDef(&wornModifier)) {
                        if (wornModifier->SuitPart == suitPart) {
                            if (!wornModifier->IsRemoveable)
                                return;
                            it = worn.erase(it);
                        } else
                            it._M_node = it._M_node->_M_next;
                    }
                }
            }
            modifiers->RemoveAllAppearanceModifiers(2);
            modifiers->AddAppearanceModifierWithReplace(item);
            if (ParentThing == thing && clothing.Object && !clothing->IsRemoveable) {
                CClothingEntryVector* entries = &ClothingEntries;
                SClothingEntry* entry = entries->_M_start;
                while (entry != ClothingEntries._M_finish) {
                    if (entry->SuitPart == clothing->SuitPart) {
                        NUISystem::CComponent* arrow = entry->Component->FindChild("PC_MENU_ENTRY_ARROW");
                        if (arrow) {
                            SActionParamPair parameter;
                            parameter.first = 1;
                            arrow->SetActionParameter(parameter);
                        }
                        entry->Component->Slot93();
                        entry = entries->erase(entry);
                    } else
                        ++entry;
                }
            }
            for (SLongListNode* node = worn._M_node->_M_next; node != worn._M_node; node = node->_M_next)
                modifiers->AddAppearanceModifierWithReplace(node->_M_data);
        }
    }
}