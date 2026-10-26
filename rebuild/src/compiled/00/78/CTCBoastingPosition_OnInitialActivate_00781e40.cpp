// CTCBoastingPosition::OnInitialActivate() -- make the thing undrawable, clear the graphic-appearance flag, and place the orb at the "PODIUM_ORB_CREATION_DUMMY" helper (or 1.7 above the thing when the helper is missing)
// Leaf: ?OnInitialActivate@CTCBoastingPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00781E40, length 202 bytes (+ int3 padding). Vtable slot 6 ("vfunc_6" in the
// catalog = CTCBase::OnInitialActivate).
// Callees: CThing::SetAsDrawable(bool) (virtual slot 28, +0x70), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
// (inlined GetTC for the interface-9 component, `test ch,2`), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString
// @ 0x0099EAE0, CTCGraphicAppearance::GetHelperPoint @ 0x004C0560 (on ParentThing+0x64), CThing::GetPosition @ 0x004C73D0.
// Literals: "PODIUM_ORB_CREATION_DUMMY" 0x0126B2C0, 1.7f 0x0126B2BC.
// Layout = Ego_r.pdb CTCBoastingPosition: OrbPosition +0x14 (no containers before it, so PDB = retail).
// Shape: the checked out-parameter GetInterface keeps the component in eax for the byte store; the helper result is negated as
// `found ? false : true` (8-bit neg/sbb/inc in bl, kept across the string destructor); the fallback position is a
// named temporary filled member by member (x, y copied, z raised by 1.7) and assigned with the implicit operator=.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {                   // interface type 9 (also the thing's appearance pointer at +0x64)
    unsigned char _pad00[0x10];
    bool m_flag10;                              // +0x10
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time);   // 0x004C0560
};
class CThing {
public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    virtual void SetAsDrawable(bool drawable);  // slot 28 (+0x70)
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
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
};
class CTCBoastingPosition {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char PBoastingPodium[8];           // +0x0c CIntelligentPointer<CThing>
    C3DVector OrbPosition;                      // +0x14
    virtual void OnInitialActivate();
};

void CTCBoastingPosition::OnInitialActivate()
{
    ParentThing->SetAsDrawable(false);
    CTCGraphicAppearance* graphic;
    if (ParentThing->GetInterface(&graphic, 9))
        graphic->m_flag10 = false;
    CTCGraphicAppearance* appearance = ParentThing->PGraphicAppearance;
    if (appearance) {
        bool missing;
        {
            CCharString name("PODIUM_ORB_CREATION_DUMMY");
            missing = appearance->GetHelperPoint(name, 0, 0, &OrbPosition, 1.0f) ? false : true;
        }
        if (missing) {
            const C3DVector& position = ParentThing->GetPosition();
            C3DVector orb;
            orb.X = position.X;
            orb.Y = position.Y;
            orb.Z = position.Z + 1.7f;
            OrbPosition = orb;
        }
    }
}