// CKeyRedefiner::CanRedefineAction(long) -- classify category IDs through retail's dense switch table (0x00557960, 175 bytes).
// Grade: RELOCATION_MATCH -- q.py verified the exact 175-byte body with embedded switch table.
// Owner established by the CKeyRedefiner::ProcessEvent caller at 0x00557f51; method name is descriptive.
// No shared CKeyRedefiner header exists; this helper does not access instance fields.
struct CKeyRedefiner { bool CanRedefineAction(long category); };
#ifndef KEY_REDEFINER_FLAG_ADDRESS
#define KEY_REDEFINER_FLAG_ADDRESS 0x013B864F
#endif
bool CKeyRedefiner::CanRedefineAction(long category)
{
    switch (category) {
    case 1: case 89: case 90: case 91: case 98: case 118: case 119:
        return false;
    case 58:
        if (*(unsigned char*)KEY_REDEFINER_FLAG_ADDRESS == 0) break;
        return false;
    default:
        break;
    }
    return true;
}