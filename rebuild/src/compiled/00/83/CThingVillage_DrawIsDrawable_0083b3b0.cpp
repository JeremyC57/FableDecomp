// CThingVillage::DrawIsDrawable() -- drawable only when the global engine object's virtual slot 9 says so and the CThing base reports drawable
// Leaf: DrawIsDrawable@CThingVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083B3B0, length 41 bytes (+ int3 padding). CThingVillage vtable 0x012451BC slot 27.
// Callees: the static accessor @ 0x004C7A70 (`mov eax,[0x013B8A1C] ; mov eax,[eax+1Ch]`; the manifest's
// "CViewBase::GetDisplayEngine" label is a guess), that object's virtual slot 9 (`call [edx+24h]`, bool),
// CThing::DrawIsDrawable @ 0x004C7790.
// Label correction: the manifest files this row under CThingHolySite; the only reference is CThingVillage's vtable slot 27.
// Shape: `return a && b;` materialises an int (`mov eax,1` / `xor eax,eax`); `this` is kept in esi across the calls.
struct CDisplayEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsDrawingEnabled();                // slot 9 (+0x24)
};

CDisplayEngine* GetDisplayEngine();                 // 0x004C7A70

struct CThing {
    bool DrawIsDrawable();                          // 0x004C7790
};

struct CThingVillage : CThing {
    bool DrawIsDrawable();
};

bool CThingVillage::DrawIsDrawable()
{
    return GetDisplayEngine()->IsDrawingEnabled() && CThing::DrawIsDrawable();
}