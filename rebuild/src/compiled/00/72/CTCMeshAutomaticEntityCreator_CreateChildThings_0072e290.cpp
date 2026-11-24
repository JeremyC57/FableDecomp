// CTCMeshAutomaticEntityCreator::CreateChildThings() -- walk the appearance's dummy-object names and, for each name that differs from the previous one, parse it: a CREATEOBJECT / CREATEBUILDING / CREATEPARTICLE prefix creates the named object, building or particle at that dummy
// Leaf: ?CreateChildThings@CTCMeshAutomaticEntityCreator@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072E290, 550 bytes, plain `ret`.
// Callees: CTCGraphicAppearance::GetAllDummyObjectNames @ 0x004BDB40, CCharString() @ 0x0099E4B0, CCharString(const char*,
//   long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CCharString::operator!= @ 0x0099E900, operator= @ 0x0099EFB0,
//   CStringParser(const CCharString&, callback, const CCharString&, long) @ 0x00404720, SkipPastNextItemIfIs @ 0x009B9E00,
//   ReadAsString @ 0x009BA6F0 (by value), CreateObject/CreateBuilding/CreateParticle @ 0x0072DDB0/0x0072DF50/0x0072E130,
//   ~CBaseClass @ 0x0099A2E0, free @ 0x00BFEA14 (comment-list nodes and the name vector's storage).
// Data: "" 0x0122D70E, "CREATEOBJECT" 0x01266A64, "CREATEBUILDING" 0x01266A54, "CREATEPARTICLE" 0x01266A44.
// Layout: CStringParser as the landed CScriptBase::DoScriptDataPersist 0x00CB7CB0 (retail 0x28 bytes, comment list +0x14);
//   CThing GraphicAppearance +0x64.
// Shape: the parser's destructor is fully inline (STLport list clear + sentinel free, then the two strings and the base);
//   the vector<CCharString> destructor is STLport's _Destroy(first, last) on by-value pointers (the end stays in edi)
//   followed by the storage free.
#include <stdlib.h>
#include "engine/CTCBase.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCommentNode {
    CCommentNode* _M_next;
    CCommentNode* _M_prev;
};
struct CStringParserCommentList {                       // STLport list<CStringParserCommentBoundaries>: sentinel only
    CCommentNode* _M_node;
    ~CStringParserCommentList()
    {
        CCommentNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            CCommentNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
};
class CStringParser : public CBaseClass { public:                     // retail 0x28 bytes
    char*                    String;                    // +0x04
    unsigned long            BaseLineOffset;            // +0x08
    CCharString              Filename;                  // +0x0c
    CCharString              TempCharString;            // +0x10
    CStringParserCommentList Comments;                  // +0x14
    void (*ErrorCallback)(CCharString*);                // +0x18
    long                     CursorPos[3];              // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename,
                  long baseLineOffset);                 // 0x00404720
    bool SkipPastNextItemIfIs(const CCharString& item); // 0x009B9E00
    CCharString ReadAsString();                         // 0x009BA6F0
};
// NF108 (2026-11-19, q4448): the local CCharStringVector stand-in replaced by the shared STLport std::vector<CCharString>
//   (stlport/fable_containers.h): GetAllDummyObjectNames 0x004BDB40 is ?GetAllDummyObjectNames@CTCGraphicAppearance@NGraphicAppearance@@
//   QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z (Ego_r.pdb 0x004C10E0 + FableWin.pdb 0x01D0B750, const).
#include "stlport/fable_containers.h"
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:   // a class in Ego_r.pdb (PAVCTCGraphicAppearance@NGraphicAppearance@@ in the PDB spellings; NF70 q10750)
    void GetAllDummyObjectNames(std::vector<CCharString>& names) const;   // 0x004BDB40
};
}
using NGraphicAppearance::CTCGraphicAppearance;

struct CTCMeshAutomaticEntityCreator : CTCBase {
private:   // Ego_r.pdb: CreateObject / CreateParticle are private (AAE); NF70 q10750
    void CreateObject(const CCharString& dummyName, const CCharString& objectName, CTCGraphicAppearance* appearance);     // 0x0072DDB0
public:    // CreateBuilding is not in the PDBs: the landed 0x0072DF50 spelling (QAE) is kept
    private:
    void CreateBuilding(const CCharString& dummyName, const CCharString& objectName, CTCGraphicAppearance* appearance);
    public:
   // 0x0072DF50
private:
    void CreateParticle(const CCharString& dummyName, const CCharString& objectName, CTCGraphicAppearance* appearance);   // 0x0072E130
public:
    private:
    void CreateChildThings();
    public:
};

void CTCMeshAutomaticEntityCreator::CreateChildThings()
{
    CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
    std::vector<CCharString> names;
    appearance->GetAllDummyObjectNames(names);
    CCharString lastName;
    for (std::vector<CCharString>::iterator it = names.begin(); it != names.end(); ++it) {
        if (lastName != *it) {
            CStringParser parser(*it, 0, CCharString(""), 0);
            if (parser.SkipPastNextItemIfIs("CREATEOBJECT")) {
                CCharString objectName = parser.ReadAsString();
                CreateObject(*it, objectName, appearance);
            } else if (parser.SkipPastNextItemIfIs("CREATEBUILDING")) {
                CCharString objectName = parser.ReadAsString();
                CreateBuilding(*it, objectName, appearance);
            } else if (parser.SkipPastNextItemIfIs("CREATEPARTICLE")) {
                CCharString objectName = parser.ReadAsString();
                CreateParticle(*it, objectName, appearance);
            }
        }
        lastName = *it;
    }
}