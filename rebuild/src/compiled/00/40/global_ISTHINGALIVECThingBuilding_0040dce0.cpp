// IS_THING_ALIVE<CThing*>(CThing* const& thing) -- the thing exists and its dead bit (+0x91 bit 0) is clear
// Leaf: ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040DCE0, 21 bytes (`mov eax,[ecx] ; test ; je ; test byte ptr [eax+91h],1 ; jne ; mov al,1 ; ret`).
// Identity: one of the 21 byte-identical IS_THING_ALIVE<T> instantiations retail keeps (same-named COMDATs merge, so each copy
//   is a distinct T); T = CThing*: the general instantiation: 121 retail call sites fed by GetPItem / Create / GetThingInPrimarySlot. FableWin.pdb names the instantiation ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z.
// Layout (retail, local model): CThing's dead bit is the low bit of the byte at +0x91 (engine/_quarantine/CThing.h CreatedBySerialisation
//   bitfield group); the class is modelled by its one byte because no engine header names the bit.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char DeadFlags;                       // +0x91 bit 0: dead
};
class CThingCreatureBase : public CThing { };
class CThingPlayerCreature : public CThingCreatureBase { };

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing != 0 && (thing->DeadFlags & 1) == 0)
        return true;
    return false;
}
template bool __fastcall IS_THING_ALIVE<CThing*>(CThing* const& thing);
