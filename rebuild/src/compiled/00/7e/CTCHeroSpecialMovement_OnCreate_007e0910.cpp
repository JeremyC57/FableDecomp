// CTCHeroSpecialMovement::OnCreate() -- look up the parent thing's def by its def index and take its hero special-movement sub-def; then clear Sprinting and preset the sprint frames from the def's sprint duration
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0910, length 105 bytes (+7 int3 padding). Vtable 0x0126FE3C slot 4 (the
// catalog's "vfunc_4"; slot 4 of the CTCBase vtable is OnCreate).
// Callees: CTCBase::GetDefinitions() @ 0x006869D0 (manifest "_Cons_val<...>", propagated), CDefinitionManager::GetDef(long,
// CDefPointer*) @ 0x0042AF3C (manifest names a propagated instantiation), CDefClassBase::GetSubDef<CHeroSpecialMovementDef>
// (CDefPointer*) @ 0x007E0A40 (propagated label), the def's slot 1 on release.
// Globals: the frames-per-second long 0x01375550.
// Shape: the def index is an unsigned short (`mov ax,[eax+8Ch] ; test ax,ax`, zero-extended for the call); the GetDef arguments
// are pushed before the GetDefinitions() object call; the def out pointer starts null in the `push ecx` frame word and is
// released by hand (null test, `dec [edi+4] ; jne`); `(float)fps * duration` loads the long first.
// Layout: Ego_r.pdb CTCHeroSpecialMovement (same offsets in retail): Sprinting +0x0c, Running +0x0d, SprintFramesRemaining +0x10,
// RunTimer +0x14, PHeroSpecialMovementDef +0x18; CHeroSpecialMovementDef sprint duration +0x28; CThing def index +0x8c.
extern long g_framesPerSecond_01375550;         // 0x01375550

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
struct CHeroSpecialMovementDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    float SprintDurationSeconds;                // +0x28
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CHeroSpecialMovementDef>* out);    // 0x007E0A40
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CHeroSpecialMovementDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CDefClassBase>* out);     // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
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
class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                    // +0x8c
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CDefinitionManager* GetDefinitions();       // 0x006869D0
};
class CTCHeroSpecialMovement : public CTCBase {
public:
    bool  Sprinting;                            // +0x0c
    bool  Running;                              // +0x0d
    float SprintFramesRemaining;                // +0x10
    long  RunTimer;                             // +0x14
    CDefPointer<const CHeroSpecialMovementDef> PHeroSpecialMovementDef;   // +0x18
    virtual void OnCreate();
};

void CTCHeroSpecialMovement::OnCreate()
{
    unsigned short defIndex = ParentThing->DefIndex;
    if (defIndex) {
        CDefPointer<const CDefClassBase> out;
        GetDefinitions()->GetDef(defIndex, &out);
        CDefClassBase* def = (CDefClassBase*)out.Object;
        def->GetSubDef(&PHeroSpecialMovementDef);
        if (def && --def->m_refCount == 0)
            def->Delete();
    }
    Sprinting = false;
    SprintFramesRemaining = (float)g_framesPerSecond_01375550 * PHeroSpecialMovementDef->SprintDurationSeconds;
}