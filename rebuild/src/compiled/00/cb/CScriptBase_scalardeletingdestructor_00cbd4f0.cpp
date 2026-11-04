// CScriptBase::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCScriptBase@@UAEPAXI@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD4F0, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012C1648 (no RTTI), the vtable the landed constructor 0x00CB8110 stores last
//   (??0CScriptBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb); FableWin.pdb ??_GCScriptBase@@UAEPAXI@Z; Ego_r.pdb ??1CScriptBase@@UAE@XZ; FableWin.pdb ??1CScriptBase@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012c1648::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00CB8110 (rebuild/src/compiled/00/cb/CScriptBase_CScriptBase_00cb8110.cpp), its own source unchanged. The constructor body is not part of this graded body.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (_STLP_USE_MALLOC node allocator, no OOM path)
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CCharString { public: void* m_pStringData; };
template <class T> class CCountedPointer { public: T* Data; void* Info; };   // 8 bytes
class CSpawnedFuncBase;
class CActiveEntityScriptBase;
class CEntityScriptBindingBase;
#include "engine/CVectorMap.h"
template <class V> class CCompareString { };                 // empty
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo { public: unsigned __int64 UID; CCharString PersistScript; };
    CVectorMap<unsigned __int64, CPersistInfo, CKeyPairCompareLess<unsigned __int64, CPersistInfo> > PersistScripts;
};
}
class CScriptBase : public CBaseClassNonCopyable {
public:
    CScriptBase();
    virtual ~CScriptBase();
protected:
    std::list<CCountedPointer<CSpawnedFuncBase> > Threads;                                    // +0x04
    CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>,
               CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > > EntityScripts;   // +0x08
    CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>,
               CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;              // +0x18
    unsigned long     LastWorldUpdateFrame;                                                    // +0x28
    CSpawnedFuncBase* PActiveThread;                                                           // +0x2c
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;                       // +0x30
};

CScriptBase::CScriptBase()
    : PActiveThread(0)
{
}
