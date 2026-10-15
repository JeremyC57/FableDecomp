// CEnginePrimitiveTypeInfo::RegisterPrimitiveManager(CEnginePrimitiveManager* manager) -- asks the manager for the primitive types it supports and records it as the manager of each of those types
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8FAD0, length 112 bytes.
// Signature from Ego_r.pdb: ?RegisterPrimitiveManager@CEnginePrimitiveTypeInfo@@QAEXPAVCEnginePrimitiveManager@@@Z
// Callees: the manager's virtual slot 2 (+0x08, GetSupportedTypes(vector<EEnginePrimitiveType>&)), 0x00BFEA14 free
// (the local vector's buffer).
// Layout: engine/CEnginePrimitiveTypeInfo.h (PDB): PrimitiveManagers[47] at +0x10.
// Shape: a local STLport vector<long> (three zeroed words, stored after the argument push) filled by the virtual
// call; the indexed loop re-counts the vector after every store (`sub ; sar 2 ; jb`) and reads each element as
// `*(begin() + i)`; the vector's inline destructor frees a non-null buffer.
#include "engine/CEnginePrimitiveTypeInfo.h"

extern "C" void __cdecl free(void* block);               // 0x00BFEA14

struct CPrimitiveTypeVector {                            // STLport vector<EEnginePrimitiveType>: first, last, end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CPrimitiveTypeVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CPrimitiveTypeVector()
    {
        if (m_pFirst)
            free(m_pFirst);
    }
    long* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    long operator[](unsigned long n) const { return *(begin() + n); }
};

struct CEnginePrimitiveManager {
    virtual void v00();
    virtual void v01();
    virtual void GetSupportedTypes(CPrimitiveTypeVector& types);   // slot 2 (+0x08)
};

struct CEnginePrimitiveTypeInfo_Methods : CEnginePrimitiveTypeInfo {
    void RegisterPrimitiveManager(CEnginePrimitiveManager* manager);
};

void CEnginePrimitiveTypeInfo_Methods::RegisterPrimitiveManager(CEnginePrimitiveManager* manager)
{
    CPrimitiveTypeVector types;
    manager->GetSupportedTypes(types);
    for (unsigned long i = 0; i < types.size(); ++i)
        PrimitiveManagers[types[i]] = manager;
}