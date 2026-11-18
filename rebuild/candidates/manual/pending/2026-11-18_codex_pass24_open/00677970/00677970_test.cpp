#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

typedef std::string CCharString;
enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0 };

class CStringParser {
    const CCharString source;
    unsigned pos;
public:
    explicit CStringParser(const char* text) : source(text), pos(0) { }
    void SkipWhitespace()
    {
        while (pos<source.size() && (source[pos]==' ' || source[pos]=='\t'))
            ++pos;
    }
    bool SkipPastNextItemIfIs(const char* text)
    {
        SkipWhitespace();
        const CCharString token(text);
        if (source.compare(pos, token.size(), token))
            return false;
        pos += static_cast<unsigned>(token.size());
        return true;
    }
    CCharString ReadAsSymbol()
    {
        SkipWhitespace();
        return pos<source.size() ? source.substr(pos++,1) : CCharString();
    }
    CCharString ReadAsStringUntilString(const char* token)
    {
        const CCharString::size_type start=pos;
        CCharString::size_type end=source.find(token,pos);
        if (end==CCharString::npos)
            end=source.size();
        pos=static_cast<unsigned>(end);
        return source.substr(start,end-start);
    }
    double Number()
    {
        SkipWhitespace();
        char* end=0;
        const char* start=source.c_str()+pos;
        const double value=std::strtod(start,&end);
        assert(end!=start);
        pos+=static_cast<unsigned>(end-start);
        return value;
    }
};

class CPersistContext {
public:
    unsigned errors;
    CPersistContext() : errors(0) { }
    void PersistError(CStringParser&, const CCharString& error)
    {
        assert(error=="Unknown command for TOpinionTransientOffsetList");
        ++errors;
    }
};

template <class T> class CPersistTraits {
public:
    void TransferIn(CPersistContext&, CStringParser& parser, T& val)
    {
        val = static_cast<T>(parser.Number());
    }
};

template <> void CPersistTraits<EOpinionReactionType>::TransferIn(
    CPersistContext&, CStringParser& parser, EOpinionReactionType& val)
{
    val = static_cast<EOpinionReactionType>(static_cast<int>(parser.Number()));
}

template <> void CPersistTraits<bool>::TransferIn(CPersistContext&, CStringParser& parser, bool& val)
{
    val = parser.Number() != 0;
}

class CMainGameComponent {
public:
    static long GetConstantFPS() { return 30; }
};

class CReactionFrequencyTraits {
public:
    int type;
    float a,b,c,d;
    bool repeats;
    explicit CReactionFrequencyTraits(int tag, float A, float B, float C, float D, bool flag)
        : type(tag),a(A),b(B),c(C),d(D),repeats(flag) { }
    virtual ~CReactionFrequencyTraits() { }
};
class CReactionFrequencyTraits_LinearTime : public CReactionFrequencyTraits {
public:
    CReactionFrequencyTraits_LinearTime(float a,float b)
        : CReactionFrequencyTraits(0,a,b,0,0,false) { }
};
class CReactionFrequencyTraits_PeriodicFrequency : public CReactionFrequencyTraits {
public:
    CReactionFrequencyTraits_PeriodicFrequency(float a,float b,float c,float d)
        : CReactionFrequencyTraits(1,a,b,c,d,false) { }
};
class CReactionFrequencyTraits_ControlledCount : public CReactionFrequencyTraits {
public:
    CReactionFrequencyTraits_ControlledCount(unsigned long a,float b,float c,bool repeats)
        : CReactionFrequencyTraits(2,float(a),b,c,0,repeats) { }
};
class CReactionFrequencyTraitsArray {
public:
    CReactionFrequencyTraits* LowHigh[2][79];
    CReactionFrequencyTraitsArray()
    {
        for (unsigned i=0;i<2;++i)
            for (unsigned j=0;j<79;++j)
                LowHigh[i][j]=0;
    }
    ~CReactionFrequencyTraitsArray()
    {
        for (unsigned i=0;i<2;++i)
            for (unsigned j=0;j<79;++j)
                delete LowHigh[i][j];
    }
};
template<> class CPersistTraits<CReactionFrequencyTraitsArray> {
public:
    void TransferIn(CPersistContext&, CStringParser&, CReactionFrequencyTraitsArray&);
};

// Selected TransferIn body.

