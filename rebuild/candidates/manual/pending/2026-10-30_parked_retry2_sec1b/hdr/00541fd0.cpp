// Grade: DIFFER(350v366) -- only the lookup temporary's destructor zero stores (mov [esp+14h],0 / mov [esp+18h],0 before ~CCharString) are dead-store-eliminated by the lane; tried: user copy ctor, named/const& icon (keeps stores but reads the source from the frame), single full expression (keeps stores but pushes before GFGetSystemManager), inline bank/helper forms
// NUISystem::CIconText::GetDataFromDef() -- load the icon-text definition: look its icon graphic up by the def-string name, copy the colour and offset, set the text, and set the alignment flag (8/0x10/0x20 for alignments 0/1/2)
// Leaf: GetDataFromDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00541FD0, length 366 bytes.
// Callees: the component's virtual slots 108 LoadAndReturnDef() const (CDefPointer by hidden pointer) and 144 (+0x240,
//   SetText on the def's text at +0x54), CDefStringTable::GetString(long) @ 0x009D49B0 on the global table @ 0x013CA828
//   (CCharString by value), GFGetSystemManager @ 0x009A4EC0 and its graphic bank (+0x84) lookup @ 0x009E2C80 (a
//   CCountedPointer by value; manifest "CMessageEventFilter_CanBeSeenOrHeard::operator()" is propagated), ~CCharString
//   @ 0x0099EAE0, operator delete @ 0x00BFE9BC (count blocks), the def's virtual slot 1 (delete at refcount zero).
// Layout: the NUISystem component base as in the landed CList/CDynamicList GetDataFromDef rows (flags byte +0x12E);
//   CIconText retail members Icon (CCountedPointer {object, count block}) +0x164, Colour (16 bytes) +0x198, Offset
//   (C2DVector) +0x1A8; the icon-text def is packed (Offset at the unaligned +0x142). No engine header; modelled locally.
// Shape: `Icon = bank->GetGraphic(name)` through the counted pointer's inline operator= (block compare, release, copy,
//   add-ref) then the temporary's inline destructor; the colour copy goes through a user copy constructor (pointer-register
//   copy); the def release has no null test (the def was already dereferenced).
#include "engine/CCountedPointer.h"
namespace NUISystem {
struct CWideString { void* m_pData; };
struct CCharString {
    void* m_pData;
    ~CCharString();                                // 0x0099EAE0
};
struct C2DVector { float X, Y; };
struct CRGBAColour {
    float R, G, B, A;
    CRGBAColour(const CRGBAColour& c) : R(c.R), G(c.G), B(c.B), A(c.A) { }
};
struct CGraphic;
struct CGraphicBank {
    ::CCountedPointer<CGraphic> GetGraphic(const CCharString& name);   // 0x009E2C80
};
struct CSystemManager {
    unsigned char _pad[0x84];
    CGraphicBank* PGraphicBank;                    // +0x84
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct CDefStringTable {
    CCharString GetString(long index);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828
#pragma pack(push, 1)
struct CIconTextDef {                              // CIVCountedPointeeBase {vptr, count} + icon text data
    virtual void v00();
    virtual void Delete() const;                   // slot 1
    long          IVRefCount;                      // +0x04
    unsigned char _pad08[0x50 - 0x08];
    long          IconName;                        // +0x50 (def string index)
    CWideString   Text;                            // +0x54
    unsigned char _pad58[0xa4 - 0x58];
    CRGBAColour   Colour;                          // +0xa4
    unsigned char _padB4[0x142 - 0xb4];
    C2DVector     Offset;                          // +0x142
    unsigned char _pad14A[0x1fc - 0x14a];
    long          Alignment;                       // +0x1fc
    void DecRef() const { --const_cast<CIconTextDef*>(this)->IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    void Release() const { DecRef(); if (GetRefCount() == 0) Delete(); }
};
#pragma pack(pop)
struct CDefPointer {
    const CIconTextDef* Data;
    ~CDefPointer() { if (Data) Data->Release(); }
    const CIconTextDef* operator->() const { return Data; }
};
struct CIconText {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer LoadAndReturnDef() const;                        // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text);                       // slot 144 (+0x240)
    unsigned char   _pad004[0x12e - 0x04];
    unsigned char   AlignmentFlags;                // +0x12e
    unsigned char   _pad12F[0x164 - 0x12f];
    ::CCountedPointer<CGraphic> Icon;                          // +0x164
    unsigned char   _pad16C[0x198 - 0x16c];
    CRGBAColour     Colour;                        // +0x198
    C2DVector       Offset;                        // +0x1a8
    void GetDataFromDef();
};

void CIconText::GetDataFromDef()
{
    CDefPointer def = LoadAndReturnDef();
    {
        CCharString name = g_DefStringTable.GetString(def->IconName);
        CGraphicBank* bank = GFGetSystemManager()->PGraphicBank;
        Icon = bank->GetGraphic(name);
    }
    Colour = def->Colour;
    Offset = def->Offset;
    SetText(def->Text);
    switch (def->Alignment) {
    case 0:
        AlignmentFlags |= 8;
        break;
    case 1:
        AlignmentFlags |= 0x10;
        break;
    case 2:
        AlignmentFlags |= 0x20;
        break;
    }
}
}
