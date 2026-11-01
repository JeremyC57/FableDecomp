// CStaticFontBank::~CStaticFontBank() -- static font bank destructor: free every glyph bank's glyph array, release the font texture when the display manager is still up, then ~CFontBank (Name) and ~CBaseClassNonCopyable
// Leaf: ??1CStaticFontBank@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB7A70, 143 bytes, thiscall, tail jump to ~CBaseClassNonCopyable 0x0099A300.
// Identity (proven): the first store is vtable 0x0129EAD4 (rtti-vtables.tsv: CStaticFontBank, RTTI .?AVCStaticFontBank@@ :
//   CFontBank : CBaseClassNonCopyable : CBaseClass); the sole caller 0x00AB9993 is the scalar deleting destructor 0x00AB9990,
//   slot 0 of that vtable. Ego_r.pdb / FableWin.pdb ??1CStaticFontBank@@UAE@XZ; the catalog label is right. After the body,
//   CFontBank's vtable 0x0129AB38 is stored with CFontBank::Name's ~CCharString: ~CFontBank is an inline user dtor (as in the
//   chunk 665 ~CStreamingFontBank 0x009A1090).
// Callees: operator delete[] 0x00BFEB1C (each GlyphBank::Glyphs), GFGetSystemManager 0x009A4EC0 (?GFGetSystemManager@@YIPAVCSystemManager@@XZ,
//   three calls), CInitBaseClass::IsValid() const 0x0099A330 (`mov al,[ecx+4]`), CTexture::Uninitialise 0x009F9F70 (inline
//   ~CTexture), operator delete 0x00BFE9BC, ~CCharString 0x0099EAE0, ~CBaseClassNonCopyable 0x0099A300.
// Layout: Ego_r.pdb CStaticFontBank (0x2034, retail offsets equal): CFontBank {FontHeight +0x04, Name +0x08, PManager +0x0C},
//   MaxHeight +0x10, Weight +0x14, Italics +0x18, TextureSize +0x1C, MinChar +0x24, MaxChar +0x28, TotalNoGlyphs +0x2C,
//   GlyphBanks[1024] +0x30 (GlyphBank 8 bytes: FirstGlyph, NoGlyphs, CGlyph* Glyphs +4), PTexture +0x2030. CSystemManager :
//   CInitBaseClass (Valid +0x04), PDisplayManager +0x60 (CCountedPointer<CDisplayManager>). engine/CStaticFontBank.h,
//   CFontBank.h and CTexture.h are flattened records, so the classes are local models with their member names and offsets.
#include "engine/CBaseClassNonCopyable.h"
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
void __cdecl operator delete[](void* block);                // 0x00BFEB1C

class CCharStringData;
class CCharString { public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
struct IDirect3DTexture9;
class CTexture { public:
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F9F70
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
};
class CInitBaseClass : public CBaseClass { public:
    bool IsValid() const;                                   // 0x0099A330
    bool Valid;                                             // +0x04
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    T* Get() const { return Data; }
};
class CDisplayManager;
class CSystemManager : public CInitBaseClass { public:
    unsigned char                    _pad_0x08[0x60 - 0x08];
    CCountedPointer<CDisplayManager> PDisplayManager;       // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0

class CFontManager;
class CFontBank : public CBaseClassNonCopyable {            // vtable 0x0129AB38
public:
    CFontBank();
    virtual ~CFontBank() { }
    unsigned long FontHeight;                               // +0x04
    CCharString   Name;                                     // +0x08
    CFontManager* PManager;                                 // +0x0C
};
class CGlyph;
struct GlyphBank {
    unsigned short FirstGlyph;                              // +0x00
    unsigned short NoGlyphs;                                // +0x02
    CGlyph*        Glyphs;                                  // +0x04
};
class C2DExtentsI { public: long X, Y; };
class CFontDataBank;
class CStaticFontBank : public CFontBank {                  // vtable 0x0129EAD4
public:
    CStaticFontBank(CFontDataBank* dataBank, unsigned long fontIndex);
    virtual ~CStaticFontBank();
    unsigned long MaxHeight;                                // +0x10
    long          Weight;                                   // +0x14
    bool          Italics;                                  // +0x18
    C2DExtentsI   TextureSize;                              // +0x1C
    long          MinChar;                                  // +0x24
    long          MaxChar;                                  // +0x28
    long          TotalNoGlyphs;                            // +0x2C
    GlyphBank     GlyphBanks[1024];                         // +0x30
    CTexture*     PTexture;                                 // +0x2030
};

CStaticFontBank::~CStaticFontBank()
{
    for (long i = 0; i < 1024; ++i) {
        if (GlyphBanks[i].Glyphs) {
            delete [] GlyphBanks[i].Glyphs;
            GlyphBanks[i].Glyphs = 0;
        }
    }
    if (GFGetSystemManager() && GFGetSystemManager()->IsValid() && GFGetSystemManager()->PDisplayManager.Data) {
        delete PTexture;
        PTexture = 0;
    }
}
