// CPSCManager::CPSCManager() -- empty registered-PSC vector map, register every particle-system component class, then sort the map if the registrations left it dirty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2970, length 51 bytes.
// Leaf: ??0CPSCManager
// Callees: NParticleEngine::CPSCManager::RegisterPSCs @ 0x00AF8170, SGI sort<pair<CCharString,
// CRegisteredPSCInfo>*, CKeyPairCompareLess> @ 0x00AF3B70 (__fastcall /Gr template: first in ecx,
// last in edx, the comparator by value on the stack; 16-byte elements, `sar esi,4`; the manifest
// label "std_introsort_recursive" names its loop).
// Layout (Ego_r.pdb): CPSCManager is one CVectorMap<CCharString, CRegisteredPSCInfo> (RegisteredPSCs);
// the retail vector map is 16 bytes: a 12-byte vector, the empty Compare at +0xc, Dirty at +0xd.
// Shape: the map's inline constructor zeroes the vector and Dirty with one callee-saved zero (ebx,
// reused after the call); the inline "sort if dirty" member copies the empty comparator's byte into
// the argument slot (`xor eax,eax ; mov al,[esi+0Ch] ; push eax`) and clears Dirty again.
namespace NParticleEngine {

struct SRegisteredPSCEntry {                // pair<CCharString, CRegisteredPSCInfo> (16 bytes)
    unsigned char _body[0x10];
};

struct CKeyPairCompareLess { };             // empty comparator, passed by value

void __fastcall SortRegisteredPSCs(SRegisteredPSCEntry* first, SRegisteredPSCEntry* last, CKeyPairCompareLess comp);    // 0x00AF3B70

struct CRegisteredPSCMap {                  // CVectorMap<CCharString, CRegisteredPSCInfo>
    SRegisteredPSCEntry* m_first;           // +0x00
    SRegisteredPSCEntry* m_last;            // +0x04
    SRegisteredPSCEntry* m_end;             // +0x08
    CKeyPairCompareLess  Compare;           // +0x0c
    bool                 Dirty;             // +0x0d
    CRegisteredPSCMap() : m_first(0), m_last(0), m_end(0), Dirty(false) { }
    void SortIfDirty()
    {
        if (Dirty) {
            SortRegisteredPSCs(m_first, m_last, Compare);
            Dirty = false;
        }
    }
};

struct CPSCManager {
    CRegisteredPSCMap RegisteredPSCs;       // +0x00
    void RegisterPSCs();                    // 0x00AF8170
    CPSCManager();
};

CPSCManager::CPSCManager()
{
    RegisterPSCs();
    RegisteredPSCs.SortIfDirty();
}
}