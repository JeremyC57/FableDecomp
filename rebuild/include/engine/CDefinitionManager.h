#ifndef FABLE_ENGINE_CDEFINITIONMANAGER_H
#define FABLE_ENGINE_CDEFINITIONMANAGER_H
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER
// Retail opt-in (Codex pass 11 consolidation 2): needs FABLE_RETAIL_CRESOURCELIST and FABLE_RETAIL_DEF_CLASS_BASE.
#include "engine/CResourceList.h"
#include "engine/CDefPointer.h"
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_SAVEBINARY_PASS23
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
class CDataOutputStream;class CMemoryDataOutputStream;
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefString.h"
#include "engine/CSymbolMap.h"
#include "engine/CDefClassInit.h"
#endif
// Original Ego_r TPI0x9fbd: public CResourceList base; retail009b02e0
// tears down complete native0xd0 object, with 12-byte STLport containers.
class CDefClassBase;
class CDefString;
class CDefinitionManager : public CResourceList {
    friend class CDefClassBase;
public:
#if defined(FABLE_RETAIL_CDEFINITIONMANAGER_CINSTANTIATEDEFINFO_DECL) && !defined(FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF)
    // agentNF28 (row 0x009B4BD0 vector<CDefinitionManager::CInstantiatedDefInfo*>::_M_insert_overflow): the nested record named
    // only, for TUs that use pointers to it without its layout (Ego_r.pdb CDefinitionManager::CInstantiatedDefInfo; InstantiatedDefs
    // +0xA4 is CArray<CInstantiatedDefInfo*>). Declaration only, no layout change.
    class CInstantiatedDefInfo;
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX
    // Retail opt-in (agentNF3, rows 0x006B3600 / 0x006B3680 / 0x006B37A0 / 0x006B3800 / 0x006B38D0, the CEnvironment theme setters):
    // Ego_r.pdb / FableWin.pdb ?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z (const), out of line in retail at
    // 0x009ACCA0; the theme setters call it through CEnvironment::DefinitionManager (+0x04) with the global theme index.
    // Declaration only, no layout change.
    long GetDefClassIndexFromGlobalIndex(long globalIndex) const;      // 0x009ACCA0
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_ADDDEFCLASS
    // Retail opt-in (row 0x004EE23F CTCBase::RegisterTCs): Ego_r.pdb / FableWin.pdb ?AddDefClass@CDefinitionManager@@QAEXABVCDefClassInit@@@Z,
    // out of line in retail at 0x009B0AC0 (landed); called 111 times by RegisterTCs on CGameDefinitionManager::Get(). Declaration only.
    void AddDefClass(const class CDefClassInit& init);                 // 0x009B0AC0
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_COMPILE
    // Retail opt-in (wfE11, row 0x0044C72B CGameDefinitionManager::InitAndCompile): Ego_r.pdb / FableWin.pdb
    // ?Compile@CDefinitionManager@@QAEXXZ, out of line in retail at 0x009B08C0 (landed). Declaration only, no layout change.
    void Compile();                                                     // 0x009B08C0
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_COMPILEDDEFDIR
    // agentFAM09 opt-in (row 0x01224050, its dynamic initializer): Ego_r.pdb / FableWin.pdb `dynamic initializer for
    // 'CDefinitionManager::CompiledDefDirectory'' (default CWideString at 0x013CA7DC; the static setter
    // ?SetCompiledDefDirectory@CDefinitionManager@@SIXABVCWideString@@@Z writes it); protected static per Anniv360 Test PDB (third
    // source) ?CompiledDefDirectory@CDefinitionManager@@1VCWideString@@A. Static member: no layout change.
protected:
    static CWideString CompiledDefDirectory;                             // 0x013CA7DC
public:
#endif
    virtual ~CDefinitionManager();
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_SAVEBINARY_PASS23
    // Both original complete records/PUBLICs; select existing INSTANTIATEDEF for its genuine nested records.
    void SaveBinaryDefinitions(CDataOutputStream&);
protected:
    void CompressDefinitionBlock(CMemoryDataOutputStream&,CArray<unsigned short>&,unsigned long);
public:
#endif
    long GetGlobalIndexFromClassIndex(const CCharString&,long) const;
    template<class T> bool GetDef(long,CDefPointer<const T>*) const;
    template<class T> bool GetDef(const CCharString&,long,CDefPointer<const T>*) const;   // ??$GetDef@VCSkyDef@@@..QBE_NABVCCharString@@JPAV..@Z (FableWin/Ego_r; chunk 727)
    template<class T> bool GetDef(const CCharString&,CDefPointer<const T>*) const;   // ??$GetDef@VCEngineVideoOptionsDef@@@..QBE_NABVCCharString@@PAV..@Z 0x00B28E40 (out of line in retail; chunk 731)
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF
    // Nested opt-in (chunk 867, row 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList): the non-template expression-def
    // lookup ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@VCExpressionDef@@@@@Z 0x005F81BE (FableWin.pdb; the landed
    // CTCExpression::OnCreate 0x007EEFE0 calls it too). The out handle is CDefPointer<CExpressionDef> (non-const T), so the
    // template overloads above do not deduce for it. Declared const: the rows reach it through CTCBase::GetDefinitionManager()
    // const (a const CGameDefinitionManager&); FableWin.pdb's spelling is the non-const QAE (noted in chunk 867's NOTES).
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSION_NONCONST
    bool GetDef(long,CDefPointer<class CExpressionDef>*); // FableWin exact QAE,005f81be
#else
    bool GetDef(long,CDefPointer<class CExpressionDef>*) const;   // 0x005F81BE
#endif
#endif
    CDefPointer<const CDefClassBase> GetPDefFromInstantiationName(const CCharString&) const;   // 0x009ADA40 ?GetPDefFromInstantiationName@..QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@@Z
    CDefPointer<const CDefClassBase> GetPDefFromClassIndex(const CCharString&,long) const;  // 0x009ADE10 ?GetPDefFromClassIndex@..QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z
    CDefString GetDefNameFromGlobalIndex(long) const;
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
    // Retail opt-in (chunk 1025, row 0x007C0AD0 NThingShot::CreateShot): Ego_r.pdb / FableWin.pdb
    // ?GetDefClassTypeNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z, out of line in retail at 0x009ACCE0. Declaration only.
    CDefString GetDefClassTypeNameFromGlobalIndex(long) const;          // 0x009ACCE0
#endif
    long GetDefGlobalIndexFromName(const CCharString&) const;
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
    // Nested opt-in (chunk 1012, rows 0x006B1A20 CAtmosProcess::UpdateDayNightAtmosSounds / 0x006B2260): Ego_r.pdb / FableWin.pdb
    // ?GetNoInstantiatedDefsOfType@CDefinitionManager@@QBEJABVCCharString@@@Z, out of line in retail at 0x009AD3B0 (Ego_r donor order
    // GetPDefClassInfo / GetGlobalIndexFromClassIndex / GetNoInstantiatedDefsOfType = retail 0x009AD2E0 / 0x009AD390 / 0x009AD3B0;
    // the callers pass CCharString("SOUND_THEME") and loop 1..count-1 over GetDef<CSoundThemeDef>). Declaration only.
    long GetNoInstantiatedDefsOfType(const CCharString&) const;         // 0x009AD3B0
#endif
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long) const;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long);
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
    // Retail opt-in (wf2, row 0x009ADC90 CDefinitionManager::InstantiateDef): the two nested records (Ego_r TPI
    // CDefinitionManager::CDefClassInfo: Name +0, AllocFunc +4, GlobalIndices +8, retail 20 bytes with the 12-byte STLport vector;
    // CDefinitionManager::CInstantiatedDefInfo, 16 bytes: ClassName +0, InstantiationName +4, ClassIndex +8, PDef +0xC), the typed
    // PSymbolMap / PDefinitionSymbolMap / InstantiatedDefs members (same offsets and sizes as the opaque default) and the two
    // protected members ?GetPDefClassInfo@CDefinitionManager@@IAEPAVCDefClassInfo@1@ABVCCharString@@@Z 0x009AD580 and
    // ?InstantiateDef@CDefinitionManager@@IAE_NABVCCharString@@0_N@Z 0x009ADC90 (Ego_r.pdb / FableWin.pdb).
    // The record's constructors are FableWin.pdb's (debug, out of line there; inline in retail and absent from Ego_r.pdb's list):
    // ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@VCDefString@@0KABV?$CDefPointer@VCDefClassBase@@@@@Z (FableWin 0x03061650:
    // two CDefString by value, an unsigned long and a `const CDefPointer<CDefClassBase>&`) and the default
    // ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@XZ (FableWin 0x0305D840; declared only, its body is not reconstructed). Public
    // symbols carry no parameter names: class_name / instantiation_name / class_index / pdef are descriptive.
    // Needs FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CSYMBOLMAP, FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT.
    class CDefClassInfo {
    public:
        CCharString Name;                                                // +0x00
        CDefClassInit::TAllocFunc AllocFunc;                             // +0x04 (__fastcall: FableWin ??0CDefClassInit@@QAE@ABVCCharString@@P6IPAVCDefClassBase@@XZ@Z)
        CArray<unsigned long> GlobalIndices;                             // +0x08
    };
    class CInstantiatedDefInfo {
    public:
        CInstantiatedDefInfo();                                          // FableWin 0x0305D840 (declaration only)
        CInstantiatedDefInfo(CDefString class_name, CDefString instantiation_name, unsigned long class_index, const CDefPointer<CDefClassBase>& pdef)
            : ClassName(class_name), InstantiationName(instantiation_name), ClassIndex(class_index), PDef(pdef) { }   // FableWin 0x03061650
        CDefString ClassName;                                            // +0x00
        CDefString InstantiationName;                                    // +0x04
        unsigned long ClassIndex;                                        // +0x08
        CDefPointer<CDefClassBase> PDef;                                 // +0x0c
    };
    CDefClassInfo* GetPDefClassInfo(const CCharString& name);            // 0x009AD580
    bool InstantiateDef(const CCharString& class_name, const CCharString& instantiation_name, bool is_template);   // 0x009ADC90
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_CLASSINFO_QUERIES
    // NF39 opt-in (rows 0x009AD390 / 0x009AD3B0; needs _INSTANTIATEDEF for CDefClassInfo): Ego_r.pdb donor order
    // ?GetPDefClassInfo@CDefinitionManager@@IBEPBVCDefClassInfo@1@ABVCCharString@@@Z 0x00B05200,
    // ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z 0x00B05260 and
    // ?GetNoInstantiatedDefsOfType@CDefinitionManager@@QBEJABVCCharString@@@Z 0x00B052D0 = retail 0x009AD2E0 / 0x009AD390 / 0x009AD3B0
    // (the const lookup by name, out of line; FableWin.pdb has the same spellings). Declarations only.
    const CDefClassInfo* GetPDefClassInfo(const CCharString& name) const;   // 0x009AD2E0
