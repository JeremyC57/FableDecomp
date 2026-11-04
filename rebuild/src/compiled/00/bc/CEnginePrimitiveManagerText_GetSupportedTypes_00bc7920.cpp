// CEnginePrimitiveManagerText::GetSupportedTypes(CEnginePrimitiveTypeList& types) const -- report the primitive types this manager creates: append 5 and 0x27
//   to the caller's type list
// Leaf: ?GetSupportedTypes@CEnginePrimitiveManagerText@@UBEXAAVCEnginePrimitiveTypeList@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7920, 122 bytes, thiscall (`this` unused), `ret 4`.
// Identity (naming wave 26, family gst): slot 2 of vtable 0x012A68B0 (RTTI .?AVCEnginePrimitiveManagerText@@), the GetSupportedTypes slot; Ego_r.pdb
//   ?GetSupportedTypes@CEnginePrimitiveManagerText@@UBEXAAVCEnginePrimitiveTypeList@@@Z. The body appends 5 then 0x27 (3D text, 2D text). Class chain from RTTI:
//   CEnginePrimitiveManagerText <- CEnginePrimitiveManager <- CEnginePrimitiveRenderUtil <- CBaseClassNonCopyable <- CBaseClass.
// Callees: STLport vector<long>::_M_insert_overflow 0x00440990 (?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z,
//   protected; same-TU body kept out of line).
// Model: the landed family-J027 row 0x00BD28A0 (CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes, the same 122-byte shape):
//   CEnginePrimitiveTypeList holds one 12-byte STLport vector<long> at +0 (member name descriptive) and its inline
//   Add(EEnginePrimitiveType) is the PDB ?Add@CEnginePrimitiveTypeList@@QAEXW4EEnginePrimitiveType@@@Z.
// Shape: each Add binds the enum-to-long temporary (the first homed in the `push ecx` word, the second in the dead `types` slot).
// Was CEnginePrimitiveManagerText::GetSupportedTypes(CRetailVector_long&) over a CRetailVector_long stand-in with InsertOverflow(..., SScalarTag,
//   unsigned long) and a GrowArrayStorage stand-in body.
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
class CEnginePrimitiveManagerText : public CEnginePrimitiveManager {   // RTTI .?AVCEnginePrimitiveManagerText@@, vtable 0x012A68B0
public:
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;   // slot 2
};

void CEnginePrimitiveManagerText::GetSupportedTypes(CEnginePrimitiveTypeList& types) const
{
    types.Add((EEnginePrimitiveType)5);
    types.Add((EEnginePrimitiveType)0x27);
}
