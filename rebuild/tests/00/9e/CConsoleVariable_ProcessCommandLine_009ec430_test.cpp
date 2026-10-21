// Behaviour test for CConsoleVariable::ProcessCommandLine (0x009EC430): a bare variable prints "<name> = <value>" and
// succeeds; a value is read into the target; a failed read reports the error and "type of <name>: <type>".
#include <cstdio>
#include <string>

struct CConsole {
    std::string Log;
    void SendMessageToUser(const std::string& m) { Log += m; Log += "\n"; }
};
struct CStringParser {
    std::string Rest;
    CStringParser(const std::string& line) {
        std::string::size_type sp = line.find(' ');
        Rest = sp == std::string::npos ? std::string() : line.substr(sp + 1);
    }
    bool NextItemExists() const { return !Rest.empty(); }
};
struct CConsoleTarget {
    long Type; std::string Value;
    void PrintToString(std::string& out) const { out = Value; }
    bool ReadFromParser(CStringParser& p) {
        if (Type == 1 && p.Rest.find_first_not_of("-0123456789") != std::string::npos) return false;
        Value = p.Rest; return true;
    }
};
struct CConsoleVariable {
    std::string Name; CConsoleTarget Target;
    bool ProcessCommandLine(const std::string& line, CConsole& console);
};
bool CConsoleVariable::ProcessCommandLine(const std::string& line, CConsole& console)
{
    CStringParser parser(line);
    bool result;
    if (!parser.NextItemExists()) {
        std::string value;
        Target.PrintToString(value);
        console.SendMessageToUser(Name + " = " + value);
        result = true;
    } else {
        result = Target.ReadFromParser(parser);
        if (!result) {
            console.SendMessageToUser("error - could not read target for variable");
            std::string message = "type of " + Name + ": ";
            if (Target.Type == 4) message += "bool";
            else if (Target.Type == 3) message += "float";
            else if (Target.Type == 1) message += "slong";
            else if (Target.Type == 5) message += "string";
            console.SendMessageToUser(message);
        }
    }
    return result;
}

int main()
{
    int ok = 1;
    CConsole console;
    CConsoleVariable v; v.Name = "fov"; v.Target.Type = 1; v.Target.Value = "60";
    if (!v.ProcessCommandLine("fov", console) || console.Log != "fov = 60\n") ok = 0;
    console.Log = "";
    if (!v.ProcessCommandLine("fov 75", console) || v.Target.Value != "75" || !console.Log.empty()) ok = 0;
    if (v.ProcessCommandLine("fov abc", console) || v.Target.Value != "75"
        || console.Log != "error - could not read target for variable\ntype of fov: slong\n") ok = 0;
    std::printf("009ec430_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}