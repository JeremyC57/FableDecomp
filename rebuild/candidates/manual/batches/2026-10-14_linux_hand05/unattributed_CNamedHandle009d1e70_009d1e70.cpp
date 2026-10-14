// CNamedHandle_009d1e70::CNamedHandle_009d1e70(CCharString name, void* object) -- two-argument constructor: reads the name's C string (result unused), keeps the object pointer, destroys the by-value name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D1E70, length 33 bytes.
// Callees: CCharString::operator const char*() @ 0x0099E4C0 (landed), CCharString::~CCharString
// @ 0x0099EAE0 (landed-trusted). `ret 8`: the first argument is a BY-VALUE CCharString (one
// pointer wide; its address is taken with `lea ecx,[esp+8]` for the two member calls, which a
// reference would not need), the second an opaque pointer stored at +0. The class has no vtable
// and nothing else is touched, so it cannot be named from the body; the manifest label
// "CAIStateGroupBase::CAIStateGroupBase" is propagated and rejected. The C-string result is
// dropped (eax is overwritten by the argument load) -- a stripped debug use of the name; the
// out-of-line conversion call survives. The by-value parameter's destructor runs before `this`
// is returned. One of two identical bodies (0x0099AE50).
#include "fable_string.h"

struct CNamedHandle_009d1e70 {
    void* m_pObject;                        // +0x00
    CNamedHandle_009d1e70(CCharString name, void* object);
};

CNamedHandle_009d1e70::CNamedHandle_009d1e70(CCharString name, void* object)
{
    const char* text = name;                // stripped debug use: the conversion call is kept, the value dropped
    m_pObject = object;
    (void)text;
}
