// CEnvironment::BuildThemes() -- build theme resources from the full lighting-table path.
// Retail006b3350 165bytes. String calls0099b6b0/e480/b8d0/b510/9110/b7d0/b720; lookup00885f50, palette00888a60, delete[]00bfeb1c.
#include <stddef.h>
#include <new>
#ifdef _MSC_VER
#include "engine/CEnvironment.h"
#include "fable_string.h"
#define CEnvironmentDef CEnvironmentDefDonor
#include "engine/_quarantine/CEnvironmentDef.h"
#undef CEnvironmentDef
#else
#define __fastcall
// Host declarations mirror fable_string.h while avoiding its mandatory 32-bit ABI assertions.
class CWideString { public:
 void* Data;
 CWideString(const wchar_t*); //0099b6b0
 CWideString(const CWideString&); //0099b720
 ~CWideString(); //0099b510
 const CWideString& operator+=(const CWideString&); //0099b8d0
 const CWideString& operator=(const CWideString&); //0099b7d0
};
class CCharString { public: void* Data; CWideString ToWideString() const; }; //0099e480
#endif
class CAFile { public: static CWideString __fastcall ConvertPathToFullPath(const CWideString&); }; //00999110 (LTCG result ecx,source edx)
// Donor CEnvironmentDef ColourLookupTexture is+40; native+3c reflects a four-byte base-layout difference.
class CEnvironmentDef { public: unsigned char beforeLookup[0x3c]; CCharString ColourLookupTexture; };
// Generated _quarantine/CGameDefinitionManager.h is unusable: sizeof1 contradicts its void* vptr. Native EnvironmentDef is audited+d8.
class CGameDefinitionManager { public: unsigned char beforeEnvironment[0xd8]; CEnvironmentDef* EnvironmentDef; };
// DDK <new> omits the ordinary array-delete declaration; declare it before the inline destructor.
void operator delete[](void*) throw();
namespace NEnvironmentInternal {
struct CEnvironmentLookupTexture {
 long Width,Height,Pitch; unsigned char* Data;
 CEnvironmentLookupTexture(CWideString); //00885f50
 ~CEnvironmentLookupTexture() { delete[] Data; }
};
class CEnvironmentThemeResourcePallette { public:
 void Build(const CGameDefinitionManager&,const CEnvironmentDef&,const CEnvironmentLookupTexture&); //00888a60
};
}
// Donor CEnvironment::ThemeSet is +1a7c, native +1954 due to smaller native theme game-state/STL layouts.
struct CEnvironmentNativeView {
 void* vtable; CGameDefinitionManager* DefinitionManager;
 unsigned char priorState[0x1954-8]; NEnvironmentInternal::CEnvironmentThemeResourcePallette ThemeSet;
 void BuildThemes();
};
void CEnvironmentNativeView::BuildThemes() {
 CEnvironmentDef* def=DefinitionManager->EnvironmentDef;
 CWideString path(L"data\\LightingTable\\");
 path+=def->ColourLookupTexture.ToWideString();
 path=CAFile::ConvertPathToFullPath(path);
 NEnvironmentInternal::CEnvironmentLookupTexture texture(path);
 ThemeSet.Build(*DefinitionManager,*def,texture);
}