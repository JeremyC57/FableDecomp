// CTCBuyableHouse::OnInitialActivate() -- re-take the buyable-house def, make sure the parent has a thing-owner component, find the house's sign posts among the building's contents (by def name), remap the wife pointer after loading, kill a live main sign post and create a new one when the house needs it
// Grade: DIFFER(734v734) -- scheduling only: retail loads ParentThing (AddTC this) before the 0,0,&name pushes (lea eax), ours after (lea ecx); the 0x21 lookup key is lea ecx before lea edi in retail, lea edx after in ours
// Leaf: OnInitialActivate
//
// Retail 0x006C14D0, 734 bytes, 8-aligned ebp frame, plain `ret`.
// Callees: CTCBase::GetDefinitions 0x006869D0, CDefinitionManager::GetDef 0x0042AF3C / GetDefGlobalIndexFromName
// (const CCharString&) 0x009AD410, CDefClassBase::GetSubDef<CBuyableHouseDef> 0x006C1B00 (same-TU stand-ins as the landed
// OnCreate), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThing::AddTC 0x004C9D60, CVectorMap
// LowerBound 0x0040F020 (0x21 CTCBuildingContents), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/
// 0x00A01B50/0x00A01B90/0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), GFRemapToThingAfterSerialisation
// 0x0088AFF0 (__fastcall), CThing::Kill(bool) 0x004C9B80, CTCBuyableHouse::NeedsMainSignPost 0x006BFB90 /
// CreateMainSignPost 0x006C0F00.
// Globals: the four sign-post def names 0x013BAAF8 / 0x013BAB00 / 0x013BAB04 / 0x013BAAFC (CCharString).
// Model: the landed CTCBuyableHouse::OnCreate (0x006BF9E0); engine/CTCBuyableHouse.h offsets (MainSignPost +0x24,
// DressingSignPost +0x2c, WifeLivingHere +0x38).
extern const struct CCharString g_SignPostDefName0;   // 0x013BAAF8
extern const struct CCharString g_SignPostDefName1;   // 0x013BAB00
extern const struct CCharString g_SignPostDefName2;   // 0x013BAB04
extern const struct CCharString g_DressingSignPostDefName;   // 0x013BAAFC
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                      // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};
template <class T> struct CArray {              // retail std::vector (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
};
struct CBuyableHouseDef : CDefPointeeBase {
    unsigned char _pad08[0x25 - 0x08];
    bool          IsBuyable;                    // +0x25
    bool          InitiallyEmpty;               // +0x26
    unsigned char _pad27;
    CArray<long>  Price;                        // +0x28
    CArray<long>  Rent;                         // +0x34
    long          DaysBetweenRentPayments;      // +0x40
    long          MaxRentBags;                  // +0x44
    bool          IsScripted;                   // +0x48
    bool          IsBuyableWithoutSignpost;     // +0x49
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CBuyableHouseDef>* out);   // 0x006C1B00
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CBuyableHouseDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CCharString;
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CDefClassBase>* out);     // 0x0042AF3C
    long GetDefGlobalIndexFromName(const CCharString& name);           // 0x009AD410
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class CTCBase;
struct CTCParamsBase;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                    // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  m_stateFlags;                // +0x91 (bit 0: dead)
    void Kill(bool immediately);                // 0x004C9B80
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem((CBaseObject*)other.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_stateFlags & 1))
            return true;
    }
    return false;
}
void __fastcall GFRemapToThingAfterSerialisation(CBaseIntelligentPointer* pointer);   // 0x0088AFF0
struct SThingListNode {
    SThingListNode*         next;
    SThingListNode*         prev;
    CIntelligentPointer<CThing> value;          // +0x08
};
struct CTCBuildingContents {                    // interface 0x21
    unsigned char   _pad00[0x0c];
    SThingListNode* ThingsHead;                 // +0x0c (VC6 list head)
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    CThing* GetParentThing() const { return ParentThing; }
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CDefinitionManager* GetDefinitions();       // 0x006869D0
};
class CTCBuyableHouse : public CTCBase {
public:
    CDefPointer<const CBuyableHouseDef> PDef;   // +0x0c
    bool          IsScripted;                   // +0x10
    bool          OwnedByPlayer;                // +0x11
    bool          Rented;                       // +0x12
    long          DaySomeoneWillMoveIn;         // +0x14
    long          DayNextRentIsDue;             // +0x18
    long          CurrentDressLevel;            // +0x1c
    long          MaxDressLevel;                // +0x20
    CIntelligentPointer<CThing> MainSignPost;   // +0x24
    CIntelligentPointer<CThing> DressingSignPost;   // +0x2c
    long          FrameToNextCheckIfWeNeedToCreateAnything;   // +0x34
    CIntelligentPointer<CThing> WifeLivingHere; // +0x38
    bool NeedsMainSignPost();                   // 0x006BFB90
    void CreateMainSignPost();                  // 0x006C0F00
    void OnInitialActivate();
};


void CTCBuyableHouse::OnInitialActivate()
{
    if (GetParentThing()->DefIndex != 0) {
        CDefPointer<const CDefClassBase> out;
        GetDefinitions()->GetDef(GetParentThing()->DefIndex, &out);
        CDefClassBase* def = (CDefClassBase*)out.Object;
        def->GetSubDef(&PDef);
        if (def && --def->m_refCount == 0)
            def->Delete();
    }
    if (!GetParentThing()->HasTC(0x21))
        GetParentThing()->AddTC("CTCThingOwner", false, 0);
    long signPost0 = GetDefinitions()->GetDefGlobalIndexFromName(g_SignPostDefName0);
    long signPost1 = GetDefinitions()->GetDefGlobalIndexFromName(g_SignPostDefName1);
    long signPost2 = GetDefinitions()->GetDefGlobalIndexFromName(g_SignPostDefName2);
    long dressingSignPost = GetDefinitions()->GetDefGlobalIndexFromName(g_DressingSignPostDefName);
    CTCBuildingContents* contents = 0;
    if (GetParentThing()->HasTC(0x21))
        contents = GetParentThing()->GetTC<CTCBuildingContents>(0x21);
    SThingListNode* head = contents->ThingsHead;
    for (SThingListNode* node = head->next; node != head; node = node->next) {
        CThing* thing = node->value.GetPItem();
        if (thing) {
            long index = thing->DefIndex;
            if (index == signPost0)
                MainSignPost = CIntelligentPointer<CThing>(thing);
            else if (index == signPost1)
                MainSignPost = CIntelligentPointer<CThing>(thing);
            else if (index == signPost2)
                MainSignPost = CIntelligentPointer<CThing>(thing);
            else if (index == dressingSignPost)
                DressingSignPost = CIntelligentPointer<CThing>(thing);
        }
    }
    GFRemapToThingAfterSerialisation(&WifeLivingHere);
    if (IS_THING_ALIVE(MainSignPost)) {
        MainSignPost.GetPItem()->Kill(true);
        MainSignPost = CIntelligentPointer<CThing>(0);
    }
    if (NeedsMainSignPost() && !IS_THING_ALIVE(MainSignPost))
        CreateMainSignPost();
}
