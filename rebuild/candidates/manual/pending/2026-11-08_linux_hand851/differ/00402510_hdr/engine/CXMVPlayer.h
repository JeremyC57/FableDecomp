#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_CXMVPLAYER_H
#define FABLE_ENGINE_CXMVPLAYER_H
#include "engine/CThreadedFileCallback.h"
// Layout: original complete128-byte type; no instance accessed by GFMain.
class CXMVPlayer:public CThreadedFileCallback{unsigned char Body_4[124];public:static void __fastcall GlobalRegisterPlayer();static void __fastcall SetLeftAlignText(bool);static void __fastcall SetNoHangulWordWrap(bool);};
#endif

#else

#endif
