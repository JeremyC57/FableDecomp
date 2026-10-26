// CMarkerInfo::IsPulsingOnMap()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A270, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; call PeekGuiDef ; mov eax,[eax+958h] ; mov ecx,[esi+8] ; cmp ecx,[eax+4] ; pop esi ; je T ; cmp ecx,[eax+0Ch] ; je T ; cmp ecx,[eax+30h] ; je T ; cmp ecx,[eax+34h] ; je T ; xor al,al ; ret ; T: mov al,1 ; ret
// engine/CMarkerInfo.h (PDB): +0x08 GraphicIndex. PeekGuiDef @ 0x00438570 (__fastcall, landed)
// returns the GUI definition; its member at +0x958 points at the mini-map marker graphics record.
// A marker pulses when its graphic is one of the four pulsing graphics in that record (the
// entries at +0x04, +0x0C, +0x30 and +0x34; their individual names are not in the PDB export).
#include "engine/CMarkerInfo.h"

struct CMiniMapMarkerGraphics {
    unsigned char _pad_0x00[4];
    long          PulsingGraphic0;      // +0x04
    unsigned char _pad_0x08[4];
    long          PulsingGraphic1;      // +0x0C
    unsigned char _pad_0x10[0x20];
    long          PulsingGraphic2;      // +0x30
    long          PulsingGraphic3;      // +0x34
};

class CPlayerGuiDef { public:
    unsigned char           _pad_0x000[0x958];
    CMiniMapMarkerGraphics* MarkerGraphics;    // +0x958
};

class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};   // 0x00438570

struct CMarkerInfo_Methods : CMarkerInfo {
    bool IsPulsingOnMap();
};

bool CMarkerInfo_Methods::IsPulsingOnMap()
{
    CMiniMapMarkerGraphics* graphics = CPlayerGui::PeekGuiDef().MarkerGraphics;
    long graphic = this->GraphicIndex;
    if (graphic == graphics->PulsingGraphic0 || graphic == graphics->PulsingGraphic1 ||
        graphic == graphics->PulsingGraphic2 || graphic == graphics->PulsingGraphic3)
        return true;
    return false;
}