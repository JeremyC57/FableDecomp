struct CAIStateGroup_HoverHornetQueen
{
    void* vfptr;
    long m_wrapAmount;
    long ScriptAIPriority;
    long m_current;
    long m_total;

    long __fastcall GetNextCirclePoint(void*);
};

long __fastcall CAIStateGroup_HoverHornetQueen::GetNextCirclePoint(void*)
{
    long result = m_total - m_current;
    if (result < 0)
        result += m_wrapAmount;
    return result;
}