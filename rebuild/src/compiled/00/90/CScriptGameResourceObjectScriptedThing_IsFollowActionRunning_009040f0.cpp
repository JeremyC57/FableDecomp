// CScriptGameResourceObjectScriptedThing::IsFollowActionRunning(const CScriptThing& target) const -- true when the live scripted thing's control is running a CActionFollowCreature (the target is only checked for life)
// Leaf: IsFollowActionRunning
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009040F0, length 168 bytes (`ret 4`), vtable 0x0128D86C slot 9.
// PDB: ?IsFollowActionRunning@CScriptGameResourceObjectScriptedThing@@UBE_NABVCScriptThing@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CTCScriptedControl::IsActionRunning(const
// CCharString&) @ 0x00712D70, CCharString::~CCharString @ 0x0099EAE0. String 0x0128D850 "CActionFollowCreature".
// Shape: the action-type name is a named CCharString in its own block (it shares the dead `target` slot with
// the lookup key); the result survives the string's destructor in bl and both false exits share `xor al,al`.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing_Retail { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x20];
    unsigned long  ThingTypeFlags;                 // +0x6c
    unsigned char  _pad70[0x21];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

class CTCScriptedControl;

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}


class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length);  // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

class CTCScriptedControl { public:                      // interface 0x1f component
    bool IsActionRunning(const CCharString& actionName) const;   // 0x00712D70
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    bool IsFollowActionRunning(const CScriptThing& target) const;
};

bool CScriptGameResourceObjectScriptedThing_Methods::IsFollowActionRunning(const CScriptThing& target) const
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (target.IsAlive()) {
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                bool running;
                {
                    CCharString name("CActionFollowCreature", -1);
                    running = control->IsActionRunning(name);
                }
                if (running)
                    return true;
            }
        }
    }
    return false;
}