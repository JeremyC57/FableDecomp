#ifdef FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#ifndef FABLE_ENGINE_SUBTITLE_DRAW_GLOBALS_H
#define FABLE_ENGINE_SUBTITLE_DRAW_GLOBALS_H
#ifdef FABLE_RETAIL_DRAW_BUTTON_TEXT_PASS29
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&, C2DVector&);
#endif
class CDisplayManager;
class CRenderManager;
extern CDisplayManager* GPDisplayManager;
extern CRenderManager* GPRenderManager;
extern double VIRTUAL_SCREEN_HEIGHT;
extern double VIRTUAL_SCREEN_WIDTH;
extern bool GLeftAlignText;
void FABLE_FASTCALL WordWrap_SetOption(unsigned int, unsigned int);
void FABLE_FASTCALL WordWrap_SetCallback(unsigned int (FABLE_FASTCALL*)(unsigned short), unsigned int (FABLE_FASTCALL*)());
const wchar_t* FABLE_FASTCALL WordWrap_FindNextLineW(const wchar_t*, unsigned int, const wchar_t**);
#endif
#endif
