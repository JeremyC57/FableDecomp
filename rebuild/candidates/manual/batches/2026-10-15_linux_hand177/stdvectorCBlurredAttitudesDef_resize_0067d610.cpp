// std::vector<CBlurredAttitudesDef>::resize(unsigned int n) -- STLport one-argument resize: build a default CBlurredAttitudesDef temporary, call resize(n, value), destroy the temporary
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067D610, length 151 bytes (padded to 160), `ret 4`.
// Label correction: the catalog's "CVectorMap::resize" is the vector of the 0x48-byte CBlurredAttitudesDef (vtable 0x0125B8AC,
// the store into the frame temporary; the callee 0x0067CDD0 is the two-argument resize stepping 0x48 bytes).
// Callees: CResource::CResource @ 0x009FBEC0 (catalog "CResource"), vector<CBlurredAttitudesDef>::resize(n, const T&)
// @ 0x0067CDD0, free @ 0x00BFEA14 (x2, the two member vectors' storage), CResource::~CResource @ 0x009FC550.
// Shape (landed 0x00F349B0 model): the inline default constructor chain CDefClassBase (dword +0x1c, three 1-bit flags at
// +0x24), CParentDefClassBase (InstantiationName -1 at +0x25, SubDefInfoMap vector +0x29 and Dirty +0x36, pack(1)) and the
// own Attitudes vector +0x3c; the intermediate vtable stores are dead; the derived classes have IMPLICIT destructors (a
// user-declared `virtual ~X() {}` keeps its vtable store alive across the free calls); they free Attitudes then the map
// vector and the inline CDefPointeeBase destructor stores 0x01230BA0 before the out-of-line CResource destructor.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct CResource {
    virtual ~CResource();                          // 0x009FC550
    unsigned long m_refCount;                      // +0x04
    void*         m_pResourceList;                 // +0x08
    CResource*    m_pPrevResource;                 // +0x0c
    CResource*    m_pNextResource;                 // +0x10
    unsigned long m_resourceSize;                  // +0x14
    unsigned long m_lastUsedFrame;                 // +0x18
    CResource();                                   // 0x009FBEC0
};
struct CDefPointeeBase : CResource {               // vtable 0x01230BA0
    virtual ~CDefPointeeBase() {}
};
#pragma pack(push, 1)
struct CDefClassBase : CDefPointeeBase {           // vtable 0x01230BCC
    long          m_defIndex;                      // +0x1c
    long          m_reserved;                      // +0x20 (not initialised here)
    unsigned char m_flag0 : 1;                     // +0x24 three 1-bit fields
    unsigned char m_flag1 : 1;
    unsigned char m_flag2 : 1;
    CDefClassBase() : m_defIndex(0), m_flag0(0), m_flag1(0), m_flag2(0) {}
};
#pragma pack(pop)
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) {}
};
struct CSubDefInfoVector {                         // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CSubDefInfoVector() : m_first(0), m_last(0), m_end(0) {}
    ~CSubDefInfoVector() { if (m_first) free(m_first); }
};
struct CKeyPairCompareLess {
};
struct CSubDefInfoMap {                            // CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, ...>
    CSubDefInfoVector   Vector;                    // +0x00
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                Dirty;                     // +0x0d
    CSubDefInfoMap() : Dirty(false) {}
};
#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {       // vtable 0x01230CBC
    CDefString     InstantiationName;              // +0x25
    CSubDefInfoMap SubDefInfoMap;                  // +0x29
    CParentDefClassBase() {}
};
#pragma pack(pop)
struct CAttitudeVector {                           // retail std::vector<EOpinionAttitudeType> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CAttitudeVector() : m_first(0), m_last(0), m_end(0) {}
    ~CAttitudeVector() { if (m_first) free(m_first); }
};
struct CBlurredAttitudesDef : CParentDefClassBase {   // vtable 0x0125B8AC, 0x48 bytes
    CAttitudeVector Attitudes;                     // +0x3c
    CBlurredAttitudesDef() {}
};
struct CBlurredAttitudesDefVector {
    CBlurredAttitudesDef* m_first;
    CBlurredAttitudesDef* m_last;
    CBlurredAttitudesDef* m_end;
    void resize(unsigned int n, const CBlurredAttitudesDef& value);   // 0x0067CDD0
    void resize(unsigned int n);
};
void CBlurredAttitudesDefVector::resize(unsigned int n)
{
    resize(n, CBlurredAttitudesDef());
}
