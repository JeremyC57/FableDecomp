// CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const -- set the usable flag on the
// thing's usable component (TC 0x0D), else on its scripted-use hook (TC 0xC2), adding the hook when it has neither
// Leaf: ?SetThingAsUsable@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008967F0, length 208 bytes, `ret 8`.
// Callees: the script thing's virtual slot 75 IsValid (+0x12C) and slot 11 GetThing (+0x2C), the TC map lower bound
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0.
// String 0x01239FE4 = "CTCActionUseScriptedHook".
// Layout: CThing interface bitset +0x20 (TC 0x0D = +0x20 bit 13, TC 0xC2 = +0x38 bit 2), TC map +0x44 {first,last};
// TC 0x0D's usable byte +0x10, CTCActionUseScriptedHook::Usable +0x0C.
// Shape: the out-parameter GetInterface template (landed 0x00896000 model) homes both lookup keys in the dead `thing`
// argument slot; the usable-TC arm returns, the hook arm is `if (!GetInterface(&hook)) { named CCharString; AddTC }`
// joined by one `hook->Usable = usable` (tail-duplicated, hook in esi on both paths). The same-TU LowerBound body
// (0x0040F020 stand-in, binary search over {type, component} pairs) tells VC7.1 the key's address does not escape,
// so the 0xC2 key and the hook name share the dead slot instead of opening a push-ecx frame word.

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CTCBase;
class CTCParamsBase;

struct CInterfaceMap_Retail {                    // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};


#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap_Retail::SEntry* CInterfaceMap_Retail::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CTCUsable_0D {                     // the TC 0x0D component
    unsigned char _pad[0x10];
    bool Usable;                          // +0x10
};

class CTCActionUseScriptedHook { public:
    unsigned char _pad[0xc];
    bool Usable;                          // +0x0C
};

class CThing { public:
    virtual void v000();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail m_interfaces;           // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&interfaceType);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const
{
    if (thing.IsAlive()) {
        CThing* t = thing.GetPThing();
        CTCUsable_0D* usableTC;
        CTCActionUseScriptedHook* hook;
        if (t->GetInterface(&usableTC, 0x0d)) {
            usableTC->Usable = usable;
            return;
        }
        if (!t->GetInterface(&hook, 0xc2)) {
            CCharString name("CTCActionUseScriptedHook", -1);
            hook = (CTCActionUseScriptedHook*)t->AddTC(name, false, 0);
        }
        hook->Usable = usable;
    }
}