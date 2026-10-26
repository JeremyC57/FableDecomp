// CCharString::operator!=(char character) const -- true unless the string is exactly the one-character text `character` (a null string is never equal)
// Leaf: ??9CCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099E520, length 127 bytes (ret 4). No calls: strcmp is the intrinsic.
// Layout: the 17-byte CCharStringData block (PData +0 = the characters).
// Shape: `!(*this == character)` over a `__forceinline` operator== written null-test-positive (`if (data) {...}
// return false;`, which places the null exit last and enters the strcmp loop with a jump over its pad); it builds the
// {character, 0} text in the argument's
// own slot (`mov [esp+0Ch],cl ; mov byte ptr [esp+0Dh],0`) and returns `strcmp(...) == 0 ? true : false` (`test eax,eax ;
// sete al`); the outer negation goes through a full register (`xor ecx,ecx ; test al,al ; sete cl ; mov al,cl`) and is
// tail-duplicated into the equal, unequal (`sbb eax,eax ; sbb eax,-1`) and null-string (`xor al,al`) exits.
#include <string.h>
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
class CCharStringData { public:                      // 17 bytes
    char*         PData;                      // +0x00
    unsigned long StringLength;               // +0x04
    unsigned long DataLength;                 // +0x08
    unsigned char Flags;                      // +0x0c
    long          NoRefs;                     // +0x0d
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    __forceinline bool operator==(char character) const
    {
        if (PStringData != 0) {
            char text[2];
            text[0] = character;
            text[1] = 0;
            return strcmp(PStringData->PData, text) == 0 ? true : false;
        }
        return false;
    }
    bool operator!=(char character) const;
};

bool CCharString::operator!=(char character) const
{
    return !(*this == character);
}