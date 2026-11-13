// CEngineResourceManager::SetupAlphaStippleTexture(bool rebuild) -- create the 32x32 alpha stipple texture and the four 2x2 dither textures; load the stipple threshold texture and the hardware stipple masks from the engine cache (StippleAlphaTexture.dat, version 8) or, when allowed, rebuild them and write the cache file
// Leaf: ?SetupAlphaStippleTexture@CEngineResourceManager@@AAEX_N@Z
// Inline-budget: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE + FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_OUTOFLINE (STLport resize bodies visible,
//   kept out of line) -- retail calls vector<CTexture>::resize(4) 0x0049B7F0 (0x00B3C665) and vector<unsigned long>::resize(0x2000, 0)
//   0x00464931 (0x00B3C8CA); with the plain inline bodies VC7.1 expands the second (1425v1363), with only resize(n, x) out of line it
//   expands the first (1387v1363).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3C5D0, 1363 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetupAlphaStippleTexture@CEngineResourceManager@@AAEX_N@Z (private, bool); retail follows the
//   PDB order BuildAlphaStippleTexture 0x00B3BE10 / BuildHWStipplePatterns 0x00B3C4B0 / SetupAlphaStippleTexture; one caller
//   (0x00B3DDB0, the resource manager's initialisation).
// Callees: malloc / free 0x00BFEA0E / 0x00BFEA14 (STLport allocator), CPixelFormat::Initialise(long,long,long,long,bool) 0x009E3790,
//   CTexture::Initialise 0x009FA280 / Lock 0x009FA450 / Unlock 0x009F9DE0 / GetSurfaceLevel 0x009F9E00, vector<CTexture>::resize(size_type)
//   0x0049B7F0, vector<unsigned long>::resize(size_type, const unsigned long&) 0x00464931, CBaseClassNonCopyable() 0x0099A2F0 / ~ 0x0099A300,
//   CWideString() 0x0099AED0 / ~ 0x0099B510, operator+(const CWideString&, const unsigned short*) 0x0099BF30, CAFile::PathExists 0x00999230,
//   CDiskFileWin32::Open 0x0099A6A0 / Close 0x0099A920, CFileDataInputStream(CAFile&, unsigned long) 0x00994700 / ~ 0x00994780,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CSurface::CopyFromMemoryAutoSwizzle 0x009F3470 / Uninitialise 0x009F2E20,
//   CFileDataOutputStream(CAFile&) 0x00994400 / ~ 0x00994430 / Write 0x00994480, CDataOutputStream::WriteSLONG 0x00993EB0,
//   BuildAlphaStippleTexture 0x00B3BE10, BuildHWStipplePatterns 0x00B3C4B0.
// Globals: CAFile::m_errorCallback 0x013BC9FC (CAFile::DoErrorCallback: FableWin.pdb ?DoErrorCallback@CAFile@@SIXXZ, its debug COMDAT
//   0x02DD7C60 placed right after this TU's FableWin bodies), the 2x2 dither patterns 0x012A1BB0 (four 16-byte entries, read only here:
//   a function-local static table, descriptive name DitherPatterns), L"StippleAlphaTexture.dat" 0x012A1B7C.
// Same-TU callees: retail keeps the stipple array's start in esi across BuildHWStipplePatterns (0x00B3C9EA .. 0x00B3CAC3), so the TU
//   saw both Build* bodies: they are defined here (the landed bodies' logic) and kept out of line as in retail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_ERRORCALLBACK
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_OUTOFLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_LOCK
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_AUTOSWIZZLE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_STIPPLE
#define FABLE_RETAIL_C2DVECTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CFileDataOutputStream.h"
#include "engine/CArray.h"
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CEngineResourceManager.h"
#include "engine/C2DVector.h"
#include <stdlib.h>

