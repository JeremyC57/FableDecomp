// 006fa4e0 NAnimationEvents::CManager::Load
// Leaf: ?Load@CManager@NAnimationEvents@@QAE_NABVCWideString@@@Z
// Flags: /Gr
#define FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_SLONGMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_SYMBOLMAP_ENUM_PASS29
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_string.h"
class CStringParser;
#include "engine/NAnimationEvents_CManager.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CStringParser.h"
#include "engine/CParsedItem.h"

extern bool GUseCompiledAnimationEvents;
extern bool GAllowDataGeneration;
namespace NBankFileManager { CWideString FABLE_FASTCALL GetHeaderDir(); }

// Inline-budget: the retail loader calls the separately emitted CSymbolMap::FindSymbol.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
bool CSymbolMap::FindSymbol(const CCharString& name, long& value) const
{
    typedef CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> > SymbolMap;
    const SymbolMap& map = SLONGMap;
    SymbolMap::const_iterator it = map.LowerBound(name);
    SymbolMap::const_iterator last = map.end();
    if (it != last && !name.FastCompare(it->first))
        last = it;
    it = last;
    if (it != SLONGMap.end()) {
        value = it->second.Val;
        return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

bool NAnimationEvents::CManager::Load(const CWideString& animation_events_save_path)
{
    if (GUseCompiledAnimationEvents) {
        CWideString bin_drive, bin_dir, bin_filename;
        CAFile::SplitPath(animation_events_save_path, &bin_drive, &bin_dir, &bin_filename, 0);
        CWideString bin_pathname = bin_drive + bin_dir + bin_filename + L".bin";
        LoadBinary(bin_pathname);
        DifferentToSavedCopy = false;
        return true;
    }
    bool file_exists = CAFile::PathExists(animation_events_save_path);
    if (file_exists) {
        AnimToExtraEventsMap.clear();
        CDiskFileWin32 game_file(animation_events_save_path, FILE_ACCESS_READ, 2);
        CCharString script(game_file, -1);
        CStringParser parser(script);
        if (parser.SkipPastString("BEGIN_ANIMATION_EVENTS")) {
            CSymbolMap mesh_symbol_map;
            mesh_symbol_map.LoadFromFile(NBankFileManager::GetHeaderDir()+L"meshdata.h");
            while (parser.SkipPastNextItemIfIs("BEGIN_EVENTS:")) {
                parser.SkipWhitespace();
                CCharString anim_name = parser.ReadAsString();
                long anim_index;
                if (mesh_symbol_map.FindSymbol(anim_name, anim_index)) {
                    while (!parser.SkipPastNextItemIfIs("END_EVENTS")) {
                        CCharString event_name = parser.ReadAsString();
                        long event_index = EventSymbolMap.GetSymbolSLONG(event_name);
                        float event_time = parser.ReadAsFloat();
                        bool stop_event = false;
                        CParsedItem parsed_item;
                        if (parser.PeekNextItem(parsed_item) && parsed_item.GetType() == PARSED_ITEM_STRING) {
                            if (parsed_item.GetString() == "STOP") {
                                stop_event = true;
                                parser.ReadNextItem(parsed_item);
                            } else if (parsed_item.GetString() == "START") {
                                parser.ReadNextItem(parsed_item);
                            }
                        }
                        CEvent event(event_index, event_time, false, stop_event);
                        CVectorMap<unsigned long, CSmallVector<CEvent, 32> >::iterator anim_pos = AnimToExtraEventsMap.Find(anim_index);
                        if (!EventExists(anim_index, event))
                            AddEventToAnim(anim_index, event);
                    }
                    DifferentToSavedCopy = false;
                    parser.SkipWhitespace();
                } else {
                    parser.SkipPastString("END_EVENTS");
                }
            }
            parser.SkipPastString("END_ANIMATION_EVENTS");
        }
    }
    AnimToExtraEventsMap.DeallocateUnusedMemory();
    if (GAllowDataGeneration) {
        CWideString bin_drive, bin_dir, bin_filename;
        CAFile::SplitPath(animation_events_save_path, &bin_drive, &bin_dir, &bin_filename, 0);
        CWideString bin_pathname = bin_drive+bin_dir+bin_filename+L".bin";
        SaveBinary(bin_pathname);
    }
    return file_exists;
}