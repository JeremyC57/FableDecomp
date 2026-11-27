#ifdef FABLE_RETAIL_FONT_PENDING_PASS29
#ifndef FABLE_ENGINE_PENDINGLOADEDGLYPH_PASS29_H
#define FABLE_ENGINE_PENDINGLOADEDGLYPH_PASS29_H
#include "engine/CCountedPointer.h"
#include "engine/CBankFileAsyncData.h"
struct PendingLoadedGlyph {
    CCountedPointer<CBankFileAsyncData> data;
    bool big;
    unsigned long index;
};
#endif
#endif
