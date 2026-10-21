// CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(const CCharString& scriptName, const C3DVector& position, float radius, std::vector<CScriptThing>& things) const -- script: collect the AI creatures in the square of half-size radius around position (all of them when the script name is empty, else those with that script name) and append their script things; returns the output vector's size (0 without touching it when none match)
// Leaf: ?GetAllCreaturesInAreaWithScriptName@CGameScriptInterface@@UBEJABVCCharString@@ABVC3DVector@@MAAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x008A86C0, 452 bytes, `ret 10h`.
// Body: centre = C2DCoordI((long) position.X, (long) position.Y), r = (long) radius (_ftol 0x00BFEA70);
// box = C2DBoxI(centre, centre) grown by r on every side;
// scriptName == "" (inline CCharString::operator==(const char*), CBasicString<char>::operator== 0x004115A0) ->
// ThingSearchTools->GetAllThingsInRoughArea<CThingAICreature, vector, CThingFilter_None> (0x008AD710), else
// <.., CThingFilter_HasScriptName> (0x008AD950; filter = a CCharString copy 0x0099EC30 / ~ 0x0099EAE0); then as the
// sibling GetAllThingsWithScriptName: empty -> 0; reserve (0x008ACD30); push_back(creature->GetScriptThing()) per
// creature (0x004C7E50); return things.size().
// Layout: models of the landed sibling GetAllThingsWithScriptName 0x008A8570 and the de-bake 0x008A9C40 (CScriptThing,
// counted PImp, vector<CThingAICreature*> over malloc, World +0x04, ThingSearchTools +0x20); C2DBoxI {TopLeft, BottomRight}.
// Shape: write-only same-TU stand-ins for both search templates keep the found vector unescaped and pass the box and
// filter addresses on (escape, as 0x008A9C40); both filters live in the dead `position` argument slot; the CCharString
// compare is the landed 0x008EDA90 model. The box is built from the centre twice and expanded in place (an inline
// Expand(r): x0 / y0 into copies, x1 / y1 added in place, retail's order); explicit corner forms reorder. Probes v/ga.
// Opt: /O2 (16-aligned entry).
#include <string.h>
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct CBaseClass {                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CCPPointerInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThing : CBaseClass {                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
};
struct __false_type { __false_type() {} };
struct CScriptThingVector {                             // STLport vector<CScriptThing> (12-byte elements)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    void reserve(unsigned long n);                      // 0x008ACD30
    void _M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x008AD5F0
    void push_back(const CScriptThing& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CScriptThing(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CThing_Retail;
struct CThingVector {                                   // STLport vector<CThingAICreature*> (malloc allocator)
    CThing_Retail** _M_start;
    CThing_Retail** _M_finish;
    CThing_Retail** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;            // 0x004115A0
};
struct CCharStringData {                                // 17 bytes
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct C3DVector { float X, Y, Z; };
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct C2DBoxI {
    C2DCoordI TopLeft, BottomRight;
    C2DBoxI(const C2DCoordI& tl, const C2DCoordI& br) : TopLeft(tl), BottomRight(br) {}
    void Expand(long d) { TopLeft.X -= d; TopLeft.Y -= d; BottomRight.X += d; BottomRight.Y += d; }
};
struct CThingFilter_None {
    CThingFilter_None() {}
};
struct CThingFilter_HasScriptName {
    CCharString m_name;
    CThingFilter_HasScriptName(const CCharString& name) : m_name(name) {}
};
struct CThing_Retail {
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
CThing_Retail** FindThings_None(const C2DBoxI* box, const CThingFilter_None* filter, CThing_Retail** capacity);
CThing_Retail** FindThings_HasScriptName(const C2DBoxI* box, const CThingFilter_HasScriptName* filter, CThing_Retail** capacity);
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_None& filter, CThingVector& out);            // 0x008AD710
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_HasScriptName& filter, CThingVector& out);   // 0x008AD950
};
// Same-TU stand-ins (see the sibling): read the box and only write the output vector's words.
#pragma auto_inline(off)
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_None& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_None(&box, &filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_HasScriptName& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_HasScriptName(&box, &filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CGameScriptInterface {
    virtual long GetAllCreaturesInAreaWithScriptName(const CCharString& scriptName, const C3DVector& position, float radius, CScriptThingVector& things) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

long CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(const CCharString& scriptName, const C3DVector& position, float radius, CScriptThingVector& things) const
{
    C2DCoordI centre((long)position.X, (long)position.Y);
    long r = (long)radius;
    C2DBoxI box(centre, centre);
    box.Expand(r);
    CThingVector found;
    if (scriptName == "")
        World->ThingSearchTools->GetAllThingsInRoughArea(box, CThingFilter_None(), found);
    else
        World->ThingSearchTools->GetAllThingsInRoughArea(box, CThingFilter_HasScriptName(scriptName), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    for (CThing_Retail** it = found._M_start; it != found._M_finish; ++it) {
        CThing_Retail* creature = *it;
        things.push_back(creature->GetScriptThing());
    }
    return things.size();
}