// CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other) -- copy constructor: CBaseObject copy, own vtable, add-ref copy of the PImp counted pointer
// Leaf: ??0CScriptGameResourceObjectScriptedThingBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00905330, length 45 bytes.
// Callees: CBaseObject::CBaseObject(const CBaseObject&) @ 0x0099A390 (stores the CBaseObject vtable
// 0x0129A7D8 and clears the intelligent-pointer link at +4: a copied object starts unlinked).
// Vtable: 0x0127094C (CScriptGameResourceObjectScriptedThingBase, rtti-vtables.tsv).
// Shape: `other` loaded into edi and pushed for the base copy constructor; the derived vtable store
// follows the base call, then the counted pointer at +8 {Data, Info} is copied by its own inline copy
// constructor (a member object, so it is scheduled after the vtable store) and the shared count block
// is add-ref'd in place (`test eax,eax ; mov [esi+0Ch],eax ; je ; inc dword ptr [eax]`).
// Layout: engine/CScriptGameResourceObjectScriptedThingBase.h agrees (PImp_Data +8, PImp_Info +0xc,
// sizeof 0x10) but is a flat record with no base class or constructor, so the copy constructor's
// base call and vptr store need this local class model with the same offsets.
// CBaseObject (engine/CBaseObject.h: {vftable, IntelligentPointer}, 8 bytes) is likewise modelled as a
// polymorphic base with its out-of-line copy constructor; the flat header cannot be derived from.
class CCPPointerInfo { public:
    long m_count;                                     // +0x00 shared reference count
};

class CScriptGameResourceObjectScriptedThingBase;

struct CCountedPointer_ScriptedThingBase {            // CCountedPointer<CScriptGameResourceObjectScriptedThingBase>
    CScriptGameResourceObjectScriptedThingBase* Data;
    CCPPointerInfo*                             Info;
    CCountedPointer_ScriptedThingBase(const CCountedPointer_ScriptedThingBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

class CBaseObject { public:
    void* IntelligentPointer;                         // +0x04
protected:
    CBaseObject(const CBaseObject& other);               // 0x0099A390
    virtual ~CBaseObject();                              // 0x0099A430
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:
    CCountedPointer_ScriptedThingBase PImp;           // +0x08 PImp_Data / +0x0c PImp_Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other);
};

CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other)
    : CBaseObject(other), PImp(other.PImp)
{
}