#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <ctype.h>
static void check(bool b){if(!b)exit(3);}
typedef std::string CCharString;
struct CStringParser {
    std::string text;
    size_t pos;
    CStringParser(const CCharString& t,int,const CCharString&,int):text(t),pos(0){}
    void SkipWhitespace(){while(pos<text.size() && isspace(text[pos]))++pos;}
    bool SkipPastNextItemIfIs(const CCharString& token){if(text.compare(pos,token.size(),token))return false;pos+=token.size();return true;}
    CCharString ReadAsStringUntilString(const CCharString& token){
        size_t end=text.find(token,pos);CCharString val=text.substr(pos,end-pos);pos=end==std::string::npos?text.size():end;return val;
    }
};
struct Map { std::map<CCharString,long> symbols; bool FindSymbol(const CCharString& s,long& l) const {
    std::map<CCharString,long>::const_iterator i=symbols.find(s);if(i==symbols.end())return false;l=i->second;return true;
}} AttitudeSymbols;
namespace NConversationTypes {
enum EConversationProtagonist { NONE = 0, SPEAKER = 1, SPEAKEE = 2 };
enum EConversationCameraPosition { IN_FRONT_OF_FACE = 0, LEFT_SHOULDER = 1, RIGHT_SHOULDER = 2, BEHIND_LEFT_SHOULDER_NEAR = 3, BEHIND_LEFT_SHOULDER_FAR = 4, BEHIND_RIGHT_SHOULDER_NEAR = 5, BEHIND_RIGHT_SHOULDER_FAR = 6, LEFT_OF_TORSO_NEAR = 7, LEFT_OF_TORSO_FAR = 8, RIGHT_OF_TORSO_NEAR = 9, RIGHT_OF_TORSO_FAR = 10, POS_LEFT = 11, POS_RIGHT = 12 };
enum EConversationCameraFocus { FOCUS_FACE = 0, FOCUS_10 = 1, FOCUS_25 = 2, FOCUS_33 = 3, FOCUS_50 = 4, FOCUS_HIGH = 5, FOCUS_LEVEL = 6, FOCUS_LOW = 7 };
enum EConversationCameraZoomType { NO_ZOOM = 0, SLOW_ZOOM = 1, FAST_ZOOM = 2 };
enum EConversationCameraView { VIEW_WIDE = 0, VIEW_MEDIUM = 1, VIEW_CLOSE = 2 };
}

using namespace NConversationTypes;
struct CCameraTag {EConversationProtagonist CameraPositionPerson; EConversationCameraPosition CameraPosition; EConversationProtagonist CameraFocusPerson;EConversationCameraFocus CameraFocusPosition; EConversationCameraZoomType CameraZoomType;};
struct Tags {std::vector<std::pair<float,CCameraTag> > data; void Insert(float t,const CCameraTag& tag){data.push_back(std::make_pair(t,tag));}};
struct CSubtitledMessage {Tags CameraTags;bool AddCameraTagFromText(float,const CCharString&);};
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

int main(){
    AttitudeSymbols.symbols["a"]=1;AttitudeSymbols.symbols["b"]=2;AttitudeSymbols.symbols["c"]=3;AttitudeSymbols.symbols["d"]=4;AttitudeSymbols.symbols["z"]=5;
    const char* bad[]={"","a,b)","(a","(a,b","(a,b)","(a,b)(c","(a,b)(c,d","(a,b)(c,d)(z","(x,b)(c,d)","(a,x)(c,d)","(a,b)(x,d)","(a,b)(c,x)","(a,b)(c,d)(x)"};
    CSubtitledMessage msg;
    for(int i=0;i<13;++i){check(!msg.AddCameraTagFromText(.1f,bad[i]));check(msg.CameraTags.data.empty());}
    const char* good[]={"(a,b)(c,d)"," (a, b) (c, d) ()","(a,b)(c,d)(z)"};
    for(int j=0;j<3;++j){
        float t=(float)j+.25f;check(msg.AddCameraTagFromText(t,good[j]));check(msg.CameraTags.data.size()==j+1);
        const std::pair<float,CCameraTag>& pair=msg.CameraTags.data[j];const CCameraTag& tag=pair.second;
        check(pair.first==t && tag.CameraPositionPerson==1 && tag.CameraPosition==2 && tag.CameraFocusPerson==3 && tag.CameraFocusPosition==4 && tag.CameraZoomType==(j==2?5:0));
    }
    puts("004cdc40_TEST PASS");return 0;
}