// The selected implementation is reproduced verbatim for this standalone fixture.
void CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn(CPersistContext& persist_context, CStringParser& parser, CReactionFrequencyTraitsArray& val)
{
    if (parser.SkipPastNextItemIfIs(".Set")) {
        if (!(parser.ReadAsSymbol() == "("))
            return;
        EOpinionReactionType reaction;
        CPersistTraits<EOpinionReactionType> reaction_traits;
        reaction_traits.TransferIn(persist_context, parser, reaction);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        parser.SkipWhitespace();
        CCharString matchtype = parser.ReadAsStringUntilString("(");
        if (!(parser.ReadAsSymbol() == "("))
            return;
        if (matchtype == "Linear") {
            CPersistTraits<float> float_traits = CPersistTraits<float>();
            float min_wait;
            float max_wait;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, min_wait);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, max_wait);
            unsigned long constant_fps = CMainGameComponent::GetConstantFPS();
            val.LowHigh[0][reaction] = new CReactionFrequencyTraits_LinearTime(min_wait * constant_fps, max_wait * constant_fps);
            val.LowHigh[1][reaction] = new CReactionFrequencyTraits_LinearTime(min_wait * constant_fps * 0.2f, max_wait * constant_fps * 0.2f);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        } else if (matchtype == "PeriodicFrequency") {
            CPersistTraits<float> float_traits = CPersistTraits<float>();
            float ramp_up_time;
            float stay_high_time;
            float low_freq_per_min;
            float high_freq_per_min;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, ramp_up_time);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, stay_high_time);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, low_freq_per_min);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, high_freq_per_min);
            val.LowHigh[0][reaction] = new CReactionFrequencyTraits_PeriodicFrequency(ramp_up_time, stay_high_time, low_freq_per_min, high_freq_per_min);
            val.LowHigh[1][reaction] = new CReactionFrequencyTraits_PeriodicFrequency(ramp_up_time * 0.2f, stay_high_time * 0.2f, low_freq_per_min * 5.0f, high_freq_per_min * 2.0f);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        } else if (matchtype == "ControlledCount") {
            CPersistTraits<float> float_traits = CPersistTraits<float>();
            CPersistTraits<unsigned long> ulong_traits;
            unsigned long max_count;
            float min_gap_secs;
            float recharge_time_secs;
            parser.SkipWhitespace();
            ulong_traits.TransferIn(persist_context, parser, max_count);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, min_gap_secs);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, recharge_time_secs);
            CCharString next_str = parser.ReadAsSymbol();
            if (next_str == ")") {
                val.LowHigh[0][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count, min_gap_secs, recharge_time_secs, true);
                val.LowHigh[1][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count * 5, min_gap_secs * 0.2f, recharge_time_secs * 0.2f, true);
            } else if (next_str == ",") {
                CPersistTraits<bool> bool_traits;
                bool allow_individual_repeats;
                parser.SkipWhitespace();
                bool_traits.TransferIn(persist_context, parser, allow_individual_repeats);
                val.LowHigh[0][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count, min_gap_secs, recharge_time_secs, allow_individual_repeats);
                val.LowHigh[1][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count * 5, min_gap_secs * 0.2f, recharge_time_secs * 0.2f, allow_individual_repeats);
                if (!(parser.ReadAsSymbol() == ")"))
                    return;
            }
        }
        parser.SkipWhitespace();
        parser.ReadAsSymbol() == ")";
    } else {
        persist_context.PersistError(parser, "Unknown command for TOpinionTransientOffsetList");
    }
}


static bool Near(float a, float b) { return std::fabs(a-b)<0.0002f; }

int main()
{
    CPersistTraits<CReactionFrequencyTraitsArray> traits;
    CPersistContext context;
    for (unsigned i=0; i<4; ++i) {
        const char* inputs[4] = {
            ".Set(2,Linear(1.5, 2.0))", ".Set(2,PeriodicFrequency(3.,4.,5.,6.))",
            ".Set(2,ControlledCount(8, 9., 10.))", ".Set(2,ControlledCount(8, 9., 10., 0))"
        };
        CStringParser parser(inputs[i]);
        CReactionFrequencyTraitsArray arrays;
        traits.TransferIn(context, parser, arrays);
        const CReactionFrequencyTraits* low=arrays.LowHigh[0][2];
        const CReactionFrequencyTraits* high=arrays.LowHigh[1][2];
        assert(low && high && low!=high && !context.errors);
        assert(low->type==high->type && low->type==int(i<2 ? i : 2));
        if (i==0) {
            assert(Near(low->a,45) && Near(low->b,60) && Near(high->a,9) && Near(high->b,12));
        } else if (i==1) {
            assert(Near(low->a,3) && Near(low->b,4) && Near(low->c,5) && Near(low->d,6));
            assert(Near(high->a,0.6f) && Near(high->b,0.8f) && Near(high->c,25) && Near(high->d,12));
        } else {
            assert(Near(low->a,8) && Near(low->b,9) && Near(low->c,10));
            assert(Near(high->a,40) && Near(high->b,1.8f) && Near(high->c,2));
            assert(low->repeats==(i==2) && high->repeats==(i==2));
        }
        assert(!arrays.LowHigh[0][0] && !arrays.LowHigh[1][3]);
    }
    const char* invalid[] = {
        ".Wrong(2,Linear(1.5,2))", ".Set{2,Linear(1.5,2))", ".Set(2;Linear(1.5,2))",
        ".Set(2,Unknown(1))", ".Set(2,Linear(1.5;2))", ".Set(2,PeriodicFrequency(1;2,3,4))",
        ".Set(2,PeriodicFrequency(1,2;3,4))", ".Set(2,PeriodicFrequency(1,2,3;4))",
        ".Set(2,ControlledCount(1;2,3))", ".Set(2,ControlledCount(1,2;3))"
    };
    for (unsigned i=0;i<sizeof(invalid)/sizeof(invalid[0]);++i) {
        CReactionFrequencyTraitsArray values;
        CStringParser parser(invalid[i]);
        traits.TransferIn(context,parser,values);
        assert(!values.LowHigh[0][2] && !values.LowHigh[1][2]);
    }
    assert(context.errors==1);
    puts("00677970_TEST PASS");
    return 0;
}
