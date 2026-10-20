// CEngineInternalPrimitiveLine::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a line descriptor (end points, colour with its alpha, width, bank index, blend mode/op, z-buffer flag; frame reset to 0), start loading the line's graphic when it has one, and apply the game-primitive fade defaults
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7F70, length 241 bytes (`ret 8`, virtual `?Update@CEngineInternalPrimitiveLine@@UAEXABVCEnginePrimitive@@K@Z`).
// Callees: CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) @ 0x009FE620,
// CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults(float) @ 0x00B8FD40.
// Globals: the engine @ 0x01436E98 (graphic bank pointer at +0x954, its 0x2c-byte record vector at +0x1e0) -- the bank model of
// the landed CEngineSurfaceCompositionManager::GetGraphicInfo 0x00B2B790.
// Layout: base = engine/CEngineInternalPrimitiveBase.h (EnginePrimitiveFlags +0x0c, Alpha +0x27); the line fields follow the PDB
// CEngineInternalPrimitiveLine / CEnginePrimitiveLine layouts exactly (Start +0x48, End +0x54, Colour +0x60, Width +0x64,
// BankIndex +0x68, Frame +0x6c, BlendMode +0x70, BlendOp +0x74, UseZBuffer +0x78; descriptor Start +0x0c .. UseZBuffer +0x38).
// The method is attached through a derived shell (the header is a plain layout struct), so it is graded non-virtual.
// Shape: the C3DVector members are whole POD assignments (`lea` of both ends); the base Alpha is copied from the stored
// colour's alpha byte; the frame pointer is `Get(i) ? Get(i)->m_pFrame : 0` over the inline bounds-checked bank getter (the
// second getter folds to `sbb ; and`), called even when null; both getters must be `__forceinline` (else GetFrame stays out of line).
#include "engine/CEngineInternalPrimitiveBase.h"

struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveLine : public CEnginePrimitive {   // PDB, 60 bytes
public:
    C3DVector     Start;                           // +0x0c
    C3DVector     End;                             // +0x18
    CRGBColour    Colour;                          // +0x24
    float         Width;                           // +0x28
    unsigned long BankIndex;                       // +0x2c
    long          BlendMode;                       // +0x30
    long          BlendOp;                         // +0x34
    bool          UseZBuffer;                      // +0x38
};

struct CGraphicInfoFrame {
    void BeginLoading(unsigned long priority, unsigned long flags);   // 0x009FE620
};

struct SBankGraphic {
    unsigned char      _pad[0x28];
    CGraphicInfoFrame* m_pFrame;                   // +0x28
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CGraphicBank {
    unsigned char               _pad[0x1e0];
    CRetailVector<SBankGraphic> m_records;         // +0x1e0
    __forceinline SBankGraphic* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
    __forceinline CGraphicInfoFrame* GetFrame(unsigned long index) { return Get(index) ? Get(index)->m_pFrame : 0; }
};

struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x954];
    CGraphicBank* m_pGraphicBank;                  // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineInternalPrimitiveLine : CEngineInternalPrimitiveBase {
    C3DVector     Start;                           // +0x48
    C3DVector     End;                             // +0x54
    CRGBColour    Colour;                          // +0x60
    float         Width;                           // +0x64
    unsigned long BankIndex;                       // +0x68
    unsigned long Frame;                           // +0x6c
    long          BlendMode;                       // +0x70
    long          BlendOp;                         // +0x74
    bool          UseZBuffer;                      // +0x78
    void SetFadeDistanceWithGamePrimitiveDefaults(float distance);   // 0x00B8FD40
    void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveLine::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveLine& line = static_cast<const CEnginePrimitiveLine&>(primitive);
    EnginePrimitiveFlags = flags;
    Start = line.Start;
    End = line.End;
    Colour = line.Colour;
    Alpha = Colour.A;
    Width = line.Width;
    BankIndex = line.BankIndex;
    Frame = 0;
    BlendMode = line.BlendMode;
    BlendOp = line.BlendOp;
    UseZBuffer = line.UseZBuffer;
    if (BankIndex)
        g_pEngine_01436e98->m_pGraphicBank->GetFrame(BankIndex)->BeginLoading(0, 0);
    SetFadeDistanceWithGamePrimitiveDefaults(0.0f);
}
