// CTCBoastingCrowdControl::GetCrowd(CThing& hero, CThing** ppCrowd, CTCCreatureGroupBoastingCrowd** ppCrowdTC, bool create) -- find the live crowd thing kept for the hero (dropping a dead entry); optionally create a new "INTERNAL_MARKER_LIGHT" crowd thing with a CTCCreatureGroupBoastingCrowd and set its hero
// Grade: DIFFER(392v393) -- names now right (private AAE, hero by reference: the parked 393v393 file spelled QAE with CThing*); left: ebx/ebp swap between this and the map (+0x10), the definition-manager load (retail `mov eax,[eax+10h]; mov ecx,eax`, ours `mov ecx,[eax+10h]`: one byte), and the UID copy registers (eax/esi v ecx/edx); tried: inline GetDefinitionManager accessor (394, 00783a30_c), named uid local (inert), named value_type entry (prior sweep)
// Leaf: ?GetCrowd@CTCBoastingCrowdControl@@AAE_NAAVCThing@@PAPAV2@PAPAVCTCCreatureGroupBoastingCrowd@@_N@Z
// Retail 0x00783A30, 400 bytes, `ret 10h`.
// Callees: map<unsigned __int64, CIntelligentPointer<CThing>>::find 0x00782E90 / erase(iterator) 0x00783CC0 / insert(const
// value_type&) 0x00784040 (labels propagated), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/
// 0x00A01B90/0x00A01C10 (vtable 0x01238C6C), CVectorMap::LowerBound 0x0040F020 (interface 0x74), CCharString(const char*,
// long) 0x0099EBF0 ("INTERNAL_MARKER_LIGHT" 0x0125BB0C, "CTCCreatureGroupBoastingCrowd" 0x0123BFC0), ~CCharString
// 0x0099EAE0, the TC's game accessor 0x00686A80 (definition manager +0x10), CDefinitionManager::GetDefGlobalIndexFromName
// 0x009AD410, CThing::GetPosition 0x004C73D0, the thing factory Create(long, const C3DVector&) 0x00870BB0 (`__fastcall`),
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60, the crowd TC's hero setter 0x007841F0.
// Layout = Ego_r.pdb CTCBoastingCrowdControl: ParentThing +4, HeroCrowdThings +0x10 (STLport map: header pointer +0,
// node value at +0x10: the 64-bit key, the intelligent pointer +0x18); CThing UID +0x18 (64-bit), bitset +0x20, map +0x44,
// dead flag +0x91.
// Shape: the key is the hero's UID read by value (a frame copy passed by reference); the found iterator lives in the dead
// `hero` slot and is passed by value to erase (user copy ctor: built in its argument slot); the def index lives in the same
// slot; the new entry is a value_type built in place from the UID and the crowd thing; insert's result reuses the key slot.
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
class CBaseObject;
class CTCParamsBase;
class C3DVector { public: float X, Y, Z; };
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UID;                               // +0x18
    unsigned long    m_presentBits[9];                  // +0x20
    CInterfaceMap    m_interfaces;                      // +0x44
    unsigned char    _pad4c[0x91 - 0x4c];
    unsigned char    m_flags;                           // +0x91 bit 0: dead
    const C3DVector& GetPos() const;                    // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool flag, const CTCParamsBase* params);   // 0x004C9D60
    unsigned __int64 GetUID() const { return UID; }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CTCDInternalMarker {
public:
    static CThing* __fastcall Create(long defGlobalIndex, const C3DVector& pos);   // 0x00870BB0
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* m_pItem;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct SCrowdEntry {                                    // pair<const unsigned __int64, CIntelligentPointer<CThing> >
    unsigned __int64            first;
    CIntelligentPointer<CThing> second;
    SCrowdEntry(unsigned __int64 key, CThing* thing) : first(key), second(thing) { }
};
struct SCrowdNode {
    long        Color;
    SCrowdNode* Parent;
    SCrowdNode* Left;
    SCrowdNode* Right;
    SCrowdEntry Value;                                  // +0x10
};
struct CCrowdIterator {
    SCrowdNode* _M_node;
    CCrowdIterator(SCrowdNode* node) : _M_node(node) { }
    CCrowdIterator(const CCrowdIterator& other) : _M_node(other._M_node) { }
    bool operator!=(const CCrowdIterator& other) const { return _M_node != other._M_node; }
    SCrowdEntry* operator->() const { return &_M_node->Value; }
};
struct CCrowdInsertResult {
    CCrowdIterator first;
    bool           second;
};
struct CCrowdMap {
    SCrowdNode*   _M_header;                            // +0x00
    unsigned long _M_node_count;                        // +0x04
    unsigned long _M_key_compare;
    SCrowdNode* find(const unsigned __int64& key);      // 0x00782E90 (iterator returned in eax)
    void erase(CCrowdIterator it);                      // 0x00783CC0
    CCrowdInsertResult insert(const SCrowdEntry& value);   // 0x00784040
    CCrowdIterator end() { return CCrowdIterator(_M_header); }
};
class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { };
class CWorld {
public:
    unsigned char           _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;          // +0x10
};
class CTCBase : public CTCBaseLayout {
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80
};
class CTCCreatureGroupBoastingCrowd {
public:
    void SetHero(CThing& hero);                         // 0x007841F0
};
class CTCBoastingCrowdControl : public CTCBase {
public:
    bool          Active;                               // +0x0c
    CCrowdMap     HeroCrowdThings;                      // +0x10
    bool          CrowdSizesSuppressed;                 // +0x1c
private:
    bool GetCrowd(CThing& hero, CThing** ppCrowd, CTCCreatureGroupBoastingCrowd** ppCrowdTC, bool create);
};

bool CTCBoastingCrowdControl::GetCrowd(CThing& hero, CThing** ppCrowd, CTCCreatureGroupBoastingCrowd** ppCrowdTC, bool create)
{
    CCrowdIterator it(HeroCrowdThings.find(hero.GetUID()));
    if (it != HeroCrowdThings.end()) {
        *ppCrowd = it->second.GetPItem();
        if (*ppCrowd && !((*ppCrowd)->m_flags & 1)) {
            if ((*ppCrowd)->HasTC(0x74))
                *ppCrowdTC = (*ppCrowd)->GetTC<CTCCreatureGroupBoastingCrowd>(0x74);
            return true;
        }
        HeroCrowdThings.erase(it);
    }
    if (create) {
        long defIndex = PeekWorld().DefinitionManager->GetDefGlobalIndexFromName("INTERNAL_MARKER_LIGHT");
        *ppCrowd = CTCDInternalMarker::Create(defIndex, ParentThing->GetPos());
        *ppCrowdTC = (CTCCreatureGroupBoastingCrowd*)(*ppCrowd)->AddTC("CTCCreatureGroupBoastingCrowd", false, 0);
        (*ppCrowdTC)->SetHero(hero);
        HeroCrowdThings.insert(SCrowdEntry(hero.GetUID(), *ppCrowd));
        return true;
    }
    return false;
}
