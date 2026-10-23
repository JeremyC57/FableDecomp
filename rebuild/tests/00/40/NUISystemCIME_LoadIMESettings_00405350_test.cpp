// BEGIN exact source snapshot SHA256 26eb480c4592293c635f6f1f52468828b37a8f54600204b847c01bea9edd155c
// 00405350 CIME::LoadIMESettings. Existing repository reconstruction, retained for RTM verification.
#include "fable_ime.h"
#include "fable_persist_context.h"
#include "fable_string.h"
// 0x004045c0 is Transfer<bool> per Ego_r PDB; 0x00405a60 is signed char.

extern "C" signed char g_FableImeSymbolHeight_013B7CF0;
extern "C" signed char g_FableImeSymbolTranslucence_013B7CF1;
extern "C" signed char g_FableImeSymbolPlacement_013B7CF2;
extern "C" signed char g_FableImeCompositionTranslucence_013B7D18;
extern "C" signed char g_FableImeCaretWidth_013B7D20;
extern "C" signed char g_FableImeCaretYMargin_013B7D21;
extern "C" bool g_FableImeCancelOnOkClick_013B7D28;
extern "C" bool g_FableImeEnabled_013B7CD8;

extern "C" fable_u32 g_FableImeSymbolColour_013B7CE4;
extern "C" fable_u32 g_FableImeSymbolColourOff_013B7CE8;
extern "C" fable_u32 g_FableImeSymbolColourText_013B7CEC;
extern "C" fable_u32 g_FableImeCandidateColourBase_013B7CF8;
extern "C" fable_u32 g_FableImeCandidateColourBorder_013B7CFC;
extern "C" fable_u32 g_FableImeCandidateColourText_013B7D00;
extern "C" fable_u32 g_FableImeCandidateColourInput_013B7D04;
extern "C" fable_u32 g_FableImeCandidateColourTargetConverted_013B7D08;
extern "C" fable_u32 g_FableImeCandidateColourConverted_013B7D0C;
extern "C" fable_u32 g_FableImeCandidateColourTargetNotConverted_013B7D10;
extern "C" fable_u32 g_FableImeCandidateColourInputError_013B7D14;

fable_u32 FABLE_FASTCALL TransferIMEColour(
    CPersistContext& context,
    const CCharString& token,
    fable_u32 defaultColour);

void FABLE_FASTCALL NUISystem::CIME::LoadIMESettings(
    CPersistContext& context)
{
    union DefaultValue { signed char byteValue; bool boolValue; } fallback;
    fallback.byteValue = 0x18;
    context.TransferSignedChar(
        "SymbolHeight",
        g_FableImeSymbolHeight_013B7CF0,
        fallback.byteValue);
    fallback.byteValue = -0x60;
    context.TransferSignedChar(
        "SymbolTranslucence",
        g_FableImeSymbolTranslucence_013B7CF1,
        fallback.byteValue);
    fallback.byteValue = 0;
    context.TransferSignedChar(
        "SymbolPlacement",
        g_FableImeSymbolPlacement_013B7CF2,
        fallback.byteValue);
    fallback.byteValue = -0x80;
    context.TransferSignedChar(
        "CompTranslucence",
        g_FableImeCompositionTranslucence_013B7D18,
        fallback.byteValue);
    fallback.byteValue = 2;
    context.TransferSignedChar(
        "CaretWidth",
        g_FableImeCaretWidth_013B7D20,
        fallback.byteValue);
    fallback.byteValue = 1;
    context.TransferSignedChar(
        "CaretYMargin",
        g_FableImeCaretYMargin_013B7D21,
        fallback.byteValue);

#define FABLE_LOAD_IME_COLOUR(text, destination, fallback) \
    { \
        CCharString token(text, -1); \
        destination = TransferIMEColour(context, token, fallback); \
    }

    FABLE_LOAD_IME_COLOUR(
        "SymbolColor",
        g_FableImeSymbolColour_013B7CE4,
        0);
    FABLE_LOAD_IME_COLOUR(
        "SymbolColorOff",
        g_FableImeSymbolColourOff_013B7CE8,
        0x00404040);
    FABLE_LOAD_IME_COLOUR(
        "SymbolColorText",
        g_FableImeSymbolColourText_013B7CEC,
        0xFF000000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorBase",
        g_FableImeCandidateColourBase_013B7CF8,
        0xFFFFFFFF);
    FABLE_LOAD_IME_COLOUR(
        "CandColorBorder",
        g_FableImeCandidateColourBorder_013B7CFC,
        0xFF000000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorText",
        g_FableImeCandidateColourText_013B7D00,
        0);
    FABLE_LOAD_IME_COLOUR(
        "CandColorInput",
        g_FableImeCandidateColourInput_013B7D04,
        0x00FFFF00);
    FABLE_LOAD_IME_COLOUR(
        "CandColorTargetConv",
        g_FableImeCandidateColourTargetConverted_013B7D08,
        0x000000FF);
    FABLE_LOAD_IME_COLOUR(
        "CandColorConverted",
        g_FableImeCandidateColourConverted_013B7D0C,
        0x0000FF00);
    FABLE_LOAD_IME_COLOUR(
        "CandColorTargetNotConv",
        g_FableImeCandidateColourTargetNotConverted_013B7D10,
        0x00FF0000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorInputErr",
        g_FableImeCandidateColourInputError_013B7D14,
        0x00FF0000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorText",
        g_FableImeCandidateColourText_013B7D00,
        0);

#undef FABLE_LOAD_IME_COLOUR

    fallback.boolValue = true;
    context.Transfer<bool>(
        "CancelOnOKClick",
        g_FableImeCancelOnOkClick_013B7D28,
        fallback.boolValue);
    fallback.boolValue = false;
    context.Transfer<bool>(
        "EnableIME",
        g_FableImeEnabled_013B7CD8,
        fallback.boolValue);
}

