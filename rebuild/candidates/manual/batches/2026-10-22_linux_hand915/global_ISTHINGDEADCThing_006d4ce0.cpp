// _global::IS_THING_DEAD<CThing*>(CThing* const&) -- true when the thing pointer is null or the thing's dead flag is set
// Leaf: IS_THING_DEAD
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D4CE0, 21 bytes, `ret`; the reference arrives in ecx (__fastcall, as the
//   landed IS_THING_ALIVE<T*> 0x0040DCE0 and the landed IS_THING_DEAD twins 0x005CBDBD / 0x0065D639).
// Layout: CThing's dead flag is bit 0 of +0x91 in retail (the landed IS_THING_DEAD bodies model it the same way;
//   the PDB CThing layout does not match retail there), so CThing is modelled locally.
// Shape: `test eax,eax ; je true ; test byte [eax+91h],1 ; jne true ; xor al,al ; ret ; true: mov al,1 ; ret`.
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags;                       // +0x91 bit 0: dead
};

template <class T>
bool __fastcall IS_THING_DEAD(T const& thing)
{
    if (thing != 0 && !(thing->m_flags & 1))
        return false;
    return true;
}

template bool __fastcall IS_THING_DEAD<CThing*>(CThing* const& thing);
