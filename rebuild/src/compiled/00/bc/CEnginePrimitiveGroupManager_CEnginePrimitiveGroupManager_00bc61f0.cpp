// Leaf: ??0CEnginePrimitiveGroupManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// CEnginePrimitiveGroupManager::CEnginePrimitiveGroupManager(void*) -- base CEnginePrimitiveManagerOcclusionBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC61F0, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEnginePrimitiveManagerOcclusionBase::CEnginePrimitiveManagerOcclusionBase ; mov dword ptr [esi],offset CEnginePrimitiveGroupManager::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00B90270; the compiler then stores this class's vtable (0x012A66AC, named
// through rtti-vtables.tsv) and returns `this`. One of 12 bodies of this shape.

// NF83 (round 7, q11043 binder + own leaf): both constructors take the scene array by reference as both PDBs spell them
//   (??0CEnginePrimitiveManagerOcclusionBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z, ??0CEnginePrimitiveGroupManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z);
//   in-place parameter edit of the pre-existing local models (engine/CArray.h for the type).
#include "engine/CArray.h"
class CEngineScene;
struct CEnginePrimitiveManagerOcclusionBase {
    virtual void v00();
    CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>& scenes);   // 0x00B90270 (Ego_r.pdb / FableWin.pdb AAV?$CArray@PAVCEngineScene@@@@)
};

class CEnginePrimitiveGroupManager : public CEnginePrimitiveManagerOcclusionBase { public:
    virtual void v00();
    CEnginePrimitiveGroupManager(CArray<CEngineScene*>& scenes);
};

CEnginePrimitiveGroupManager::CEnginePrimitiveGroupManager(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManagerOcclusionBase(scenes)
{
}