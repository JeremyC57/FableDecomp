// CTCInventoryClothing::AddClothingToThing(long, CThing*) -- dress a thing in a clothing item or a whole hero suit, replacing the worn pieces it displaces
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Name fix (agentNF 2026-11-10): `worn` is the shared STLport 4.6 list<long> (was a local CLongList): _List_base ctor 0x0042AB58 /
//   dtor 0x0042AB73, list<long>::erase 0x00429B02 and GetAppearanceModifierList 0x00706800 bound under their real spellings; the ctor keeps
//   its same-TU out-of-line body (R&D I-3 below), now the STLport _List_base body as an explicit specialisation.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "stlport/fable_containers.h"   // free 0x00BFEA14

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                            // 0x0099EAE0
};
// STLport 4.6 list<long> (stlport/fable_containers.h): _List_base constructor 0x0042AB58 / destructor 0x0042AB73 out of line
// (FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE), erase(iterator) out of line at 0x00429B02 (explicit specialisation, declared only).
template<> std::list<long>::iterator std::list<long>::erase(std::list<long>::iterator position);   // 0x00429B02
struct CLongVector {                                           // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    void push_back_005bba94(const long& value);                         // 0x005BBA94
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
template<class T> class CDefPointer;
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // GetSubDef<CHeroSuitDef> 0x005935C4 / <CAppearanceModifierDef> 0x0057FF92 (Ego_r / FableWin templates)
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    FABLE_FORCEINLINE ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
// Layout: retail CAppearanceModifierDef (Ego_r.pdb - 8, the two 16-byte containers are 12 in retail).
class CAppearanceModifierDef : public CDefClassBase { public:
    unsigned char _pad08[0x44 - 0x08];
    bool IsRemoveable;                                         // +0x44
    unsigned char _pad45[0x4c - 0x45];
    long SuitPart;                                             // +0x4c
};
class CHeroSuitDef : public CDefClassBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CSuitPartMap Parts;                                        // +0x28
};
class CThingBaseDef : public CDefClassBase { public: };
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
class CGameDefinitionManager : public CDefinitionManager
{
};
void* FindDef_43a2_0();
void* FindDef_43a2_1();
void* FindDef_43a2_2();
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CHeroSuitDef>(CDefPointer<const CHeroSuitDef>* out) const
{
    void* def = FindDef_43a2_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out) const
{
    void* def = FindDef_43a2_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingBaseDef>(long index, CDefPointer<const CThingBaseDef>* out) const
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
class CComponent { public:
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

enum EHeroAttachableAppearanceModifierType { };      // Ego_r enum (value 2 used here)
class CTCHeroAttachableAppearanceModifiers { public:                  // interface 0x5E
    void GetAppearanceModifierList(std::list<long>& out, EHeroAttachableAppearanceModifierType type) const;   // 0x00706800
    void RemoveAppearanceModifier(long def);                   // 0x00706370
    void RemoveAllAppearanceModifiers(long type);              // 0x00706640
    void AddAppearanceModifierWithReplace(long def);           // 0x00706950
};
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    std::bitset<274>  m_present;                                  // +0x20
    CInterfaceMap  m_interfaces;                               // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((void*)((it == map->end() || wanted < it->first) ? map->end() : it)->second);
            return true;
        }
        return false;
    }
};

// Layout: retail CTCInventoryClothing (12-byte containers): ParentThing +0x04, the clothing menu entries vector +0x134.
class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
    long GetNumberOfItemsOfTypeInInventory(long type) const;   // 0x005BDF08 (Ego_r.pdb QBE)
};
}

class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    void*   __vftable;
    CThing* ParentThing;                                       // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CClothingEntryVector ClothingEntries;                      // +0x134
    void AddClothingToThing(long item, CThing* thing);
};
// R&D I-3: the list ctor's same-TU out-of-line body (it builds only the sentinel) lets the default allocator
// temporary be homed in the dead byte of the `thing` argument slot, as in retail.
#pragma auto_inline(off)
template<> std::_List_base<long, std::allocator<long> >::_List_base(const std::allocator<long>&)
    : _M_node(std::allocator<std::_List_node<long> >(), 0)
{
    std::_List_node<long>* n = _M_node.allocate(1);
    n->_M_next = n;
    n->_M_prev = n;
    _M_node._M_data = n;
}
#pragma auto_inline(on)
void CTCInventoryClothing::AddClothingToThing(long item, CThing* thing)
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (thing->GetInterface(&modifiers, 0x5e)) {
        CDefPointer<const CThingBaseDef> itemDef;
        GetDefinitionManager().GetDef(item, &itemDef);
        CDefPointer<const CHeroSuitDef> suitDef;
        std::list<long> worn;
        modifiers->GetAppearanceModifierList(worn, (EHeroAttachableAppearanceModifierType)2);
        if (itemDef->GetSubDef(&suitDef)) {
            std::list<long>::iterator node = worn.begin();
            CLongVector keptParts;
            for (; node != worn.end(); ++node) {
                CDefPointer<const CThingBaseDef> wornDef;
                CDefPointer<const CAppearanceModifierDef> wornModifier;
                if (GetDefinitionManager().GetDef(*node, &wornDef)) {
                    if (wornDef->GetSubDef(&wornModifier)) {
                        if (wornModifier->IsRemoveable)
                            modifiers->RemoveAppearanceModifier(*node);
                        else
                            keptParts.push_back_005bba94(wornModifier->SuitPart);
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
            std::list<long>::iterator it = worn.begin();
            while (it != worn.end()) {
                CDefPointer<const CThingBaseDef> wornDef;
                CDefPointer<const CAppearanceModifierDef> wornModifier;
                if (GetDefinitionManager().GetDef(*it, &wornDef)) {
                    if (wornDef->GetSubDef(&wornModifier)) {
                        if (wornModifier->SuitPart == suitPart) {
                            if (!wornModifier->IsRemoveable)
                                return;
                            it = worn.erase(it);
                        } else
                            ++it;
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
            for (std::list<long>::iterator node = worn.begin(); node != worn.end(); ++node)
                modifiers->AddAppearanceModifierWithReplace(*node);
        }
    }
}