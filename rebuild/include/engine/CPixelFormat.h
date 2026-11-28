#ifdef FABLE_RETAIL_CPIXELFORMAT
#ifndef FABLE_ENGINE_CPIXELFORMAT_H
#define FABLE_ENGINE_CPIXELFORMAT_H
#include "engine/Direct3D9Types.h"
#include "engine/CD3DFormatInfo.h"
class CPixelFormat {
protected: long FormatIndex;
public:
 CPixelFormat() :FormatIndex(-1) {}
#ifdef FABLE_RETAIL_SHADOW_BUFFERS_PASS27
 bool IsValid() const {return FormatIndex!=-1;}
#endif
 CPixelFormat(_D3DFORMAT format) { Initialise(format); }
#ifdef FABLE_RETAIL_VOLUME_SERIALIZATION_PASS30
 class CPixelFormatInit GetInit() const;
#endif
 // Movie row 0x006286F0: FableWin.pdb ??0CPixelFormat@@QAE@JJJJ@Z (debug only: inline in release). 0x006286F0 calls
 // Initialise(8,8,8,8,false) on a stack CPixelFormat with no -1 store first, so this constructor does not run the
 // default's FormatIndex(-1): it is the Initialise call alone (the bool is pushed as 0).
 CPixelFormat(long r,long g,long b,long a) { Initialise(r,g,b,a,false); }
 void Initialise(long,long,long,long,bool);                     // 0x009E3790 ?Initialise@CPixelFormat@@QAEXJJJJ_N@Z (Ego_r + FableWin)
 void Initialise(_D3DFORMAT);
#ifdef FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX
 // NF136 opt-in (q4290, row 0x009BE420 CDisplayManager::ClearRenderTarget): Ego_r.pdb 0x00B35470 / FableWin.pdb 0x0308CAB0
 // ?FindFormatInfoIndexFromD3DFormat@CPixelFormat@@SIJW4_D3DFORMAT@@@Z (public static __fastcall: format in ecx, returns the GD3DFormats
 // index or -1; retail 0x009E3620, called out of line at 0x009BE44A). Declaration only: no layout change.
 static long __fastcall FindFormatInfoIndexFromD3DFormat(_D3DFORMAT format);   // 0x009E3620
#endif
#if defined(FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE) && !defined(FABLE_RETAIL_CPIXELFORMAT_CONVERT)
 // Retail opt-in (chunk 1023, row 0x00B67270 CEngineLandscapeRenderer::BuildBlendingTables): Ego_r.pdb / FableWin.pdb
 // ?GetByteColourDepth@CPixelFormat@@QBEKXZ called out of line at 0x009E3800 in that TU (`lea ecx,[ebp-0Ch] ; call` at 0x00B67294;
 // the landed bound spelling). Declaration only; the FABLE_RETAIL_CPIXELFORMAT_CONVERT branch keeps the inline body.
 unsigned long GetByteColourDepth() const;                      // 0x009E3800
#endif
#if defined(FABLE_RETAIL_CPIXELFORMAT_CONVERTRGBTOPFDDATA_OUTOFLINE) && !defined(FABLE_RETAIL_CPIXELFORMAT_CONVERT)
 // Re-port opt-in (2026-11-19, rows 0x00A67950 CVolume::Clear / 0x00A67A20 CVolume::ClearArea): Ego_r.pdb / FableWin.pdb
 // ?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z called out of line at 0x009E3F60 in TUs that also call GetByteColourDepth
 // out of line (0x009E3800, 0x00A679C6 `call 9E3F60h` then 0x00A679CF `call 9E3800h` for every texel), i.e. with FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE. The
 // FABLE_RETAIL_CPIXELFORMAT_CONVERT branch declares the same member together with the inline accessor bodies. Declaration only.
 void ConvertRGBToPFDData(const class CRGBColour& colour,unsigned char* data) const;   // 0x009E3F60
#endif
#ifdef FABLE_RETAIL_CPIXELFORMAT_INIT
 // Retail opt-in (chunk Q20, row 0x009FB750 CTexture::LoadFromDataStreamToPreallocatedSurface): Ego_r.pdb
 // ?Initialise@CPixelFormat@@QAEXABVCPixelFormatInit@@@Z (retail 0x009E3640, from the texture file's 6-byte CPixelFormatInit) and
 // ?IsCompressed@CPixelFormat@@QBE_NXZ (retail 0x009E3940), both out of line. Declarations only: no layout change.
 // FableWin.pdb ??0CPixelFormat@@QAE@ABVCPixelFormatInit@@@Z (debug copy 0x03087A20, 28 bytes: Initialise only, no -1 store;
 // inline in release: 0x009FB7B5 pushes the init and calls Initialise on the uninitialised local).
 CPixelFormat(const class CPixelFormatInit& init) { Initialise(init); }
 void Initialise(const class CPixelFormatInit& init);           // 0x009E3640
 bool IsCompressed() const;                                     // 0x009E3940
#endif
#ifdef FABLE_RETAIL_CPIXELFORMAT_ISLINEAR
 // Opt-in (wfE5, row 0x00BB0970): FableWin.pdb ?IsLinear@CPixelFormat@@QBE_NXZ, inline in retail (GD3DFormats[i].Linear: byte 0x0129BA60 =
 // 0x0129BA40 + 0x20 read at 0x00BB0DC6).
 bool IsLinear() const {return GD3DFormats[FormatIndex].Linear;}
#endif
#ifdef FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
 unsigned long GetColourDepth()const;
#endif
#ifdef FABLE_RETAIL_CPIXELFORMAT_ISEQUIVALENT
 // Retail opt-in (chunk 2004, row 0x009FED30): Ego_r.pdb ?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z, out of line in retail (0x009E37F0, landed).
 bool IsEquivalent(const CPixelFormat& other) const;                 // 0x009E37F0
#endif
#if defined(FABLE_RETAIL_CPIXELFORMAT_OPERATOREQUAL) && defined(FABLE_RETAIL_CPIXELFORMAT_ISEQUIVALENT)
 // NF98 opt-in (round 8, q11274, row 0x009BF350 CDisplayManager::BlitRenderTargetSurfaceToTexture): FableWin.pdb 0x03023F90
 // ??8CPixelFormat@@QBE_NABV0@@Z (debug only, inline in release: 0x009BF3C5..0x009BF3E4 computes both GetPixelFormat temporaries, the
 // right operand first, then calls IsEquivalent 0x009E37F0 on the left one). Needs FABLE_RETAIL_CPIXELFORMAT_ISEQUIVALENT.
 bool operator==(const CPixelFormat& other) const { return IsEquivalent(other); }
#endif
 unsigned long GetNoABits() const {return GD3DFormats[FormatIndex].AlphaBits;}
 unsigned long GetNoRBits() const {return GD3DFormats[FormatIndex].RBits;}
 unsigned long GetNoGBits() const {return GD3DFormats[FormatIndex].GBits;}
 unsigned long GetNoBBits() const {return GD3DFormats[FormatIndex].BBits;}
#if defined(FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF)
 // Retail opt-in (chunk F1, row 0x009E3B60 ConvertPFDToRGB): the FableWin.pdb debug bodies of ?GetGPosShift / ?GetRPosShift /
 // ?GetAPosShift@CPixelFormat@@QBEKXZ (0x0308F0F0 36 bytes / 14 instructions, 0x0308F0B0 42 / 16, 0x0308F310 48 / 18) grow by exactly
 // `mov reg,[ebp-8] ; add eax,[reg+disp8]` per channel: a local `const CD3DFormatInfo&` and one, two and three field reads summed,
 // not a chain of accessor calls. Summed blue first (retail 0x009E3B78 `mov ebx,[eax+129BA58h] ; add ebx,[eax+129BA54h] ; add ebx,edx`).
 unsigned long GetGPosShift() const {const CD3DFormatInfo& format=GD3DFormats[FormatIndex];return format.BBits;}
 unsigned long GetRPosShift() const {const CD3DFormatInfo& format=GD3DFormats[FormatIndex];return format.BBits+format.GBits;}
 unsigned long GetAPosShift() const {const CD3DFormatInfo& format=GD3DFormats[FormatIndex];return format.BBits+format.GBits+format.RBits;}
#elif defined(FABLE_RETAIL_CPIXELFORMAT_CONVERT)
 // (chunk 1014) the FableWin.pdb chain: each position shift is the previous position plus that channel's bit count (blue at 0), every
 // count re-read from GD3DFormats[FormatIndex] (retail 0x009E3F60 adds blue + green + red for alpha, `[eax+129BA58h]` first).
 unsigned long GetRPosShift() const {return GetGPosShift()+GetNoGBits();}
 unsigned long GetAPosShift() const {return GetRPosShift()+GetNoRBits();}
#else
 unsigned long GetRPosShift() const {return GetNoBBits()+GetNoGBits();}
 unsigned long GetAPosShift() const {const CD3DFormatInfo& format=GD3DFormats[FormatIndex];return format.RBits+format.GBits+format.BBits;}
#endif
 unsigned long GetAMask() const {return ((1UL<<GetNoABits())-1)<<GetAPosShift();}
#ifdef FABLE_RETAIL_CPIXELFORMAT_CONVERT
 // Retail opt-in (chunk 1014, row 0x009E3F60 ConvertRGBToPFDData): FableWin.pdb ?GetRScaleShift / GetGScaleShift / GetBScaleShift /
 // GetAScaleShift / GetGPosShift / GetBPosShift / GetByteColourDepth @CPixelFormat@@QBEKXZ (debug bodies 0x0308EFF0..0x0308F120, inline in
 // retail: 8 minus the channel's bit count, the position shifts as running sums from blue, the byte depth BitDepth / 8 signed) and
 // ?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z (0x009E3F60).
 unsigned long GetRScaleShift() const {return 8-GetNoRBits();}
 unsigned long GetGScaleShift() const {return 8-GetNoGBits();}
 unsigned long GetBScaleShift() const {return 8-GetNoBBits();}
 unsigned long GetAScaleShift() const {return 8-GetNoABits();}
#ifndef FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF
 unsigned long GetGPosShift() const {return GetBPosShift()+GetNoBBits();}
#endif
 unsigned long GetBPosShift() const {return 0;}
 unsigned long GetByteColourDepth() const {return GD3DFormats[FormatIndex].BitDepth/8;}
 void ConvertRGBToPFDData(const class CRGBColour& colour,unsigned char* data) const;   // 0x009E3F60
#endif
#ifdef FABLE_RETAIL_CPIXELFORMAT_PFDTORGB
 // Retail opt-in (chunk F1, row 0x009E3B60 ConvertPFDToRGB; needs FABLE_RETAIL_CPIXELFORMAT_CONVERT): FableWin.pdb ?GetRMask / ?GetGMask /
 // ?GetBMask@CPixelFormat@@QBEKXZ (debug bodies 0x0308EF30 / 0x0308EF70 / 0x0308EFB0, the GetAMask shape; inline in retail) and
 // ?ConvertPFDToRGB@CPixelFormat@@QBEXABVCPFDColour@@AAVCRGBColour@@@Z (0x009E3B60). Declarations with bodies only: no layout change.
 unsigned long GetRMask() const {return ((1UL<<GetNoRBits())-1)<<GetRPosShift();}
 unsigned long GetGMask() const {return ((1UL<<GetNoGBits())-1)<<GetGPosShift();}
 unsigned long GetBMask() const {return ((1UL<<GetNoBBits())-1)<<GetBPosShift();}
 void ConvertPFDToRGB(const class CPFDColour& pfd,class CRGBColour& rgb) const;        // 0x009E3B60
#endif
#ifdef FABLE_RETAIL_CPIXELFORMAT_PFDDATATORGB
 // Retail opt-in (workflow wf1, row 0x009E3DF0 ConvertPFDDataToRGB; needs FABLE_RETAIL_CPIXELFORMAT_CONVERT + _PFDTORGB): Ego_r.pdb /
 // FableWin.pdb ?ConvertPFDDataToRGB@CPixelFormat@@QBEXPBEAAVCRGBColour@@@Z (0x009E3DF0). Declaration only: no layout change.
 void ConvertPFDDataToRGB(const unsigned char* data,class CRGBColour& colour) const;   // 0x009E3DF0
#endif
#ifdef FABLE_RETAIL_CPIXELFORMAT_SIGNED
 // Retail opt-in (workflow wf10, row 0x009F4390 CSurface::CopyFromMemory): Ego_r.pdb ?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z (0x009E37F0)
 // and ?IsSigned@CPixelFormat@@QBE_NXZ (0x009E3970), both called out of line; FableWin.pdb ?GetAsD3DPixelFormat@CPixelFormat@@QBE?AW4_D3DFORMAT@@XZ
 // (debug only, inline in release: 0x009F4562 `mov eax,[ebp] ; lea edx,[eax+eax*8] ; mov eax,[edx*4+GD3DFormats]`).
 bool IsEquivalent(const CPixelFormat& other) const;            // 0x009E37F0
 bool IsSigned() const;                                         // 0x009E3970
 _D3DFORMAT GetAsD3DPixelFormat() const {return GD3DFormats[FormatIndex].D3DFormat;}
#endif
};
#endif
#else
#ifndef FABLE_ENGINE_CPIXELFORMAT_H
#define FABLE_ENGINE_CPIXELFORMAT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CPixelFormat = 0x4.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CPixelFormat {
public:
    long FormatIndex;                                        // +0x00
#ifdef FABLE_RETAIL_CPIXELFORMAT_FLAT_GETINIT
    // Add-only opt-in (naming agent NF302, 2026-11-23, binder 0x00993F60 CDataOutputStream::WritePixelFormat): Ego_r.pdb / FableWin.pdb
    // ?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ (const, CPixelFormatInit by value; retail 0x009E3870, landed). Declaration only.
    class CPixelFormatInit GetInit() const;                  // 0x009E3870
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(120, sizeof(CPixelFormat) == 0x4);
FABLE_STATIC_ASSERT_AT(121, offsetof(CPixelFormat, FormatIndex) == 0x0);

#endif // FABLE_ENGINE_CPIXELFORMAT_H

#endif
