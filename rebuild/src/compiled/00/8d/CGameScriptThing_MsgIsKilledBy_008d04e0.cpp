// CGameScriptThing::MsgIsKilledBy(const CCharString& killer) const -- whether a "killed" message (type 9) aimed at this thing exists and the killer matches: any killer for an empty name, else the live originator whose script name equals the given one
// Leaf: MsgIsKilledBy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D04E0, length 299 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?MsgIsKilledBy@CGameScriptThing@@UBE_NABVCCharString@@@Z (vtable slot 18).
// Callees: CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, IsTargettedAtThing>> @ 0x008D46D0 (the landed
//   MsgWhoKilledMe 0x008D0610 filter), CBasicString<char>::operator==(const char*) @ 0x004115A0 ("" @ 0x0122D70E), the
//   strcmp intrinsic, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::GetScriptThing @ 0x004C7E50 (by value),
//   the script thing's slot 1 GetName, the CCharString compare @ 0x00411570 (__fastcall; its landed body compiled above as a
//   same-TU stand-in), ~CScriptThing @ 0x004AA840.
// Layout: engine/CGameScriptThing.h (ThingUID +0x18, World +0x44), engine/CWorld.h (PMessageEventManager_Data +0x60);
//   CMessageEvent POriginator +0x48; retail CThing ThingTypeFlags +0x6c, Dead bit +0x91.
// Shape: the MsgWhoKilledMe filter frame and typed-originator test (zero mask copied to a local: the dead store into the
//   dead `killer` slot plus `xor cl,cl ; jne`); the empty-name test is hand-split in the caller (strcmp("", "") on the
//   unassigned case, the block compare otherwise -- the landed SetButton1OverrideText 0x0048F7C0 form), every hit
//   `goto`-joined to one true exit; the GetScriptThing temporary exists only on the last `&&` arm, so VC keeps its
//   destruction flag in the first frame word and the comparison's result in bl.
#include <string.h>
#include "engine/CGameScriptThing.h"
#include "engine/CWorld.h"

#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Data.StringLength == other.PStringData->Data.StringLength)
            return Compare_00411570(PStringData->Data.PData, other.PStringData->Data.PData) ? false : true;
        return false;
    }
};

class CScriptThing { public:                                   // vtable 0x01238C8C
    virtual ~CScriptThing();                            // slot 0 (0x004AA840 is the out-of-line destructor)
    virtual const CCharString& GetName() const;         // slot 1
    void* PImp_Data;
    void* PImp_Info;
};
class CBaseObject;
struct CThingRetail {
    enum { ThingTypeMask = 0 };
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _bits91 : 7;
    CScriptThing GetScriptThing() const;                // 0x004C7E50
};
template <class T> inline bool IsThingOfType(const CThingRetail* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CThingRetail* GetPTypedItem() const
    {
        return (GetPItem() != 0 && IsThingOfType<CThingRetail>((CThingRetail*)GetPItem())) ? (CThingRetail*)GetPItem() : 0;
    }
};
class CMessageEvent { public:
    unsigned char           _pad00[0x48];
    CBaseIntelligentPointer POriginator;                // +0x48
    CBaseIntelligentPointer PTarget;                    // +0x50
};
class CMessageEventFilter_IsOfType { public:
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
class CMessageEventFilter_IsTargettedAtThing { public:
    unsigned __int64 UID;
    CMessageEventFilter_IsTargettedAtThing(unsigned __int64 uid) : UID(uid) { }
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsTargettedAtThing> CKilledMeFilter;
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CKilledMeFilter& filter);   // 0x008D46D0
};

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsKilledBy(const CCharString& killer) const;
};

bool CGameScriptThingMethods::MsgIsKilledBy(const CCharString& killer) const
{
    CMessageEventManager* messages = World->PMessageEventManager_Data;
    CMessageEvent* message = messages->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(9) && CMessageEventFilter_IsTargettedAtThing(ThingUID));
    if (message) {
        if (killer.PStringData == 0) {
            if (strcmp("", "") == 0)
                goto killed;
        } else if (killer.PStringData->Data == "")
            goto killed;
        CThingRetail* thing = message->POriginator.GetPTypedItem();
        if (thing && !thing->Dead && killer == thing->GetScriptThing().GetName())
            goto killed;
    }
    return false;
killed:
    return true;
}