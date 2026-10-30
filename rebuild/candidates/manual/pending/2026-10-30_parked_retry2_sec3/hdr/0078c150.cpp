// CTCBerserk::PlayHitEffectAtPos(const C3DVector& position) -- spawn one of the two berserk hit effects (picked at random) from the berserk def's special-effects sub-def at the position
// Grade: DIFFER(192v192) -- header-moved (engine/CDefPointer.h verify copy with the proposed ENullDef ctor, CSubDefClassBase chain, natural source: CDefPointer<const CSpecialEffectsDef> effects; GetSubDef(&effects); the header dtor gives retail's dec [esi+4] release and the length now matches). 59 difflines (the parked ReleaseDef stand-in model: 202v192, 29): this edi v esi, effects.Object loaded into esi right after GetSubDef in retail, frame 0xc v 8 (the name temporaries share a slot in retail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078C150, length 192 bytes, `ret 4`.
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) @ 0x00669580 (on PDef, +0x68),
// CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetPRandomSeed @ 0x0049D810, GFFloatRandom(float, unsigned long*) @ 0x004978A0
// (__fastcall), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString
// @ 0x00669D30, the __fastcall particle-effect spawner @ 0x0072AB70 (manifest "CreateParticleEffect"), the def's virtual
// Destroy (slot 1). Literals "BERSERK_HIT_EFFECT_01" 0x0126BC1C / "_02" 0x0126BC04; 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CTCBerserk PDef +0x6C sits at retail +0x68 (12-byte DependentThings vector).
// Shape: the TeleportIn/UndeadReturnToEarth sub-def lookup (0x008EC320): the out slot is block-scoped, so each arm's effect-name
// string reuses its frame word; the def is copied into esi and released by hand at the end; the two arms are complete
// statements that VC7.1 tail-merges after the string constructor, the lookup result riding in bl across the string's destructor.
struct C3DVector { float X, Y, Z; };
class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CSpecialEffectsDef : public CDefClassBase {
public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
class CSpecialAbilitiesBerserkDef : public CSubDefClassBase { };
class CWorld {
public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0
class CThing;
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* position);   // 0x0072AB70

class CTCBerserk {
public:
    virtual void v00();
    unsigned char _body04[0x68 - 4];
    CDefPointer<const CSpecialAbilitiesBerserkDef> PDef;   // +0x68
    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    void PlayHitEffectAtPos(const C3DVector& position);
};

void CTCBerserk::PlayHitEffectAtPos(const C3DVector& position)
{
    long effectType = 0;
    CDefPointer<const CSpecialEffectsDef> effects(NULL_DEF);
    if (PDef->GetSubDef(&effects)) {
        bool known;
        if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) > 0.5f)
            known = effects->GetEffectFromString(CCharString("BERSERK_HIT_EFFECT_01"), effectType);
        else
            known = effects->GetEffectFromString(CCharString("BERSERK_HIT_EFFECT_02"), effectType);
        if (known)
            CreateParticleEffect(effectType, &position);
    }
}
