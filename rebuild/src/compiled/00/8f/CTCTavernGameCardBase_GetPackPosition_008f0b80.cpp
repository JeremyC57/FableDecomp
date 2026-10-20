// CTCTavernGameCardBase::GetPackPosition() -- the card pack's table position: the def's pack offset scaled by the card size, on the table plane (z 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0B80, length 71 bytes (ret 4: hidden C3DVector result). Name: Ego_r.pdb
// `?GetPackPosition@CTCTavernGameCardBase@@QAE?AVC3DVector@@XZ`; the catalog label "CTCTavernGame::GetBanter" is
// propagated over this body and rejected.
// Callee: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F0B20 (on `this`), then the
// def's virtual slot 1 (destroy) when the local reference is the last one.
// Ego_r.pdb CTavernGameCardBaseDef: CardWidth +0x2c, CardHeight +0x30, PackOffset (C2DVector) +0x64.
// Shape: `fld [a] ; fmul [b]` is `b * a`; the constructor arguments are evaluated right to left (Y before X) and the 0.0f
// is an immediate store; the local def pointer is null-initialised in the `push ecx` slot after its address is pushed and
// released unconditionally after the read (load/dec/store of the count, no null test, flags from the `dec`) through a
// local pointer and a local count -- the member form re-reads the slot after the store (82v73).
struct C2DVector { float X, Y; };

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CDefPointee {                         // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+4)
    long m_refCount;                         // +0x04
};

struct CTavernGameCardBaseDef : CDefPointee {
    unsigned char _pad_0x08[0x24];
    float         CardWidth;                 // +0x2c
    float         CardHeight;                // +0x30
    unsigned char _pad_0x34[0x30];
    C2DVector     PackOffset;                // +0x64
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        long count = object->m_refCount - 1;
        object->m_refCount = count;
        if (count == 0)
            object->Destroy();
    }
    T* operator->() const { return Object; }
};

struct CTCTavernGameCardBase {
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F0B20
    C3DVector GetPackPosition();
};
void* FindRef_CTCTavernGameCardBase_GetCardDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CTCTavernGameCardBase::GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    void* p = FindRef_CTCTavernGameCardBase_GetCardDef_0();
    if (!p)
        return;
    *(void**)out = p;
    
}
#pragma auto_inline(on)

C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset.X, def->CardHeight * def->PackOffset.Y, 0.0f);
}