// CTCLight::OnAppearanceClear() -- the appearance was cleared: empty the light primitive list (Lights) and the flare sprite list (FlareSprites) through STLport erase(begin(), end())
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00641210, length 77 bytes.
// Leaf: OnAppearanceClear
// Callees: memmove @ 0x00BFEAE6 (the STLport __copy_trivial path, `__cdecl`).
// Vtable: slot 1 of CTCLight's SECOND vtable (0x0125A208): the method overrides a virtual introduced by the secondary
// appearance-listener base at +0x0c, so VC compiles it with `this` pointing at that subobject (no adjustor thunk) and every
// member offset is 0x0c lower: FlareSprites (CTCLight +0x2c) is read at +0x20, Lights (CTCLight +0x38 with the 12-byte retail
// vector) at +0x2c. Layout: PDB CTCLight (0x4c) with retail 12-byte vectors; no engine header exists, local model.
// Shape: the landed STLport clear (0x00AA5BD0): `_M_finish = CopyTrivial(_M_finish, _M_finish, _M_start)`, the retail
// `cmp eax,eax ; jne` on the twice-loaded finish pointer; Lights is cleared first.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct CPrimitive;

struct CPrimitiveVector {                           // STLport vector<NGraphicAppearance::CPrimitive*>
    CPrimitive** _M_start;
    CPrimitive** _M_finish;
    CPrimitive** _M_end_of_storage;
    void clear()
    {
        CPrimitive** i = (CPrimitive**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

struct CThing;
struct CLightDef;

struct CTCBase {
    virtual void v00();
    CThing* m_pThing;                              // +0x04
    long    m_baseWord;                            // +0x08
};

struct CAppearanceListener {                       // secondary base at +0x0c
    virtual void v00();
    virtual void OnAppearanceClear();              // slot 1
};

struct CLightInfoPointer {                         // CCountedPointer<CTCLight::CLightInfo>
    void* m_pObject;
    void* m_pBlock;
};

struct CTCLight : CTCBase, CAppearanceListener {
    CLightInfoPointer PLightInfo;                  // +0x10
    const CLightDef*  PLightDef;                   // +0x18
    bool              Active;                      // +0x1c
    bool              Overridden;                  // +0x1d
    float             AccumilativeFlickerFactor;   // +0x20
    float             OldAccumilativeFlickerFactor;// +0x24
    bool              Initialised;                 // +0x28
    CPrimitiveVector  FlareSprites;                // +0x2c
    CPrimitiveVector  Lights;                      // +0x38
    virtual void OnAppearanceClear();
};

void CTCLight::OnAppearanceClear()
{
    Lights.clear();
    FlareSprites.clear();
}
