// CGraphicBankStateBlock::GetDefault(CBankStateBlock&) -- PDB00B56490 virtual void, not Boolean return
// Leaf: GetDefault
#include "engine/CGraphicBankStateBlock.h"
#include "engine/CPixelFormat.h"
#include <string.h>
// Native D3DFORMAT values, SDK enum parameter at009E3830/PDB00B356E0.
enum _D3DFORMAT {D3DFMT_V8U8=60,D3DFMT_DXT1=0x31545844,D3DFMT_DXT3=0x33545844,D3DFMT_DXT5=0x35545844};
// Explicit native scalar storage keeps donor long32 fields representable on host long64.
struct PixelFormat {fable_i32 FormatIndex;void Initialise(long,long,long,long,bool);void Initialise(_D3DFORMAT);}; //009E3790 void four long channel widths,bool;009E3830 void enum
struct PixelFormats {PixelFormat NonAlpha,Alpha,BooleanAlpha,InterpolatedAlpha,BumpMap,BumpMapAlpha,Signed;};
struct GraphicState {PixelFormats BankFormats,CompressedBankFormats;fable_i32 CompressionType,CompressionQuality,MinTextureWidth,MinTextureHeight,MaxTextureWidth,MaxTextureHeight,ResourceSelectionIndex,BumpmapFactor;bool ResourceIsSprite,ResourceIsMultiFile,ResourceIsBumpMapWithAlpha,EnableDXT1Format,EnableSizeReduction,EnableCompression,EnableSurfaceSection,EnableSignedFormats;};
struct GraphicStateBlock {void*VTable;GraphicState State;};
class CBankStateBlock;
struct CGraphicBankStateBlockMethods:CGraphicBankStateBlock {void GetDefault(CBankStateBlock&);};
void CGraphicBankStateBlockMethods::GetDefault(CBankStateBlock& block){
 GraphicStateBlock& s=*reinterpret_cast<GraphicStateBlock*>(&block);
 memset(&s.State,0,sizeof(s.State));
 s.State.BankFormats.NonAlpha.Initialise(8,8,8,8,false);
 s.State.BankFormats.Alpha.Initialise(8,8,8,8,false);
 s.State.BankFormats.InterpolatedAlpha.Initialise(8,8,8,8,false);
 s.State.BankFormats.BooleanAlpha.Initialise(8,8,8,8,false);
 s.State.BankFormats.BumpMap.Initialise(8,8,8,8,false);
 s.State.BankFormats.BumpMapAlpha.Initialise(8,8,8,8,false);
 s.State.BankFormats.Signed.Initialise(D3DFMT_V8U8);
 s.State.CompressedBankFormats.NonAlpha.Initialise(D3DFMT_DXT1);
 s.State.CompressedBankFormats.Alpha.Initialise(D3DFMT_DXT3);
 s.State.CompressedBankFormats.InterpolatedAlpha.Initialise(D3DFMT_DXT5);
 s.State.CompressedBankFormats.BooleanAlpha.Initialise(D3DFMT_DXT1);
 s.State.CompressedBankFormats.BumpMap.Initialise(D3DFMT_DXT1);
 s.State.CompressedBankFormats.BumpMapAlpha.Initialise(D3DFMT_DXT3);
 s.State.CompressedBankFormats.Signed.Initialise(D3DFMT_V8U8);
 s.State.CompressionType=4;s.State.CompressionQuality=75;s.State.MinTextureWidth=32;s.State.MinTextureHeight=32;s.State.MaxTextureWidth=1024;s.State.MaxTextureHeight=1024;s.State.ResourceSelectionIndex=-1;s.State.BumpmapFactor=-1;
 s.State.ResourceIsSprite=false;s.State.ResourceIsMultiFile=false;s.State.ResourceIsBumpMapWithAlpha=false;s.State.EnableSizeReduction=false;s.State.EnableSurfaceSection=false;s.State.EnableSignedFormats=false;s.State.EnableDXT1Format=true;s.State.EnableCompression=true;
}