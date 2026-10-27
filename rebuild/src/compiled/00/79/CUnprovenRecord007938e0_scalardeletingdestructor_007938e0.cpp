// CUnprovenRecord_007938e0::`scalar deleting destructor'(unsigned int flags) -- destroy the record's sub-object and free the record when bit 0 is set
// Leaf: ??_GCUnprovenRecord_007938e0@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007938E0, 45 bytes, thiscall, `ret 4`.
// Identity (descriptive): unreferenced COMDAT; one scalar word, then a {intelligent pointer, word, intelligent pointer} sub-object at +4; both destructors are CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (+0x10 then +4). No caller, vtable or PDB symbol ties it to a named record.
// Callees: 0x00A01C10, 0x00A01C10 (the sub-object's member destructors), operator delete 0x00BFE9BC.
// Shape (family M117, representative 0x00526EF0): the compiler-generated non-virtual `??_G` -- the implicit destructor of the
//   sub-object at +4 inline (its `this` in edi, members destroyed in reverse), then the flag test; the delete-expression
//   in DeleteOne forces it into the object (not part of the retail body).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CCharString { public: ~CCharString(); void* PStringData; };          // ~ 0x0099EAE0
class CWideString { public: ~CWideString(); void* PStringData; };          // ~ 0x0099B510
class CBaseIntelligentPointer {                             // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* PData;
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
class CUnprovenPointerPair_007938e0 {                      // descriptive: {intelligent pointer, word, intelligent pointer}
public:
    CBaseIntelligentPointer First;                          // +0x0
    unsigned long Word;                                     // +0x8
    CBaseIntelligentPointer Second;                         // +0xC
};
class CUnprovenRecord_007938e0 {                                               // descriptive
public:
    unsigned long Words[1];                              // +0x0
    CUnprovenPointerPair_007938e0 Value;                                          // +0x4
};
void DeleteOne(CUnprovenRecord_007938e0* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}