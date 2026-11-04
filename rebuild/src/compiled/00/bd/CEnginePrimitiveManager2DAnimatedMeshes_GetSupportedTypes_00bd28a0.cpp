// CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes(CEnginePrimitiveTypeList& types) const -- report the primitive types this manager creates: append 0x25 and 0x26 to the caller's type list
// Leaf: ?GetSupportedTypes@CEnginePrimitiveManager2DAnimatedMeshes@@UBEXAAVCEnginePrimitiveTypeList@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD28A0, 122 bytes (+ int3 padding to 128), thiscall (`this` unused), `ret 4`.
// Identity (proven): CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes(CEnginePrimitiveTypeList&) const; evidence: the address is slot 2 of vtable 0x012A7964 (RTTI .?AVCEnginePrimitiveManager2DAnimatedMeshes@@, base array CEnginePrimitiveManager2DAnimatedMeshes <- CEnginePrimitiveManagerAnimatedMeshesBase <- CEnginePrimitiveManagerMeshBase <- CEnginePrimitiveManagerOcclusionBase <- CEnginePrimitiveManager <- CEnginePrimitiveRenderUtil <- CBaseClassNonCopyable <- CBaseClass), the GetSupportedTypes slot (const, one CEnginePrimitiveTypeList& argument: PDB ?GetSupportedTypes@CEnginePrimitiveManager2DAnimatedMeshes@@UBEXAAVCEnginePrimitiveTypeList@@@Z); its slot 1 (CreateInternalPrimitive, 0x00BD6B90) is shared or table-driven (no direct compare of both values); the body appends 0x25 then 0x26
// Label: the manifest's "CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes" names the right class (family J027: six two-type GetSupportedTypes overrides of one shape).
// Callees: STLport vector<long>::_M_insert_overflow 0x00440990 (the PDB-format STLport binding
//   ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z, protected; same-TU body kept out of
//   line, as in the landed 0x00B29E30 model).
// Layout: CEnginePrimitiveTypeList (PDB class: ctor, dtor, Add(EEnginePrimitiveType), Add(EEngineInternalPrimitiveType); no
//   layout record) holds one 12-byte STLport vector<long> at +0 (the body reads finish +4 / end of storage +8 of the argument);
//   the member name `Types` is descriptive. EEnginePrimitiveType's enumerator names are not in the PDBs: the values are written
//   as casts. Class chain from RTTI: CEnginePrimitiveManager2DAnimatedMeshes <- CEnginePrimitiveManagerAnimatedMeshesBase <- CEnginePrimitiveManagerMeshBase <- CEnginePrimitiveManagerOcclusionBase <- CEnginePrimitiveManager <- CEnginePrimitiveRenderUtil <- CBaseClassNonCopyable <- CBaseClass.
// Shape: two inline CEnginePrimitiveTypeList::Add calls (PDB ?Add@CEnginePrimitiveTypeList@@QAEXW4EEnginePrimitiveType@@@Z):
//   each push_back(const long&) binds the enum-to-long temporary (the first homed in the `push ecx` word, the second in the
//   dead `types` slot); STLport's inline push_back with the placement-new null test and the __true_type tag.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E (in the same-TU overflow body)
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

#include "engine/CArray.h"


enum EEnginePrimitiveType { };
class CEngineInternalPrimitiveBase;

class CEnginePrimitiveTypeList {
public:
    CArray<long> Types;                                     // +0x00 (descriptive member name)
    void Add(EEnginePrimitiveType type) { Types.push_back(type); }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
};
class CEngineScene;
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil {   // Ego_r.pdb 0xC bytes
public:
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type) = 0;   // slot 1
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const = 0;                       // slot 2
    CArray<CEngineScene*>* SceneList;                       // +0x04
    bool                   Enable;                          // +0x08
};
class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager {
};
class CEnginePrimitiveManagerMeshBase : public CEnginePrimitiveManagerOcclusionBase {
};
class CEnginePrimitiveManagerAnimatedMeshesBase : public CEnginePrimitiveManagerMeshBase {
};
class CEnginePrimitiveManager2DAnimatedMeshes : public CEnginePrimitiveManagerAnimatedMeshesBase {   // RTTI .?AVCEnginePrimitiveManager2DAnimatedMeshes@@, vtable 0x012A7964
public:
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;   // slot 2
};

void CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes(CEnginePrimitiveTypeList& types) const
{
    types.Add((EEnginePrimitiveType)0x25);
    types.Add((EEnginePrimitiveType)0x26);
}