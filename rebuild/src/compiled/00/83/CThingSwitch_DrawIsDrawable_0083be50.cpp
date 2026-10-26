// CThingSwitch::DrawIsDrawable() -- drawable only when the display engine says so (slot 9) AND the CThing base test passes (catalog label CThingHolySite is propagated; vtable 0x012453FC is CThingSwitch)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BE50, length 41 bytes (vtable 0x012453FC slot 27).
// Callees: CViewBase-style GetDisplayEngine() @ 0x004C7A70 (on this), CDisplayEngine virtual slot 9
// (+0x24), CThing::DrawIsDrawable() @ 0x004C7790.
// Shape: `return a && b;` with a bool result materialises the full-register 0/1
// (`mov eax,1` / `xor eax,eax`) with two exits; `this` lives in esi across the first call.
class CDisplayEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual bool IsDrawingEnabled();                 // slot 9 (+0x24)
};
class CThing { public:
    virtual void v00();
    CDisplayEngine* GetDisplayEngine();              // 0x004C7A70
    virtual bool DrawIsDrawable();                   // 0x004C7790 (called non-virtually)
};
class CThingSwitch : public CThing { public:
    virtual bool DrawIsDrawable();
};

bool CThingSwitch::DrawIsDrawable()
{
    return GetDisplayEngine()->IsDrawingEnabled() && CThing::DrawIsDrawable();
}