// 009ebdd0 CConsoleCommandBase::ProcessCommandLine
// Flags: none
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
// Leaf: ?ProcessCommandLine@CConsoleCommandBase@@MAE_NABVCCharString@@AAVCConsole@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_GETCHAR
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CGENERICVAR_PASS27
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CPARSEDITEM_SYMBOL
#define FABLE_RETAIL_PROCESS_COMMAND_LINE_PASS29
#include "engine/CConsoleCommandBase.h"
#include "engine/CStringParser.h"
CCharString FABLE_FASTCALL GFIntToCharString(long);
namespace {extern CCharString OPEN_BRACKET,CLOSE_BRACKET,SEPARATOR;}
bool CConsoleCommandBase::ProcessCommandLine(const CCharString& cmd, CConsole& user)
{
    CStringParser parser(cmd);
    CParsedItem parsed_item;
    std::vector<CGenericVar> function_parameters;
    bool next_item_exists = false;
    bool help_required = false;
    parser.ReadAsStringUntilString(OPEN_BRACKET, parsed_item);
    next_item_exists = parser.ReadNextItem(parsed_item);
    if (!next_item_exists || !parsed_item.IsSymbol() || parsed_item.GetSymbol() != OPEN_BRACKET.GetChar(0)) {
        user.SendMessageToUser("command requires leading bracket");
        help_required = true;
    }
    long param_count = 0;
    parser.PeekNextItem(parsed_item);
    while (param_count < ParamTypes.GetSize() && !help_required) {
        CGenericVar next_param = ParamTypes.GetCorrectlyTypedParameter(param_count);
        if (next_param.ReadFromParser(parser)) {
            function_parameters.push_back(next_param);

        } else {
            user.SendMessageToUser("parameter " + GFIntToCharString(param_count) + " is of incorrect type");
            help_required = true;
            break;
        }
            next_item_exists = parser.ReadNextItem(parsed_item);
            ++param_count;
            if (param_count < ParamTypes.GetSize() && (!next_item_exists || !parsed_item.IsSymbol() || parsed_item.GetSymbol() != SEPARATOR)) {
                user.SendMessageToUser(CCharString("expected ") + SEPARATOR + " after parameter " + GFIntToCharString(param_count));
                help_required = true;
                break;
            }
    }
    if (param_count == 0)
        next_item_exists = parser.ReadNextItem(parsed_item);
    if (param_count == ParamTypes.GetSize() && !help_required && next_item_exists) {
        if (!parsed_item.IsSymbol()) {
            user.SendMessageToUser("cannot interpret " + parsed_item.ConvertToString());
            help_required = true;
        } else if (parsed_item.GetSymbol() != CLOSE_BRACKET.GetChar(0)) {
            user.SendMessageToUser(CCharString("expected ") + CLOSE_BRACKET + " instead of " + parsed_item.GetSymbol());
            help_required = true;
        } else if (parser.ReadNextItem(parsed_item)) {
            user.SendMessageToUser("ignoring stuff after closing bracket");
        }
    }
    if (help_required) {
    CCharString help_message = GetName().GetAsLowerCase() + " takes ";
    if (ParamTypes.GetSize() == 0) {
        help_message += "no parameters";
    } else {
        help_message += GFIntToCharString(ParamTypes.GetSize()) + " parameter";
        if (ParamTypes.GetSize() == 1) help_message += ": "; else help_message += "s: ";
        if(false) {} else for (long c1 = 0; c1 < ParamTypes.GetSize(); ++c1)
            help_message += ParamTypes.GetParameterTypeAsString(c1) + " ";
    }
    user.SendMessageToUser(help_message);
    return false;
    }
    CallCommandFunction(function_parameters);
    return true;
}