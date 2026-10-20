// CTCHeroAttachableAppearanceModifiers::GetMeshesUsed(vector<NGraphicAppearance::CMeshInfo>& meshes) const -- append one mesh-info record (bank index, render size, not primary) per modifier graphic
// Leaf: ?GetMeshesUsed@CTCHeroAttachableAppearanceModifiers@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706C90, length 118 bytes (+10 int3 padding), `ret 4`.
// Callees: STLport vector<NGraphicAppearance::CMeshInfo>::_M_insert_overflow(CMeshInfo*, const CMeshInfo&, const __true_type&,
// unsigned, bool) @ 0x007091A0 (the inline push_back's full-storage path).
// Shape: GetMeshesUsed is a virtual of the secondary NGraphicAppearance::CAGraphicAppearanceComponent base at +0xc, so VC
// compiles it with `this` at that subobject and every member offset is 0xc lower (Graphics, retail CTCHero... +0x3c, is read
// at +0x30). The CMeshInfo temporary (Index, SizeX, PrimaryMesh bit cleared) is built in the frame, then the inline STLport
// push_back: placement copy (null-tested) and `_M_finish += 1`, or _M_insert_overflow with a fresh `__true_type()` that
// lands in the dead `meshes` argument slot (its value lives in esi).
// Layout: local model (engine/CTCHeroAttachableAppearanceModifiers.h is the PDB layout with 16-byte containers): retail
// FrameLastChanged +0x10, flags +0x14, PrimHandles +0x18, HiddenMaterialNames +0x24, AppearanceModifiers +0x30, Graphics +0x3c
// (12-byte STLport containers); CModifierGraphic = {CDefPointer PTextureReplacement, long BankIndex, float RenderSizeX}.
inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type { __true_type() {} };
namespace NGraphicAppearance {
struct CMeshInfo {
    long          Index;
    float         SizeX;
    unsigned char PrimaryMesh : 1;
    CMeshInfo(long index, float sizeX, bool primary) : Index(index), SizeX(sizeX) { PrimaryMesh = primary; }
};
}
struct CMeshInfoVector {                        // STLport vector<NGraphicAppearance::CMeshInfo>
    NGraphicAppearance::CMeshInfo* _M_start;
    NGraphicAppearance::CMeshInfo* _M_finish;
    NGraphicAppearance::CMeshInfo* _M_end_of_storage;
    void _M_insert_overflow(NGraphicAppearance::CMeshInfo* pos, const NGraphicAppearance::CMeshInfo& x,
                            const __true_type& tag, unsigned int fillLen, bool atEnd);   // 0x007091A0
    void push_back(const NGraphicAppearance::CMeshInfo& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) NGraphicAppearance::CMeshInfo(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* FindRef_CMeshInfoVector__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CMeshInfoVector::_M_insert_overflow(NGraphicAppearance::CMeshInfo* pos, const NGraphicAppearance::CMeshInfo& x, const __true_type& tag, unsigned int fillLen, bool atEnd)
{
    void* p = FindRef_CMeshInfoVector__M_insert_overflow_0();
    if (!p)
        return;
    *(void**)pos = p;
    if (*(void**)&x != p)
        return;
}
#pragma auto_inline(on)
struct CDefPointerTex { void* Object; };
struct CModifierGraphic {
    CDefPointerTex PTextureReplacement;         // +0x00
    long           BankIndex;                   // +0x04
    float          RenderSizeX;                 // +0x08
};
struct CModifierGraphicVector {
    CModifierGraphic* _M_start;
    CModifierGraphic* _M_finish;
    CModifierGraphic* _M_end_of_storage;
};
struct CRetailContainer12 { void* a; void* b; void* c; };

class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
namespace NGraphicAppearance {
class CAGraphicAppearanceComponent {
public:
    virtual void OnAppearanceClear();
    virtual void GetMeshesUsed(CMeshInfoVector& meshes) const;
};
}
class CTCHeroAttachableAppearanceModifiers : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent {
public:
    unsigned long          FrameLastChanged;    // +0x10
    unsigned char          AppearanceModifiersLoaded : 1;   // +0x14
    unsigned char          GraphicsDirty : 1;
    CRetailContainer12     PrimHandles;         // +0x18
    CRetailContainer12     HiddenMaterialNames; // +0x24
    CRetailContainer12     AppearanceModifiers; // +0x30
    CModifierGraphicVector Graphics;            // +0x3c
    virtual void OnAppearanceClear();
    virtual void GetMeshesUsed(CMeshInfoVector& meshes) const;
};

void CTCHeroAttachableAppearanceModifiers::GetMeshesUsed(CMeshInfoVector& meshes) const
{
    for (const CModifierGraphic* it = Graphics._M_start; it != Graphics._M_finish; ++it)
        meshes.push_back(NGraphicAppearance::CMeshInfo(it->BankIndex, it->RenderSizeX, false));
}