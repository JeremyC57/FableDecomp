// Behaviour test for 0x00615920 CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::TransferIn, on the shared headers:
// the row's defines, includes and body verbatim. The traits object is default-built (Region 0, UIDefName "" through the one
// CCharString(const char*, long) constructor with length -1, the rest zero), the out-of-line traits reader gets the context, the
// parser and &vector[index], and the traits string is destroyed after the call.
#include <stdio.h>
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableVector.h"
#include "engine/CTCInventoryMap.h"

// The traits reader is out of line in retail (0x006159A0, its own landed row, an explicit specialisation there): declared only.
template <> void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& context, CStringParser& parser,
                                                                                CTCInventoryMap::CUIMiniMapMarker& value);   // 0x006159A0

template <class T>
void CPersistContext_TransferableVector<T>::TransferIn(CPersistContext& context, CStringParser& parser, long index)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, (*TransVector)[index]);
}
template void CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext&, CStringParser&, long);

static int g_strings = 0, g_emptyStrings = 0, g_calls = 0, g_defaultsOk = 0;
static void* g_ctx = 0; static void* g_parser = 0; static void* g_value = 0;
CCharString::CCharString(const char* text, long length) { ++g_strings; if (text[0] == 0 && length == -1) ++g_emptyStrings; }
CCharString::~CCharString() { --g_strings; }
template <> void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& context, CStringParser& parser,
                                                                               CTCInventoryMap::CUIMiniMapMarker& value)
{
    ++g_calls; g_ctx = &context; g_parser = &parser; g_value = &value;
    g_defaultsOk = DefaultVal.Region == 0 && DefaultVal.Position.X == 0.0f && DefaultVal.Position.Y == 0.0f && DefaultVal.DefNameLength == 0
        && DefaultVal.GraphicIndex == 0 && DefaultVal.Angle == 0.0f && g_strings == 1 && g_emptyStrings == 1;
}
template <> long CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::Size() { return 0; }
template <> void CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::Resize(long) { }
template <> void CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::TransferOut(CPersistContext&, CCharString&, long) { }

static double g_markers[4 * 0x1c / 8];   // four raw 0x1c-byte markers (never constructed)
static double g_context[16], g_parserImage[16];
int main()
{
    if (sizeof(void*) == 4 && sizeof(CTCInventoryMap::CUIMiniMapMarker) != 0x1c) { printf("00615920_TEST FAIL size\n"); return 1; }
    char* first = (char*)g_markers;
    void* raw[3] = { first, first + 4 * 0x1c, first + 4 * 0x1c };   // STLport vector: start, finish, end of storage
    std::vector<CTCInventoryMap::CUIMiniMapMarker>& markers = *(std::vector<CTCInventoryMap::CUIMiniMapMarker>*)raw;
    CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker> wrapper(markers);
    CPersistContext_TransferableVectorGenericType* generic = &wrapper;
    CPersistContext& context = *(CPersistContext*)g_context;
    CStringParser& parser = *(CStringParser*)g_parserImage;
    generic->TransferIn(context, parser, 2);
    int ok = g_calls == 1 && g_ctx == (void*)g_context && g_parser == (void*)g_parserImage && g_value == (void*)(first + 2 * 0x1c)
        && g_defaultsOk && g_strings == 0;
    printf(ok ? "00615920_TEST PASS\n" : "00615920_TEST FAIL\n");
    return ok ? 0 : 1;
}