// END exact source snapshot
#include <stdio.h>
#include <string.h>

#include "fable_ime.h"
#include "fable_persist_context.h"
#include "fable_string.h"

extern "C" signed char g_FableImeSymbolHeight_013B7CF0;
extern "C" signed char g_FableImeSymbolTranslucence_013B7CF1;
extern "C" signed char g_FableImeSymbolPlacement_013B7CF2;
extern "C" signed char g_FableImeCompositionTranslucence_013B7D18;
extern "C" signed char g_FableImeCaretWidth_013B7D20;
extern "C" signed char g_FableImeCaretYMargin_013B7D21;
extern "C" bool g_FableImeCancelOnOkClick_013B7D28;
extern "C" bool g_FableImeEnabled_013B7CD8;

extern "C" fable_u32 g_FableImeSymbolColour_013B7CE4;
extern "C" fable_u32 g_FableImeSymbolColourOff_013B7CE8;
extern "C" fable_u32 g_FableImeSymbolColourText_013B7CEC;
extern "C" fable_u32 g_FableImeCandidateColourBase_013B7CF8;
extern "C" fable_u32 g_FableImeCandidateColourBorder_013B7CFC;
extern "C" fable_u32 g_FableImeCandidateColourText_013B7D00;
extern "C" fable_u32 g_FableImeCandidateColourInput_013B7D04;
extern "C" fable_u32 g_FableImeCandidateColourTargetConverted_013B7D08;
extern "C" fable_u32 g_FableImeCandidateColourConverted_013B7D0C;
extern "C" fable_u32 g_FableImeCandidateColourTargetNotConverted_013B7D10;
extern "C" fable_u32 g_FableImeCandidateColourInputError_013B7D14;

extern "C" signed char g_FableImeSymbolHeight_013B7CF0 = 0;
extern "C" signed char g_FableImeSymbolTranslucence_013B7CF1 = 0;
extern "C" signed char g_FableImeSymbolPlacement_013B7CF2 = 0;
extern "C" signed char g_FableImeCompositionTranslucence_013B7D18 = 0;
extern "C" signed char g_FableImeCaretWidth_013B7D20 = 0;
extern "C" signed char g_FableImeCaretYMargin_013B7D21 = 0;
extern "C" bool g_FableImeCancelOnOkClick_013B7D28 = 0;
extern "C" bool g_FableImeEnabled_013B7CD8 = 0;

extern "C" fable_u32 g_FableImeSymbolColour_013B7CE4 = 0;
extern "C" fable_u32 g_FableImeSymbolColourOff_013B7CE8 = 0;
extern "C" fable_u32 g_FableImeSymbolColourText_013B7CEC = 0;
extern "C" fable_u32 g_FableImeCandidateColourBase_013B7CF8 = 0;
extern "C" fable_u32 g_FableImeCandidateColourBorder_013B7CFC = 0;
extern "C" fable_u32 g_FableImeCandidateColourText_013B7D00 = 0;
extern "C" fable_u32 g_FableImeCandidateColourInput_013B7D04 = 0;
extern "C" fable_u32 g_FableImeCandidateColourTargetConverted_013B7D08 = 0;
extern "C" fable_u32 g_FableImeCandidateColourConverted_013B7D0C = 0;
extern "C" fable_u32 g_FableImeCandidateColourTargetNotConverted_013B7D10 = 0;
extern "C" fable_u32 g_FableImeCandidateColourInputError_013B7D14 = 0;

namespace
{
    struct ByteTransfer
    {
        const char* name;
        signed char* destination;
        signed char fallback;
    };

    struct ColourTransfer
    {
        const char* name;
        fable_u32 fallback;
    };

    ByteTransfer g_byteTransfers[8];
    ColourTransfer g_colourTransfers[12];
    unsigned int g_byteTransferCount;
    unsigned int g_colourTransferCount;
    unsigned int g_constructorCount;
    unsigned int g_destructorCount;