public:
    // GetGlobalIndexFromClassIndex is the header's existing public declaration (0x009AD390).
#ifndef FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
    long GetNoInstantiatedDefsOfType(const CCharString&) const;         // 0x009AD3B0
#endif
protected:
#endif
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF
    // Retail opt-in (wf5, row 0x009ADAE0 CDefinitionManager::InstantiateSubDef; needs FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF):
    // Ego_r.pdb / FableWin.pdb ?InstantiateSubDef@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@ABVCCharString@@K@Z
    // 0x009ADAE0 and ?LoadDef@CDefinitionManager@@IBEXK@Z 0x009AD6E0 (protected, const), plus the inline body of the protected
    // non-const GetPDefFromGlobalIndex(long) (below the class; retail expands it inline in InstantiateSubDef and also keeps
    // out-of-line copies at 0x009AD9E0 / 0x009ADA10). Declarations and an inline body only, no layout change.
    CDefPointer<CDefClassBase> InstantiateSubDef(const CCharString& class_name, unsigned long parent_index);   // 0x009ADAE0
    void LoadDef(unsigned long index) const;                             // 0x009AD6E0
#endif
    unsigned char PDefTypeDebugInfoMap[4];
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_PATHLISTS
    // Retail opt-in (wfE11, row 0x0044C72B CGameDefinitionManager::InitAndCompile): Ego_r.pdb CDefinitionManager SymbolPathList /
    // CompilePathList are std::vector<CWideString> (retail 12-byte STLport vectors +0x40 / +0x4C); InitAndCompile assigns them with
    // vector<CWideString>::operator= 0x00433DF0 (ecx = this+0x40 / this+0x4C). Include fable_string.h and stlport/fable_containers.h first.
    std::vector<CWideString> SymbolPathList,CompilePathList;
