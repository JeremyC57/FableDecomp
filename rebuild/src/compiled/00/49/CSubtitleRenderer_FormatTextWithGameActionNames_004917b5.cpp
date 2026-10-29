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