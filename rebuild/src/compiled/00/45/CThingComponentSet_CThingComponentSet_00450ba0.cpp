// CThingComponentSet::~CThingComponentSet() -- the Entries small vector's storage is freed (operator delete[] 0x00BFEB1C) and the
// vector left empty (Start 0, Size 0).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00450BA0, 26 bytes, `this` in ecx.
// Name: Ego_r.pdb ??1CThingComponentSet@@QAE@XZ (the manifest names the same); called from ~CThingBaseDef 0x00454B4C on its
// Components member and from 0x00677360.
// Layout (Ego_r.pdb, no engine header yet): CThingComponentSet { CSmallVector<CThingComponentSet::CEntry, 8> Entries; } (8 bytes),
// CSmallVector<T, N> { T* Start +0; schar Size +4 }. The element release is the small vector's inline destructor; the CEntry type
// is opaque here.
// Leaf: ??1CThingComponentSet
void operator delete[](void*);                          // 0x00BFEB1C

template <class T, int N> class CSmallVector { public:
    T*          Start;                                  // +0x00
    signed char Size;                                   // +0x04
    ~CSmallVector()
    {
        operator delete[](Start);
        Start = 0;
        Size = 0;
    }
};

class CThingComponentSet { public:
    class CEntry;
    CSmallVector<CEntry, 8> Entries;                    // +0x00
    ~CThingComponentSet();
};

CThingComponentSet::~CThingComponentSet()
{
}
