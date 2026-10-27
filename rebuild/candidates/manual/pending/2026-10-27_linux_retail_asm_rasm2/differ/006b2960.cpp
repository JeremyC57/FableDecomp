// CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const -- convert the requested box to whole navigation cells and ask the navigation manager to fill the area update for it
// Leaf: GetNavigationInformation
// Retail-asm: 0x006B2970..0x006B2999 `mov [ebp + 8], eax ; fld [ebp + 8] ; fistp [ebp - 0x1c] ; mov edx, [ecx + 8] ; mov [ebp + 8], edx ; fld [ebp + 8] ; fistp [ebp - 0x18] ; mov eax, [ecx + 4] ; mov [ebp - 0xc], eax ; mov [ebp - 8], 0x3f000000 ; mov [ebp + 8], 0x3f800000 ; fld [ebp - 0xc]` and 0x006B297C..0x006B29A5 `mov [ebp + 8], edx ; fld [ebp + 8] ; fistp [ebp - 0x18] ; mov eax, [ecx + 4] ; mov [ebp - 0xc], eax ; mov [ebp - 8], 0x3f000000 ; mov [ebp + 8], 0x3f800000 ; fld [ebp - 0xc] ; fsub [ebp - 8] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp + 8]` and 0x006B2999..0x006B29B8 `fld [ebp - 0xc] ; fsub [ebp - 8] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp + 8] ; fstp [ebp - 0x10] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 0x10] ; jne 0x6b29b6 ; inc [ebp - 4] ; mov ecx, [ecx] ; mov [ebp - 8], ecx` and 0x006B29C9..0x006B29E9 `fld [ebp - 8] ; fsub [ebp - 0xc] ; fistp [ebp + 8] ; fild [ebp + 8] ; fadd [ebp - 0x10] ; fstp [ebp - 0x14] ; mov eax, [ebp - 8] ; cmp eax, [ebp - 0x14] ; jne 0x6b29e6 ; inc [ebp + 8] ; mov edx, [ebp + 8] ; mov ecx, [ebp - 0x18]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2960; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Two bare roundings (Y1, X1 through the dead box slot) and two floor blocks (Y0, X0).
#pragma optimize("y", off)
struct C2DBoxF { float X0, Y0, X1, Y1; };
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct C2DCoordI { long X, Y; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CNavigationAreaUpdate;
struct CNavMap {
    void InitialiseHeightmap();                         // 0x00509A20
};
struct CNavRegion {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void* GetRegionAt(const C2DCoordI& pos);    // slot 17 (+0x44)
};
struct CNavManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CNavMap* Query(void* region, const C2DBoxI& box, const CCharString& name, CNavigationAreaUpdate& update);   // slot 13
};
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long RoundToInt(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
__forceinline long FloorToInt(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}


struct CGameNavigatorManagerExternalInterface {
    void* _vptr;
    CNavManager* Manager;                               // +0x04
    void GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const;
};

void CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const
{
    long y1 = RoundToInt(box.Y1);
    long x1 = RoundToInt(box.X1);
    long y0 = FloorToInt(box.Y0);
    long x0 = FloorToInt(box.X0);
    C2DBoxI cells = { x0, y0, x1, y1 };
    C2DCoordI corner = { cells.X0, cells.Y0 };
    void* region = ((CNavRegion*)Manager)->GetRegionAt(corner);
    CCharString name("NULL", -1);
    Manager->Query(region, cells, name, update)->InitialiseHeightmap();
}
