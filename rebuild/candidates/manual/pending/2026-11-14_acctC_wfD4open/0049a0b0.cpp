// CProgressDisplay::InitialiseTextures(bool single_background, bool reduced) -- load the loading-screen textures (splash, shuffled backgrounds, guild seals, progress bar parts) and the shuffled loading-tip text tags
// Grade: DIFFER(2922v2920) -- every instruction equal to retail with esp displacements masked (md.sh) except the frame: 0xb4 v 0xb0. Slot membership differs: ours packs {texture, debug C2DExtentsI temp, debug CSurface temp} in a 16-byte slot above file (-100) and the else-branch size with the debug size at the bottom (-172); retail has {size, debug temps} at -132 below this and texture with the debug size at -172, plus fewer 4-byte slots (format / display / seeds). The 2 extra bytes are disp8 -> disp32 crossings.
// Leaf: ?InitialiseTextures@CProgressDisplay@@UAEX_N0@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_DATA_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_NEXTVALIDDIMENSIONS
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND
#define FABLE_RETAIL_CDISPLAYMANAGER_TEXTUREFORMATS
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_ISOPEN_INLINE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CProgressDisplay.h"
#include "engine/GFGetTime.h"
#include <string.h>

extern bool g_bAutoSaveProgressShown;                                    // 0x013B89C1
extern CWideString GResetProgressTipText;                                // 0x013B89D8
extern CWideString GResetProgressTipTitleText;                           // 0x013B89DC
namespace NResourceDirectoryNames { CWideString __fastcall GetMiscDir(); }   // 0x0041A0A0
CWideString __fastcall LongToWideString(long value);                    // 0x00405F00
float __fastcall GFFloatRandom(float range, unsigned long* seed);       // 0x004978A0

struct RandFunc {
    unsigned long* Seed;                                                 // +0x00
    RandFunc(unsigned long* seed) : Seed(seed) { }
    long operator()(long n) { return (long)GFFloatRandom((float)n, Seed); }
};

void CProgressDisplay::InitialiseTextures(bool single_background, bool reduced)
{
    if (g_bAutoSaveProgressShown)
        return;
    CDisplayManager* display = GFGetSystemManager()->GetDisplayManager();
    if (display == 0)
        return;
    if (!ProgressTextures.empty())
        return;
    CPixelFormat format;
    display->GetBestCompressedOpaqueTextureFormat(16, format);
    if (DebugDisplay) {
        ProgressTextures.resize(1);
        C2DExtentsI size = CTexture::GetNextValidDimensions(
            C2DExtentsI(display->GetBackBuffer().GetDimensions().X, display->GetBackBuffer().GetDimensions().Y), format);
        ProgressTextures[0].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
        ProgressTextures[0].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
            CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"splash.tga"), 2);
    } else {
        LoadingScreenTextures.clear();
        if (reduced && !g_bAutoSaveProgressShown) {
            ProgressTextures.clear();
            GuildSealTextures.clear();
        } else {
            ProgressTextures.resize(7);
            GuildSealTextures.resize(16);
        }
        C2DExtentsI size;
        {
            size.X = 0x400;
            size.Y = 0x200;
            if (FullScreen) {
                unsigned long seed = (unsigned long)GFGetTime();
                std::vector<long> backgrounds;
                for (long i = 2; i <= 9; ++i)
                    backgrounds.push_back(i);
                RandFunc random(&seed);
                std::random_shuffle(backgrounds.begin(), backgrounds.end(), random);
                long count = 2;
                if (reduced)
                    count = 1;
                CTexture texture(size, 1, format, 0, (ESurfacePool)1);
                for (long i = 0; i < count; ++i) {
                    CWideString name = NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\Fable_BG_PC_0";
                    name += LongToWideString(backgrounds[i]);
                    name += L".dds";
                    texture.GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(name, 2);
                    LoadingScreenTextures.push_back(texture);
                }
            }
            display->GetBestCompressedAlphaTextureFormat(16, format);
            if (FullScreen && !reduced) {
                size = C2DExtentsI(0x40, 0x40);
                long seals = 16;
                if (single_background)
                    seals = 1;
                for (long i = 0; i < seals; ++i) {
                    GuildSealTextures[i].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
                    CWideString name(L"LoadProgress\\guild_seal_");
                    if (i + 1 < 10)
                        name += L"0";
                    name += LongToWideString(i + 1);
                    name += L".dds";
                    GuildSealTextures[i].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                        CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + name), 2);
                }
            }
        }
        if ((FullScreen && !reduced) || g_bAutoSaveProgressShown) {
            size = C2DExtentsI(0x40, 0x20);
            ProgressTextures[1].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
            ProgressTextures[1].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\slot_left_bar_off.dds"), 2);
            size = C2DExtentsI(8, 0x20);
            ProgressTextures[2].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
            ProgressTextures[2].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\slot_mid_bar_off.dds"), 2);
            size = C2DExtentsI(0x40, 0x20);
            ProgressTextures[3].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
            ProgressTextures[3].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\slot_right_bar_off.dds"), 2);
            size = C2DExtentsI(4, 0x10);
            ProgressTextures[4].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
            ProgressTextures[4].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\Ybar_seg.dds"), 2);
            if (!g_bAutoSaveProgressShown) {
                size = C2DExtentsI(0x100, 0x80);
                ProgressTextures[5].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
                ProgressTextures[5].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                    CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\fable_BG_bar_border.dds"), 2);
                size = C2DExtentsI(0x80, 0x80);
                ProgressTextures[6].Initialise(size, 1, format, 0, (ESurfacePool)1, false, false);
                ProgressTextures[6].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                    CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\rc_seal_border.dds"), 2);
            }
        }
    }
    TextTags.clear();
    TextTagsPC.clear();
    if (FullScreen && !g_bAutoSaveProgressShown) {
        SetToDisplayText(true);
        CDiskFileWin32 file(CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + L"LoadProgress\\TextTags.txt"), FILE_ACCESS_READ, 2);
        if (file.IsOpen()) {
            CCharString contents;
            contents.LoadFromFile(file, 0, -1);
            CStringParser parser(contents);
            CPersistContext context(parser, 0, true, 0, 0, 0);
            context.TransferVector("TextTags", TextTags);
            context.TransferVector("TextTagsPC", TextTagsPC);
            file.Close();
        }
        unsigned long seed = (unsigned long)GFGetTime();
        RandFunc random(&seed);
        std::random_shuffle(TextTags.begin(), TextTags.end(), random);
        std::random_shuffle(TextTagsPC.begin(), TextTagsPC.end(), random);
        DisplayResetProgressTipText = true;
        if (GResetProgressTipText == L"") {
            CalculateNextTextTag();
            const NGameText::CDataBank* bank = GetPTextBank();
            if (bank != 0) {
                CCharString tag(TextTag);
                if (tag.EndsWith("0"))
                    tag = tag.GetSubString(0, tag.GetLength() - 3);
                GResetProgressTipText = bank->GetTextBySymbol(TextTag);
                GResetProgressTipTitleText = bank->GetTextBySymbol(tag + "_TITLE");
            }
            DisplayResetProgressTipText = false;
        }
    }
}
