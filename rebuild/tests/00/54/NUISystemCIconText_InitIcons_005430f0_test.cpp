#define private public
#define protected public
#define FABLE_RETAIL_ICON_REVERSE_ITERATOR_PASS29
#define FABLE_RETAIL_ICONTEXT_FONT_INLINE_PASS29
#define FABLE_RETAIL_ICONTEXT_WIDTH_PAREN_PASS29
#define FABLE_RETAIL_ICONTEXT_CLEAR_OUTOFLINE_PASS29
#define FABLE_RETAIL_ICON_FLAGS_PASS29
// Leaf: ?InitIcons@CIconText@NUISystem@@UAEXXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CICONTEXT_VSLOTS
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#include "fable_string.h"
void* __cdecl operator new[](unsigned int);
#include "engine/NUISystem_CIconText.h"
#ifdef _MSC_VER
// Inline-budget: retail 0048da89 and 0048daa2 select the size-built font accessors.
#pragma optimize("s",on)
#endif
#include "engine/CFontBank.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/GFFloatToLongCeil.h"
#include <math.h>
#include <wchar.h>
#define FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include "stlport/fable_algorithm.h"
namespace NUISystem {
void CIconText::InitIcons()
{
    float space_width = PFont->GetStringWidth(L"  ") - PFont->GetStringWidth(L" ");
    long num_spaces = 0;
    for (CEmbeddedIcon* icon_it = Icons.begin(); icon_it != Icons.end();) {
        const CEmbeddedIcon& icon = *icon_it;
        if (icon.Flags != ICON_INSIDE_TEXT) { ++icon_it; continue; }
        float total_icon_width = icon.Size.X;
        CEmbeddedIcon* follow_it = icon_it + 1;
        for (; follow_it != Icons.end(); ++follow_it) {
            CEmbeddedIcon& follow_icon = *icon_it;
            if (follow_icon.Flags != ICON_INSIDE_TEXT) continue;
            if (follow_icon.CharIndex != icon.CharIndex) break;
            total_icon_width += follow_icon.Size.X;
        }
        num_spaces += GFFloatToLongCeil(total_icon_width / space_width);
        icon_it = follow_it;
    }
    long source_length = Text.GetLength();
    long dest_length = source_length + num_spaces + 1;
    const wchar_t* source_text = Text.GetAsCharArray();
    wchar_t* dest_text = new wchar_t[dest_length];
    long source_index = 0;
    long dest_index = 0;
    for (CEmbeddedIcon* icon_it = Icons.begin(); icon_it != Icons.end();) {
        CEmbeddedIcon& icon = *icon_it;
        if (icon.Flags != ICON_INSIDE_TEXT) { ++icon_it; continue; }
        long num_characters;
        if (icon.CharIndex > source_length) num_characters = source_length - source_index;
        else num_characters = icon.CharIndex - source_index;
        if (num_characters) {
            wcsncpy(dest_text + dest_index, source_text + source_index, num_characters);
            source_index += num_characters;
            dest_index += num_characters;
        }
        dest_text[dest_index] = 0;
        float space_start = PFont->GetStringWidth(dest_text);
        float total_icon_width = icon.Size.X;
        float max_icon_height = icon.Size.Y;
        CEmbeddedIcon* follow_it = icon_it + 1;
        for (; follow_it != Icons.end(); ++follow_it) {
            CEmbeddedIcon& follow_icon = *icon_it;
            if (follow_icon.Flags == ICON_INSIDE_TEXT) {
                if (follow_icon.CharIndex != icon.CharIndex) break;
                total_icon_width += follow_icon.Size.X;
                if (follow_icon.Size.Y > max_icon_height) max_icon_height = follow_icon.Size.Y;
            }
        }
        num_spaces = GFFloatToLongCeil(total_icon_width / space_width);
        for (long space = 0; space < num_spaces; ++space) dest_text[dest_index++] = L' ';
        dest_text[dest_index] = 0;
        float space_end = PFont->GetStringWidth(dest_text);
        float icon_offset;
        if(space_start != 0) icon_offset=floor(space_end + space_start - total_icon_width)/2.0f;
        else icon_offset=0;
        for (follow_it = icon_it; follow_it != Icons.end(); ++follow_it) {
            CEmbeddedIcon& follow_icon = *follow_it;
            if (follow_icon.Flags != ICON_INSIDE_TEXT) continue;
            if (follow_icon.CharIndex != icon.CharIndex) break;
            follow_icon.Offset.X = icon_offset;
            follow_icon.Offset.Y = float(floor(PFont->GetFontMaxHeight() - max_icon_height)) / 2.0f;
            icon_offset += follow_icon.Size.X;
        }
        icon_it = follow_it;
    }
    long num_characters = source_length - source_index;
    if (num_characters) {
        wcsncpy(dest_text + dest_index, source_text + source_index, num_characters);
        source_index += num_characters;
        dest_index += num_characters;
    }
    dest_text[dest_index] = 0;
    float current_position = PositionOffset.X;
    for (std::vector<CEmbeddedIcon>::reverse_iterator icon_it = Icons.rbegin(); icon_it != Icons.rend(); ++icon_it) {
        CEmbeddedIcon& icon = *icon_it;
        if (icon.Flags != ICON_RIGHT_JUSTIFIED) continue;
        icon.Offset.X = current_position - icon.Size.X;
        icon.Offset.Y = (PFont->GetFontMaxHeight() - icon.Size.Y) / 2.0f;
        current_position = current_position - icon.Size.X - 2.0f;
    }
    std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    IconHandles.resize(Icons.size());
    DisplayText = CWideString(dest_text);
    C2DExtentsF extents = PFont->GetStringExtents(DisplayText, false);
    Size.X = extents.X;
    Size.Y = extents.Y;
    delete[] dest_text;
}
}
#undef private
#undef protected
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace NUISystem;