#else
    unsigned char SymbolPathList[12],CompilePathList[12];
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
    CCountedPointer<CSymbolMap> PSymbolMap,PDefinitionSymbolMap;
#else
    unsigned char PSymbolMap[8],PDefinitionSymbolMap[8];
#endif
    unsigned char NameCRCToIndexMap[16],DefClassInfo[16];
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_SAVEBINARY_PASS23
    CArray<unsigned char> CompressedDefData;
    CVectorMap<unsigned long,unsigned long> CompressedOffsets;
#else
    unsigned char CompressedDefData[12],CompressedOffsets[16];
#endif
    CArray<CInstantiatedDefInfo*> InstantiatedDefs;
#else
    unsigned char CompressedDefData[12],CompressedOffsets[16],InstantiatedDefs[12];
#endif
    unsigned char ErrorCallback[4],CurrentFilename[4],CompiledFileName[4];
    unsigned long DependencyCheckCRC;
    bool UseSafeBinaryFormat;
    unsigned char _pad_0xc1[3];
    unsigned char RecompiledDefs[12];
};
FABLE_STATIC_ASSERT_AT(146, sizeof(CDefinitionManager)==0xd0);
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF
// ?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z (Ego_r.pdb / FableWin.pdb): make sure the
// def is loaded, then hand out a counted copy of its record's handle (retail 0x009ADBE5..0x009ADBFF inside InstantiateSubDef).
inline CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    LoadDef(index);
    return InstantiatedDefs[index]->PDef;
}
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
// Original FableWin01ccd240 and native00513160: genuine global-index lookup.
#if defined(_MSC_VER)
template<class T> __declspec(noinline)
#else
template<class T>
#endif
bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out)const
{
 if(index>0){
  CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
  if(def.Get()){
   out->SetDynamicCast(def);
   return true;
  }
 }
 return false;
}
#endif
#ifdef FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
// Original FableWin01ccd0f0: genuine lookup, const conversion and output assignment.
#ifdef _MSC_VER
template<class T> __declspec(noinline)
#else
template<class T>
#endif
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out)const
{
 const CSubDefInfo* info=GetPSubDefInfo(T::StatGetTypeName());
 if(info){
  CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
  out->SetDynamicCast(def);
  return true;
 }
 return false;
}
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY
// Nested opt-in (chunk 1012, row 0x006B1A20 CAtmosProcess::UpdateDayNightAtmosSounds): the same-TU body of the by-class-index
// lookup ??$GetDef@V..@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBV..@@@@@Z (Ego_r.pdb / FableWin.pdb; the
// CSoundThemeDef instance is the landed 0x006B25E0 of the atmos TU: `index <= 0` fails, GetPDefFromClassIndex 0x009ADE10, the
// converting assignment on a hit). Called out of line in retail; with the body visible VC7.1 knows the caller's out-handle does not
// escape, keeps the def in esi across the def's virtual calls and drops the release's null test (`dec dword ptr [esi+4] ; jne`,
// 0x006B1B8A), which the declaration-only template reloads and null-tests.
#if defined(_MSC_VER)
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const T>* out) const
#else
template<class T> bool CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const T>* out) const
#endif
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromClassIndex(type_name, index);
    if (!def.IsNull()) {
        *out = def;
        return true;
    }
    return false;
}
#if defined(_MSC_VER)
#pragma auto_inline(on)
#endif
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_INLINE
// Opt-in (w24B, row 0x00B625E0 CEngineSkyRenderer::CEngineSkyRenderer): the by-class-index lookup
// ??$GetDef@VCSkyDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCSkyDef@@@@@Z (FableWin.pdb 0x02D46A60 only) EXPANDED
// in the caller (no CSkyDef instance in Ego_r.pdb): the constructor calls GetPDefFromClassIndex 0x009ADE10 itself at 0x00B62F7A and
// assigns the handle in line (0x00B62F7F..0x00B62FB6, re-reading the temporary after the release = SetDynamicCast, FableWin.pdb
// ??$SetDynamicCast@$$CBVCDefClassBase@@@?$CDefPointer@$$CBVCSkyDef@@@@QAEX..@Z). The text is the ..._GETDEF_INDEX_BODY body with
// SetDynamicCast on a hit, left to the inliner. Do not combine with ..._GETDEF_INDEX_BODY / ..._PASS22_GETDEF_INDEX_BODY.
template<class T> inline bool CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromClassIndex(type_name, index);
    if (!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY
// Retail opt-in (Codex pass 22, 0x006B16B0): the genuine called GetDef<CSoundThemeDef> 0x006B25E0
// uses SetDynamicCast with a reread of the source after releasing the old output. Its 113-byte body is
// independently verified from this TU; the older assignment-body opt-in is kept unchanged.
#if defined(_MSC_VER)
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const T>* out) const
#else
template<class T> bool CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const T>* out) const
#endif
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromClassIndex(type_name, index);
    if (!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#if defined(_MSC_VER)
#pragma auto_inline(on)
#endif
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE
// Opt-in (chunk 2055, row 0x00821A80 CMap::LoadFromFile): the by-name lookup
// ??$GetDef@V..@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBV..@@@@@Z EXPANDED in the caller (retail calls
// GetPDefFromInstantiationName 0x009ADA40 itself at 0x00821E95 and assigns the handle in line, 0x00821E9A..0x00821EC4). The text
// is the by-global-index body above with the name lookup: `def.Get()` test, SetDynamicCast on a hit. FableWin.pdb's debug copy
// 0x02247E20 is 40 instructions / 139 bytes and this text compiles to exactly that at /Od /EHsc (fwsize); the `!def.IsNull()` /
// `*out = def` text of ..._GETDEF_NAME_BODY is 41 / 142. Do not combine with ..._GETDEF_NAME_BODY.
template<class T> inline bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out) const
{
    CDefPointer<const CDefClassBase> def = GetPDefFromInstantiationName(name);
    if (def.Get()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
// Nested opt-in (chunk 1025, row 0x006B1D30 CAtmosProcess::SetAtmosSoundsAsMuted): the same-TU body of the by-name lookup
// ??$GetDef@V..@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBV..@@@@@Z (Ego_r.pdb / FableWin.pdb; the CSoundThemeDef
// instance is the landed 0x006B2660 of the atmos TU, FableWin.pdb 0x02247E20): GetPDefFromInstantiationName 0x009ADA40, the
// assignment on a hit (retail 0x006B2673..0x006B26BA). Called out of line in retail; with the body visible VC7.1 knows the caller's
// out-handle is only assigned through `out`.
#if defined(_MSC_VER)
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out) const
#else
template<class T> bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out) const
#endif
{
    CDefPointer<const CDefClassBase> def = GetPDefFromInstantiationName(name);
    if (!def.IsNull()) {
        *out = def;
        return true;
    }
    return false;
}
#if defined(_MSC_VER)
#pragma auto_inline(on)
#endif
#endif
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDefinitionManager = 0xf0.
// Retail layout: 12-byte STLport vector/map, 0x10 CVectorMap, 0xc CArray (landed dtor 0x009B02E0) -> sizeof 0xd0.
// Retail evidence: 4 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCPPointerInfo;
class CCharStringData;
class CSymbolMap;
class CWideStringData;
struct _func___cdecl_void_CCharString_ptr;

