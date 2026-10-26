// CDefinitionManager::GetNumberOfDefsOfType(const CCharString& typeName) -- the number of global def indices registered for a def class, 0 when the class is unknown
// Leaf: GetNumberOfDefsOfType
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AD3B0, 33 bytes, `ret 4`.
// Identity: the manifest label LTextSystem::GetIDFromSymbol is propagated. Landed callers (CTCInventoryAbilities::
// InitialiseExpressions 0x005F3E3E, InitialiseAbilities 0x005EEDE2, CEventHitBySpecialAbility 0x00811830) call it as
// CDefinitionManager::GetNumberOfDefsOfType(const CCharString&) (e.g. "EXPRESSION"); this row keeps that symbol.
// Callee: the class-info lookup @ 0x009AD2E0 (Ego_r.pdb GetPDefClassInfo(const CCharString&); bound as GetClassInfo by the
// landed CDefinitionManager::AddDefClass 0x009B0AC0, whose declaration is reused so the address keeps one symbol).
// Layout: Ego_r.pdb CDefClassInfo {CCharString Name +0, AllocFunc +4, CArray<unsigned long> GlobalIndices +8}; the retail
// array is {start +8, finish +0x0C, end +0x10}. engine/CDefinitionManager.h is a data-only layout with no member functions,
// so `this` is a local model like the landed CDefinitionManager rows (the function touches no member directly).
// Shape: the null result is copied to ecx for the test; the count is (finish - start) >> 2.
class CCharString;
typedef void* (__cdecl* TDefCreateFn)();

template <class T> struct CRetailVector {          // CArray / STLport vector body: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};

struct CDefClassInfo {
    CCharString*                 Name;             // +0x00 (CCharString data pointer)
    TDefCreateFn                 Create;           // +0x04 AllocFunc
    CRetailVector<unsigned long> Instances;        // +0x08 GlobalIndices
};

class CDefinitionManager { public:
    CDefClassInfo* GetClassInfo(const CCharString& className);   // 0x009AD2E0
    long GetNumberOfDefsOfType(const CCharString& typeName);
};

long CDefinitionManager::GetNumberOfDefsOfType(const CCharString& typeName)
{
    CDefClassInfo* info = GetClassInfo(typeName);
    if (info)
        return info->Instances.size();
    return 0;
}