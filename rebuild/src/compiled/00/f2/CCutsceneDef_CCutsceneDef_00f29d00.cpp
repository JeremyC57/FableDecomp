// CCutsceneDef::CCutsceneDef() -- inlined CDefClassBase over CResource (PDefManager and the three flag bits cleared), the packed scalar members after it (-1 and zeros), own vtable, then the eight empty CCharString vectors
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the -1 member is a CDefIndex-style record with an inline constructor and the scalar band
// +0x25..+0x36 sits in an intermediate novtable base: both keep the bit clear / -1 store above the vtable store (a plain `long` and
// plain derived members let the scheduler sink them below it).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F29D00, length 144 bytes.
// Callees: CResource::CResource @ 0x009FBEC0. This class's vtable is 0x012FB6E0.
// Layout: CResource +0x00..+0x1c (vtable, ref count, list links, size, frame); CDefClassBase (Ego_r.pdb, 37 bytes, packed):
// PDefManager +0x1c, GlobalIndex +0x20 (left for the loader), Setup/Template/DefaultValsApplied bits 0..2 of +0x24. The
// cutscene's own scalars follow unaligned at +0x25 (the class is pack(1) in retail as in the PDB); its eight
// vector<CCharString> members (Macro, SkipCond, Lights, LightScene, SetupCond, Sound, Answer0, Answer1) are retail's 12-byte
// STLport vectors at +0x3c..+0x9c (the PDB's are 16 bytes).
// Shape: the intermediate CDefClassBase is an abstract `__declspec(novtable)` base with an inline constructor, so its stores
// and the cutscene's plain initialisers precede the vtable store; the vectors' inline constructors (three zero words each)
// follow it. One zero register serves every store.
#pragma pack(push, 1)
struct CResource {
    virtual void v00();
    unsigned char _body[0x18];              // +0x04..+0x1c
    CResource();                            // 0x009FBEC0
};

struct CDefinitionManager;
struct __declspec(novtable) CDefClassBase : CResource {   // abstract: no vtable store of its own
    CDefinitionManager* PDefManager;        // +0x1c
    unsigned long GlobalIndex;              // +0x20 (not initialised here)
    unsigned char Setup : 1;                // +0x24 bit 0
    unsigned char Template : 1;             // +0x24 bit 1
    unsigned char DefaultValsApplied : 1;   // +0x24 bit 2
    CDefClassBase() : CResource(), PDefManager(0), Setup(0), Template(0), DefaultValsApplied(0) { }
};

struct CCharString;
template <class T> struct CAllocProxy {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >: the end-of-storage pointer with its own constructor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
struct CCharStringVector {                  // std::vector<CCharString> (STLport _Vector_base, 12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CAllocProxy<CCharString> _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CDefIndex {                          // a def index member with its own inline constructor (-1)
    long Index;
    CDefIndex() : Index(-1) { }
};

struct __declspec(novtable) CCutsceneDefBase : CDefClassBase {   // the scalar band before the vtable store
    CDefIndex     m_index;                  // +0x25 (-1)
    long          m_value29;                // +0x29
    long          m_value2d;                // +0x2d
    long          m_value31;                // +0x31
    unsigned char m_byte35;                 // +0x35 (not initialised here)
    unsigned char m_flag36;                 // +0x36
    unsigned char _pad37[5];                // +0x37..+0x3c
    CCutsceneDefBase() : m_value29(0), m_value2d(0), m_value31(0), m_flag36(0) { }
};
struct CCutsceneDef : CCutsceneDefBase {       // vtable 0x012FB6E0
    CCharStringVector Macro;                // +0x3c
    CCharStringVector SkipCond;             // +0x48
    CCharStringVector Lights;               // +0x54
    CCharStringVector LightScene;           // +0x60
    CCharStringVector SetupCond;            // +0x6c
    CCharStringVector Sound;                // +0x78
    CCharStringVector Answer0;              // +0x84
    CCharStringVector Answer1;              // +0x90
    virtual void v00();
    CCutsceneDef();
};
#pragma pack(pop)

CCutsceneDef::CCutsceneDef()
{
}