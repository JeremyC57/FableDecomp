// Behaviour test for 0x00998A30 CAFile::IsFilePathSyntaxValid: the drive-label / separator / component state machine
// accepts "C:\dir\file" as absolute and "dir\file" as relative, and rejects a wrong opening item, a doubled or trailing
// separator, an empty path and a path longer than 260 characters.
#include <cstdio>
#include <string>

struct Parser { std::string Text; unsigned int Pos; };
struct Expected { bool ComponentOK, SeparatorOK, DriveLabelOK, FinishedOK; };
static void Set(Expected& e, bool c, bool s, bool d, bool f) { e.ComponentOK = c; e.SeparatorOK = s; e.DriveLabelOK = d; e.FinishedOK = f; }

static bool IsSep(char c) { return c == '\\' || c == '/'; }
static bool ParserIsSeparatorNext(const Parser& p) { return p.Pos < p.Text.size() && IsSep(p.Text[p.Pos]); }
static bool ParserReadPastSeparator(Parser& p) { if (!ParserIsSeparatorNext(p)) return false; ++p.Pos; return true; }
static bool ParserReadPastComponent(Parser& p, std::string& component)
{
    if (ParserIsSeparatorNext(p) || p.Pos >= p.Text.size()) return false;
    unsigned int start = p.Pos;
    while (p.Pos < p.Text.size() && !IsSep(p.Text[p.Pos])) ++p.Pos;
    component = p.Text.substr(start, p.Pos - start);
    return component.find(':') == std::string::npos;          // ':' is a forbidden component symbol
}
static bool ParserReadPastDriveLabel(Parser& p)
{
    unsigned int at = p.Pos;
    while (at < p.Text.size() && p.Text[at] != ':' && !IsSep(p.Text[at])) ++at;
    if (at == p.Pos || at >= p.Text.size() || p.Text[at] != ':') return false;
    p.Pos = at + 1;
    return true;
}
static bool ParserIsComponentNext(Parser& p) { unsigned int pos = p.Pos; std::string c; bool r = ParserReadPastComponent(p, c); p.Pos = pos; return r; }
static bool ParserIsDriveLabelNext(Parser& p) { unsigned int pos = p.Pos; bool r = ParserReadPastDriveLabel(p); p.Pos = pos; return r; }
static bool ParserIsFinished(const Parser& p) { return p.Pos >= p.Text.size(); }

static bool IsFilePathSyntaxValid(const std::string& path, bool absolute)
{
    if (path.size() > 260)
        return false;
    Parser parser; parser.Text = path; parser.Pos = 0;
    Expected expected;
    if (absolute) Set(expected, false, false, true, false);
    else Set(expected, true, false, false, false);
    bool ok = true, finished = false;
    while (ok && !finished) {
        if (ParserIsSeparatorNext(parser)) {
            ok = expected.SeparatorOK; Set(expected, true, false, false, false); ParserReadPastSeparator(parser);
        } else if (ParserIsComponentNext(parser)) {
            ok = expected.ComponentOK; Set(expected, false, true, false, true);
            std::string component; ParserReadPastComponent(parser, component);
        } else if (ParserIsDriveLabelNext(parser)) {
            ok = expected.DriveLabelOK; Set(expected, false, true, false, false); ParserReadPastDriveLabel(parser);
        } else if (ParserIsFinished(parser)) {
            ok = expected.FinishedOK; finished = true;
        } else
            ok = false;
    }
    return ok && finished;
}

int main()
{
    int ok = 1;
    ok &= IsFilePathSyntaxValid("C:\\data\\file.txt", true);
    ok &= IsFilePathSyntaxValid("data\\levels/file.txt", false);
    ok &= IsFilePathSyntaxValid("file", false);
    ok &= !IsFilePathSyntaxValid("C:\\data", false);         // a relative path may not open with a drive
    ok &= !IsFilePathSyntaxValid("data\\file", true);        // an absolute one must
    ok &= !IsFilePathSyntaxValid("data\\", false);           // may not end on a separator
    ok &= !IsFilePathSyntaxValid("data\\\\file", false);     // no doubled separator
    ok &= !IsFilePathSyntaxValid("C:", true);                // a bare drive is not a file path
    ok &= !IsFilePathSyntaxValid("", false);                 // nothing to finish on
    ok &= !IsFilePathSyntaxValid(std::string(261, 'a'), false);
    ok &= IsFilePathSyntaxValid(std::string(260, 'a'), false);
    std::printf("00998a30_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}