static int errors, clears, resizes, heights, measures;
static CEnginePrimitiveHandle* first_handle;
static void check(bool v) { if (!v) ++errors; }
namespace std {
template<> void vector<CEnginePrimitiveHandle>::resize(size_type n) {
    ++resizes; check(n<=4); _M_finish=_M_start+n;
}
}
void CEnginePrimitiveHandle::Clear() { check(this==first_handle+clears); ++clears; }
struct StringStorage { long count; wchar_t chars[128]; };
static StringStorage*& storage(const CWideString& s) {return *(StringStorage**)&s;}
static void drop(CWideString& s){StringStorage* b=storage(s); if(b && --b->count==0) free(b); storage(s)=0;}
CWideString::CWideString(const wchar_t* data) {
    StringStorage* b=(StringStorage*)malloc(sizeof(StringStorage));check(wcslen(data)<128);b->count=1;wcscpy(b->chars,data);storage(*this)=b;
}
CWideString::~CWideString(){drop(*this);}
const CWideString& CWideString::operator=(const CWideString& s){if(this!=&s){drop(*this);storage(*this)=storage(s);if(storage(s))++storage(s)->count;}return *this;}
const wchar_t* CWideString::GetAsCharArray() const {return storage(*this)?storage(*this)->chars:L"";}
long CWideString::GetLength() const {return (long)wcslen(GetAsCharArray());}
static float space_pixels, ascent;
static CIconText* active;static CFontBank* old_font;static CFontBank* next_font;
static bool change_font;
static float width(const wchar_t* s){float sum=0;for(;*s;++s)sum+=(*s==L' '?space_pixels:3+(*s%5));return sum;}
static C2DExtentsF* __fastcall metrics(void* receiver,void*,C2DExtentsF* out,const wchar_t* s,bool spaces) {
    // The /Od source test follows the editor's distinct call order and receiver reload.
    check(receiver==(change_font && measures>=1?next_font:old_font));
    if (measures < 2) check(wcscmp(s, measures == 0 ? L"  " : L" ") == 0);
    if(change_font && measures==0)active->PFont.Data=next_font;
    ++measures;check(!spaces);out->X=width(s);out->Y=ascent;return out;
}
static float __fastcall height(void* receiver,void*){
    check(receiver==(change_font?next_font:old_font));
    ++heights;
    return ascent;
}
struct Expected {float x,y;};
int main(){
    unsigned long object[sizeof(CIconText)/4]={0}, handle_words[sizeof(CEnginePrimitiveHandle)]={0};
    CIconText& t=*(CIconText*)object;first_handle=(CEnginePrimitiveHandle*)handle_words;
    t.IconHandles._M_start=first_handle;void* slots[5]={0};slots[1]=(void*)metrics;slots[2]=(void*)height;void** font=slots;void** replacement=slots;old_font=(CFontBank*)&font;next_font=(CFontBank*)&replacement;active=&t;
    CIconText::CEmbeddedIcon icons[4];t.Icons._M_start=icons;
    int total=0;
    for(int sample=0;sample<3;++sample)for(int count=0;count<=4;++count)
    for(int switch_receiver=0;switch_receiver<2;++switch_receiver)for(unsigned pattern=0;pattern<(1u<<(2*count));++pattern)for(int group=0;group<3;++group){
        change_font=switch_receiver!=0;t.PFont.Data=old_font;
        const wchar_t* input=sample==0?L"":sample==1?L"aB":L"gHi";
        const int len=(int)wcslen(input);
        space_pixels=sample==1?3.0f:4.0f;ascent=sample==2?15.5f:12.0f;
        const int pos=group==0?0:group==1?1:len+3;
        CWideString in(input);t.Text=in;t.Icons._M_finish=icons+count;
        const int prior=(count+group)%5;t.IconHandles._M_finish=first_handle+prior;
        t.PositionOffset.X=45.25f+group;long first=-1,inside=0,right=0;Expected expected[4];float cursor=t.PositionOffset.X;
        for(int k=0;k<count;++k){
            const unsigned flag=(pattern>>(k*2))&3;
            icons[k].Flags=(EIconFlags)flag;icons[k].CharIndex=pos;icons[k].Size.X=1.5f+k*2;icons[k].Size.Y=3.0f+k;
            icons[k].Offset.X=expected[k].x=-200-k;icons[k].Offset.Y=expected[k].y=-300-k;
            if(flag==1){if(first<0)first=k;++inside;} if(flag==2)++right;
        }
        int sourcepos=pos<len?pos:len;wchar_t output[128];wcscpy(output,input);
        if(inside){
            const float total_width=(count-first)*icons[first].Size.X;
            const int num=(int)ceil(total_width/space_pixels);
            wchar_t prefix[64];wcsncpy(prefix,input,sourcepos);prefix[sourcepos]=0;float start=width(prefix);
            wcscpy(output,prefix);for(int j=0;j<num;++j)output[sourcepos+j]=L' ';wcscpy(output+sourcepos+num,input+sourcepos);
            float x=start!=0?(float)floor((start+num*space_pixels)+start-total_width)*.5f:0;
            for(int k=0;k<count;++k)if(icons[k].Flags==1){expected[k].x=x;expected[k].y=(float)floor(ascent-icons[first].Size.Y)*.5f;x+=icons[k].Size.X;}
        }
        for(int k=count-1;k>=0;--k)if(icons[k].Flags==2){expected[k].x=cursor-icons[k].Size.X;expected[k].y=(ascent-icons[k].Size.Y)*.5f;cursor=expected[k].x-2.0f;}
        clears=resizes=measures=heights=0;t.CIconText::InitIcons();
        check(wcscmp(t.DisplayText.GetAsCharArray(),output)==0);check(t.Size.X==width(output)&&t.Size.Y==ascent);
        check(measures==(inside?5:3)&&heights==inside+right&&clears==prior&&resizes==1&&t.IconHandles.size()==(unsigned)count);
        for(int k=0;k<count;++k)check(icons[k].Offset.X==expected[k].x&&icons[k].Offset.Y==expected[k].y);
        ++total;
    }

    // A later, distinct text index breaks the placement loop. Its zero width fits
    // within the original first-pass allocation even after the next group is visited.
    for(int swap=0;swap<2;++swap){
        CWideString in(L"Ab"); t.Text=in;t.PFont.Data=old_font;change_font=swap!=0;
        space_pixels=4;ascent=12;t.Icons._M_finish=icons+2;t.IconHandles._M_finish=first_handle;
        icons[0].Flags=icons[1].Flags=ICON_INSIDE_TEXT;icons[0].CharIndex=0;icons[1].CharIndex=1;
        icons[0].Size.X=4;icons[1].Size.X=0;icons[0].Size.Y=3;icons[1].Size.Y=7;
        clears=resizes=measures=heights=0;t.CIconText::InitIcons();
        check(wcscmp(t.DisplayText.GetAsCharArray(),L"  Ab")==0);
        check(icons[0].Offset.X==0 && icons[0].Offset.Y==4.5f && icons[1].Offset.X==11 && icons[1].Offset.Y==2.5f);
        check(heights==2 && measures==7 && clears==0 && resizes==1);++total;
    }
    drop(t.DisplayText);drop(t.Text);check(total==6140);
    if(errors){printf("errors %d cases %d\n",errors,total);return 1;}puts("005430f0_TEST PASS");return 0;
}