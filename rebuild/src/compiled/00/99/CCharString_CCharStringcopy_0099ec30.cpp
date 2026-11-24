extern "C" long g_CCharString_liveCount;

class CCharString
{ public:
    void* data;

    protected: void UnassignString(); public:   // 0x0099E9B0 (Ego_r 0x00AFA2E0 / FableWin 0x02F624F0, protected; was ReleaseData, NF218 q12377)

    CCharString(const CCharString& rhs);
};

long g_CCharString_liveCount = 0;

CCharString::CCharString(const CCharString& rhs)
{
    data = 0;
    ++g_CCharString_liveCount;
    if (&rhs != this)
    {
        if (data != 0)
        {
            UnassignString();
        }
        void* rhsData = rhs.data;
        if (rhsData != 0)
        {
            data = rhsData;
            ++*(long*)((char*)rhsData + 0xd);
        }
    }
}