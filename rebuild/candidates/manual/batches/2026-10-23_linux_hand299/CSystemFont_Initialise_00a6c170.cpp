// CSystemFont::Initialise() -- builds the static character -> glyph-bitmap table of the debug system font once
// Retail 0x00A6C170, 0x325 bytes. Callees: CInitBaseClass::Init 0x0099A350 (`Valid = true; return true`, the
// manifest label CXMVPlaybackInfo is propagated) on `this`.
// Shape: `Init() == true` gives retail's `cmp al,1 ; jne`; the static `Initialised` flag (0x013D2CF0) guards the
// one-time fill; every one of the 256 table slots (0x013D28F0) first points at the blank glyph (0x013D2CF4,
// rep stosd), then letters share one bitmap for both cases (lower case stored first), digits and punctuation
// get their own 36-byte bitmaps (the .data block from 0x01397C60, A..Z, 1..9, 0, then the symbols).
struct SGlyph { unsigned char Bits[36]; };

extern const SGlyph GlyphA, GlyphB, GlyphC, GlyphD, GlyphE, GlyphF, GlyphG, GlyphH, GlyphI, GlyphJ, GlyphK,
    GlyphL, GlyphM, GlyphN, GlyphO, GlyphP, GlyphQ, GlyphR, GlyphS, GlyphT, GlyphU, GlyphV, GlyphW, GlyphX,
    GlyphY, GlyphZ;                                                  // 0x01397C60 + 0x24 * n
extern const SGlyph Glyph1, Glyph2, Glyph3, Glyph4, Glyph5, Glyph6, Glyph7, Glyph8, Glyph9, Glyph0; // 0x01398008..0x0139814C
extern const SGlyph GlyphApostrophe;   // 0x01398170
extern const SGlyph GlyphUnderscore;   // 0x01398194
extern const SGlyph GlyphSlash;        // 0x013981B8
extern const SGlyph GlyphBackslash;    // 0x013981DC
extern const SGlyph GlyphFullStop;     // 0x01398200
extern const SGlyph GlyphComma;        // 0x01398224
extern const SGlyph GlyphColon;        // 0x01398248
extern const SGlyph GlyphSemicolon;    // 0x0139826C
extern const SGlyph GlyphOpenBracket;  // 0x01398290
extern const SGlyph GlyphCloseBracket; // 0x013982B4
extern const SGlyph GlyphOpenSquare;   // 0x013982D8
extern const SGlyph GlyphCloseSquare;  // 0x013982FC
extern const SGlyph GlyphMinus;        // 0x01398320
extern const SGlyph GlyphPlus;         // 0x01398344
extern const SGlyph GlyphQuote;        // 0x01398368
extern const SGlyph GlyphEquals;       // 0x0139838C
extern const SGlyph GlyphLess;         // 0x013983B0
extern const SGlyph GlyphGreater;      // 0x013983D4
extern const SGlyph GlyphExclamation;  // 0x013983F8
extern const SGlyph GlyphQuestion;     // 0x0139841C
extern const SGlyph GlyphBar;          // 0x01398440
extern const SGlyph GlyphAmpersand;    // 0x01398464
extern const SGlyph GlyphStar;         // 0x01398488
extern const SGlyph GlyphPercent;      // 0x013984AC

struct CInitBaseClass {
    virtual void v00();
    bool Valid;                                // +0x04
    bool Init();                               // 0x0099A350
};

struct CSystemFont : CInitBaseClass {
    static bool Initialised;                   // 0x013D2CF0
    static const SGlyph* Glyphs[256];          // 0x013D28F0
    static SGlyph BlankGlyph;                  // 0x013D2CF4
    bool Initialise();
};

