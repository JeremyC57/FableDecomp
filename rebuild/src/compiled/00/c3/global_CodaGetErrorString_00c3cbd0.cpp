// CodaGetErrorString(long) -- map a GameCODA C_RESULT code (C_OK = 0, C_ERR_* = -1 .. -26) to its name string
// Leaf: ?CodaGetErrorString@@YAPBDJ@Z
// Retail 0x00C3CBD0, 304 bytes (body 0xC0 + nop + 27-entry jump table, one function; true end 0x00C3CCFC).
// Descriptive name: no PDB / RTTI for the CODA block. Evidence: (1) every case returns the address of a literal "C_OK" /
// "C_ERR_<NAME>" / "UNRECOGNIZED" string in .rdata 0x012B5878..0x012B5A78 (the SDK's error enum spelled as text);
// (2) its only caller 0x00C33440 is a one-argument __stdcall forwarder (push arg ; call ; add esp,4 ; ret 4) that returns
// the pointer -- the public "error code to string" entry point.
// Shape: `add eax,0x1a ; cmp eax,0x1a ; ja default ; jmp [table]` = a dense switch over -26..0, every arm `mov eax,str ; ret`.
// The case/string pairing below was checked by hand against the retail jump table (index i -> code i-26).

const char* CodaGetErrorString(long result)
{
    switch (result)
    {
    case 0:   return "C_OK";
    case -1:  return "C_ERR_CODANOTCREATED";
    case -2:  return "C_ERR_CODACREATED";
    case -3:  return "C_ERR_INVALIDPARAM";
    case -4:  return "C_ERR_INVALIDCALL";
    case -5:  return "C_ERR_OUTOFMEMORY";
    case -6:  return "C_ERR_ALLOCATED";
    case -7:  return "C_ERR_NOTALLOCATED";
    case -8:  return "C_ERR_PLATFORM";
    case -9:  return "C_ERR_NORESOURCES";
    case -10: return "C_ERR_CREATED";
    case -11: return "C_ERR_NOTCREATED";
    case -12: return "C_ERR_NOTREADY";
    case -13: return "C_ERR_FILENOTFOUND";
    case -14: return "C_ERR_BUFFERTYPE";
    case -15: return "C_ERR_QUEUEFULL";
    case -16: return "C_ERR_CAPS";
    case -17: return "C_ERR_PLAYING";
    case -18: return "C_ERR_EXPIRED";
    case -19: return "C_ERR_REMOTELOAD";
    case -20: return "C_ERR_REMOTEBIND";
    case -21: return "C_ERR_REMOTECOMMS";
    case -22: return "C_ERR_REMOTEVERSION";
    case -23: return "C_ERR_OUTOFREMOTEMEMORY";
    case -24: return "C_ERR_CPUNOTSUPPORTED";
    case -25: return "C_ERR_INTERRUPT";
    case -26: return "C_ERR_FASTLOADTHREAD";
    }
    return "UNRECOGNIZED";
}