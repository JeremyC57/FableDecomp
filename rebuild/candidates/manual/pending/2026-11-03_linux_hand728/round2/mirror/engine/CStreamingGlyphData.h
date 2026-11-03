#ifndef FABLE_ENGINE_CSTREAMINGGLYPHDATA_H
#define FABLE_ENGINE_CSTREAMINGGLYPHDATA_H
// Layout: original public fields, native element stride8, allocation count multiplied by8.
class CStreamingGlyphData {public:signed char OffsetX,OffsetY,Width,Height;unsigned long MemOffset:23,Advance:8,BigChar:1;};
#endif
