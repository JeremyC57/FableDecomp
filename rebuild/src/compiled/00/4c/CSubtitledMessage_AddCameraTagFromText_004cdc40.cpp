// 004cdc40 CSubtitledMessage::AddCameraTagFromText
// Leaf: ?AddCameraTagFromText@CSubtitledMessage@@AAE_NMABVCCharString@@@Z
// Flags: (none)
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_CSTRINGPARSER

#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CStringParser.h"
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CSUBTITLEDMESSAGE_PASS29
#include "engine/CSubtitledMessage.h"
#include "engine/CCRCSymbolMap.h"
namespace { extern CCRCSymbolMap AttitudeSymbols; }

bool CSubtitledMessage::AddCameraTagFromText(float percentage_through, const CCharString& tag_text)
{
    CStringParser parser(tag_text, 0, CCharString(""), 0);
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("("))
        return false;
    CCharString camera_positon_person = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString camera_position = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")"))
        return false;
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("("))
        return false;
    CCharString camera_focus_person = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString camera_focus_position = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")"))
        return false;
    parser.SkipWhitespace();
    CCharString camera_zoom_type;
    if (parser.SkipPastNextItemIfIs("(")) {
        camera_zoom_type = parser.ReadAsStringUntilString(")");
        if (!parser.SkipPastNextItemIfIs(")"))
            return false;
    }
    CCameraTag camera_tag;
    long val_camera_positon_person;
    if (AttitudeSymbols.FindSymbol(camera_positon_person, val_camera_positon_person))
        camera_tag.CameraPositionPerson = (NConversationTypes::EConversationProtagonist)val_camera_positon_person;
    else
        return false;
    long val_camera_position;
    if (AttitudeSymbols.FindSymbol(camera_position, val_camera_position))
        camera_tag.CameraPosition = (NConversationTypes::EConversationCameraPosition)val_camera_position;
    else
        return false;
    long val_camera_focus_person;
    if (AttitudeSymbols.FindSymbol(camera_focus_person, val_camera_focus_person))
        camera_tag.CameraFocusPerson = (NConversationTypes::EConversationProtagonist)val_camera_focus_person;
    else
        return false;
    long val_camera_focus_position;
    if (AttitudeSymbols.FindSymbol(camera_focus_position, val_camera_focus_position))
        camera_tag.CameraFocusPosition = (NConversationTypes::EConversationCameraFocus)val_camera_focus_position;
    else
        return false;
    camera_tag.CameraZoomType = NConversationTypes::NO_ZOOM;
    if (camera_zoom_type != "") {
        long val_camera_zoom_type;
        if (AttitudeSymbols.FindSymbol(camera_zoom_type, val_camera_zoom_type))
            camera_tag.CameraZoomType = (NConversationTypes::EConversationCameraZoomType)val_camera_zoom_type;
        else
            return false;
    }
    CameraTags.Insert(percentage_through, camera_tag);
    return true;
}