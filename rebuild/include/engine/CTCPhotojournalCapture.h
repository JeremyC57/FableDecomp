#ifndef FABLE_ENGINE_CTCPHOTOJOURNALCAPTURE_H
#define FABLE_ENGINE_CTCPHOTOJOURNALCAPTURE_H
#include "engine/CTCBase.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
namespace NUISystem {class CComponent;}
// Both original complete records; native ctor/dtor/alloc confirm48-byte retail layout.
class CTCPhotojournalCapture : public CTCBase {
#ifdef FABLE_RETAIL_PHOTOJOURNAL_CAPTION_PASS29
public:
 void OnCaptionEntered(const CWideString&);
 void ClosePhotojournalCapture();
#endif
private:
 void CreateScreen();
 CWideString Description;
 bool DiscardScreenShot;
 long FrameCounter;
 bool FirstRenderFrame,ScreenOpen;
 double LastRenderTime;
 CWideString FileName;
 std::vector<NUISystem::CComponent*> Components;
 CThing* PMannequin;
 bool WasPlayerGuiOpen;
 long TakeScreenshotAfterFrames;
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CTCPhotojournalCapture)==0x48);
#endif
