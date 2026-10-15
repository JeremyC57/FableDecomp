// CScriptThing::GetDataString() -- the pointee's DataString string, or a copy of the static empty string when there is no pointee
// Leaf: GetDataString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA900, length 42 bytes (`?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ`, a const virtual).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30; virtual slot 3 of the pointee.
// CScriptThing is a pimpl handle {vptr, CCountedPointer<CScriptThing> PImp}: every query forwards to the
// implementation object at PImp_Data (+0x04) through its vtable. engine/CScriptThing.h gives the
// handle layout; the pointee's vtable is reproduced through a local slot view cast from PImp_Data
// (the landed GetPos 0x004AA980 / GetCurrentMapName 0x004AA9E0 siblings use the same model).
// Shape: `PImp_Data` loaded straight into ecx and null-tested; the hidden result pointer
// read into esi once for both arms. Null arm: the out-of-line CCharString copy constructor
// @ 0x0099EC30 builds the result from the global; else the virtual (slot 3, `call [eax+Ch]`)
// writes the result through the forwarded hidden pointer.
#include "engine/CScriptThing.h"

struct CCharString {
    char* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30 (out of line in retail; fable_string.h would inline it)
};
extern CCharString g_EmptyString;                    // 0x0143E8EC (static CCharString(""), initialiser 0x01224C50)

struct CScriptThingVirtuals {                       // vtable view of the implementation object
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual CCharString GetDataString() const;                 // slot 3 (+0x0c)
};

struct CScriptThing_Methods : CScriptThing {
    CCharString GetDataString() const;
};

CCharString CScriptThing_Methods::GetDataString() const
{
    const CScriptThingVirtuals* imp = (const CScriptThingVirtuals*)PImp_Data;
    if (imp == 0)
        return g_EmptyString;
    return imp->GetDataString();
}
