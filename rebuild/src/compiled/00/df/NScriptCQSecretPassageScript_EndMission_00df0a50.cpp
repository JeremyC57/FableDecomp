// CQ_SecretPassageScript::EndMission() -- finish the prison escape: fade to black, complete and retire the active quest and teleport the hero to the PrisonHSP marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF0A50, length 287 bytes, plain `ret`.
// Callees: CQ_SecretPassageScript::PrisonEscape @ 0x00DF0B70; through the game script interface (+0x40, retail
// vtable 0x01260F0C): FadeScreenOut(float, float, CRGBColour) (slot 373, +0x5d4, 0x00890820),
// SetAllowScreenFadingOnNextRegionChange(bool) (slot 381, +0x5f4, 0x0088E910), GetThingWithScriptName (slot 72,
// +0x120, 0x008A7D60, CScriptThing by value), GetActiveQuestName (slot 655, +0xa3c, 0x00891880, x2),
// SetQuestAsCompleted(const CCharString&, bool, bool, bool) (slot 288, +0x480, 0x00892F80),
// DeactivateQuestLater(const CCharString&, unsigned long) (slot 281, +0x464, 0x00892F10), GetHero (slot 70, +0x118,
// 0x00891CA0), EntityTeleportToThing (slot 472, +0x760, 0x0088E540); CCharString(const char*, long) 0x0099EBF0,
// ~CCharString 0x0099EAE0, operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0. Literal "PrisonHSP" 0x012DC730.
// Shape: the CRGBColour argument is built in its stack slot (`push ecx ; mov eax,esp`) with the R, G, B, A stores in
// constructor-body order (D3DCOLOR byte order B,G,R,A); the marker is a named CScriptThing whose inline destructor
// (landed CQ_GuildTrainingDepartureScript::Init 0x00D506B0 model) runs at the end; one zero register (ebx)
// serves every false/0 argument.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }

class CRGBColour { public:                                    // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { R = other.R; G = other.G; B = other.B; A = other.A; }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SecretPassageScript {
public:
    virtual void v00();
    unsigned char          _base[0x3c];              // CScriptBase body
    CGameScriptInterface*  m_pInterface;             // +0x40
    void PrisonEscape();                             // 0x00DF0B70
    void EndMission();
};
} // namespace NScript
using NScript::CQ_SecretPassageScript;

void NScript::CQ_SecretPassageScript::EndMission()
{
    PrisonEscape();
    m_pInterface->FadeScreenOut(0.5f, 0.0f, CRGBColour(0, 0, 0, 255));
    m_pInterface->SetAllowScreenFadingOnNextRegionChange(false);
    CScriptThing marker = m_pInterface->GetThingWithScriptName("PrisonHSP");
    m_pInterface->SetQuestAsCompleted(m_pInterface->GetActiveQuestName(), false, false, false);
    m_pInterface->DeactivateQuestLater(m_pInterface->GetActiveQuestName(), 0);
    m_pInterface->EntityTeleportToThing(m_pInterface->GetHero(), marker, false);
}