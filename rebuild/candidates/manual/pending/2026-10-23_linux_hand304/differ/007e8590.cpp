// Grade: DIFFER(645v650) -- 70 difflines: constant CSE -- retail keeps the interface bit mask 2 in bl across both interface-0x21 lookups and uses 0x21 as an immediate; ours enregisters 0x21 in ebx, which cascades into ebp/ebx swaps. Tried ternary/if GetInterface, enum key types, byte bitset test. The "" literal shows as mov ecx,0 (relocation).
// CTCOwnedEntity::SetPOwner(CThing* owner)
// Leaf: SetPOwner
#include "engine/CTCBase.h"
extern "C" int __cdecl _strnicmp(const char* a, const char* b, unsigned int n);   // 0x00F3FB90
struct CCharStringData {
    char* m_pData;                                      // +0x00
    long  m_length;                                     // +0x04
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    __forceinline bool BeginsWithNoCase(const CCharString& prefix) const
    {
        return _strnicmp(GetPString(), prefix.GetPString(), prefix.Length()) == 0;
    }
};
struct CBaseClass {
    CBaseClass();                                       // 0x0099A2D0
    ~CBaseClass();                                      // 0x0099A2E0
    virtual void v00();
};
struct CThing;
struct CUID { unsigned long Low, High; };              // ulong64 kept 4-aligned in the retail layout
namespace NTCMessages {
struct CMessageNewOwner : CBaseClass {                  // vtable 0x01270BF0
    CThing* POwner;                                     // +0x04
    CMessageNewOwner(CThing* owner) : POwner(owner) { }
    virtual void v01();
};
}
class CTCBaseComponent;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CInterfaceBits {                                 // bitset<> at CThing+0x20
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
struct CTCThingOwner {
    void RemoveDependentThing(CThing* thing);           // 0x0071B1B0
    void AddDependentThing(CThing* thing);              // 0x0071B300
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual long GetPlayerOwnership() const;            // slot 27 (+0x6c)
    virtual void SetPlayerOwnership(long value);        // slot 28 (+0x70)
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingType;                            // +0x10
    unsigned char _pad11[0x18 - 0x11];
    CUID UID;                                           // +0x18
    CInterfaceBits present;                             // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x78 - 0x4c];
    CCharString ScriptName;                             // +0x78
    unsigned char _pad7c[0x91 - 0x7c];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _91_1 : 4;
    unsigned char InLimbo : 1;                          // +0x91 bit 5
    unsigned char _91_6 : 2;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
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
    void SendMessage(const CBaseClass& message);        // 0x004C7820
    void RemoveTC(long type);                           // 0x004C9840
    CTCBaseComponent* AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    void SetInLimbo(bool inLimbo);                      // 0x004C8CF0
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                      // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    __forceinline bool IsAlive() const { return GetPItem() && !GetPItem()->Dead; }
};
#pragma pack(pop)
struct CThingFlags {
    unsigned char _pad00[0x2c];
    unsigned long Flags2C;                              // +0x2c
    unsigned char _pad30[0x38 - 0x30];
    unsigned long Flags38;                              // +0x38
};
struct CTCOwnedEntity : CTCBase {
    bool SwitchableNavigationTCAdded;                   // +0x0c
    CIntelligentPointer<CThing> POwner;                 // +0x0d
    unsigned char _pad15[3];
    CUID OwnerUID;                                      // +0x18
    void SetPOwner(CThing* owner);
};

void CTCOwnedEntity::SetPOwner(CThing* owner)
{
    if (POwner.GetPItem()) {
        CTCThingOwner* thingOwner;
        POwner.GetPItem()->GetInterface(&thingOwner, 0x21);
        thingOwner->RemoveDependentThing(ParentThing);
    }
    ParentThing->SendMessage(NTCMessages::CMessageNewOwner(owner));
    if (SwitchableNavigationTCAdded) {
        ParentThing->RemoveTC(0x79);
        SwitchableNavigationTCAdded = false;
    }
    POwner = CIntelligentPointer<CThing>(owner);
    if (POwner.IsAlive()) {
        OwnerUID = POwner.GetPItem()->UID;
        CTCThingOwner* thingOwner;
        if (!POwner.GetPItem()->GetInterface(&thingOwner, 0x21))
            thingOwner = (CTCThingOwner*)POwner.GetPItem()->AddTC("CTCThingOwner", 0, 0);
        thingOwner->AddDependentThing(ParentThing);
        ParentThing->SetPlayerOwnership(POwner.GetPItem()->GetPlayerOwnership());
        ParentThing->SetInLimbo(POwner.GetPItem()->InLimbo);
        bool addNavigation = !(((CThingFlags*)ParentThing)->Flags2C & 0x2000000) &&
            (((CThingFlags*)POwner.GetPItem())->Flags38 & 0x200000) && ParentThing->ThingType == 5 &&
            ParentThing->ScriptName.BeginsWithNoCase("Dress");
        if (addNavigation) {
            ParentThing->AddTC("CTCSwitchableNavigation", 0, 0);
            SwitchableNavigationTCAdded = true;
        }
    }
}
