// Behaviour test for 0x00C3CBD0 CodaGetErrorString: every C_RESULT code maps to its own name, out-of-range to UNRECOGNIZED.
#include <cstdio>
#include <cstring>

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

int main()
{
    static const char* const names[27] = {
        "C_OK", "C_ERR_CODANOTCREATED", "C_ERR_CODACREATED", "C_ERR_INVALIDPARAM", "C_ERR_INVALIDCALL",
        "C_ERR_OUTOFMEMORY", "C_ERR_ALLOCATED", "C_ERR_NOTALLOCATED", "C_ERR_PLATFORM", "C_ERR_NORESOURCES",
        "C_ERR_CREATED", "C_ERR_NOTCREATED", "C_ERR_NOTREADY", "C_ERR_FILENOTFOUND", "C_ERR_BUFFERTYPE",
        "C_ERR_QUEUEFULL", "C_ERR_CAPS", "C_ERR_PLAYING", "C_ERR_EXPIRED", "C_ERR_REMOTELOAD",
        "C_ERR_REMOTEBIND", "C_ERR_REMOTECOMMS", "C_ERR_REMOTEVERSION", "C_ERR_OUTOFREMOTEMEMORY",
        "C_ERR_CPUNOTSUPPORTED", "C_ERR_INTERRUPT", "C_ERR_FASTLOADTHREAD" };
    for (int i = 0; i < 27; ++i)
    {
        if (std::strcmp(CodaGetErrorString(-i), names[i]) != 0) { std::printf("00c3cbd0_TEST FAIL\n"); return 1; }
    }
    if (std::strcmp(CodaGetErrorString(1), "UNRECOGNIZED") != 0 || std::strcmp(CodaGetErrorString(-27), "UNRECOGNIZED") != 0
        || std::strcmp(CodaGetErrorString(-1000), "UNRECOGNIZED") != 0) { std::printf("00c3cbd0_TEST FAIL\n"); return 1; }
    std::printf("00c3cbd0_TEST PASS\n");
    return 0;
}