#pragma pack(push, 1)
class CDefinitionManager {
public:
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_CINSTANTIATEDEFINFO_DECL
    // agentNF28 (row 0x009B4BD0 vector<CDefinitionManager::CInstantiatedDefInfo*>::_M_insert_overflow): the nested record named
    // only, for TUs that use pointers to it without its layout (Ego_r.pdb CDefinitionManager::CInstantiatedDefInfo; InstantiatedDefs
    // +0xA4 is CArray<CInstantiatedDefInfo*>). Declaration only, no layout change.
    class CInstantiatedDefInfo;
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX
    // Retail opt-in (agentNF3, rows 0x006B3600 / 0x006B3680 / 0x006B37A0 / 0x006B3800 / 0x006B38D0, the CEnvironment theme setters):
    // Ego_r.pdb / FableWin.pdb ?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z (const), out of line in retail at
    // 0x009ACCA0; the theme setters call it through CEnvironment::DefinitionManager (+0x04) with the global theme index.
    // Declaration only, no layout change.
    long GetDefClassIndexFromGlobalIndex(long globalIndex) const;      // 0x009ACCA0
#endif
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_ADDDEFCLASS
    // Retail opt-in (row 0x004EE23F CTCBase::RegisterTCs): Ego_r.pdb / FableWin.pdb ?AddDefClass@CDefinitionManager@@QAEXABVCDefClassInit@@@Z,
    // out of line in retail at 0x009B0AC0 (landed); called 111 times by RegisterTCs on CGameDefinitionManager::Get(). Declaration only.
    void AddDefClass(const class CDefClassInit& init);                 // 0x009B0AC0
#endif
    void*                               __vftable;           // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                       _pad_0x04[0x4];      // +0x04
    unsigned char                       _pad_0x08[0x4];      // +0x08
    unsigned char                       _pad_0x0c[0x4];      // +0x0c
    unsigned char                       _pad_0x10[0x4];      // +0x10
    unsigned char                       _pad_0x14[0x4];      // +0x14
    void*                               m_classArray;        // +0x18 retail-only const CDefClassBase** (CDefinitionManager_GetPDefFromClassIndex_00c96430.cpp)
    unsigned char                       _pad_0x1c[0x4];      // +0x1c
    unsigned char                       _pad_0x20[0x4];      // +0x20
    unsigned char                       _pad_0x24[0x4];      // +0x24
    unsigned char                       _pad_0x28[0x4];      // +0x28
    unsigned char                       _pad_0x2c[0x4];      // +0x2c
    unsigned char                       _pad_0x30[0x4];      // +0x30
    unsigned char                       _pad_0x34[0x4];      // +0x34
    unsigned char                       _pad_0x38[0x4];      // +0x38
    void*                               PDefTypeDebugInfoMap; // +0x3c map<CCharString,CDefinitionManager::CDefTypeDebugInfo,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CDefinitionManager::CDefTypeDebugInfo>_>_> *
    unsigned char                       SymbolPathList[0xc]; // +0x40 vector<CWideString,std::allocator<CWideString>_> (retail 12-byte STLport vector)
    unsigned char                       CompilePathList[0xc]; // +0x4c vector<CWideString,std::allocator<CWideString>_>
    CSymbolMap*                         PSymbolMap_Data;     // +0x58 CCountedPointer<CSymbolMap>::Data
    CCPPointerInfo*                     PSymbolMap_Info;     // +0x5c CCountedPointer<CSymbolMap>::Info
    CSymbolMap*                         PDefinitionSymbolMap_Data; // +0x60 CCountedPointer<CSymbolMap>::Data
    CCPPointerInfo*                     PDefinitionSymbolMap_Info; // +0x64 CCountedPointer<CSymbolMap>::Info
    unsigned char                       NameCRCToIndexMap[0x10]; // +0x68 CVectorMap<unsigned_long,long,CKeyPairCompareLess<unsigned_long,long>_>
    unsigned char                       DefClassInfo[0x10];  // +0x78 CVectorMap<unsigned_long,CDefinitionManager::CDefClassInfo,CKeyPairCompareLess<unsigned_long,CDefinitionManager::CDefClassInfo>_>
    unsigned char                       CompressedDefData[0xc]; // +0x88 CArray<unsigned_char>
    unsigned char                       CompressedOffsets[0x10]; // +0x94 CVectorMap<unsigned_long,unsigned_long,CKeyPairCompareLess<unsigned_long,unsigned_long>_>
    unsigned char                       InstantiatedDefs[0xc]; // +0xa4 CArray<CDefinitionManager::CInstantiatedDefInfo_*>
    _func___cdecl_void_CCharString_ptr* ErrorCallback;       // +0xb0
    CCharStringData*                    CurrentFilename_PStringData; // +0xb4 CCharString::PStringData
    CWideStringData*                    CompiledFileName_PStringData; // +0xb8 CWideString::PStringData
    unsigned long                       DependencyCheckCRC;  // +0xbc
    bool                                UseSafeBinaryFormat; // +0xc0
    unsigned char                       _pad_0xc1[0x1];      // +0xc1
    unsigned char                       _pad_0xc2[0x2];      // +0xc2
    unsigned char                       RecompiledDefs[0xc]; // +0xc4 map<unsigned_long,CDefPointer<CDefClassBase>,std::less<unsigned_long>,std::allocator<std::pair<unsigned_long_const_,CDefPointer<CDefClassBase>_>_>_> (retail 12-byte STLport map)
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(336, sizeof(CDefinitionManager) == 0xd0);
FABLE_STATIC_ASSERT_AT(337, offsetof(CDefinitionManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(338, offsetof(CDefinitionManager, m_classArray) == 0x18);
FABLE_STATIC_ASSERT_AT(339, offsetof(CDefinitionManager, PDefTypeDebugInfoMap) == 0x3c);
FABLE_STATIC_ASSERT_AT(340, offsetof(CDefinitionManager, SymbolPathList) == 0x40);
FABLE_STATIC_ASSERT_AT(341, offsetof(CDefinitionManager, CompilePathList) == 0x4c);
FABLE_STATIC_ASSERT_AT(342, offsetof(CDefinitionManager, PSymbolMap_Data) == 0x58);
FABLE_STATIC_ASSERT_AT(343, offsetof(CDefinitionManager, PSymbolMap_Info) == 0x5c);
FABLE_STATIC_ASSERT_AT(344, offsetof(CDefinitionManager, PDefinitionSymbolMap_Data) == 0x60);
FABLE_STATIC_ASSERT_AT(345, offsetof(CDefinitionManager, PDefinitionSymbolMap_Info) == 0x64);
FABLE_STATIC_ASSERT_AT(346, offsetof(CDefinitionManager, NameCRCToIndexMap) == 0x68);
FABLE_STATIC_ASSERT_AT(347, offsetof(CDefinitionManager, DefClassInfo) == 0x78);
FABLE_STATIC_ASSERT_AT(348, offsetof(CDefinitionManager, CompressedDefData) == 0x88);
FABLE_STATIC_ASSERT_AT(349, offsetof(CDefinitionManager, CompressedOffsets) == 0x94);
FABLE_STATIC_ASSERT_AT(350, offsetof(CDefinitionManager, InstantiatedDefs) == 0xa4);
FABLE_STATIC_ASSERT_AT(351, offsetof(CDefinitionManager, ErrorCallback) == 0xb0);
FABLE_STATIC_ASSERT_AT(352, offsetof(CDefinitionManager, CurrentFilename_PStringData) == 0xb4);
FABLE_STATIC_ASSERT_AT(353, offsetof(CDefinitionManager, CompiledFileName_PStringData) == 0xb8);
FABLE_STATIC_ASSERT_AT(354, offsetof(CDefinitionManager, DependencyCheckCRC) == 0xbc);
FABLE_STATIC_ASSERT_AT(355, offsetof(CDefinitionManager, UseSafeBinaryFormat) == 0xc0);
FABLE_STATIC_ASSERT_AT(356, offsetof(CDefinitionManager, RecompiledDefs) == 0xc4);

#endif // FABLE_RETAIL_CDEFINITIONMANAGER
#endif // FABLE_ENGINE_CDEFINITIONMANAGER_H
