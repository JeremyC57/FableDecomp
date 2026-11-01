// CThingCreatureDef::Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const -- the release build keeps only the side effects of the def checks (results unused): the creature graphic's mesh info (static/animated by graphic type), the appearance def's DEFAULT animation, and the mesh's dummy object / helper point named by the global strings
// Leaf: ?Validate@CThingCreatureDef@@UBEXABVCValidateDefinitionInitBase@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00670D50, length 533 bytes. `ret 8`.
// Callees: CCharString copy 0x0099EC30 / (const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CMeshDataBank::PeekStaticMesh 0x00A24280 / PeekAnimatedMesh 0x00A242B0 (Ego_r ?PeekStaticMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z),
//   CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::AnimExists 0x005DC2E0 (Ego_r
//   ?AnimExists@CAnimationSet@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z; CAnimFilterNoGroup temporary, vtables 0x0122FFDC /
//   base 0x0122FFCC), C3DMeshInfo::GetMeshStats 0x00A25530, CCRC::Calc 0x004014A0 (inline CCharString::GetCRC),
//   C3DMeshStats::HasHelperPoint 0x00A2EA20 / HasDummyObject 0x00A2EA60 (Ego_r order HasHelperPoint, HasDummyObject),
//   operator delete 0x00BFE9BC.
// Shape: "DEFAULT" reaches AnimExists through the implicit const char* -> CCharString conversion: the conversion temporary
//   is built before the argument pushes (so it takes cleanup bit 1 and the frame slot above the filter, and the filter's
//   inline base dtor keeps its 0x0122FFCC store ahead of the string dtor call); an explicit CCharString("DEFAULT") is an
//   ordinary right-to-left argument and builds the filter first. The type-5 mesh bank goes through a named
//   CMeshDataBank* (index then bank load order). GetSubDef is the landed 0x0042B0A2 body as a same-TU stand-in (as
//   0x004BFC50 does): the def pointer does not escape, so the release is the header's `dec [edi+4]` with no zero store.
// Layout: retail CThingCreatureDef = Ego_r less 8 here: PrimaryMeleeWeaponDef +0xd8, Graphic (CEngineGraphic) +0x100, its
//   Type byte +0x10d; CAppearanceDef Animation +0x34. init +8 is the retail-only resource holder whose +0x3c is the mesh
//   bank (descriptive). Globals: CCharStrings 0x013BAC30, 0x013BAC34, 0x013BAC48 (bound placeholder spellings).
class CCharStringData { public:
    char*         PData;                                    // +0x00 (CBasicString<char> Data)
    unsigned long StringLength;                             // +0x04
};
class CCRC { public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetAsCharArray(), GetLength()); }
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#define CDefinitionManager CDefinitionManager_PDB
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CDefinitionManager : public CDefinitionManager_PDB {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};
#include "engine/CCountedPointer.h"
extern CCharString g_CCharString_013bac30;                  // 0x013BAC30
extern CCharString g_CCharString_013bac34;                  // 0x013BAC34
extern CCharString g_CCharString_013bac48;                  // 0x013BAC48
class C3DMeshStats { public:
    bool HasHelperPoint(unsigned long key) const;           // 0x00A2EA20
    bool HasDummyObject(unsigned long key) const;           // 0x00A2EA60
};
class C3DMeshInfo { public:
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};
class CMeshDataBank { public:
    const C3DMeshInfo* PeekStaticMesh(unsigned long index) const;     // 0x00A24280
    const C3DMeshInfo* PeekAnimatedMesh(unsigned long index) const;   // 0x00A242B0
};
class CValidateResources { public: unsigned char _pad[0x3c]; CMeshDataBank* MeshBank; };   // descriptive
class CValidateDefinitionInitBase { public:
    virtual ~CValidateDefinitionInitBase();
    class CDefinitionManager* Manager;                      // +0x04
    CValidateResources*       Resources;                    // +0x08 (retail-only, descriptive)
};
class CAnimationEntry;
class CAnimFilterBase { public:                             // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual bool operator()(const CAnimationEntry& entry) const = 0;
    virtual CAnimFilterBase* Clone() const = 0;
};
class CAnimFilterNoGroup : public CAnimFilterBase { public: // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
    virtual bool operator()(const CAnimationEntry& entry) const;   // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                        // 0x00429A2F
};
class CAnimationSet { public:
    bool AnimExists(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x005DC2E0
};
class CAppearanceDef : public CSubDefClassBase {                    // RTTI .?AVCAppearanceDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAppearanceDef", -1); }   // ?StatGetTypeName@CAppearanceDef@@SI?AVCCharString@@XZ (inline)
    virtual void Transfer(CPersistContext&);                      // ?Transfer@CAppearanceDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                      // ?Copy@CAppearanceDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CAppearanceDef@@UAEKXZ
    unsigned char _pad_0x25[0x34 - 0x25];
    CAnimationSet Animation;                                      // +0x34 (PDB +0x38)
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (the landed body, as the landed
// 0x004BFC50 carries one): VC7.1 then knows the def pointer's address does not escape. Only the target row is graded.
template <>
bool CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CAppearanceDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
#pragma pack(push, 1)
class CEngineGraphic { public:                              // Ego_r.pdb, 14 bytes
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d (5 static mesh, 4 animated mesh)
};
#pragma pack(pop)
class CThingCreatureDef : public CDefClassBase { public:
    unsigned char  _pad25[0xd8 - sizeof(CDefClassBase)];
    long           PrimaryMeleeWeaponDef;                   // +0xd8 (Ego_r +0xe0)
    unsigned char  _padDC[0x100 - 0xdc];
    CEngineGraphic Graphic;                                 // +0x100 (Ego_r +0x108)
    virtual void Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const;
};

void CThingCreatureDef::Validate(const CValidateDefinitionInitBase& init, const CCharString&) const
{
    CValidateResources* resources = init.Resources;
    CCharString dummyName(g_CCharString_013bac30);
    CCharString helperName(g_CCharString_013bac34);
    const C3DMeshInfo* mesh = 0;
    if (Graphic.Type == 5) {
        CMeshDataBank* bank = resources->MeshBank;
        mesh = bank->PeekStaticMesh(Graphic.BankIndex);
    } else if (Graphic.Type == 4)
        mesh = resources->MeshBank->PeekAnimatedMesh(Graphic.BankIndex);
    CDefPointer<const CAppearanceDef> appearance;
    GetSubDef(&appearance);
    Graphic.Type != 4 || appearance->Animation.AnimExists("DEFAULT", CAnimFilterNoGroup());
    if (PrimaryMeleeWeaponDef)
        !mesh || mesh->GetMeshStats()->HasDummyObject(dummyName.GetCRC());
    !mesh || mesh->GetMeshStats()->HasHelperPoint(g_CCharString_013bac48.GetCRC());
}