    bool CheckByteTransfers()
    {
        static const char* const names[8] =
        {
            "SymbolHeight",
            "SymbolTranslucence",
            "SymbolPlacement",
            "CompTranslucence",
            "CaretWidth",
            "CaretYMargin",
            "CancelOnOKClick",
            "EnableIME"
        };
        static const signed char fallbacks[8] =
        {
            0x18, -0x60, 0, -0x80, 2, 1, 1, 0
        };
        signed char* destinations[8] =
        {
            &g_FableImeSymbolHeight_013B7CF0,
            &g_FableImeSymbolTranslucence_013B7CF1,
            &g_FableImeSymbolPlacement_013B7CF2,
            &g_FableImeCompositionTranslucence_013B7D18,
            &g_FableImeCaretWidth_013B7D20,
            &g_FableImeCaretYMargin_013B7D21,
            reinterpret_cast<signed char*>(&g_FableImeCancelOnOkClick_013B7D28),
            reinterpret_cast<signed char*>(&g_FableImeEnabled_013B7CD8)
        };

        if (g_byteTransferCount != 8)
        {
            return false;
        }
        for (unsigned int index = 0; index != 8; ++index)
        {
            if (
                strcmp(g_byteTransfers[index].name, names[index]) != 0 ||
                g_byteTransfers[index].destination != destinations[index] ||
                g_byteTransfers[index].fallback != fallbacks[index] ||
                *destinations[index] !=
                    static_cast<signed char>(index < 6 ? fallbacks[index] + 1 : !fallbacks[index]))
            {
                return false;
            }
        }
        return true;
    }

    bool CheckColourTransfers()
    {
        static const char* const names[12] =
        {
            "SymbolColor",
            "SymbolColorOff",
            "SymbolColorText",
            "CandColorBase",
            "CandColorBorder",
            "CandColorText",
            "CandColorInput",
            "CandColorTargetConv",
            "CandColorConverted",
            "CandColorTargetNotConv",
            "CandColorInputErr",
            "CandColorText"
        };
        static const fable_u32 fallbacks[12] =
        {
            0,
            0x00404040,
            0xFF000000,
            0xFFFFFFFF,
            0xFF000000,
            0,
            0x00FFFF00,
            0x000000FF,
            0x0000FF00,
            0x00FF0000,
            0x00FF0000,
            0
        };

        if (
            g_colourTransferCount != 12 ||
            g_constructorCount != 12 ||
            g_destructorCount != 12)
        {
            return false;
        }
        for (unsigned int index = 0; index != 12; ++index)
        {
            if (
                strcmp(g_colourTransfers[index].name, names[index]) != 0 ||
                g_colourTransfers[index].fallback != fallbacks[index])
            {
                return false;
            }
        }

        return
            g_FableImeSymbolColour_013B7CE4 == 1 &&
            g_FableImeSymbolColourOff_013B7CE8 == 0x00404042 &&
            g_FableImeSymbolColourText_013B7CEC == 0xFF000003 &&
            g_FableImeCandidateColourBase_013B7CF8 == 3 &&
            g_FableImeCandidateColourBorder_013B7CFC == 0xFF000005 &&
            g_FableImeCandidateColourText_013B7D00 == 12 &&
            g_FableImeCandidateColourInput_013B7D04 == 0x00FFFF07 &&
            g_FableImeCandidateColourTargetConverted_013B7D08 == 0x107 &&
            g_FableImeCandidateColourConverted_013B7D0C == 0x0000FF09 &&
            g_FableImeCandidateColourTargetNotConverted_013B7D10 ==
                0x00FF000A &&
            g_FableImeCandidateColourInputError_013B7D14 ==
                0x00FF000B;
    }
}

CCharString::CCharString(const char* text, long length)
{
    ++g_constructorCount;
    storage_ = reinterpret_cast<CCharStringData*>(
        const_cast<char*>(text));
    if (length != -1)
    {
        storage_ = 0;
    }
}

CCharString::~CCharString()
{
    ++g_destructorCount;
}

void CPersistContext::TransferSignedChar(
    const char* name,
    signed char& value,
    const signed char& defaultValue)
{
    const unsigned int index = g_byteTransferCount++;
    if (index < 8)
    {
        g_byteTransfers[index].name = name;
        g_byteTransfers[index].destination = &value;
        g_byteTransfers[index].fallback = defaultValue;
    }
    value = static_cast<signed char>(defaultValue + 1);
}

fable_u32 FABLE_FASTCALL TransferIMEColour(
    CPersistContext&,
    const CCharString& token,
    fable_u32 defaultColour)
{
    const unsigned int index = g_colourTransferCount++;
    const char* name = reinterpret_cast<const char*>(
        *reinterpret_cast<void* const*>(&token));
    if (index < 12)
    {
        g_colourTransfers[index].name = name;
        g_colourTransfers[index].fallback = defaultColour;
    }
    return defaultColour + index + 1;
}

int main()
{
    CPersistContext context;
    NUISystem::CIME::LoadIMESettings(context);

    const bool passed =
        CheckByteTransfers() &&
        CheckColourTransfers();
    if (!passed)
    {
        printf("00405350_TEST FAIL\n");
        return 1;
    }

    printf("00405350_TEST PASS\n");
    return 0;
}

template<class T> void CPersistContext::Transfer(const char* name,T& value,const T& fallback) {
 unsigned i=g_byteTransferCount++; if(i<8) { g_byteTransfers[i].name=name; g_byteTransfers[i].destination=reinterpret_cast<signed char*>(&value); g_byteTransfers[i].fallback=fallback; }
 value=!fallback;
}