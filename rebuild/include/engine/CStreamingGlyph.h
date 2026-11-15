#ifdef FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#ifndef FABLE_ENGINE_CSTREAMINGGLYPH_H
#define FABLE_ENGINE_CSTREAMINGGLYPH_H
// Retail opt-in (Codex pass22): complete original Ego_r TPI0x61FCE,4bytes.
class CDataInputStream;class CDataOutputStream;
class CStreamingGlyph {public:unsigned long GlyphIndex:24;unsigned long AdjustmentIndex:8;
 void Read(CDataInputStream&);void Write(CDataOutputStream&);
};
#endif
#endif
