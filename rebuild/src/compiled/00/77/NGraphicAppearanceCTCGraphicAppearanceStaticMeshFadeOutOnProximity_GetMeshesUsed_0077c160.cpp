// NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::GetMeshesUsed(vector<CMeshInfo>& meshes) const -- add the appearance def's fade-out-when-close graphic (as the primary mesh) when it has one
// Leaf: GetMeshesUsed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C160, length 167 bytes (+ int3 padding). __fastcall, `ret 4`; `this` is the
//         CAGraphicAppearanceComponent subobject at +0x0C (ParentThing read as [ecx-8]).
//         Ego_r.pdb: `?GetMeshesUsed@CTCGraphicAppearanceStaticMeshFadeOutOnProximity@NGraphicAppearance@@UBEXAAV?$vector@...`.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const T>*) const @ 0x0042B0A2,
//          vector<CMeshInfo>::_M_insert_overflow(pos, const T&, const __true_type&, size_type, bool) @ 0x007091A0,
//          the defs' virtual slot 1 (destroy on the last release).
// Shape: the owner's base def is copied by hand (`d = PBaseDef; out = 0; if (d) ++count;`: the out slot's zero store lands
//        between the load and its null test) and released right after GetSubDef; the CMeshInfo is filled member by member
//        (the PrimaryMesh bit set on the uninitialised byte) and pushed through the inline STLport push_back (placement
//        copy, else _M_insert_overflow with the tag in the dead argument slot); the appearance def's release needs no
//        null test after its flag was read.
// Layout: retail CThing PBaseDef.Object +0x70; retail CAppearanceDef (PDB -4): FadeOutWhenCloseGraphic.BankIndex +0x5C,
//         .RenderSizeX +0x64, HasFadeOutWhenCloseGraphic +0x6A; CMeshInfo {Index, SizeX, PrimaryMesh:1} (12 bytes).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
namespace NGraphicAppearance {

class CMeshInfo {
public:
    long Index;                                     // +0x00
    float SizeX;                                    // +0x04
    unsigned char PrimaryMesh : 1;                  // +0x08
};

}

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0042B0A2 for CAppearanceDef
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CAppearanceDef : public CDefClassBase {
public:
    unsigned char _pad08[0x5c - 0x08];
    unsigned long FadeOutWhenCloseGraphicBankIndex; // +0x5C FadeOutWhenCloseGraphic.BankIndex
    float FadeOutWhenCloseGraphicAnimStep;          // +0x60
    float FadeOutWhenCloseGraphicRenderSizeX;       // +0x64
    unsigned char _pad68[2];                        // .AdditiveAlpha, .Type
    bool HasFadeOutWhenCloseGraphic;                // +0x6A
};

struct __true_type { __true_type() {} };           // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x007091A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

class CThing {
public:
    unsigned char _pad00[0x70];
    const CDefClassBase* PBaseDef;                  // +0x70 CDefPointer<const CThingBaseDef>::Object
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
};

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent {
public:
    virtual void GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const;
};

class CTCGraphicAppearanceStaticMeshFadeOutOnProximity : public CTCBase, public CAGraphicAppearanceComponent {
public:
    void* PrimHandle;                               // +0x10 CPrimitive*
    virtual void GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const;
};

void CTCGraphicAppearanceStaticMeshFadeOutOnProximity::GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const
{
    const CDefClassBase* baseDef = ParentThing->PBaseDef;
    CDefPointer<const CAppearanceDef> out;
    if (baseDef)
        ++baseDef->RefCount;
    baseDef->GetSubDef(&out);
    if (baseDef && --baseDef->RefCount == 0)
        baseDef->Destroy();
    const CAppearanceDef* def = out.Object;
    if (def->HasFadeOutWhenCloseGraphic) {
        CMeshInfo info;
        info.Index = def->FadeOutWhenCloseGraphicBankIndex;
        info.SizeX = def->FadeOutWhenCloseGraphicRenderSizeX;
        info.PrimaryMesh = true;
        meshes.push_back(info);
    }
    if (--def->RefCount == 0)
        def->Destroy();
}

}