bool CSystemFont::Initialise()
{
    if (Init() == true)
    {
        if (!Initialised)
        {
            for (int i = 0; i < 256; ++i)
                Glyphs[i] = &BlankGlyph;
            Glyphs[' '] = &BlankGlyph;
            Glyphs['a'] = &GlyphA; Glyphs['A'] = &GlyphA;
            Glyphs['b'] = &GlyphB; Glyphs['B'] = &GlyphB;
            Glyphs['c'] = &GlyphC; Glyphs['C'] = &GlyphC;
            Glyphs['d'] = &GlyphD; Glyphs['D'] = &GlyphD;
            Glyphs['e'] = &GlyphE; Glyphs['E'] = &GlyphE;
            Glyphs['f'] = &GlyphF; Glyphs['F'] = &GlyphF;
            Glyphs['g'] = &GlyphG; Glyphs['G'] = &GlyphG;
            Glyphs['h'] = &GlyphH; Glyphs['H'] = &GlyphH;
            Glyphs['i'] = &GlyphI; Glyphs['I'] = &GlyphI;
            Glyphs['j'] = &GlyphJ; Glyphs['J'] = &GlyphJ;
            Glyphs['k'] = &GlyphK; Glyphs['K'] = &GlyphK;
            Glyphs['l'] = &GlyphL; Glyphs['L'] = &GlyphL;
            Glyphs['m'] = &GlyphM; Glyphs['M'] = &GlyphM;
            Glyphs['n'] = &GlyphN; Glyphs['N'] = &GlyphN;
            Glyphs['o'] = &GlyphO; Glyphs['O'] = &GlyphO;
            Glyphs['p'] = &GlyphP; Glyphs['P'] = &GlyphP;
            Glyphs['q'] = &GlyphQ; Glyphs['Q'] = &GlyphQ;
            Glyphs['r'] = &GlyphR; Glyphs['R'] = &GlyphR;
            Glyphs['s'] = &GlyphS; Glyphs['S'] = &GlyphS;
            Glyphs['t'] = &GlyphT; Glyphs['T'] = &GlyphT;
            Glyphs['u'] = &GlyphU; Glyphs['U'] = &GlyphU;
            Glyphs['v'] = &GlyphV; Glyphs['V'] = &GlyphV;
            Glyphs['w'] = &GlyphW; Glyphs['W'] = &GlyphW;
            Glyphs['x'] = &GlyphX; Glyphs['X'] = &GlyphX;
            Glyphs['y'] = &GlyphY; Glyphs['Y'] = &GlyphY;
            Glyphs['z'] = &GlyphZ; Glyphs['Z'] = &GlyphZ;
            Glyphs['0'] = &Glyph0;
            Glyphs['1'] = &Glyph1;
            Glyphs['2'] = &Glyph2;
            Glyphs['3'] = &Glyph3;
            Glyphs['4'] = &Glyph4;
            Glyphs['5'] = &Glyph5;
            Glyphs['6'] = &Glyph6;
            Glyphs['7'] = &Glyph7;
            Glyphs['8'] = &Glyph8;
            Glyphs['9'] = &Glyph9;
            Glyphs['_'] = &GlyphUnderscore;
            Glyphs['\''] = &GlyphApostrophe;
            Glyphs['/'] = &GlyphSlash;
            Glyphs['\\'] = &GlyphBackslash;
            Glyphs['.'] = &GlyphFullStop;
            Glyphs[','] = &GlyphComma;
            Glyphs[':'] = &GlyphColon;
            Glyphs[';'] = &GlyphSemicolon;
            Glyphs['('] = &GlyphOpenBracket;
            Glyphs[')'] = &GlyphCloseBracket;
            Glyphs['{'] = &GlyphOpenBracket;
            Glyphs['}'] = &GlyphCloseBracket;
            Glyphs['['] = &GlyphOpenSquare;
            Glyphs[']'] = &GlyphCloseSquare;
            Glyphs['"'] = &GlyphQuote;
            Glyphs['-'] = &GlyphMinus;
            Glyphs['+'] = &GlyphPlus;
            Glyphs['='] = &GlyphEquals;
            Glyphs['<'] = &GlyphLess;
            Glyphs['>'] = &GlyphGreater;
            Glyphs['!'] = &GlyphExclamation;
            Glyphs['?'] = &GlyphQuestion;
            Glyphs['|'] = &GlyphBar;
            Glyphs['&'] = &GlyphAmpersand;
            Glyphs['*'] = &GlyphStar;
            Glyphs['%'] = &GlyphPercent;
            Initialised = true;
        }
        return true;
    }
    return false;
}
