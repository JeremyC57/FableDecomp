// CArray<C3DGroup2>::AddDefault() -- appends a default-constructed C3DGroup2 (BoneIndex -1, empty VertexBlends) to the array
// Leaf: AddDefault
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE8490, length 45 bytes.
// Callees: C3DGroup2::C3DGroup2() @ 0x00AC40C0 (landed), CArray<C3DGroup2>::push_back(const C3DGroup2&)
//          @ 0x00AE83D0 (`Vector_PushBackExpand`), free @ 0x00BFEA14 (the temporary's VertexBlends release).
// Label: the manifest's "CTCInventory::SetHighlightedItemAsQuickAccessItem" is propagated; the body works on
//        `this` as a 16-byte-element vector and builds C3DGroup2 temporaries exactly like
//        CParticleProgram::AddGroup (0x00AE37D0) does for SimGroups (+0xFC) and NonSimGroups (+0x108).
// Layout: retail CArray<T> is 12 bytes {first, last, end_of_storage}; C3DGroup2 is {long BoneIndex,
//         CArray<C3DVertexBlend2> VertexBlends} = 16 bytes (the landed 0x00AC40C0 model).
// Shape: a bare temporary argument reuses the constructor's returned `this` (push eax); its inline
//        destructor is the CArray release `if (data) free(data)`.

extern "C" void __cdecl free(void* p);                    // 0x00BFEA14

class C3DVertexBlend2;

struct CArrayVertexBlend2 {                               // retail CArray<C3DVertexBlend2>
    C3DVertexBlend2* m_pFirst;                            // +0x00
    C3DVertexBlend2* m_pLast;                             // +0x04
    C3DVertexBlend2* m_pEnd;                              // +0x08
    ~CArrayVertexBlend2() { if (m_pFirst) free(m_pFirst); }
};

class C3DGroup2 { public:
    long               BoneIndex;                         // +0x00
    CArrayVertexBlend2 VertexBlends;                      // +0x04
    C3DGroup2();                                          // 0x00AC40C0
};

struct CArrayGroup2 {                                     // retail CArray<C3DGroup2>
    C3DGroup2* m_pFirst;                                  // +0x00
    C3DGroup2* m_pLast;                                   // +0x04
    C3DGroup2* m_pEnd;                                    // +0x08
    void push_back(const C3DGroup2& value);               // 0x00AE83D0
    void AddDefault();
};

void CArrayGroup2::AddDefault()
{
    push_back(C3DGroup2());
}