// Same-TU bodies (retail 0x00B3BE10 / 0x00B3C4B0, landed): see the landed rows for their byte-exact forms.
long __fastcall GFROR13(unsigned long);                          // 0x00497890
inline unsigned long __fastcall GFRandom(unsigned long range,unsigned long* seed) { *seed=(unsigned long)GFROR13(*seed*0x24a1+0x24df); return range?*seed%range:0; }
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CEngineResourceManager::BuildAlphaStippleTexture(long size,CArray<unsigned char>&buffer)
{
 CArray<long> point_order_list;
 for(long p=0;p<size*size;++p)point_order_list.push_back(p);
 unsigned long seed=0;
 for(long p=0;p<size*size;++p){long tmp=point_order_list[p];unsigned long q=GFRandom(size*size,&seed);point_order_list[p]=point_order_list[q];point_order_list[q]=tmp;}
 float max_density=0.0f;
 for(long sy=0;sy<size;++sy)for(long sx=0;sx<size;++sx)if(sx||sy){
  C2DVector dist((float)min(min(labs(-sx),labs(-sx+size)),labs(-sx-size)),(float)min(min(labs(-sy),labs(-sy+size)),labs(-sy-size)));
  max_density+=1.0/(dist.GetSquaredMagnitude()*dist.GetSquaredMagnitude());
 }
 CArray<bool>global_point_mask(point_order_list.size(),false);
 CArray<long>point_list;
 float density_step=max_density*0.125f;
 for(float density_threshold=density_step;density_threshold<max_density+density_step*2;density_threshold+=density_step){
  CArray<bool>section_point_mask(point_order_list.size(),false);
  for(long n=0;n<8;++n){
   for(long rp=0;rp<(long)point_order_list.size();++rp){
    long src_point=point_order_list[rp];
    if(!global_point_mask[src_point]){
     long x=src_point%size,y=src_point/size;
     float total_density=0.0f;
     for(long p=0;p<(long)global_point_mask.size();++p){
      if(src_point!=p&&(global_point_mask[p]!=false||section_point_mask[p]!=false)){
       long sx=p%size,sy=p/size;
       C2DVector dist((float)min(min(labs(x-sx),labs(x-sx+size)),labs(x-sx-size)),(float)min(min(labs(y-sy),labs(y-sy+size)),labs(y-sy-size)));
       float density=1.0/(dist.GetSquaredMagnitude()*dist.GetSquaredMagnitude());total_density+=density;
       if(total_density>density_threshold)break;
      }
     }
     if(total_density<density_threshold)section_point_mask[src_point]=true;else section_point_mask[src_point]=false;
    }
   }
  }
  for(long p=0;p<(long)point_order_list.size();++p){long point=point_order_list[p];if(section_point_mask[point]){global_point_mask[point]=true;point_list.push_back(point);}}
 }
 buffer.resize(size*size);
 for(long p=0;p<(long)point_list.size();++p){long point=point_list[p];long sx=point%size,sy=point/size;long alpha=min(p*255/point_list.size()+1,255UL);buffer[sy*size+sx]=(unsigned char)alpha;}
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CEngineResourceManager::BuildHWStipplePatterns(const CArray<unsigned char>& stipple)
{
    HWInverseStipplePatterns.resize(0x2000, 0);
    for (long level = 0; level < 256; ++level) {
        for (long row = 0; row < 32; ++row) {
            unsigned long bits = 0;
            for (long column = 0; column < 32; ++column) {
                if (stipple[row * 32 + column] <= level + 1)
                    bits |= 1 << (31 - column);
            }
            HWInverseStipplePatterns[level * 32 + 31 - row] =
                (((bits & 0x00ff0000) | (bits >> 16)) >> 8) | (((bits & 0x0000ff00) | (bits << 16)) << 8);
        }
    }
}

void CEngineResourceManager::SetupAlphaStippleTexture(bool rebuild)
{
    // The four 2x2 ordered-dither patterns, one ARGB texel per dword (retail .rdata 0x012A1BB0, only read here).
    static const unsigned long DitherPatterns[4][4] = {
        { 0x06060606, 0x02020202, 0x00000000, 0x04040404 },
        { 0x02020202, 0x06060606, 0x04040404, 0x00000000 },
        { 0x00000000, 0x04040404, 0x06060606, 0x02020202 },
        { 0x04040404, 0x00000000, 0x02020202, 0x06060606 },
    };
    CArray<unsigned char> stipple(0x1000);
    CPixelFormat format(8, 8, 8, 8);
    StipleFadeTexture.Initialise(C2DExtentsI(32, 32), 1, format, 0, SURFACE_POOL_MANAGED, false, false);
    DitherTextureList.resize(4);
    for (unsigned long i = 0; i < DitherTextureList.size(); ++i) {
        DitherTextureList[i].Initialise(C2DExtentsI(2, 2), 1, format, 0, SURFACE_POOL_MANAGED, false, false);
        CSurfaceLockedRect rect = DitherTextureList[i].Lock(0, 0);
        ((unsigned long*)rect.PMem)[0] = DitherPatterns[i][0];
        ((unsigned long*)rect.PMem)[1] = DitherPatterns[i][1];
        ((unsigned long*)(rect.PMem + rect.Pitch))[0] = DitherPatterns[i][2];
        ((unsigned long*)(rect.PMem + rect.Pitch))[1] = DitherPatterns[i][3];
        DitherTextureList[i].Unlock(0);
    }
    CDiskFileWin32 file;
    CWideString path = EngineCachePath + L"StippleAlphaTexture.dat";
    if (CAFile::PathExists(path)) {
        file.Open(path, FILE_ACCESS_READ, 2);
        CFileDataInputStream in(file, 0x4000);
        if (in.ReadSLONG() == 8) {
            in.Read(stipple.GetAsCArray(), 0x1000);
            StipleFadeTexture.GetSurfaceLevel(0).CopyFromMemoryAutoSwizzle(stipple.GetAsCArray(), 0x80);
            HWInverseStipplePatterns.resize(0x2000, 0);
            in.Read(HWInverseStipplePatterns.GetAsCArray(), 0x8000);
            return;
        }
        file.Close();
    }
    if (!rebuild) {
        CAFile::DoErrorCallback();
        return;
    }
    file.Open(path, FILE_ACCESS_CREATE, 2);
    CFileDataOutputStream out(file);
    out.WriteSLONG(8);
    CArray<unsigned char> pattern;
    BuildAlphaStippleTexture(32, pattern);
    for (long i = 0; i < 0x400; ++i) {
        stipple[i * 4 + 0] = pattern[i];
        stipple[i * 4 + 1] = pattern[i];
        stipple[i * 4 + 2] = pattern[i];
        stipple[i * 4 + 3] = pattern[i];
    }
    out.Write(stipple.GetAsCArray(), 0x1000);
    StipleFadeTexture.GetSurfaceLevel(0).CopyFromMemoryAutoSwizzle(stipple.GetAsCArray(), 0x80);
    BuildHWStipplePatterns(pattern);
    out.Write(((const CArray<unsigned long>&)HWInverseStipplePatterns).GetAsCArray(), 0x8000);
    file.Close();
}