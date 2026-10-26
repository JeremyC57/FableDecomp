// CThingPlayerCreature::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, const CThingPlayerCreatureInit* init) -- report progress, construct the creature base, reset health from the creature def and add the physics-controlled component
// Leaf: ConstructFromParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9DD0, length 285 bytes (+ int3 padding), `ret 10h`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, DisplayProgress(const CCharString&,
// float, bool, bool) @ 0x009E9F40 (`__fastcall`, landed spelling), CThingCreatureBase::ConstructFromParams(long, const
// C3DVector*, long, CThingPhysical*, CThingPhysical*, const CCharString&, bool) @ 0x00662880, CDefClassBase::GetSubDef<T>
// @ 0x0042B0A2 on the base def (+0x70; result unused), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
// the sub-def's slot 1 (destroy) on the last release.
// Strings: 0x0125D188 / 0x0125D15C "CThingPlayerCreature::ConstructFromParams 1" / "... 2", 0x0122D70E "",
// 0x01239F4C "CTCPhysicsControlled".
// Layout: CThingPlayerCreatureInit (Ego_r.pdb) {OriginalThing, PreviousThing, PlaceAtEndOfList}; CThingPhysical MaxHealth
// +0xb0 / Health +0xb4 (retail); CThingCreatureBase PDef +0xe0 (CThingCreatureDef, Health +0x50 as CThingBaseDef).
// Shape: every string is an implicit conversion temporary (re-addressed with `lea`; the later ones homed in the dead
// `init` slot); the base result is kept in bl across the "" destructor; `MaxHealth = Health = def->Health` (Health
// stored first); the unused sub-def is a CDefPointer homed in the dead `a3` slot, released through the pointee's
// Release() (DecRefToZero; ecx form) once GetSubDef has a same-TU `auto_inline(off)` stand-in (without it VC7.1 reloads
// the def into eax and emits the long count update).
class C3DVector;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&, float, bool, bool); }   // 0x009E9F40   // 0x009E9F40

__forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefObject {
    virtual void v00();
    virtual void Destroy();                                    // slot 1 (+0x04)
    long m_refCount;                                           // +0x04
    void Release() { if (DecRefToZero(m_refCount)) Destroy(); }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
};
struct CPlayerCreatureSubDef : CDefObject { };
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CPlayerCreatureSubDef>* out);   // 0x0042B0A2
};
void* FindDef_CDefClassBase_GetSubDef_0042b0a2();
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<T> 0x0042B0A2: writes the out slot only inline, so VC7.1 knows the def pointer's
// address does not escape the call (retail's LTCG knowledge; the landed 0x00894BF0 idiom).
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CPlayerCreatureSubDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0042b0a2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThingCreatureDef { public:
    unsigned char _pad00[0x50];
    float         Health;                                      // +0x50
};
class CThingPhysical;
class CThingPlayerCreatureInit { public:
    CThingPhysical* OriginalThing;                             // +0x00
    CThingPhysical* PreviousThing;                             // +0x04
    bool            PlaceAtEndOfList;                          // +0x08
};
class CTCBase;
class CTCParamsBase;

extern const char g_strConstructFromParams1[];                 // 0x0125D188
extern const char g_strConstructFromParams2[];                 // 0x0125D15C
extern const char g_emptyString_0122d70e[];                    // 0x0122D70E ("")
extern const char g_strCTCPhysicsControlled[];                 // 0x01239F4C

class CThing
{
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};
class CThingPhysical : public CThing
{
public:
};
class CThingGameObject : public CThingPhysical
{
public:
};

class CThingCreatureBase : public CThingGameObject {
public:
    unsigned char      _pad00[0x70];
    CDefClassBase*     PBaseDef;                               // +0x70
    unsigned char      _pad74[0xb0 - 0x74];
    float              MaxHealth;                              // +0xb0
    float              Health;                                 // +0xb4
    unsigned char      _pad_b8[0xe0 - 0xb8];
    CThingCreatureDef* PDef;                                   // +0xe0
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, CThingPhysical* originalThing,
                             CThingPhysical* previousThing, const CCharString& scriptName, bool placeAtEnd);   // 0x00662880
};
class CThingPlayerCreature : public CThingCreatureBase {
public:
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, const CThingPlayerCreatureInit* init);
};

bool CThingPlayerCreature::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, const CThingPlayerCreatureInit* init)
{
    NProgressDisplay::DisplayProgress(g_strConstructFromParams1, -1.0f, false, false);
    if (CThingCreatureBase::ConstructFromParams(defIndex, pos, a3, init->OriginalThing, init->PreviousThing,
                                                g_emptyString_0122d70e, init->PlaceAtEndOfList)) {
        NProgressDisplay::DisplayProgress(g_strConstructFromParams2, -1.0f, false, false);
        MaxHealth = Health = PDef->Health;
        CDefPointer<CPlayerCreatureSubDef> subDef;
        PBaseDef->GetSubDef(&subDef);
        AddTC(g_strCTCPhysicsControlled, true, 0);
        return true;
    }
    return false;
}