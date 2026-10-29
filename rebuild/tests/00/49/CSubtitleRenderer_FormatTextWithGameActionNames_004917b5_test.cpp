#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
// UNGRADED two-source naming/model proposal; see naming_audit/4917B5_PREFLIGHT.md.
// CSubtitleRenderer::FormatTextWithGameActionNames(const CWideString&, std::vector<CWideString>*) -- substitute bracketed game-action tags and optionally collect highlighted action names
// Leaf: ?FormatTextWithGameActionNames@CSubtitleRenderer@@SI?AVCWideString@@ABV2@PAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z
// Flags: /Zc:wchar_t
// Retail: 0x004917B5, 797 bytes. PDB static __fastcall signature, GetActionNameFromEmbeddedTag neighbour 0x0049124E.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();
    CWideString(const CWideString&);
    CWideString(const wchar_t*);
    ~CWideString();
    const CWideString& operator=(const CWideString&);
    bool operator!=(const wchar_t*) const;
    long FindCharPos(wchar_t) const;
    long GetLength() const;
    CWideString GetSubString(unsigned long,unsigned long) const;
    void Replace(const CWideString&, const CWideString&);
};
CWideString __fastcall operator+(const CWideString&,const CWideString&);
CWideString __fastcall operator+(const CWideString&,const wchar_t*);
CWideString __fastcall operator+(const wchar_t*,const CWideString&);
namespace std {
template<class T> class allocator {};
template<class Value,class T,class A> class _STLP_alloc_proxy : public A { public: Value _M_data; };
template<class T,class A> class _Vector_base {
protected:
    T* _M_start; T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> > class vector : public _Vector_base<T,A> {
public:
    void push_back(const T&);
};
}
class CBaseClass {public:CBaseClass();virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass {protected:CBaseClassNonCopyable();~CBaseClassNonCopyable();};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;};
class CRGBColour {public:unsigned char B,G,R,A;};
#define CFadeInFadeOutBase CFadeInFadeOutBaseLayout
#include "engine/CFadeInFadeOutBase.h"
#undef CFadeInFadeOutBase
class CFadeInFadeOutBase {public:bool Active;float FadeInTime,FadeOutTime;bool Closing,Opening;float OpenTimer,CloseTimer;CRGBColour ToColour;};
FABLE_STATIC_ASSERT(sizeof(CFadeInFadeOutBase)==sizeof(CFadeInFadeOutBaseLayout));
class CEnginePrimitiveHandle;
enum EPlayerMode {};
namespace std {
template<class T>struct _Nonconst_traits {};
template<class T>struct _Deque_iterator_base {T* _M_cur;T* _M_first;T* _M_last;T** _M_node;};
template<class T,class Traits>struct _Deque_iterator:public _Deque_iterator_base<T> {};
template<class T,class A>class _Deque_base {protected:_Deque_iterator<T,_Nonconst_traits<T> > _M_start,_M_finish;_STLP_alloc_proxy<T**,T*,allocator<T*> > _M_map;_STLP_alloc_proxy<unsigned int,T,A> _M_map_size;};
template<class T,class A=allocator<T> >class deque:protected _Deque_base<T,A> {};
template<class T,class Sequence=deque<T> >class queue {protected:Sequence c;};
}
class CSubtitleRenderer : public CBaseClassNonCopyable {
public:
    class CSetTextInfo;class CEmbeddedGraphic;class CEmbeddedHighlight;
    C2DBoxF VWindow,VTelevisionWindow;
    long TextCharactersShowing;bool TextGroupHasEntriesRemaining;long TextGroupIndex;
    CWideString TextGroupTag;
    std::queue<CSetTextInfo,std::deque<CSetTextInfo,std::allocator<CSetTextInfo> > > TextInfoQueue;
    CWideString Text,NameText,Button1Text,Button2Text,Button3Text,Button1OverrideText,Button2OverrideText;
    unsigned char Button1Alpha,Button2Alpha;unsigned long DisplayFlags;
    CFontBank* PFont;CGraphicDataBank* Graphics;float MaxDisplayTime,TextTimer;EPlayerMode PlayerModeAdded;
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles,ButtonPrimitiveHandles,EmbeddedPrimitiveHandles;
    std::vector<CEmbeddedGraphic> EmbeddedGraphics;std::vector<CEmbeddedHighlight> EmbeddedHighlights;std::vector<CWideString> EmbeddedHighlightActionNames;
    bool PausedGame,AllCharactersDisplayed,InCutscene,CutsceneSkipped,CutsceneSkippable,CutsceneSkippableWhilePaused,CutsceneActionMode,Button1TextOverridden,Button2TextOverridden,DisplayingText,UpdateWhilePaused,UpdateTimer;
    CFadeInFadeOutBase SkipCutsceneTimer,ButtonPulseTimer;
    static CWideString __fastcall FormatTextWithGameActionNames(const CWideString&, std::vector<CWideString>*);
private:
    static bool __fastcall GetActionNameFromEmbeddedTag(const CWideString&, CWideString&);
};
FABLE_STATIC_ASSERT(sizeof(CSubtitleRenderer)==sizeof(CSubtitleRendererLayout));
CWideString __fastcall CSubtitleRenderer::FormatTextWithGameActionNames(const CWideString& original, std::vector<CWideString>* highlights)
{
    CWideString text(original);
    bool replaceBraces = false;
    long start = text.FindCharPos(L'[');
    while (start != -1) {
        long end = text.FindCharPos(L']');
        if (end == -1)
            break;
        {
        CWideString tag = text.GetSubString(start + 1, end);
        if (tag != L"") {
            CWideString prefix;
            if (start > 0)
                prefix = text.GetSubString(0, start);
            CWideString suffix;
            if (end < text.GetLength())
                suffix = text.GetSubString(end + 1, text.GetLength());
            CWideString actionName;
            if (GetActionNameFromEmbeddedTag(tag, actionName)) {
                if (highlights) {
                    text = prefix + L"'" + actionName + L"'" + suffix;
                    highlights->push_back(L"'" + actionName + L"'");
                }
                else
                    text = prefix + L"<" + actionName + L">" + suffix;
            }
            else {
                text = prefix + L"{" + tag + L"}" + suffix;
                replaceBraces = true;
            }
        }
        else
            break;
        }
        start = text.FindCharPos(L'[');
    }
    if (replaceBraces) {
        { CWideString replacement(L"["); CWideString find(L"{"); text.Replace(find, replacement); }
        { CWideString replacement(L"]"); CWideString find(L"}"); text.Replace(find, replacement); }
    }
    return text;
}

class TestWideData { public: wchar_t text[2048]; };
static const wchar_t* chars(const CWideString& s) { return ((TestWideData*)s.PStringData)->text; }
static int equal(const CWideString& s,const wchar_t* x) { return wcscmp(chars(s),x)==0; }
CWideString::CWideString() { PStringData=reinterpret_cast<CWideStringData*>(new TestWideData); ((TestWideData*)PStringData)->text[0]=0; }
CWideString::CWideString(const CWideString& s) { PStringData=reinterpret_cast<CWideStringData*>(new TestWideData); wcscpy(((TestWideData*)PStringData)->text,chars(s)); }
CWideString::CWideString(const wchar_t* s) { PStringData=reinterpret_cast<CWideStringData*>(new TestWideData); wcscpy(((TestWideData*)PStringData)->text,s); }
CWideString::~CWideString() { delete (TestWideData*)PStringData; }
const CWideString& CWideString::operator=(const CWideString& s) { if(this!=&s) wcscpy(((TestWideData*)PStringData)->text,chars(s)); return *this; }
bool CWideString::operator!=(const wchar_t* s) const { return !equal(*this,s); }
long CWideString::FindCharPos(wchar_t c) const { const wchar_t* p=wcschr(chars(*this),c); return p ? (long)(p-chars(*this)) : -1; }
long CWideString::GetLength() const { return (long)wcslen(chars(*this)); }
CWideString CWideString::GetSubString(unsigned long first,unsigned long last) const {
 wchar_t b[2048];unsigned long n=(unsigned long)wcslen(chars(*this));
 if(first>n)first=n;if(last>n)last=n;if(last<first)last=first;
 unsigned long i;for(i=first;i<last;++i)b[i-first]=chars(*this)[i];b[last-first]=0;return CWideString(b);
}
void CWideString::Replace(const CWideString& find,const CWideString& replacement) {
 wchar_t result[2048];const wchar_t* p=chars(*this);unsigned int n=0;
 const unsigned int k=(unsigned int)wcslen(chars(find));
 while(*p) {if(k && wcsncmp(p,chars(find),k)==0){const wchar_t* q=chars(replacement);while(*q)result[n++]=*q++;p+=k;}else result[n++]=*p++;}
 result[n]=0;wcscpy(((TestWideData*)PStringData)->text,result);
}
CWideString __fastcall operator+(const CWideString& a,const CWideString& b) { wchar_t t[2048];wcscpy(t,chars(a));wcscat(t,chars(b));return CWideString(t); }
CWideString __fastcall operator+(const CWideString& a,const wchar_t* b) { wchar_t t[2048];wcscpy(t,chars(a));wcscat(t,b);return CWideString(t); }
CWideString __fastcall operator+(const wchar_t* a,const CWideString& b) { wchar_t t[2048];wcscpy(t,a);wcscat(t,chars(b));return CWideString(t); }
static int lookupCalls;
bool __fastcall CSubtitleRenderer::GetActionNameFromEmbeddedTag(const CWideString& tag,CWideString& action) {
 ++lookupCalls;if(equal(tag,L"JUMP")){action=CWideString(L"Space");return true;}
 if(equal(tag,L"USE")){action=CWideString(L"E");return true;}return false;
}
namespace std {
template<class T,class A> void vector<T,A>::push_back(const T& s) {
 if(this->_M_finish==this->_M_end_of_storage._M_data) {puts("unexpected capacity exhaustion");exit(91);}*this->_M_finish=s;++this->_M_finish;
}
}
class TestVector : public std::vector<CWideString> {
public:
 TestVector(CWideString* first,unsigned int count) {this->_M_start=first;this->_M_finish=first;this->_M_end_of_storage._M_data=first+count;}
 unsigned int count() const {return (unsigned int)(this->_M_finish-this->_M_start);}
};
static int check(const wchar_t* input,const wchar_t* expected,std::vector<CWideString>* v,int expectedLookups) {
 lookupCalls=0;CWideString result=CSubtitleRenderer::FormatTextWithGameActionNames(CWideString(input),v);
 if(!equal(result,expected)||lookupCalls!=expectedLookups) return 1;return 0;
}
int main() {
 int failed=0;
 failed+=check(L"plain",L"plain",0,0);
 failed+=check(L"Press [JUMP] now",L"Press <Space> now",0,1);
 failed+=check(L"[JUMP] then [USE]",L"<Space> then <E>",0,2);
 failed+=check(L"[UNKNOWN]",L"[UNKNOWN]",0,1);
 failed+=check(L"[JUMP] and [UNKNOWN]",L"<Space> and [UNKNOWN]",0,2);
 failed+=check(L"before [JUMP",L"before [JUMP",0,0);
 failed+=check(L"[] [USE]",L"[] [USE]",0,0);
 CWideString buffer[8];TestVector names(buffer,8);
 failed+=check(L"[JUMP] then [USE]",L"'Space' then 'E'",&names,2);
 if(names.count()!=2||!equal(buffer[0],L"'Space'")||!equal(buffer[1],L"'E'"))++failed;
 if(failed){printf("004917b5_TEST FAIL %d\n",failed);return 1;}
 puts("004917b5_TEST PASS");return 0;
}