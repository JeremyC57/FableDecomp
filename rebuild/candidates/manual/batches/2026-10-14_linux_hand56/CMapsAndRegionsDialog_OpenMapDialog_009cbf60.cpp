// CMapsAndRegionsDialog::OpenMapDialog(long map, long region, long flags) -- forward to the dialog interface at +0x7c: its virtual slot 5 (+0x14) with the map, then slot 3 (+0xc) with (region, flags, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBF60, length 48 bytes.
// Leaf: OpenMapDialog
// Shape: `this` in esi, the interface and its vtable re-read for the second call, `ret 0Ch`.
struct IMapDialog_009cbf60 {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual void Open(long map, long region, long flags);   // slot 3 (+0x0c)
    virtual void v004();
    virtual void Select(long map);                 // slot 5 (+0x14)
};

struct CMapsAndRegionsDialog {
    unsigned char        _pad00[0x7c];
    IMapDialog_009cbf60* m_pDialog;                // +0x7c
    void OpenMapDialog(long map, long region, long flags);
};

void CMapsAndRegionsDialog::OpenMapDialog(long map, long region, long flags)
{
    m_pDialog->Select(map);
    m_pDialog->Open(region, flags, 0);
}
