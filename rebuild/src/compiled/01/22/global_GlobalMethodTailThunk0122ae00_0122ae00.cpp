// The atexit destructor thunk of the static libpk CString at 0x0143BB3C (initializer 0x01224B60): ~CString 0x00C0BA40
// ??1CString@@QAE@XZ (name-fix NF38; was AutoTinyThunkTarget::Invoke_00c0ba40 on a stand-in object).
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "engine/CString.h"
extern CString g_CString_0143bb3c;                  // 0x0143BB3C
void __fastcall GlobalMethodTailThunk_0122ae00()
{
    g_CString_0143bb3c.CString::~CString();
}
